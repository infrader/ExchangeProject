#pragma once
#ifdef _LOGGER_HPP_
#define _LOGGER_HPP_
#include<string>
#include <string_view>
#include<iostream>
#include<fstream>

//===================Макросы==========================
#define Log_Intence Logger::GetIntence
#define Log_Info(msg) Logger::GetIntence().info(msg)
#define Log_Trace(msg) Logger::GetIntence().trace(msg)
#define Log_Debug(msg) Logger::GetIntence().debug(msg)
#define Log_Critical(msg) Logger::GetIntence().critical(msg)
#define Log_Warn(msg) Logger::GetIntence().warn(msg)
#define Log_Error(msg) Logger::GetIntence().error(msg)
#define Log_SetLoggerFile(msg) Logger::GetIntence().setLogger_file(msg)
//====================================================

class Logger {
public:
	enum class State_Level {INFO,TRACE,DEBUG,CRITICAL,WARN,ERROR};
	static Logger& GetIntence();
	 Logger(const Logger&) = delete;
	 Logger& operator=(const Logger&) = delete;
	 void setLogger_file(std::string_view msg);
	 void info(std::string_view msg);
	 void trace(std::string_view msg);
	 void debug(std::string_view msg);
	 void critical(std::string_view msg);
	 void warn(std::string_view msg);
	 void error(std::string_view msg);
	 ~Logger() = default;
private:
	State_Level currentLevel;
	Logger() { std::cout << "\nLgger/file успешно создан!\n"; };
	void log(State_Level level, std::string_view msg);
};
#endif // #ifdef _LOGGER_HPP_