#pragma once
#include "polynomial.h"
#include <vector>
class Polynomial_Exponential
{
	/*member variable*/
public:
	//数字系数
	Fraction<sint64> coefficient;
	//底数（多项式）
	Polynomial number;
	//指数
	Fraction<sint64> exponential;
public:
	bool error;

	/*member function*/
public:
	Polynomial_Exponential();
	Polynomial_Exponential(Polynomial value);
	Polynomial_Exponential(Fraction<sint64> coefficient, Polynomial number, Fraction<sint64> exponential);
public:
	//化简，将底数中的系数提到数字系数中
	void simplify();
	//合并，将系数合并到底数中
	void merge();

public:
	static bool IsValid(std::string value);

/*标准输入*/
public:
	/*标准字符串输入*/
	void SetValue(std::string std_value);
	void SetValue(Polynomial value);
public:
	/*非标准字符串输入*/
	void Input(std::string value);

/*标准输出*/
public:
	/*标准字符串输出*/
	std::string GetValue();
/*非标准输出*/
public:
	/*非标准字符串输出*/
	std::string Out();

public:
	//判断是否可以转成polynomial类型
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
