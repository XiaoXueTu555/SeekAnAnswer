#pragma once
#include <iostream>
#include <string>
#include "High_int.h"
#include "High_float.h"

std::ostream& operator<<(std::ostream& cout, High_int data);
std::istream& operator>>(std::istream& cin, High_int& data);
std::ostream& operator<<(std::ostream& cout, High_float data);
std::istream& operator>>(std::istream& cin, High_float& data);

std::istream& operator>>(std::istream& cin, High_int& data)
{
	data.Data().clear();
	std::string temp;
	cin >> temp;
	for (uint64_t i = 0; i < temp.size(); i++)
	    if(temp.at(i) - 48 >= 0 && temp.at(i) - 48 <= 9)
		    data.Data().push_back(temp.at(i) - 48);
	if (data.Data().size() == 0)
		data.Data().push_back(0);
	while (data.Data().at(0) == 0 && data != (High_int)0)
	{
		data.Data().erase(data.Data().begin());
	}
	if (temp.at(0) == '-' && data != (High_int)0)
		data.Sign() = false;
	return cin;
}

std::ostream& operator<<(std::ostream& cout, High_int data)
{
	if (data.Sign() == 0) std::cout << "-";
	for (uint64_t i = 0; i != data.Data().size(); i++)
	{
		std::cout << (int64_t)data.Data().at(i);
	}
	return cout;
}

std::istream& operator>>(std::istream& cin, High_float& data)
{
	data.Integer().Data().clear();
	data.Decimals().Data().clear();
	data.Decimals_leading_zero() = 0;
	std::string temp;
	cin >> temp;
	int64_t integer_size = temp.find(".");
	if (integer_size < 0)
	{
		for (uint64_t i = 0; i < temp.size(); i++)
			if (temp.at(i) - 48 >= 0 && temp.at(i) - 48 <= 9)
				data.Integer().Data().push_back(temp.at(i) - 48);
	}
	else
	{
		for (int64_t i = 0; i < integer_size; i++)
			if (temp.at(i) - 48 >= 0 && temp.at(i) - 48 <= 9)
				data.Integer().Data().push_back(temp.at(i) - 48);

		for (uint64_t i = integer_size + 1; i < temp.size(); i++)
		{
			if (temp.at(i) - 48 >= 0 && temp.at(i) - 48 <= 9)
				data.Decimals().Data().push_back(temp.at(i) - 48);
		}
	}

	if (temp.at(0) == '-' && data != (High_float)0)
		data.Sign() = false;

	while (data.Decimals().Data().at(data.Decimals().Data().size() - 1) == 0 && data.Decimals() != (High_int)0)
	{
		data.Decimals().Data().pop_back();
	}
	while (true)
	{
		if (data.Decimals().Data().at(0) == 0 && data.Decimals().Data().size() > 1)
		{
			data.Decimals().Data().erase(data.Decimals().Data().begin());
			++data.Decimals_leading_zero();
		}
		else
			break;
	}
	if (data.Decimals() == (High_int)0)
		data.Decimals_leading_zero() = 0;

	return cin;
}

std::ostream& operator<<(std::ostream& cout, High_float data)
{
	if (data.Sign() == false) std::cout << "-";
	std::cout << data.Integer() << ".";
	for (int64_t i = 0; i < data.Decimals_leading_zero(); i++)
		std::cout << 0;
	std::cout << data.Decimals();
	return cout;
}
