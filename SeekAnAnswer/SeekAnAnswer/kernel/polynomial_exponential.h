#pragma once
#include "polynomial.h"
#include <vector>
class Polynomial_Exponential
{
	/*member variable*/
public:
	//����ϵ��
	Fraction<sint64> coefficient;
	//����������ʽ��
	Polynomial number;
	//ָ��
	Fraction<sint64> exponential;
public:
	bool error;

	/*member function*/
public:
	Polynomial_Exponential();
	Polynomial_Exponential(Polynomial value);
	Polynomial_Exponential(Fraction<sint64> coefficient, Polynomial number, Fraction<sint64> exponential);
public:
	//���򣬽������е�ϵ���ᵽ����ϵ����
	void simplify();
	//�ϲ�����ϵ���ϲ���������
	void merge();

public:
	static bool IsValid(std::string value);

/*��׼����*/
public:
	/*��׼�ַ�������*/
	void SetValue(std::string std_value);
	void SetValue(Polynomial value);
public:
	/*�Ǳ�׼�ַ�������*/
	void Input(std::string value);

/*��׼���*/
public:
	/*��׼�ַ������*/
	std::string GetValue();
/*�Ǳ�׼���*/
public:
	/*�Ǳ�׼�ַ������*/
	std::string Out();

public:
	//�ж��Ƿ����ת��polynomial����
	bool IsPolynomial();
	bool IsNumber();
public:
	operator Polynomial();
public:
	Polynomial_Exponential operator+(Polynomial_Exponential val);
	Polynomial_Exponential operator-(Polynomial_Exponential val);
	Polynomial_Exponential operator*(Polynomial_Exponential val);
	Polynomial_Exponential operator/(Polynomial_Exponential val);
	Polynomial_Exponential operator%(Polynomial_Exponential val);
public:
	bool operator==(Polynomial_Exponential val);
	bool operator!=(Polynomial_Exponential val);
public:
	bool Error();
};
