#include "Exchange.hpp"
std::chrono::seconds Exchange::caсhe::update_period = 4s;
void Exchange::get_all_tickers(){
	
}

void Exchange::get_token()	{

}

std::unordered_map<std::string, TokenInfo> Exchange::excange_cache(){
	if (!expired_cache()) {
		return cashe_map.data;
	}
	else {
		try {
			exchange_session.SetUrl(cpr::Url(Api));
			exchange_session.SetTimeout({ 300 });
			exchange_response = exchange_session.Get();
			if (exchange_response.status_code != 200) {
				throw std::runtime_error("Api status isn`t 200!");
			}
		}
		catch (const std::runtime_error& er) {
			Log_Warn(er.what());
		}
		catch (...) {

		}
	}
}

bool Exchange::expired_cache()
{
	try {
		if (cashe_map.last_update == std::chrono::system_clock::time_point{}) {
			return true;
		}
		auto expired_time = cashe_map.last_update + Exchange::caсhe::update_period;
		if (std::chrono::system_clock::now() > expired_time) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (...) {
		Log_Critical("Критическая ошибка с определением expired_cache!");
	}
}