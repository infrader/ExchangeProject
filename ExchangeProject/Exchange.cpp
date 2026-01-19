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
{
	uploading_state.store(false);
}

void Exchange::uploading_data(){ // 1 писатель не беcпокоимся что здесь будет 2 и более потока!
	try {
		exchange_session.SetUrl(cpr::Url{ Api });
		exchange_session.SetTimeout({ 300 });
		while (uploading_state) {
			while (time_now <= upload_time && data_upload_count != 0) {
				std::this_thread::yield();
			}
			exchange_response = exchange_session.Get();
			time_now = std::chrono::steady_clock::now();
			data_upload = parse(exchange_response);
			std::atomic_signal_fence(std::memory_order_seq_cst); // Барьер set_cst чтобы время мы получили после Get точно и компилятор не путаялся
			{
				std::unique_lock<std::mutex> guard(move_data); // Мютекс флаг который говорит перестать работать с data_cache будеь свап
				data_cache = std::move(data_upload);
			}
			uploud_succses.notify_all();
			upload_time = time_now + std::chrono::milliseconds(3000);
			data_upload_count++;
		}
	}
	catch (...) {

	}
}

