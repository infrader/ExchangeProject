#pragma once
#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP
#include<unordered_map>
#include<string>
#include<cpr/cpr.h>
#include<chrono>
#include"Logger.hpp"
#include<memory>
#include<thread>
#include <atomic>
#include<mutex>
#include<deque>
#include<nlohmann/json.hpp>
using namespace nlohmann;
class Exchange{
public:
	Exchange() :data_upload_count(0) {};
	void set_api(std::string api_name);
	std::string get_api(); 
	void spin_upload_start(); // Включить обновление данных нужно просто добавить в другой поток метод там уже есть бесконечный цикл
	void spin_upload_stop(); // Выключить обновление данных
	void Exception_Exc();
protected:
	struct TokenInfo {  // Структура для информации о токена с парса
		std::string symbolpair;
		double price_of_sell;
		double vol;
		double price_of_buy;
	};
	struct Exception_Exchange { // структура для поимки исключений в многопоточности и в классе Exchange
		std::deque<std::exception_ptr> deq_exceptions;
		std::atomic<bool> exception_flag{false};
	};
	Exception_Exchange exceptions;
	std::atomic<bool> uploading_state{true}; // Включить - выключить обновление данных!
	std::chrono::steady_clock::time_point time_now;
	std::chrono::steady_clock::time_point upload_time;
	int data_upload_count; // Сколько циклов обновления данных прошли
	void uploading_data();
	
	std::atomic<bool> flag_upload{false};// флаг для предикада для читателей данных
	
	std::unordered_map<std::string, TokenInfo> data_upload; // обновление в фоне/ move(data_upload)
	std::unordered_map<std::string, TokenInfo> data_cache; // - то чем мы работаем, 
	std::string Api;
	virtual std::unordered_map<std::string, TokenInfo> parse(const cpr::Response& exchange_response) = 0;
	cpr::Session exchange_session;
	cpr::Response exchange_response;
	std::unordered_map<std::string, TokenInfo>& get_data(); // метод для получения данных инкапсулированный
};

#endif // !EXCHANGE_HPP
