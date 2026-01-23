#include "KuCoin.hpp"

Exchange::TokenInfo KuCoin::find_token(const std::string& token)
{
        auto& temp = get_data();
        auto it = temp.find(token);
        if (it != temp.end()) {
            return it->second;
        }
        Log_Critical("Error find_token!");
        throw std::runtime_error("Error find_token!");
}

std::unordered_map<std::string, Exchange::TokenInfo> KuCoin::parse(const cpr::Response& exchange_response){
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
        if (!exchange_parse["data"].is_object()) {
            throw std::runtime_error("KuCoin, field data isn`t array!");
        }
        if (exchange_parse["code"] != "200000") {
            throw std::runtime_error("KuCoin, status_code isn`t 200000!");
        }
        if (!exchange_parse["data"].contains("ticker")) {
            throw std::runtime_error("[KuCoin] isn`t have field ticker.");
        }
        if (!exchange_parse["data"]["ticker"].is_array()) {
            throw std::runtime_error("[KuCoin] field isn`t array.");
        }
        std::unordered_map<std::string, Exchange::TokenInfo> temp;
        for (auto& item : exchange_parse["data"]["ticker"]) {
            try {
                if (!item.contains("symbol")) {
                    throw std::runtime_error("[KuCoin] Token isn`t have field symbol. Skiped! Token #");
                }
                if (!item.contains("buy")) {
                    throw std::runtime_error("[KuCoin] Token isn`t have field buy. Skiped! Token #");
                }
                if (!item.contains("sell")) {
                    throw std::runtime_error("[KuCoin] Token isn`t have field sell. Skiped! Token #");
                }
                if (!item.contains("vol")) {
                    throw std::runtime_error("[KuCoin] Token isn`t have field vol. Skiped! Token #");
                }
                std::string symbol = item["symbol"].get<std::string>();
                std::string buy_str = item["buy"].get<std::string>();
                std::string sell_str = item["sell"].get<std::string>();
                std::string vol_str = item["vol"].get<std::string>();
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
        return temp;
    }
    catch (...) {
        Log_Critical("KuCoin, critical error!");
        std::rethrow_exception(std::current_exception());
    }
}
