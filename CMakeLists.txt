cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(ExchangeProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIERED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
add_executable(exchange_bot ExchangeProject/main.cpp)

target_link_libraries(exchange_bot PRIVATE 
cpr::cpr
nlohmann_json::nlohmann_json
spdlog::spdlog
)

