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
		try {
			auto it = ku.find_token("BTC-USDT");
			std::cout << "=============\nName: " << it.symbolpair << "\nPrice of sell: " << it.price_of_sell << "\nPrice of buy: "
				<< it.price_of_buy << "\nVol: " << it.vol << "\n=============\n";
				std::cout.flush(); 
		}
		catch (...) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}
//
