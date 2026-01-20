#pragma once
#include"Exchange.hpp"
class KuCoin :
    public Exchange
{
public:
    TokenInfo find_token(const std::string& token);
private:
    int skiped;
    int contains;
    std::unordered_map<std::string, TokenInfo> parse(const cpr::Response& exchange_response) override;
};

