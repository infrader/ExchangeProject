#pragma once
#include"Exchange.hpp"
class KuCoin :
    public Exchange
{
private:
    int skiped;
    int contains;
    std::unordered_map<std::string, TokenInfo> parse(cpr::Response exchange_response) override;
};

