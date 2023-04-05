#pragma once
#include <vector>
#include "polynomial.h"
#include "polynomial_exponential.h"
#include "polynomial_exponential_sum.h"
#include "fraction.h"
#include "Fraction_PES.h"
#include "../misinttypes/saint.h"
class Equation
{
public:
	//未知数的系数
	std::vector<Polynomial> coefficient_left;
	std::vector<Polynomial> coefficient_right;
	//未知数的指数
	std::vector<Fraction<sint64>> exponent_left;
	std::vector<Fraction<sint64>> exponent_right;
public:
	void SetValue(std::string value); //标准输入
	void Input(std::string value); //非标准输入
	std::string GetValue();
	std::string Out();
public:
	void DeleteZero();
public:
	Fraction<sint64> GetTheHighestDegreeTermOfTheUnknown();
	sint64 FindDegree(Fraction<sint64> degree);
public:
	//移项
	void ShiftItem();
	//合并同类项
	void Unite_like_terms();
public:
	//求解一元一次方程
	Fraction<Polynomial> linear_equation_with_one_unknown();
	//求解一元二次方程
	std::vector<Fraction<Polynomial_Exponential_Sum>> quadratic_equation_in_one_unknown();
};

//将Fraction<Polynomial>类型转换成字符串
std::string Out(Fraction<Polynomial> val);
