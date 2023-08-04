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
	//合并同类项
	void Unite_like_terms();
public:
	void Split(); //“提”系数
	void Merge(); //“合并”系数
public:
	//去除多项式中的0
	void DeleteZero();
public:
	/*添加一个多项式指数*/
	void Push(Polynomial_Exponential val);


	/*非标准输出*/
public:
	/*非标准字符串输出*/
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
