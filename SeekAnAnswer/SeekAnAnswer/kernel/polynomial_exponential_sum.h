#pragma once
#include <vector>
#include "polynomial_exponential.h"
class Polynomial_Exponential_Sum
{
	/*member variable*/
public:
	std::vector<Polynomial_Exponential> list;

	/*member function*/
public:
	Polynomial_Exponential_Sum();
	Polynomial_Exponential_Sum(Polynomial_Exponential val);
public:
	//�ϲ�ͬ����
	void Unite_like_terms();
public:
	void Split(); //���ᡱϵ��
	void Merge(); //���ϲ���ϵ��
public:
	//ȥ������ʽ�е�0
	void DeleteZero();
public:
	/*���һ������ʽָ��*/
	void Push(Polynomial_Exponential val);


	/*�Ǳ�׼���*/
public:
	/*�Ǳ�׼�ַ������*/
	std::string Out();

public:
	bool IsPolynomial();
	bool IsNumber();
	bool IsPolynomial_Exponential();
public:
	operator Polynomial();
	operator Polynomial_Exponential();

public:
	bool operator==(Polynomial_Exponential_Sum val);
	bool operator!=(Polynomial_Exponential_Sum val);
};
