#include<iostream>
#include"Logger.hpp"
#include <cpr/cpr.h>
#include<chrono>
int main() {// Главная функция main
	setlocale(LC_ALL, "ru");
	Log_SetLogFile("Log.txt");
	Log_Info("Бот для отслеживания бирж открыт!");
	Log_Info("Бот для отслеживания бирж закрыт!");
}