#pragma once
#include "polynomial.h"
#include "fraction.h"
#include "mathematical_expression.h"

#define __A_FACTOR_BEGIN this->a.list.at(i).GetFactor().begin()
#define __A_FACTOR_END this->a.list.at(i).GetFactor().end()
#define __B_FACTOR_BEGIN this->b.list.at(i).GetFactor().begin()
#define __B_FACTOR_END this->b.list.at(i).GetFactor().end()

//字母因数的迭代器
typedef std::map<sint8, Fraction<sint64>>::iterator FACTOR;

template<>
class Fraction <Polynomial>
{
public:
	Polynomial a; //分子
	Polynomial b; //分母
	
public:
	Fraction()
	{
		this->b.Input("1");
	}
	Fraction(Polynomial a, Polynomial b)
	{
		this->a = a;
		this->b = b;
		this->StandardizationDisposal();
	}
	Fraction(std::string a, std::string b)
	{
		this->a.Input(a);
		this->b.Input(b);
		this->StandardizationDisposal();
	}
	Fraction(std::string value)
	{
		this->Input(value);
	}
	Fraction(Polynomial value)
	{
		this->b.Input("1");
		this->a = value;
	}

public:
	Fraction<Polynomial> operator+(Fraction<Polynomial> b)
	{
		Fraction<Polynomial> result;
		result.b = this->b * b.b;
		result.a = (this->a * b.b) + (b.a * this->b);
		result.StandardizationDisposal();
		return result;
	}
	Fraction<Polynomial> operator-(Fraction<Polynomial> b)
	{
		Fraction<Polynomial> result;
		result.b = this->b * b.b;
		result.a = (this->a * b.b) - (b.a * this->b);
		result.StandardizationDisposal();
		return result;
	}
	Fraction<Polynomial> operator*(Fraction<Polynomial> b)
	{
		Fraction<Polynomial> result;
		result.b = this->b * b.b;
		result.a = this->a * b.a;
		result.StandardizationDisposal();
		return result;
	}
	Fraction<Polynomial> operator/(Fraction<Polynomial> b)
	{
		Fraction<Polynomial> temp;
		temp.a = b.b;
		temp.b = b.a;
		return *this * temp;
	}
public:
	Fraction<Polynomial> operator+=(Fraction<Polynomial> b)
	{
		*this = *this + b;
		return *this;
	}
	Fraction<Polynomial> operator-=(Fraction<Polynomial> b)
	{
		*this = *this - b;
		return *this;
	}
	Fraction<Polynomial> operator*=(Fraction<Polynomial> b)
	{
		*this = *this * b;
		return *this;
	}
	Fraction<Polynomial> operator/=(Fraction<Polynomial> b)
	{
		*this = *this / b;
		return *this;
	}

public:
	/*标准输入格式*/
	void SetValue(std::string value)
	{
		std::string temp;

		/*格式处理*/
		value = DeleteCircumjacentParentheses(value);
		value = DeleteTheBlank(value);

		sint64 location = -1;

		/*寻找一个不在括号内的'/'符号*/
		for (suint64 i = 0; i < value.size(); i++)
		{
			location = value.find('/', i);
			if (!CharacterInParentheses(value, location))
				break;
			i = location;
		}

		/*处理分子*/
		temp = value.substr(0, location);
		temp = DeleteCircumjacentParentheses(temp);
		this->a.Input(temp);

		//清空内容
		temp.clear();

		//处理分母
		temp = value.substr(location + 1, value.size());
		temp = DeleteCircumjacentParentheses(temp);
		this->b.Input(temp);

		this->StandardizationDisposal();
	}
	/*非标准输入格式*/
	void Input(std::string value)
	{
		/*格式处理*/
		value = DeleteCircumjacentParentheses(value);
		value = DeleteTheBlank(value);

		//如果整个结构是多项式
		//则判定为结构：a或者(a)
		if (Polynomial::IsValid(value))
		{
			this->a.Input(value);
			this->b.Input("1");
			return;
		}

		sint64 location = -1;

		/*寻找一个不在括号内的'/'符号*/
		for (suint64 i = 0; i < value.size(); i++)
		{
			location = value.find('/', i);
			//如果找到了一个不在括号内的'/'符号
			//判定为情况：(a) / (b)
			if (!CharacterInParentheses(value, location))
			{
				this->SetValue(value);
				return;
			}
			i = location;
		}

		//如果没找到一个不在括号内的'/'符号
		//判定为情况：a / b

		std::string temp;
		location = value.rfind('/');

		//处理分子
		temp = value.substr(0, location);
		this->a.Input(temp);

		temp.clear();

		//处理分母
		temp = value.substr(location + 1, value.size());
		this->b.Input(temp);

		this->StandardizationDisposal();
	}

public:
	/*标准输出格式*/
	std::string GetValue()
	{
		std::string result =
			"(" + this->a.Out() + ") / "
			+ "(" + this->b.Out() + ")";
		return result;
	}
	/*非标准输出格式*/
	std::string Out()
	{
		//如果分母处为1
		if (this->b == (Polynomial)"(1/1)")
		{
			return this->a.Out();
		}

		//如果分子和分母都是单项式，并且系数部分都没有'/'符号
		if(this->a.IsMonomial() && this->b.IsMonomial()
			&& this->a.list.at(0).GetCoefficient().b == 1
			&& this->b.list.at(0).GetCoefficient().b == 1)
		{
			return this->a.Out() + " / " + this->b.Out();
		}

		return this->GetValue();
	}

public:
	/*标准化处理
	* 例如：( (1/2)a ) / ( (2/1)b ) -->  a / b
	* 例如：a / b   ----->    a / b
	*/
	void StandardizationDisposal()
	{
		//满足因式分解的必要条件
		if (IsParentPolynomial(this->a, this->b) &&
			this->b.GetDegree().GetDegree() > Fraction<sint64>(0, 1) &&
			this->a.GetDegree().GetDegree() >= this->b.GetDegree().GetDegree())
		{
			Polynomial temp_number = this->a / this->b;
			//如果分子可以整除分母
			if (this->a - (temp_number * this->b) == Polynomial())
			{
				this->a = temp_number;
				this->b.Input("1");
			}
		}

		Monomial temp(Rational_number(1, 1));

		//遍历所有单项式
		for (suint64 i = 0; i < this->a.list.size(); i++)
		{
		a_retry:
			//遍历当前单项式的所有字母因数
			for (FACTOR j = __A_FACTOR_BEGIN; j != __A_FACTOR_END; j++)
			{
				//如果字母次数小于0
				if (j->second < Fraction<sint64>())
				{
					temp.Clear_factor();
					temp.Push(j->first, Rational_number() - j->second);

					this->a *= temp;
					this->b *= temp;

					//重新计算之后需要重新遍历一遍迭代器
					goto a_retry;
				}
			}
		}

		//遍历所有单项式
		for (suint64 i = 0; i < this->b.list.size(); i++)
		{
		b_retry:
			//遍历当前单项式的所有字母因数
			for (FACTOR j = __B_FACTOR_BEGIN; j != __B_FACTOR_END; j++)
			{
				//如果字母次数小于0
				if (j->second < Fraction<sint64>())
				{
					temp.Clear_factor();
					temp.Push(j->first, Rational_number() - j->second);

					this->a *= temp;
					this->b *= temp;

					//重新计算之后需要重新遍历一遍迭代器
					goto b_retry;
				}
			}
		}

		/*化简*/
		temp = this->a.CommonFactor(this->b);
		this->a /= temp;
		this->b /= temp;
	}
};

/* Rational_function <=> Fraction<Polynomial>*/
typedef Fraction<Polynomial> Rational_function;
