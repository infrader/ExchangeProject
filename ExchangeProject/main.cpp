#include<iostream>
#include"Logger.hpp"
#include<string>
#include <cpr/cpr.h>
#include<chrono>
#include"KuCoin.hpp"
int main() {// Главная функция main
	KuCoin ku;
	ku.set_api("https://api.kucoin.com/api/v1/market/allTickers");
	std::thread th([&ku]() {ku.spin_upload_start(); });
	auto timer = std::chrono::steady_clock::now() + std::chrono::minutes(1);
	while (std::chrono::steady_clock::now() <= timer) {
		auto it = ku.find_token("BTC-USDT");
		std::cout << "=============\nName: " << it.first << "\nPrice of sell: " << it.second.price_of_sell << "\nPrice of buy: "
			<< it.second.price_of_buy << "\nVol: " << it.second.vol << "\n=============\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//
