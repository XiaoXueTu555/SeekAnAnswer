#pragma once
#include "..\misinttypes\saint.h"
#include "..\..\High\Public\High_int.h"
#include <iostream>
/*��ʽ*/
template <class T>
class Fraction
{
public:
	T a; //����
	T b; //��ĸ
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
		Ϊ��֤��ʽa��b�ķ��ӷ�ĸ�ǻ��ʵģ�
		���������Ҫ���л���ŷ������㷨������
		*/
		this->Simplification();
		b.Simplification();
		//�ж�a��b�ķ��ӷ�ĸ�����Ƿ����
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
	/*ŷ������㷨*/
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
	//����
	void Simplification()
	{
		sint64 temp = this->gcd(this->a, this->b);
		this->a /= temp;
		this->b /= temp;
	}

/*�ַ�������*/
public:
	/*��׼�ַ�������*/
	void SetValue(std::string std_value)
	{
		std::string container; //����
		suint64 i = 0;
		for (i; std_value.at(i) != '/'; i++)
		{
			container.push_back(std_value.at(i));
		}
		//��container�еġ����֡�ͨ��High_int��ת������ת��sint64����
		this->a = (sint64)(High_int)container;
		container.clear(), ++i;

		for (i; i < std_value.size(); i++)
		{
			container.push_back(std_value.at(i));
		}
		//��container�еġ����֡�ͨ��High_int��ת������ת��sint64����
		this->b = (sint64)(High_int)container;

		this->Simplification();
	}
	/*�Ǳ�׼�ַ�������*/
	void Input(std::string value)
	{
		std::string container; //����
		suint64 i = 0; //��ǰ��ȡ������λ

		//��ȡ��ʽ�ķ���λ����
		for (i; i < value.size(); i++)
		{
			//�����ȡ�����ţ����˳�ѭ��
			if (value.at(i) == '/') break;
			else container.push_back(value.at(i));
		}

		//��ֵ����λ
		this->a = (sint64)(High_int)container;

		//�����ȡ��ĩβ���򷵻�
		if (i == value.size())
		{
			this->b = 1;
			return;
		}

		container.clear(), ++i; //�������; Խ��'/'λ

		//��ȡ��ĸλ
		for (i; i < value.size(); i++)
		{
			container.push_back(value.at(i));
		}

		//��ֵ��ĸλ
		this->b = (sint64)(High_int)container;

		this->Simplification();
	}

/*�ַ������*/
public:
	/*��׼�ַ������*/
	std::string GetValue()
	{
		std::string value;
		value += (std::string)(High_int)this->a;
		value.push_back('/');
		value += (std::string)(High_int)this->b;
		return value;
	}
	/*�Ǳ�׼�ַ������*/
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