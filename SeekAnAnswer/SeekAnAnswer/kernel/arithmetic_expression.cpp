#include <math.h>
#include "arithmetic_expression.h"

void Arithmetic_Expression::Input(std::string value)
{
	std::string container;

	sint64 mul_size = 0;
	sint64 mul_location = -1;

	//ͳ��������ĳ˺�����ŵ�����
	for (suint64 i = 0; i < value.size(); i++)
	{
		if (value.at(i) == '*' && !CharacterInParentheses(value, i))
		{
			++mul_size;
			mul_location = i;
			this->_operator = '*';
		}

		if (value.at(i) == '/' && !CharacterInParentheses(value, i))
		{
			++mul_size;
			mul_location = i;
			this->_operator = '/';
		}
	}

	//���ڳ˺Ż��߳���
	if (mul_size != 0)
	{
		//��ȡa
		for (sint64 i = 0; i < mul_location; i++)
		{
			container.push_back(value.at(i));
		}
		this->a.Input(container);

		container.clear();

		//��ȡb
		for (suint64 i = mul_location + 1; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}
		this->b.Input(container);
		return;
	}

	//Ѱ�Ҳ������Ű�Χ�ļӺ�
	sint64 location = -1;
	for (suint64 i = 0; i < value.size();)
	{
		if (value.find('+', i) != std::string::npos)
		{
			if (!CharacterInParentheses(value, value.find('+', i)))
			{
				location = value.find('+', i++);
			}
			else
				++i;
		}
		else
			break;
	}

	//����ҵ��˲���������ļӺ�
	if (location != -1)
	{
		this->_operator = '+';
		//��ȡa
		for (sint64 i = 0; i < location; i++)
		{
			container.push_back(value.at(i));
		}
		this->a.Input(container);

		container.clear();

		//��ȡb
		for (suint64 i = location + 1; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}
		this->b.Input(container);

		return;
	}

	this->_operator = 0;
	this->a.Input(value);

	return;
}

bool Arithmetic_Expression::IsValid(std::string value)
{
	std::string container;
	sint64 mul_size = 0;
	sint64 mul_location = -1;
	//ͳ��������ĳ˺�����ŵ�����
	for (suint64 i = 0; i < value.size(); i++)
	{
		if (value.at(i) == '*' && !CharacterInParentheses(value, i))
		{
			++mul_size;
			mul_location = i;
		}

		if (value.at(i) == '/' && !CharacterInParentheses(value, i))
		{
			++mul_size;
			mul_location = i;
		}
		
		if (mul_size > 1) 
			return false;
	}

	//���ڳ˺Ż��߳���
	if (mul_size != 0)
	{
		//��ȡa
		for (sint64 i = 0; i < mul_location; i++)
		{
			container.push_back(value.at(i));
		}
		if (!Polynomial_Exponential::IsValid(container))
			return false;

		container.clear();

		//��ȡb
		for (suint64 i = mul_location + 1; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}
		if (!Polynomial_Exponential::IsValid(container))
			return false;

		return true;
	}

	//Ѱ�Ҳ������Ű�Χ�ļӺ�
	sint64 location = -1;
	for (suint64 i = 0; i < value.size();)
	{
		if (value.find('+', i) != std::string::npos)
		{
			if (!CharacterInParentheses(value, value.find('+', i)))
			{
				location = value.find('+', i++);
			}
			else
				++i;
		}
		else
			break;
	}

	//����ҵ��˲���������ļӺ�
	if (location != -1)
	{
		//��ȡa
		for (sint64 i = 0; i < location; i++)
		{
			container.push_back(value.at(i));
		}
		if (!Polynomial_Exponential::IsValid(container))
			return false;

		container.clear();

		//��ȡb
		for (suint64 i = location + 1; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}
		if (!Polynomial_Exponential::IsValid(container))
			return false;

		return true;
	}

	if (!Polynomial_Exponential::IsValid(value))
		return false;

	return true;
}

Arithmetic_Expression::Arithmetic_Expression()
{
	this->_operator = 0;
}

bool Arithmetic_Expression::IsNumber()
{
	if (this->a.IsNumber() && this->b.IsNumber())
	{
		return true;
	}
	return false;
}

std::string Arithmetic_Expression::Out()
{
	std::string result;
	//���κβ�������ֻ��һ��Polynomial_Exponential�ṹ����
	if (this->_operator == 0)
	{
		return this->a.Out();
	}

	result += this->a.Out();

	//���bΪ0
	if (this->b.Out() == "0")
	{
		return result;
	}

	result.push_back('\40');
	result += this->_operator;
	result.push_back('\40');
	result += this->b.Out();

	return result;
}

void Arithmetic_Expression::Calculate()
{
	Polynomial_Exponential result;
	switch (_operator)
	{
	case '+':
	{
		result = a + b;
		if (!result.error)
		{
			this->a = result;
			this->b.Input("0");
			this->_operator = 0;
		}
		break;
	}
	case '*':
	{
		result = a * b;
		if (!result.error)
		{
			this->a = result;
			this->b.Input("0");
			this->_operator = 0;
		}
		break;
	}
	case '/':
	{
		result = a / b;
		if (!result.error)
		{
			this->b = a % b;
			this->a = result;
			this->_operator = '+';
		}
		break;
	}
	default:
		break;
	}

	this->a.merge();
	this->b.merge();

	return;
}

std::string Arithmetic_Expression::Approximate_Out()
{
	std::string result;
	long double a, b;
	long double temp;
	if (this->a.IsNumber() && this->b.IsNumber())
	{
		if (this->a.exponential.b % 2 == 0 
			&& ((Fraction<sint64>)(Monomial)this->a.number) < Fraction<sint64>(0, 1))
		{
			return this->Out();
		}
		if (this->b.exponential.b % 2 == 0 
			&& ((Fraction<sint64>)(Monomial)this->a.number) < Fraction<sint64>(0, 1))
		{
			return this->Out();
		}

		temp = (long double)((Fraction<sint64>)(Monomial)this->a.number).a
			/ (long double)((Fraction<sint64>)(Monomial)this->a.number).b;
		a = this->Rooting(temp, this->a.exponential.b);
		a = pow(a, this->a.exponential.a);
		temp = (long double)this->a.coefficient.a / (long double)this->a.coefficient.b;
		a *= temp;

		if (this->_operator == 0)
		{
			result = (std::string)(High_float)a;
			return result;
		}

		temp = (long double)((Fraction<sint64>)(Monomial)this->b.number).a
			/ (long double)((Fraction<sint64>)(Monomial)this->b.number).b;
		b = Rooting(temp, this->b.exponential.b);
		b = pow(b, this->b.exponential.a);
		temp = (long double)this->b.coefficient.a / (long double)this->b.coefficient.b;
		b *= temp;

		switch (_operator)
		{
		case '+':
		{
			result = (std::string)(High_float)(a + b);
			break;
		}
		case '*':
		{
			result = (std::string)(High_float)(a * b);
			break;
		}
		case '/':
		{
			result = (std::string)(High_float)(a / b);
			break;
		}
		default:
			break;
		}
	}
	return result;
}

long double Arithmetic_Expression::Rooting(long double a, sint64 n)
{
	if (a == 0.0)
	{
		return 0.0f;
	}
	long double guess = 0.5f * a;
	for (int64_t i = 0; i < 10; i++)
	{
		guess = ((a / pow(guess, n - 1)) + (n - 1) * guess) / n;
	}
	return guess;
}
