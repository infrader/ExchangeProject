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
using namespace std::chrono_literals;
struct TokenInfo{
	std::string symbolpair;
	double price_of_sell;
	double vol;
	double price_of_buy;
};
class Exchange{
public:
	void set_api(std::string api_name);
	std::string get_api(); 
	~Exchange();
private:
	std::atomic<bool> lock_update{false};
	std::mutex update_mtx;
	std::mutex load_lock;
	enum cache_state { FRESH, EXPIRED, UNLOADING };
	struct cache {
	public:
		std::atomic<cache_state> state;
		std::chrono::steady_clock::time_point last_update;
		static std::chrono::seconds update_period;
		std::unordered_map<std::string, TokenInfo> data_buffer;
	};
	std::atomic<cache*> active_cache{ &cache_A };
	std::string Api;
	cache cache_B;
	cache cache_A;
	void excange_cache(cache& cache);
	virtual std::unordered_map<std::string, TokenInfo> parse(cpr::Response exchange_response) = 0;
	cpr::Session exchange_session;
	cpr::Response exchange_response;
};

#endif // !EXCHANGE_HPP
