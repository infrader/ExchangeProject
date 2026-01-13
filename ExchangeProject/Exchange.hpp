#pragma once
#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP
#include<unordered_map>
#include<string>
#include<cpr/cpr.h>
#include<chrono>
#include"Logger.hpp"
using namespace std::chrono_literals;
struct TokenInfo{
	std::string symbolpair;
	double price_of_sell;
	double price_of_vol;
	double price_of_buy;
};
class Exchange{
public:
	void get_all_tickers();
	void set_api();
	void get_api(); 
	void get_token();
private:
	struct cañhe {
	public:
		std::chrono::system_clock::time_point last_update;
		static std::chrono::seconds update_period;
		std::unordered_map<std::string, TokenInfo> data;
	};
	std::string Api;
	cañhe cashe_map;
	std::unordered_map<std::string, TokenInfo> excange_cache();
	bool expired_cache();
	virtual void parse() = 0;
	cpr::Session exchange_session;
	cpr::Response exchange_response;
};

#endif // !EXCHANGE_HPP