#include"Logger.hpp"

Logger& Logger::GetIntence() {
	static Logger intence;
	return intence;
}
//==============Абстрактные классы для log=======
void Logger::info(std::string_view msg)
{log(Logger::State_Level::INFO, msg);}

void Logger::trace(std::string_view msg)
{log(Logger::State_Level::TRACE, msg);}

void Logger::debug(std::string_view msg)
{log(Logger::State_Level::DEBUG, msg);}

void Logger::critical(std::string_view msg)
{log(Logger::State_Level::CRITICAL, msg);}

void Logger::warn(std::string_view msg)
{log(Logger::State_Level::WARN, msg);}

void Logger::error(std::string_view msg)
{log(Logger::State_Level::ERROR, msg);}
//================================================
//======log()=====================================
void Logger::log(State_Level level, std::string_view msg){
	
}
std::string Logger::getCurrentTime() // функция времени
{
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::current_zone()->to_local(now);
    auto time_without_ms = std::chrono::floor<std::chrono::seconds>(local_time);
    return std::format("{:%H:%M:%S}", time_without_ms);
}
//================================================