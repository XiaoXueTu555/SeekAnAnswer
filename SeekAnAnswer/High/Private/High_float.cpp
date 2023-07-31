#include <sstream>
#include <string>
#include <math.h>
#include"../Public/High_float.h"
#include"../Public/High_int.h"

#define High_float_NULL (High_float)0.0
#define High_int_NULL (High_int)0

#define MAX(a,b) (a > b) ? a : b

//初始化默认精度
int64_t High_float::precision = PRECISION;

/*构造函数*/
High_float::High_float()
{
	this->sign = true;
	this->integer = 0;
	this->decimals = 0;
	this->decimals_leading_zero = 0;
	return;
}

High_float::High_float(High_int data)
{
	*this = data;
}

High_float::High_float(long double data)
{
	*this = data;
}

High_float::High_float(std::string data)
{
	*this = data;
}

/*析构函数*/
High_float::~High_float()
{
	return;
}

/*转换函数*/
High_float::operator High_int()
{
	return this->integer;
}

High_float::operator std::string()
{
	std::string data;
	data = (std::string)this->Integer();
	if (this->decimals != High_int_NULL)
	{
		data.push_back((const char)'.');
		for (int64_t i = 0; i < this->Decimals_leading_zero(); i++)
		{
			data.push_back('0');
		}
		data += (std::string)this->decimals;
	}
	if (!this->sign)
	{
		data.insert(data.begin(), '-');
	}
	return data;
}

High_float::operator long double()
{
	long double result;
	std::stringstream ss;
	ss << (std::string)*this;
	ss >> result;
	return result;
}

/*设置除法的计算精度*/
void High_float::setprecision(int64_t precision)
{
	High_float::precision = precision;
	return;
}

void High_float::private_Shrink_to_fit_of_decimals()
{
	this->Shrink_to_fit_of_decimals();
}

/*赋值运算符重载*/
High_float High_float::operator=(High_float data)
{
	this->integer = data.integer;
	this->decimals = data.decimals;
	this->sign = data.sign;
	this->decimals_leading_zero = data.decimals_leading_zero;
	return *this;
}

High_float High_float::operator=(High_int data)
{
	this->sign = data.Sign();
	this->integer = data;
	this->integer.Sign() = true;
	this->decimals = 0;
	this->decimals_leading_zero = 0;
	return *this;
}

High_float High_float::operator=(long double data)
{
	if (data < 0)
		this->sign = false;
	else
		this->sign = true;
	data = abs(data);
	std::stringstream ss;
	ss.precision(16);
	ss << data;
	std::string temp = ss.str();
	uint64_t location = temp.find('.');
	this->integer.Data().clear();
	this->decimals.Data().clear();
	this->decimals_leading_zero = 0;
	if (location == -1)
	{
		this->integer = (int64_t)data;
		this->decimals.Data().push_back(0);
		return *this;
	}
	uint64_t count = 0;
	while (count < location)
	{
		this->integer.Data().push_back(temp.at(count) - 48);
		++count;
	}
	++location;
	while (location < temp.size())
	{
		this->decimals.Data().push_back(temp.at(location) - 48);
		++location;
	}
	this->Shrink_to_fit_of_decimals();
	return *this;
}

/*判断运算符重载*/
bool High_float::operator==(High_float data)
{
	return (this->integer == data.integer && 
		this->decimals == data.decimals && 
		this->sign == data.sign &&
		this->decimals_leading_zero == data.decimals_leading_zero) 
		? true : false;
}

bool High_float::operator!=(High_float data)
{
	return (*this == data) ? false : true;
}

High_float High_float::operator=(std::string data)
{
	if (data.at(0) == '-') this->sign = false;
	else this->sign = true;

	std::string container; //容器
	uint64_t i = 0;
	for (i; i < data.size(); i++)
	{
		if (data.at(i) == '.') break;
		else container.push_back(data.at(i));
	}

	this->integer = container;
	container.clear();

	for (i; i < data.size(); i++)
	{
		container.push_back(data.at(i));
	}

	for (uint64_t j = 0; j < container.size(); j++)
	{
		if (container.at(0) == '0')
		{
			++this->decimals_leading_zero;
			container.erase(container.begin());
		}
		else break;
	}

	if (container.size() > 0) this->decimals = container;

	return *this;
}

bool High_float::operator>(High_float data)
{
	if (this->sign && !data.sign)
		return true;
	else if (!this->sign && data.sign)
		return false;
	if (this->sign)
	{
		if ((this->integer > data.integer) ? true : false) return true;
		else if ((this->integer < data.integer) ? true : false) return false;
		if (this->decimals_leading_zero < data.decimals_leading_zero && this->decimals != High_int_NULL)
			return true;
		else if (data.decimals_leading_zero < this->decimals_leading_zero && data.decimals != High_int_NULL)
			return false;
		return (this->decimals > data.decimals) ? true : false;
	}
	else
	{
		if ((this->integer < data.integer) ? true : false) return true;
		else if ((this->integer > data.integer) ? true : true) return false;
		if (this->decimals_leading_zero < data.decimals_leading_zero && this->decimals != High_int_NULL)
			return false;
		else if (data.decimals_leading_zero < this->decimals_leading_zero && data.decimals != High_int_NULL)
			return true;
		return (this->decimals >= data.decimals) ? false : true;
	}
}

bool High_float::operator>=(High_float data)
{
	return (*this > data || *this == data) ? true : false;
}

bool High_float::operator<(High_float data)
{
	return (*this >= data) ? false : true;
}

bool High_float::operator<=(High_float data)
{
	return (*this < data || *this == data) ? true : false;
}

/*四则运算符重载*/
High_float High_float::operator+(High_float b)
{
	High_float result;
	if (*this >= High_float_NULL && b < High_float_NULL)
	{
		b.sign = true;
		result = *this - b;
		return result;
	}
	else if (*this < High_float_NULL && b >= High_float_NULL)
	{
		this->sign = true;
		result = b - *this;
		this->sign = false;
		return result;
	}
	else if (*this < High_float_NULL && b < High_float_NULL)
	{
		this->sign = true;
		b.sign = true;
		result = *this + b;
		this->sign = false;
		return result;
	}
	this->AlignToSuitableSize(b);
	result.integer += this->integer + b.integer;
	result.decimals += this->decimals + b.decimals;
	if (result.decimals.Data().size() > 
		(MAX(this->decimals_leading_zero + this->decimals.Data().size(),
			b.decimals_leading_zero + b.decimals.Data().size())))
	{
		++result.integer;
		result.decimals.Data().erase(result.decimals.Data().begin());
	}
	result.decimals_leading_zero += 
		(MAX(this->decimals_leading_zero + this->decimals.Data().size(),
			b.decimals_leading_zero + b.decimals.Data().size())) - result.decimals.Data().size();
	this->Shrink_to_fit_of_decimals();
	result.Shrink_to_fit_of_decimals();
	return result;
}

High_float High_float::operator-(High_float b)
{
	High_float result;

	if (*this >= High_float_NULL && b < High_float_NULL)
	{
		b.sign = true;
		result = *this + b;
		return result;
	}
	else if (*this < High_float_NULL && b >= High_float_NULL)
	{
		this->sign = true;
		result = *this + b;
		result.sign = false;
		this->sign = false;
		return result;
	}
	else if (*this < High_float_NULL && b < High_float_NULL)
	{
		this->sign = true;
		b.sign = true;
		result = b - *this;
		this->sign = false;
		return result;
	}
	if (*this < b)
	{
		result = b - *this;
		result.sign = false;
		return result;
	}
	this->AlignToSuitableSize(b);

	if (b.decimals > this->decimals)
	{
		High_int temp = 1;
		//计算借位后的数值
		for (uint64_t i = 0; i < 
			this->decimals_leading_zero + this->decimals.Data().size(); i++)
		{
			temp *= 10;
		}
		temp += this->decimals;

		result.decimals = temp - b.decimals;

		--result.integer;

		result.decimals_leading_zero = 0;
		result.Shrink_to_fit_of_decimals();
	}
	else
	{
		result.decimals = this->decimals - b.decimals;
		result.decimals_leading_zero =
			this->decimals.Data().size() - 
			result.decimals.Data().size() + this->decimals_leading_zero;
	}
	result.integer += this->integer - b.integer;

	this->Shrink_to_fit_of_decimals();
	result.Shrink_to_fit_of_decimals();
	return result;
}

High_float High_float::operator*(High_float b)
{
	High_float result;
	High_int multiplier_a = this->integer;
	High_int multiplier_b = b.integer;
	multiplier_a.Data().
		insert(multiplier_a.Data().end(), 
			this->decimals_leading_zero, 0);

	multiplier_b.Data().
		insert(multiplier_b.Data().end(),
			b.decimals_leading_zero, 0);

	if (this->decimals != High_int_NULL)
		multiplier_a.Data().
		insert(multiplier_a.Data().end(),
			this->decimals.Data().begin(),
			this->decimals.Data().end());

	if (b.decimals != High_int_NULL)
		multiplier_b.Data().
		insert(multiplier_b.Data().end(),
			b.decimals.Data().begin(),
			b.decimals.Data().end());

	multiplier_a.private_Shrink_to_fit();
	multiplier_b.private_Shrink_to_fit();

	multiplier_a *= multiplier_b;

	int64_t digit_decimals = this->decimals_leading_zero + b.decimals_leading_zero;
	if (this->decimals != High_int_NULL)
		digit_decimals += this->decimals.Data().size();
	if (b.decimals != High_int_NULL)
		digit_decimals += b.decimals.Data().size();

	int64_t digit_integer =
		multiplier_a.Data().size() - digit_decimals;

	if (digit_integer <= 0)
	{
		result.integer = 0;
		result.decimals.Data().clear();
		result.decimals.Data().
			insert(result.decimals.Data().begin(),
				digit_decimals - multiplier_a.Data().size(), 0);

		result.decimals.Data().
			insert(result.decimals.Data().end(),
				multiplier_a.Data().begin(),
				multiplier_a.Data().end());
	}
	else
	{
		result.integer.Data().clear();
		result.decimals.Data().clear();
		if (digit_integer >= (int64_t)multiplier_a.Data().size())
		{
			result.integer.Data().
				insert(result.integer.Data().end(),
					multiplier_a.Data().begin(), 
					multiplier_a.Data().end());

			result.integer.Data().
				insert(result.integer.Data().end(), 
					digit_integer - multiplier_a.Data().size(), 0);

			result.decimals.Data().push_back(0);
		}
		else
		{
			result.integer.Data().
				insert(result.integer.Data().end(),
					multiplier_a.Data().begin(),
					multiplier_a.Data().begin() + digit_integer);

			result.decimals.Data().
				insert(result.decimals.Data().end(),
					multiplier_a.Data().begin() + digit_integer,
					multiplier_a.Data().end());
		}
	}
	result.integer.private_Shrink_to_fit();
	result.Shrink_to_fit_of_decimals();

	if (this->sign == true && b.sign == false)
		result.sign = false;
	else if (this->sign == false && b.sign == true)
		result.sign = false;
	return result;
}

High_float High_float::operator/(High_float b)
{
	if (b == High_float_NULL)
		throw "Division by zero condition!";
	High_float result;
	High_float dividend_a;
	dividend_a = *this;

	High_int divisor_b = b.integer;

	if (*this == High_float_NULL)
		return High_float_NULL;
	if (*this > High_float_NULL && b < High_float_NULL)
	{
		b.sign = true;
		result = *this / b;
		result.sign = false;
		return result;
	}
	else if (*this < High_float_NULL && b > High_float_NULL)
	{
		result = *this;
		result.sign = true;
		result /= b;
		if (result == High_float_NULL)
			return result;
		result.sign = false;
		return result;
	}
	else if (*this < High_float_NULL && b < High_float_NULL)
	{
		result = *this;
		result.sign = true;
		b.sign = true;
		return result / b;
	}

	//被除数和除数进行处理
	if (b.decimals != High_int_NULL)
	{
		High_int ten = 1;
		ten.Data().insert(ten.Data().end(), b.decimals.Data().size() + b.decimals_leading_zero, 0);
		dividend_a *= (High_float)ten;
	}

	divisor_b.Data().insert(divisor_b.Data().end(), 
		b.decimals_leading_zero, 0);
	if (b.decimals != High_int_NULL)
		divisor_b.Data().insert(divisor_b.Data().end(),
			b.decimals.Data().begin(), b.decimals.Data().end());
	divisor_b.private_Shrink_to_fit();

	//获取余数
	result.integer = dividend_a.integer / divisor_b;
	dividend_a.integer -= result.integer * divisor_b;

	//如果被除数等于0说明此计算可以整除直接返回结果，否则进入死循环
	if (dividend_a == High_float_NULL)
	{
		return result;
	}

	//处理余数
	while (dividend_a.integer < divisor_b)
	{
		dividend_a *= 10;
		if (dividend_a.integer < divisor_b)
		{
			result.decimals_leading_zero += 1;
		}
	}

	//要计算的小数点位数
	for (int64_t i = 0; i < High_float::precision - 1; i++)
	{
		dividend_a *= 10;
	}
	result.decimals = dividend_a.integer / divisor_b;
	result.Shrink_to_fit_of_decimals();
	return result;
}

High_float High_float::operator%(High_float b)
{
	return *this - ((*this / b) * b);
}

/*自操作运算符*/
High_float High_float::operator+=(High_float data)
{
	*this = *this + data;
	return *this;
}

High_float High_float::operator-=(High_float data)
{
	*this = *this - data;
	return *this;
}

High_float High_float::operator*=(High_float data)
{
	*this = *this * data;
	return *this;
}

High_float High_float::operator/=(High_float data)
{
	*this = *this / data;
	return *this;
}

High_float High_float::operator%=(High_float data)
{
	*this = *this % data;
	return *this;
}

/*返回内部对象*/
High_int& High_float::Integer()
{
	return this->integer;
}

High_int& High_float::Decimals()
{
	return this->decimals;
}

bool& High_float::Sign()
{
	return this->sign;
}

int64_t& High_float::Decimals_leading_zero()
{
	return this->decimals_leading_zero;
}

void High_float::AlignToSuitableSize(High_float& data)
{
	if (this->decimals == High_int_NULL || data.decimals == High_int_NULL)
		return;
	int64_t count = (this->decimals.Data().size() + this->decimals_leading_zero) - 
		(data.decimals.Data().size() + data.decimals_leading_zero);
	if (count >= 0)
		for (int64_t i = 0; i < count; i++)
			data.decimals.Data().push_back(0);
	else
		data.AlignToSuitableSize(*this);
	return;
}

void High_float::Shrink_to_fit_of_decimals()
{
	while (this->decimals.Data().at(this->decimals.Data().size() - 1) == 0 && this->decimals.Data().size() > 1)
	{
		this->decimals.Data().pop_back();
	}

	while (true)
	{
		if (this->decimals.Data().at(0) == 0 && this->decimals.Data().size() > 1)
		{
			this->decimals.Data().erase(this->decimals.Data().begin());
			++this->decimals_leading_zero;
		}
		else
			break;
	}
	if (this->decimals == High_int_NULL)
		this->decimals_leading_zero = 0;
	return;
}
