#include "KuCoin.hpp"

std::unordered_map<std::string, Exchange::TokenInfo> KuCoin::parse(cpr::Response exchange_response){
    try {
        if (exchange_response.status_code != 200) {
            throw std::runtime_error("KuCoin, status_code isn`t 200!");
        }
        auto exchange_parse = json::parse(exchange_response.text);
        if (!exchange_parse.contains("code")) {
            throw std::runtime_error("KuCoin, have not field code!");
        }
        if (!exchange_parse.contains("data")) {
            throw std::runtime_error("KuCoin, have not field data!");
        }
        if (!exchange_parse["data"].is_array()) {
            throw std::runtime_error("KuCoin, field data isn`t array!");
        }
        if (exchange_parse["code"] != "200000") {
            throw std::runtime_error("KuCoin, status_code isn`t 200000!");
        }
        std::unordered_map<std::string, Exchange::TokenInfo> temp;
        for (auto& item : exchange_parse["data"]) {
            try {
                if (!item.contains("ticker")) {
                    throw std::exception("[KuCoin] Token isn`t have field ticker. Skiped! Token #");
                }
                if (!item["ticker"].is_object()) {
                    throw std::exception("[KuCoin] Token field ticker isn`t object. Skiped! Token #");
                }
                if (!item["ticker"].contains("symbol")) {
                    throw std::exception("[KuCoin] Token isn`t have field symbol. Skiped! Token #");
                }
                if (!item["ticker"].contains("buy")) {
                    throw std::exception("[KuCoin] Token isn`t have field buy. Skiped! Token #");
                }
                if (!item["ticker"].contains("sell")) {
                    throw std::exception("[KuCoin] Token isn`t have field sell. Skiped! Token #");
                }
                if (!item["ticker"].contains("vol")) {
                    throw std::exception("[KuCoin] Token isn`t have field vol. Skiped! Token #");
                }
                std::string symbol = item["ticker"]["symbol"].get<std::string>();
                std::string buy_str = item["ticker"]["buy"].get<std::string>();
                std::string sell_str = item["ticker"]["sell"].get<std::string>();
                std::string vol_str = item["ticker"]["vol"].get<std::string>();
                try {
                    double buy = std::stod(buy_str);
                    double sell = std::stod(sell_str);
                    double vol = std::stod(vol_str);
                    Exchange::TokenInfo temp_token;
                    temp_token.symbolpair = symbol;
                    temp_token.price_of_buy = buy;
                    temp_token.vol = vol;
                    temp_token.price_of_sell = sell;
                    temp.emplace(symbol, temp_token);
                    ++contains;
                }
                catch (const std::invalid_argument& ex) {
                    Log_Warn(ex.what());
                    ++skiped;
                }
                catch (const std::out_of_range& ex) {
                    Log_Warn(ex.what());
                    ++skiped;
                }
            }
            catch(const std::exception& ex){
                Log_Warn(ex.what() + std::to_string(contains));
                ++skiped;
            }
        }

    }
    catch (...) {
        Log_Critical("KuCoin, critical error!");
        std::rethrow_exception(std::current_exception());
    }
    
}
