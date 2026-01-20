//
#include"Logger.hpp"
std::mutex Logger::mtx_intence;
std::atomic<Logger*> Logger::intence{ nullptr };

Logger* Logger::GetIntence() {
    Logger* ptr = intence.load(std::memory_order_acquire);
    if (ptr == nullptr) {
        std::unique_lock<std::mutex> guard(mtx_intence);
        ptr = intence.load(std::memory_order_relaxed);
        if (ptr == nullptr) {
            ptr = new Logger;
            intence.store(ptr, std::memory_order_release);
        }
    }
    return ptr;
}
void Logger::setLogger_file(std::string msg) {
    nameFile = msg; // это не разделяеммый ресурс 1 файл
}
//==============Абстрактные классы для log=======
void Logger::info(const std::string& msg)
{log(Logger::State_Level::INFO, msg);}

void Logger::trace(const std::string& msg)
{log(Logger::State_Level::TRACE, msg);}

void Logger::debug(const std::string& msg)
{log(Logger::State_Level::DEBUG, msg);}

void Logger::critical(const std::string& msg)
{log(Logger::State_Level::CRITICAL, msg);}

void Logger::warn(const std::string& msg)
{log(Logger::State_Level::WARN, msg);}

void Logger::setCurrenLevel(State_Level lvl){
    currentLevel = lvl;
}
//================================================
//======log()=====================================
void Logger::log(State_Level level, const std::string& msg){
    std::unique_lock<std::mutex> guard(mtx_file); // Перекрываю все мютексом
    if (static_cast<int>(currentLevel) > static_cast<int>(level)) {
        std::string msg_format = std::format("[{}] [{}] {}",
            Logger::getCurrentTime(),
            Logger::LeveltoString(level),
            msg);
        try {
            logger_file.open(nameFile, std::ofstream::app);
            if (logger_file.is_open()) {
                std::string str_info_log = msg_format;
                logger_file << str_info_log << "\n";
                logger_file.close();
                std::cout << msg_format << "\n";
            }
            else {
                std::string error_open = std::format("[{}] [{}] {}", Logger::getCurrentTime(), "File", "Error file isn`t Open");
                std::cout << error_open << "\n";
            };
        }
        catch (...) {
            std::string error_of = std::format("[{}] [{}] {}", Logger::getCurrentTime(), "File", "Error Ofstream");
            std::cout << error_of << "\n";
        }
    }
    else{ return; }
}
//================================================
std::string Logger::getCurrentTime() // функция времени
{
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::current_zone()->to_local(now);
    auto time_without_ms = std::chrono::floor<std::chrono::seconds>(local_time);
    std::string view = std::format("{:%H:%M:%S}", time_without_ms);
    return view;
}

const std::string& Logger::LeveltoString(State_Level level) // функция чтобы в текст писало какой режим
{
    static std::pair<int,std::string> level_pair[] =
    {
        {0,"TRACE"},
        {1,"DEBUG"},
        {2,"INFO"},
        {3,"WARN"},
        {4,"CRITICAL"},
    };
    auto result = std::find_if(begin(level_pair), end(level_pair),
        [level](std::pair<int, std::string> x) {return x.first == static_cast<int>(level);});
    if (result == end(level_pair)) {
        return "UNKNOW";
    }
    return result->second;
}
//

