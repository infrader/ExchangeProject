#include "Exchange.hpp"
std::chrono::seconds Exchange::cache::update_period = 4s;

void Exchange::set_api(std::string api_name){
	this->Api = api_name;
}
std::string Exchange::get_api() {
	return this->Api;
}

Exchange::~Exchange(){
	while (lock_update.load()) {
		std::this_thread::yield();
	}
}

void Exchange::excange_cache(cache& cache){
	std::lock_guard<std::mutex> cache_lc(load_lock);
		try {
			cache.state = cache_state::UNLOADING;
			exchange_session.SetUrl(cpr::Url(Api));
			exchange_session.SetTimeout({ 300 });
			exchange_response = exchange_session.Get();
			if (exchange_response.status_code != 200) {
				throw std::runtime_error("Api status isn`t 200!");
			}

		}
		catch (const std::runtime_error& er) {
			Log_Warn(er.what());
			cache.state = cache_state::EXPIRED;
			this->lock_update = false;
		}
		catch (...) {
			Log_Critical("Неизвестная ошибка excange_cache");
			cache.state = cache_state::EXPIRED;
			this->lock_update = false;
		}
	}

