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
    if (static_cast<int>(currentLevel) > static_cast<int>(level)) {
        return;
    }
    std::string str = std::format("[{}] [{}] {}",
        Logger::getCurrentTime(),
        Logger::LeveltoString(level),
        msg);
}
//================================================
std::string_view Logger::getCurrentTime() // функция времени
{
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::current_zone()->to_local(now);
    auto time_without_ms = std::chrono::floor<std::chrono::seconds>(local_time);
    std::string_view view = std::format("{:%H:%M:%S}", time_without_ms);
    return view;
}

std::string_view Logger::LeveltoString(State_Level level)
{
    static constexpr std::pair<int, std::string_view> level_pair[] =
    {
        {0,static_cast<std::string_view>("TRACE")},    
        {1,static_cast<std::string_view>("DEBUG")},
        {2,static_cast<std::string_view>("INFO")},
        {3,static_cast<std::string_view>("WARN")},
        {4,static_cast<std::string_view>("ERROR")},
        {5,static_cast<std::string_view>("CRITICAL")},
    };
    auto result = std::find_if(begin(level_pair), end(level_pair),
        [level](std::pair<int, std::string_view> x) {return x.first == static_cast<int>(level);});
    if (result == end(level_pair)) {
        return "UNKNOW";
    }
    return result->second;
}
