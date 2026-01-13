#include<iostream>
#include"Logger.hpp"
#include <cpr/cpr.h>
#include<chrono>
#include"Exchange.hpp"
int main() {// Главная функция main
	setlocale(LC_ALL, "ru");
	// Простой GET-запрос
        cpr::Response r = cpr::Get(cpr::Url{ "https://httpbin.org/get" });

    // Проверяем результат
    std::cout << "Status code: " << r.status_code << std::endl;
    std::cout << "Response: " << r.text << std::endl;

    return 0;
}
//