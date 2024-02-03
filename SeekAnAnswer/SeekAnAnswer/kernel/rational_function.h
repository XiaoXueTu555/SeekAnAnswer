#pragma once
#include "polynomial.h"
#include "fraction.h"
#include "mathematical_expression.h"

#define __A_FACTOR_BEGIN this->a.list.at(i).GetFactor().begin()
#define __A_FACTOR_END this->a.list.at(i).GetFactor().end()
#define __B_FACTOR_BEGIN this->b.list.at(i).GetFactor().begin()
#define __B_FACTOR_END this->b.list.at(i).GetFactor().end()

//��ĸ�����ĵ�����
typedef std::map<sint8, Fraction<sint64>>::iterator FACTOR;

template<>
class Fraction <Polynomial>
{
public:
	Polynomial a; //����
	Polynomial b; //��ĸ
	
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
	/*��׼�����ʽ*/
	void SetValue(std::string value)
	{
		std::string temp;

		/*��ʽ����*/
		value = DeleteCircumjacentParentheses(value);
		value = DeleteTheBlank(value);

		sint64 location = -1;

		/*Ѱ��һ�����������ڵ�'/'����*/
		for (suint64 i = 0; i < value.size(); i++)
		{
			location = value.find('/', i);
			if (!CharacterInParentheses(value, location))
				break;
			i = location;
		}

		/*�������*/
		temp = value.substr(0, location);
		temp = DeleteCircumjacentParentheses(temp);
		this->a.Input(temp);

		//�������
		temp.clear();

		//�����ĸ
		temp = value.substr(location + 1, value.size());
		temp = DeleteCircumjacentParentheses(temp);
		this->b.Input(temp);

		this->StandardizationDisposal();
	}
	/*�Ǳ�׼�����ʽ*/
	void Input(std::string value)
	{
		/*��ʽ����*/
		value = DeleteCircumjacentParentheses(value);
		value = DeleteTheBlank(value);

		//��������ṹ�Ƕ���ʽ
		//���ж�Ϊ�ṹ��a����(a)
		if (Polynomial::IsValid(value))
		{
			this->a.Input(value);
			this->b.Input("1");
			return;
		}

		sint64 location = -1;

		/*Ѱ��һ�����������ڵ�'/'����*/
		for (suint64 i = 0; i < value.size(); i++)
		{
			location = value.find('/', i);
			//����ҵ���һ�����������ڵ�'/'����
			//�ж�Ϊ�����(a) / (b)
			if (!CharacterInParentheses(value, location))
			{
				this->SetValue(value);
				return;
			}
			i = location;
		}

		//���û�ҵ�һ�����������ڵ�'/'����
		//�ж�Ϊ�����a / b

		std::string temp;
		location = value.rfind('/');

		//�������
		temp = value.substr(0, location);
		this->a.Input(temp);

		temp.clear();

		//�����ĸ
		temp = value.substr(location + 1, value.size());
		this->b.Input(temp);

		this->StandardizationDisposal();
	}

public:
	/*��׼�����ʽ*/
	std::string GetValue()
	{
		std::string result =
			"(" + this->a.Out() + ") / "
			+ "(" + this->b.Out() + ")";
		return result;
	}
	/*�Ǳ�׼�����ʽ*/
	std::string Out()
	{
		//�����ĸ��Ϊ1
		if (this->b == (Polynomial)"(1/1)")
		{
			return this->a.Out();
		}

		//������Ӻͷ�ĸ���ǵ���ʽ������ϵ�����ֶ�û��'/'����
		if(this->a.IsMonomial() && this->b.IsMonomial()
			&& this->a.list.at(0).GetCoefficient().b == 1
			&& this->b.list.at(0).GetCoefficient().b == 1)
		{
			return this->a.Out() + " / " + this->b.Out();
		}

		return this->GetValue();
	}

public:
	/*��׼������
	* ���磺( (1/2)a ) / ( (2/1)b ) -->  a / b
	* ���磺a / b   ----->    a / b
	*/
	void StandardizationDisposal()
	{
		//������ʽ�ֽ�ı�Ҫ����
		if (IsParentPolynomial(this->a, this->b) &&
			this->b.GetDegree().GetDegree() > Fraction<sint64>(0, 1) &&
			this->a.GetDegree().GetDegree() >= this->b.GetDegree().GetDegree())
		{
			Polynomial temp_number = this->a / this->b;
			//������ӿ���������ĸ
			if (this->a - (temp_number * this->b) == Polynomial())
			{
				this->a = temp_number;
				this->b.Input("1");
			}
		}

		Monomial temp(Rational_number(1, 1));

		//�������е���ʽ
		for (suint64 i = 0; i < this->a.list.size(); i++)
		{
		a_retry:
			//������ǰ����ʽ��������ĸ����
			for (FACTOR j = __A_FACTOR_BEGIN; j != __A_FACTOR_END; j++)
			{
				//�����ĸ����С��0
				if (j->second < Fraction<sint64>())
				{
					temp.Clear_factor();
					temp.Push(j->first, Rational_number() - j->second);

					this->a *= temp;
					this->b *= temp;

					//���¼���֮����Ҫ���±���һ�������
					goto a_retry;
				}
			}
		}

		//�������е���ʽ
		for (suint64 i = 0; i < this->b.list.size(); i++)
		{
		b_retry:
			//������ǰ����ʽ��������ĸ����
			for (FACTOR j = __B_FACTOR_BEGIN; j != __B_FACTOR_END; j++)
			{
				//�����ĸ����С��0
				if (j->second < Fraction<sint64>())
				{
					temp.Clear_factor();
					temp.Push(j->first, Rational_number() - j->second);

					this->a *= temp;
					this->b *= temp;

					//���¼���֮����Ҫ���±���һ�������
					goto b_retry;
				}
			}
		}

		/*����*/
		temp = this->a.CommonFactor(this->b);
		this->a /= temp;
		this->b /= temp;
	}
};

/* Rational_function <=> Fraction<Polynomial>*/
typedef Fraction<Polynomial> Rational_function;
