#include<iostream>
#include"Logger.hpp"
#include <cpr/cpr.h>
#include<chrono>
int main() {// Главная функция main
	setlocale(LC_ALL, "ru");
	std::string logFile = "Log.txt";
	Log_SetLogFile(logFile);
	Log_Info("Бот для отслеживания бирж открыт!");
	Log_Info("Бот для отслеживания бирж закрыт!");
}
//