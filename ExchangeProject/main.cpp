#include<iostream>
#include"Logger.hpp"
#include<string>
#include <cpr/cpr.h>
#include<chrono>
#include"KuCoin.hpp"
int main() {// Главная функция main
	KuCoin ku;
	ku.set_api("https://api.kucoin.com/api/v1/market/allTickers");
	ku.spin_upload_start();
	ku.Exception_Exc();
}
//