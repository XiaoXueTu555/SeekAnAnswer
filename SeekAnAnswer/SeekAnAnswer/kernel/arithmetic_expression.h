#pragma once
#include <string>
#include "polynomial_exponential.h"
#include "..\..\High\Public\High_float.h"
#include "..\..\High\Public\High_math.h"

//�������ʽ
class Arithmetic_Expression
{
public:
	Polynomial_Exponential a;
	Polynomial_Exponential b;
	sint8 _operator;
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
	//�������ֵ
	std::string Approximate_Out();
};