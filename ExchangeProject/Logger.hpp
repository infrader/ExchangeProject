//===================Защита от двойного включения===========
#pragma once 
#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_
//===========================================================
#include<string>
#include <string_view>
#include<iostream>
#include<fstream>
#include<format>
#include<chrono>
#include<vector>
#include<algorithm>
#include<mutex>
//===================Макросы==========================
#define Log_Intence Logger::GetIntence
#define Log_SetLogFile(msg) Logger::GetIntence()->setLogger_file(msg)
#define Log_Info(msg) Logger::GetIntence()->info(msg)
#define Log_Trace(msg) Logger::GetIntence()->trace(msg)
#define Log_Debug(msg) Logger::GetIntence()->debug(msg)
#define Log_Critical(msg) Logger::GetIntence()->critical(msg)
#define Log_Warn(msg) Logger::GetIntence()->warn(msg)
#define Log_SetLoggerFile(msg) Logger::GetIntence()->setLogger_file(msg)
//====================================================


//========================Logger__Class====================
class Logger {
public:
	enum class State_Level { TRACE,DEBUG,INFO,WARN,CRITICAL};
	static Logger* GetIntence();
	 Logger(const Logger&) = delete;
	 Logger& operator=(const Logger&) = delete;
	 void setLogger_file(std::string msg);
	 void info(const std::string& msg);
	 void trace(const std::string& msg);
	 void debug(const std::string& msg);
	 void critical(const std::string& msg);
	 void warn(const std::string& msg);
	 void setCurrenLevel(State_Level lvl);
	 ~Logger() = default;
private:
	State_Level currentLevel;
	std::mutex mtx_file;
	static std::atomic<Logger*> intence;
	static std::mutex mtx_intence;
	Logger():currentLevel(State_Level::CRITICAL), nameFile("Log.txt") {};
	void log(State_Level level, const std::string& msg);
	std::string getCurrentTime();
	const std::string& LeveltoString(State_Level level);
	std::ofstream logger_file;
	std::string nameFile;
};
//=============================================================

//===================Защита от двойного включения===========
#endif // #ifdef _LOGGER_HPP_
//===========================================================
