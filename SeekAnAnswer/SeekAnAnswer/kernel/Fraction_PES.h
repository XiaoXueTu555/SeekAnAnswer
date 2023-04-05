#pragma once
#include "polynomial_exponential_sum.h"

template<>
class Fraction <Polynomial_Exponential_Sum>
{
public:
	Polynomial_Exponential_Sum a;
	Polynomial_Exponential_Sum b;
public:
	std::string Out()
	{
		std::string value;

		if (this->a == Polynomial_Exponential_Sum())
		{
			value += this->a.Out();
			return value;
		}

		if (this->b == Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
			Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1))))
		{
			value += this->a.Out();
			return value;
		}
		else
		{
			if (this->a.list.size() > 1)
			{
				value.push_back('(');
			}

			value += this->a.Out();

			if (this->a.list.size() > 1)
			{
				value.push_back(')');
			}

			value.push_back('\40');
			value.push_back('/');
			value.push_back('\40');
			if (this->b.list.size() > 1)
			{
				value.push_back('(');
			}

			value += this->b.Out();

			if (this->b.list.size() > 1)
			{
				value.push_back(')');
			}
		}
		return value;
	}
};