#pragma once
#include "..\misinttypes\saint.h"
#include "..\..\High\Public\High_int.h"
#include "mathematical_expression.h"
#include <iostream>
/*分式*/
template <class T>
class Fraction
{
public:
	T a; //分子
	T b; //分母
public:
	Fraction<T> operator=(Fraction<T> b)
	{
		this->a = b.a;
		this->b = b.b;
		return *this;
	}
public:
	Fraction<T> operator+(Fraction<T> b)
	{
		Fraction<T> result();
		result.b = this->b * b.b;
		result.a = (this->a * b.b) + (b.a * this->b);
		return result;
	}
	Fraction<T> operator-(Fraction<T> b)
	{
		Fraction<T> result();
		result.b = this->b * b.b;
		result.a = (this->a * b.b) - (b.a * this->b);
		return result;
	}
	Fraction<T> operator*(Fraction<T> b)
	{
		Fraction<T> result();
		result.b = this->b * b.b;
		result.a = this->a * b.a;
		return result;
	}
	Fraction<T> operator/(Fraction<T> b)
	{
		Fraction<T> temp();
		temp.a = b.b;
		temp.b = b.a;
		return *this * temp;
	}
};

template <>
class Fraction <sint64>
{
public:
	sint64 a;
	sint64 b;
public:
	Fraction(std::string value)
	{
		this->Input(value);
	}
	Fraction(sint64 a, sint64 b)
	{
		this->a = a;
		this->b = b;
		this->Simplification();
	}
	Fraction()
	{
		this->a = 0;
		this->b = 1;
	}
public:
	Fraction<sint64> operator=(sint64 value)
	{
		this->a = value;
		this-> b = 1;
	}
public:
	bool operator==(Fraction<sint64> b)
	{
		/*
		为保证分式a和b的分子分母是互质的，
		因此这里需要进行化简（欧几里得算法）操作
		*/
		this->Simplification();
		b.Simplification();
		//判断a和b的分子分母部分是否相等
		if (this->a == b.a && this->b == b.b)
			return true;
		else
			return false;
	}
	bool operator!=(Fraction<sint64> b)
	{
		if (*this == b) return false;
		return true;
	}
public:
	bool operator>(Fraction<sint64> b)
	{
		if (this->a * b.b > b.a * this->b) return true;
		else return false;
	}
	bool operator>=(Fraction<sint64> b) 
	{
		if (this->a * b.b >= b.a * this->b) return true;
		else return false;
	}
	bool operator<(Fraction<sint64> b)
	{
		if (this->a * b.b < b.a * this->b) return true;
		else return false;
	}
	bool operator<=(Fraction<sint64> b)
	{
		if (this->a * b.b <= b.a * this->b) return true;
		else return false;
	}
public:
	Fraction<sint64> operator+(Fraction<sint64> b)
	{
		Fraction<sint64> result;
		result.b = this->b * b.b;
		result.a = (this->a * b.b) + (b.a * this->b);
		result.Simplification();
		return result;
	}
	Fraction<sint64> operator-(Fraction<sint64> b)
	{
		Fraction<sint64> result;
		result.b = this->b * b.b;
		result.a = (this->a * b.b) - (b.a * this->b);
		result.Simplification();
		return result;
	}
	Fraction<sint64> operator*(Fraction<sint64> b)
	{
		Fraction<sint64> result;
		result.b = this->b * b.b;
		result.a = this->a * b.a;
		result.Simplification();
		return result;
	}
	Fraction<sint64> operator/(Fraction<sint64> b)
	{
		Fraction<sint64> temp;
		temp.a = b.b;
		temp.b = b.a;
		return *this * temp;
	}
	Fraction<sint64> operator%(Fraction<sint64> b)
	{
		return *this - ((*this / b) * b);
	}
public:
	Fraction<sint64> operator+=(Fraction<sint64> b)
	{
		*this = *this + b;
		return *this;
	}
	Fraction<sint64> operator-=(Fraction<sint64> b)
	{
		*this = *this - b;
		return *this;
	}
	Fraction<sint64> operator*=(Fraction<sint64> b)
	{
		*this = *this * b;
		return *this;
	}
	Fraction<sint64> operator/=(Fraction<sint64> b)
	{
		*this = *this / b;
		return *this;
	}
	Fraction<sint64> operator%=(Fraction<sint64> b)
	{
		*this = *this % b;
		return *this;
	}
public:
	/*欧几里得算法*/
	sint64 gcd(sint64 a, sint64 b)
	{
		sint64 mod = a % b;
		while (mod != 0)
		{
			a = b;
			b = mod;
			mod = a % b;
		}
		return b;
	}
public:
	//化简
	void Simplification()
	{
		//如果分子和分母都是负数，则化简
		if (this->a < 0 && this->b < 0)
		{
			this->a = 0 - this->a;
			this->b = 0 - this->b;
		}
		//如果a与b其中一个为负数，则统一为a为负数，b为正数
		if (this->a * this->b < 0)
		{
			if (this->a > 0) this->a = 0 - this->a;
			this->b = abs(this->b);
		}

		//经过上述处理之后，b必定为正数，因此只需要记录a的正负
		sint64 a_sign = this->a > 0 ? 1 : -1;

		this->a = abs(this->a);

		sint64 temp = this->gcd(this->a, this->b);

		this->a /= temp;
		this->b /= temp;

		this->a *= a_sign;
	}
public:
	//该分式是否为整数
	bool IsInteger()
	{
		this->Simplification();
		if (this->b == 1) return true;
		return false;
	}
	//正确格式检查
	static bool IsValid(std::string val)
	{
		//如果字符串不满足括号语法规则
		if (!ParenthesisSyntax(val)) return false;
		//删除val外围的括号
		val = DeleteCircumjacentParentheses(val);
		//删除val中的所有空格
		val = DeleteTheBlank(val);

		if (val.size() == 0) return false;

		//如果存在‘/’
		if (val.find('/') != std::string::npos)
		{
			//遍历'/'之前的所有字符
			for (suint64 i = 0; i < val.find('/'); i++)
			{
				if (val.at(i) == '-' && i != 0) return false;
				else if (val.at(i) == '-') continue;
				//如果其中一个不是数字字符
				else if (!(val.at(i) >= '0' && val.at(i) <= '9'))
				{
					return false;
				}
			}

			//遍历'/'之前的所有字符
			for (suint64 i = val.find('/') + 1; i < val.size(); i++)
			{
				if (val.at(i) == '-' && i != val.find('/') + 1) return false;
				else if (val.at(i) == '-') continue;
				//如果其中一个不是数字字符
				else if (!(val.at(i) >= '0' && val.at(i) <= '9'))
				{
					return false;
				}
			}
			return true;
		}
		
		//说明没有'/'
		//遍历整个字符串
		for (suint64 i = 0; i < val.size(); i++)
		{
			if (val.at(i) == '-' && i != 0) return false;
			else if (val.at(i) == '-') continue;
			//如果其中一个不是数字字符
			else if (!(val.at(i) >= '0' && val.at(i) <= '9'))
			{
				return false;
			}
		}
		return true;
	}

/*字符串输入*/
public:
	/*标准字符串输入*/
	void SetValue(std::string std_value)
	{
		std::string container; //容器
		suint64 i = 0;
		for (i; std_value.at(i) != '/'; i++)
		{
			container.push_back(std_value.at(i));
		}
		//将container中的“数字”通过High_int的转换函数转成sint64类型
		this->a = (sint64)(High_int)container;
		container.clear(), ++i;

		for (i; i < std_value.size(); i++)
		{
			container.push_back(std_value.at(i));
		}
		//将container中的“数字”通过High_int的转换函数转成sint64类型
		this->b = (sint64)(High_int)container;

		this->Simplification();
	}
	/*非标准字符串输入*/
	void Input(std::string value)
	{
		std::string container; //容器
		suint64 i = 0; //当前读取的数据位

		//读取分式的分子位数据
		for (i; i < value.size(); i++)
		{
			//如果读取到除号，则退出循环
			if (value.at(i) == '/') break;
			else container.push_back(value.at(i));
		}

		//赋值分子位
		this->a = (sint64)(High_int)container;

		//如果读取到末尾，则返回
		if (i == value.size())
		{
			this->b = 1;
			return;
		}

		container.clear(), ++i; //清空容器; 越过'/'位

		//读取分母位
		for (i; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}

		//赋值分母位
		this->b = (sint64)(High_int)container;

		this->Simplification();
	}

/*字符串输出*/
public:
	/*标准字符串输出*/
	std::string GetValue()
	{
		std::string value;
		value += (std::string)(High_int)this->a;
		value.push_back('/');
		value += (std::string)(High_int)this->b;
		return value;
	}
	/*非标准字符串输出*/
	std::string Out()
	{
		std::string value;
		value += (std::string)(High_int)this->a;

		if (this->a == 0) return value;

		if (this->b != 1)
		{
			value.push_back('/');
			value += (std::string)(High_int)this->b;
		}
		return value;
	}
};
