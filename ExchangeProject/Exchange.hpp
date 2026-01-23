#pragma once
#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP
#include<unordered_map>
#include<string>
#include<cpr/cpr.h>
#include<chrono>
#include"Logger.hpp"
#include<memory>
#include<thread>
#include <atomic>
#include<mutex>
#include<deque>
#include<nlohmann/json.hpp>
using namespace nlohmann;
class Exchange{
public:
	Exchange() :data_upload_count(0) {};
	void set_api(std::string api_name);
	std::string get_api(); 
	void spin_upload_start(); // �������� ���������� ������ ����� ������ �������� � ������ ����� ����� ��� ��� ���� ����������� ����
	void spin_upload_stop(); // ��������� ���������� ������
	void Exception_Exc();
protected:
	struct TokenInfo {  // ��������� ��� ���������� � ������ � �����
		std::string symbolpair;
		double price_of_sell;
		double vol;
		double price_of_buy;
	};
	struct Exception_Exchange { // ��������� ��� ������ ���������� � ��������������� � � ������ Exchange
		std::deque<std::exception_ptr> deq_exceptions;
		std::atomic<bool> exception_flag{false};
	};
	Exception_Exchange exceptions;
	std::atomic<bool> uploading_state{true}; // �������� - ��������� ���������� ������!
	std::chrono::steady_clock::time_point time_now;
	std::chrono::steady_clock::time_point upload_time;
	std::atomic<int> data_upload_count{0}; // ������� ������ ���������� ������ ������
	void uploading_data();
	
	std::atomic<bool> flag_upload{false};// ���� ��� ��������� ��� ��������� ������
	
	std::unordered_map<std::string, TokenInfo> data_upload; // ���������� � ����/ move(data_upload)
	std::unordered_map<std::string, TokenInfo> data_cache; // - �� ��� �� ��������, 
	std::string Api;
	virtual std::unordered_map<std::string, TokenInfo> parse(const cpr::Response& exchange_response) = 0;
	std::unordered_map<std::string, TokenInfo>& get_data(); // ����� ��� ��������� ������ �����������������
};

#endif // !EXCHANGE_HPP
