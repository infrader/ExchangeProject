#include "Exchange.hpp"
std::chrono::seconds Exchange::cache::update_period = 4s;

void Exchange::set_api(std::string api_name){
	this->Api = api_name;
}
std::string Exchange::get_api() {
	return this->Api;
}

std::unordered_map<std::string, TokenInfo> Exchange::double_buffer() {
	


}
void Exchange::excange_cache(cache& cache){
	load_lock.lock();
		try {
			cache.state = cache_state::UNLOADING;
			exchange_session.SetUrl(cpr::Url(Api));
			exchange_session.SetTimeout({ 300 });
			exchange_response = exchange_session.Get();
			if (exchange_response.status_code != 200) {
				throw std::runtime_error("Api status isn`t 200!");
			}
			cache.data_buffer = parse(exchange_response);
			cache.last_update = std::chrono::steady_clock::now();
			cache.state = cache_state::FRESH;
		}
		catch (const std::runtime_error& er) {
			Log_Warn(er.what());
		}
		catch (...) {

		}
	load_lock.unlock();
	}

void Exchange::expired_cache(cache& cache)
{
	try {
		if (cache_A.data_buffer.size() == 0 && cache_B.data_buffer.size() == 0) {
			excange_cache(cache_A);
			cache_B.data_buffer = cache_A.data_buffer;
		}
		auto expired_time = cache.last_update + Exchange::cache::update_period;
		if (std::chrono::steady_clock::now() > expired_time) {
			cache.state = cache_state::EXPIRED;
			}
		else {
			cache.state = cache_state::FRESH;
		}
	}
	catch (...) {
		Log_Critical("Критическая ошибка с определением expired_cache!");
	}
}


Exchange::cache* Exchange::get_active()
{
	return active_cache.load();
}

Exchange::cache* Exchange::get_inactive()
{
	return get_active() == &cache_A  ? &cache_B: &cache_A;
}

void Exchange::switch_active(){
	get_active() ? &cache_A : &cache_B;
}

