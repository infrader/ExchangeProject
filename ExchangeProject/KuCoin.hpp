#pragma once
#include"Exchange.hpp"
class KuCoin :
    public Exchange
{
public:
    std::pair<std::string, TokenInfo> find_token(const std::string& token) {
        auto temp = get_data();
       auto it = temp.find(token);
       return *it;
    }
private:
    int skiped;
    int contains;
    std::unordered_map<std::string, TokenInfo> parse(const cpr::Response& exchange_response) override;
};

