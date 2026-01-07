#pragma once
#include<string>
#include<iostream>
#include<fstream>

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