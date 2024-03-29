#pragma once
#include <string>
#include "polynomial_exponential.h"
#include "..\..\High\Public\High_float.h"

//算术表达式
class Arithmetic_Expression
{
	/*member variable*/
public:
	Polynomial_Exponential a;
	Polynomial_Exponential b;
	sint8 _operator;

	/*member function*/
public:
	Arithmetic_Expression();
public:
	void Input(std::string value);
public:
	bool IsValid(std::string value);
public:
	bool IsNumber();
public:
	void Calculate();
public:
	std::string Out();
	//输出近似值
	std::string Approximate_Out();
protected:
	long double Rooting(long double a, sint64 n);
};