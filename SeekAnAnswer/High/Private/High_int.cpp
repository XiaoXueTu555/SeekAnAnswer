#include <math.h>
#include "../Public/High_int.h"

#define High_int_NULL (High_int)0

/*构造函数*/
High_int::High_int()
{
	this->sign = true;
	this->data.push_back(0);
}

High_int::High_int(const int64_t data)
{
	this->sign = true;
	this->data.push_back(0);
	*this = data;
}
High_int::High_int(const std::string data)
{
	*this = data;
}
/*析构函数*/
High_int::~High_int()
{
	return;
}
/*赋值运算符重载*/
High_int High_int::operator=(int64_t data)
{
	High_int result;
	result.Data().clear();
	result.Sign() = (data >= 0) ? true : false;
	data = abs(data);
	while (data >= 10)
	{
		result.Data().push_back(data % 10);
		data /= 10;
	}
	result.Data().push_back((char)data);
	*this = result;
	for (int64_t i = 0; i != result.Data().size(); i++)
	{
		this->Data().at(i) = result.Data().at(result.Data().size() - 1 - i);
	}
	return *this;
} 

High_int High_int::operator=(High_int data)
{
	this->sign = data.sign;
	this->data = data.data;
	return *this;
}

High_int High_int::operator=(std::string temp)
{
	High_int data;
	data.Data().clear();
	for (uint64_t i = 0; i < temp.size(); i++)
		if (temp.at(i) - 48 >= 0 && temp.at(i) - 48 <= 9)
			data.Data().push_back(temp.at(i) - 48);
	if (data.Data().size() == 0)
		data.Data().push_back(0);
	while (data.Data().at(0) == 0 && data != (High_int)0)
	{
		data.Data().erase(data.Data().begin());
	}
	if (temp.at(0) == '-' && data != (High_int)0)
		data.Sign() = false;
	*this = data;
	return data;
}

/*转换函数*/
High_int::operator int64_t()
{
	int64_t data = 0;
	int64_t multiple = 1;
	for (int64_t i = this->Data().size() - 1; i > 0; i--)
	{
		data += this->Data().at(i) * multiple;
		multiple *= 10;
	}
	data += this->Data().at(0) * multiple;
	if (this->Sign() == false)
		data = -data;
	return data;
}

High_int::operator std::string()
{
	std::string data;
	if (!this->sign)
	{
		data.push_back('-');
	}
	for (uint64_t i = 0; i < this->data.size(); i++)
	{
		data.push_back(this->data.at(i) + 48);
	}
	return data;
}

/*判断运算符重载*/
bool High_int::operator==(High_int data)
{
	return (this->data == data.data && this->sign == data.sign) ? true : false;
}

bool High_int::operator!=(High_int data)
{
	return (*this == data) ? false : true;
}

bool High_int::operator>(High_int data)
{
	if (this->sign > data.sign) return true;
	else if (this->sign < data.sign) return false;
	else if (this->sign == true && data.sign == true) {
		if (this->data.size() > data.Data().size()) return true;
		if (this->data.size() < data.Data().size()) return false;
		for (uint64_t i = 0; i < this->data.size(); i++)
		{
			if (this->data.at(i) > data.Data().at(i)) return true;
			else if (this->data.at(i) < data.Data().at(i)) return false;
		}
	}
	else if (this->sign == false && data.sign == false)
	{
		if (this->data.size() > data.Data().size()) return false;
		if (this->data.size() < data.Data().size()) return true;
		for (uint64_t i = 0; i < this->data.size(); i++)
		{
			if (this->data.at(i) > data.Data().at(i)) return false;
			else if (this->data.at(i) < data.Data().at(i)) return true;
		}
	}
	return false;
}

bool High_int::operator>=(High_int data)
{
	return (*this > data || *this == data) ? true : false;
}

bool High_int::operator<(High_int data)
{
	return (*this >= data) ? false : true;
}

bool High_int::operator<=(High_int data)
{
	return (*this < data || *this == data) ? true : false;
}

/*四则运算符重载*/

High_int High_int::operator+(High_int b)
{
	High_int result;
	if (*this == High_int_NULL)
		return b;
	else if (b == High_int_NULL)
		return *this;
	if (*this >= High_int_NULL && b < High_int_NULL)
	{
		b.sign = true;
		return *this - b;
	}
	else if (*this < High_int_NULL && b >= High_int_NULL)
	{
		result = *this;
		result.sign = true;
		result -= b;
		result.sign = false;
		return result;
	}
	else if (*this < High_int_NULL && b < High_int_NULL)
	{
		this->sign = true;
		b.Sign() = true;
		result = *this + b;
		result.sign = false;
		this->sign = false;
		return result;
	}
	if (*this < b)
		return b + *this;
	b.AlignToSuitableSize(*this);
	result.AlignToSuitableSize(*this);
	for (uint64_t i = 0; i < this->data.size(); i++)
	{
		result.Data().at(this->data.size() - 1 - i) +=
			this->data.at(this->data.size() - 1 - i) + b.Data().at(this->data.size() - 1 - i);
		if (result.Data().at(this->data.size() - 1 - i) >= 10
			&&
			(this->data.size() - 1 - i) == 0)
		{
			result.Data().at(this->data.size() - 1 - i) -= 10;
			result.Data().insert(result.Data().begin(), 1);
		}
		else if (result.Data().at(this->data.size() - 1 - i) >= 10)
		{
			result.Data().at(this->data.size() - 1 - i) -= 10;
			++result.Data().at(this->data.size() - 2 - i);
		}
	}
	return result;
}

High_int High_int::operator-(High_int b)
{
	if (*this >= High_int_NULL && b < High_int_NULL)
	{
		b.sign = true;
		return *this + b;
	}
	else if (*this < High_int_NULL && b >= High_int_NULL)
	{
		this->sign = true;
		High_int result = *this + b;
		result.sign = false;
		this->sign = false;
		return result;
	}
	else if (*this < High_int_NULL && b < High_int_NULL)
	{
		this->sign = true;
		b.sign = true;
		High_int result = b - *this;
		this->sign = false;
		return result;
	}
	if (*this < b)
	{
		High_int result = b - *this;
		result.sign = false;
		return result;
	}
	High_int a = *this;
	High_int result;
	a.AlignToSuitableSize(b);
	a.AlignToSuitableSize(result);
	for (uint64_t i = 0; i < a.data.size(); i++)
	{
		if (a.Data().at(a.Data().size() - 1 - i) < b.Data().at(a.Data().size() - 1 - i))
		{
			a.Data().at(a.Data().size() - 1 - i) += 10;
			--a.Data().at(a.Data().size() - 2 - i);
		}
		result.Data().at(a.Data().size() - 1 - i) +=
			a.Data().at(a.Data().size() - 1 - i) - b.Data().at(a.Data().size() - 1 - i);
	}
	result.Shrink_to_fit();
	return result;
}

High_int High_int::operator*(High_int b)
{
	High_int result;
	High_int temp_result;
	int64_t a_size = this->data.size();
	int64_t b_size = b.Data().size();
	if (*this == High_int_NULL || b == High_int_NULL)
	{
		return result;
	}
	if (*this < High_int_NULL && b >= High_int_NULL)
	{
		result = *this;
		result.Sign() = true;
		result *= b;
		result.Sign() = false;
		return result;
	}
	else if (*this > High_int_NULL && b < High_int_NULL)
	{
		b.sign = true;
		result = *this * b;
		result.sign = false;
		return result;
	}
	else if (*this < High_int_NULL && b < High_int_NULL)
	{
		b.sign = true;
		result = *this;
		result.sign = true;
		result *= b;
		return result;
	}
	if (*this < b)
		return b * *this;
	this->AlignToSuitableSize(b);
	this->AlignToSuitableSize(temp_result);
	for (int64_t i = 0; i < b_size; i++)
	{
		for (int64_t j = 0; j < a_size; j++)
		{
			temp_result.Data().at(this->data.size() - 1 - j) +=
				b.Data().at(this->data.size() - 1 - i) * this->data.at(this->data.size() - 1 - j);
			if (temp_result.Data().at(this->data.size() - 1 - j) >= 10 && this->data.size() - 1 - j == 0)
			{
				temp_result.Data().insert(temp_result.data.begin(), temp_result.Data().at(this->data.size() - 1 - j) / 10);
				temp_result.Data().at(this->data.size() - j) %= 10;
			}
			else if (temp_result.Data().at(this->data.size() - 1 - j) >= 10)
			{
				temp_result.Data().at(this->data.size() - 2 - j) +=
					temp_result.Data().at(this->data.size() - 1 - j) / 10;
				temp_result.Data().at(this->data.size() - 1 - j) %= 10;
			}
		}
		temp_result.Data().insert(temp_result.Data().end(), i, 0);
		result += temp_result;
		temp_result = 0;
		this->AlignToSuitableSize(temp_result);
	}
	result.Shrink_to_fit();
	return result;
}

High_int High_int::operator/(High_int b)
{	
	if (b == High_int_NULL)
		throw "Division by zero condition!";
	High_int result;
	High_int remainder;
	uint64_t align_position = 0;
	uint64_t temp_record = 0;
	result.Data().clear();
	remainder.Data().clear();
	if (*this == High_int_NULL)
		return High_int_NULL;
	if (*this > High_int_NULL && b < High_int_NULL)
	{
		b.sign = true;
		result = *this / b;
		result.sign = false;
		return result;
	}
	else if (*this < High_int_NULL && b >= High_int_NULL)
	{
		result = *this;
		result.Sign() = true;
		result /= b;
		if (result == High_int_NULL)
			return result;
		result.sign = false;
		return result;
	}
	else if (*this < High_int_NULL && b < High_int_NULL)
	{
		result = *this;
		result.sign = true;
		b.sign = true;
		return result / b;
	}

	while (remainder < b)
	{
		if (temp_record == this->data.size())
			break;
		remainder.Data().push_back(this->data.at(temp_record++));
	}
	align_position = remainder.data.size() - 1;

	while (result.data.size() + align_position < this->data.size())
	{
		result.Data().push_back(remainder.basic_Division(b));
		remainder -= (High_int)result.Data().at(result.Data().size() - 1) * b;

		if (remainder.data.size() == 1 && remainder.data.at(0) == 0)
			remainder.data.clear();

		while (remainder < b)
		{
			if (temp_record == this->data.size())
				break;
			remainder.Data().push_back(this->data.at(temp_record++));
			if (remainder < b)
				result.data.push_back(0);
		}
		remainder.Shrink_to_fit();
	}
	return result;
}

High_int High_int::operator%(High_int b)
{
	return *this - ((*this / b) * b);
}

High_int High_int::operator++()
{
	*this += (High_int)1;
	return *this;
}

High_int High_int::operator++(int)
{
	High_int temp = *this;
	*this += (High_int)1;
	return temp;
}

High_int High_int::operator--()
{
	*this -= (High_int)1;
	return *this;
}

High_int High_int::operator--(int)
{
	High_int temp = *this;
	*this -= (High_int)1;
	return temp;
}

High_int High_int::operator+=(High_int data)
{
	*this = *this + data;
	return *this;
}

High_int High_int::operator-=(High_int data)
{
	*this = *this - data;
	return *this;
}

High_int High_int::operator*=(High_int data)
{
	*this = *this * data;
	return *this;
}

High_int High_int::operator/=(High_int data)
{
	*this = *this / data;
	return *this;
}

High_int High_int::operator%=(High_int data)
{
	*this = *this % data;
	return *this;
}

bool& High_int::Sign()
{
	return this->sign;
}

std::vector<int8_t>& High_int::Data()
{
	return this->data;
}

void High_int::private_Shrink_to_fit()
{
	this->Shrink_to_fit();
}

void High_int::AlignToSuitableSize(High_int& data)
{
	if (*this > data)
		data.Data().insert(data.Data().begin(), this->data.size() - data.Data().size(), 0);
	else if (*this < data)
		data.AlignToSuitableSize(*this);
	return;
}

void High_int::Shrink_to_fit()
{
	if (this->data.size() == 0)
	{
		this->data.push_back(0);
		return;
	}
	while (this->data.at(0) == 0 && *this != High_int_NULL)
	{
		this->data.erase(this->data.begin());
	}
	return;
}

char High_int::basic_Division(High_int b)
{
	char result = 0;
	if (*this < b)
		return result;
	High_int temp;
	while (temp <= *this)
	{
		temp += b;
		++result;
	}
	if (temp > b)
		--result;
	return result;
}
