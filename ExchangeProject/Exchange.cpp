#include "Exchange.hpp"

void Exchange::set_api(std::string api_name){
	this->Api = api_name;
}
std::string Exchange::get_api() {
	return this->Api;
}

void Exchange::spin_upload_start() // Мы запускаем в другом потоке spin_lock
{
	uploading_state.store(true);
	uploading_data();
}

void Exchange::spin_upload_stop() // Перед тем как за джоинить upload_start - нужно обязательно выполнить остановку upload_stop().
{								// иначе поток никогда не заджониться у нас бесконечный цикл завязан на переменной uploading_state
	uploading_state.store(false);
}

void Exchange::Exception_Exc(){ // 1 читатель исключений
	bool excpected = false;
	while (!exceptions.exception_flag.compare_exchange_strong(excpected, true, std::memory_order_acquire)
		&& exceptions.deq_exceptions.empty()) { // Читатель один, поэтому никто не может уменьшить дек. 
		std::this_thread::yield();					//Только читатель может удалить из дека данные когда они прочитаны
	}
	while (!exceptions.deq_exceptions.empty()) {
		try {
			auto exp = exceptions.deq_exceptions.back(); 
			exceptions.deq_exceptions.pop_back(); // Только читатель когда прочитал, удаляет исключение чтобы дважды не читать его
			std::rethrow_exception(exp);  // кидаем исключение 
		}
		catch (std::exception& exc) {
			Log_Warn(exc.what()); // Из потока будет Warn исключения.
		}
	}
	exceptions.exception_flag.store(false, std::memory_order_release);
}

void Exchange::uploading_data(){ // 1 писатель не беcпокоимся что здесь будет 2 и более потока!
	try {
		while (uploading_state) {
			while (std::chrono::steady_clock::now() <= upload_time && data_upload_count != 0) {
				std::this_thread::yield();
			}
			exchange_session.SetUrl(cpr::Url{ Api });
			exchange_session.SetTimeout({ 3000 });
			exchange_response = exchange_session.Get();
			upload_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(3000); 
			data_upload = parse(exchange_response);
			std::atomic_signal_fence(std::memory_order_seq_cst); // Барьер set_cst чтобы время мы получили после Get точно и компилятор не путаялся
			bool excpected = false;
			flag_upload.compare_exchange_strong(excpected, true, std::memory_order_acquire);
			data_cache = std::move(data_upload);
			flag_upload.store(false, std::memory_order_release);
			data_upload_count++;
		}
	}
	catch (...) {
		bool excpected = false;
		while (!exceptions.exception_flag.compare_exchange_strong(excpected, true, std::memory_order_acquire)) { 
			std::this_thread::yield();
		}
		exceptions.deq_exceptions.push_front(std::current_exception());
		exceptions.exception_flag.store(false, std::memory_order_release);
	}
}

std::unordered_map<std::string, Exchange::TokenInfo>& Exchange::get_data(){
	while (data_upload_count == 0) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	while (flag_upload.load(std::memory_order_acquire)) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return data_cache;
}

