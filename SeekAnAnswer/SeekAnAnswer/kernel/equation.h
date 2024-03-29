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
	/*member variable*/
public:
	//未知数的系数
	std::vector<Polynomial> coefficient_left;
	std::vector<Polynomial> coefficient_right;
	//未知数的指数
	std::vector<Fraction<sint64>> exponent_left;
	std::vector<Fraction<sint64>> exponent_right;
public:
	Fraction<Polynomial> root1; //一元一次方程的根
	//一元二次方程的根
	std::vector<Fraction<Polynomial_Exponential_Sum>> root2;

	/*member function*/
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
	//是否为纯数字方程（未知数系数为纯数字）
	bool IsPurelyNumericalEquation();
	//该方程根的数量
	sint64 NumberOfRoot();
public:
	//求解一元一次方程
	void linear_equation_with_one_unknown();
	//求解一元二次方程
	void quadratic_equation_in_one_unknown();
public:
	bool IsValid(std::string val);
};

//判断整数n是否为素数
inline bool IsPrime(sint64 n) {
	if (n <= 3) {
		return n > 1;
	}
	// 只有6x-1和6x+1的数才有可能是质数
	if (n % 6 != 1 && n % 6 != 5) {
		return false;
	}
	// 判断这些数能否被小于sqrt(n)的奇数整除
	sint64 sqrt_n = (sint64)sqrt(n);
	for (sint64 i = 5; i <= sqrt_n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}
	return true;
}

//生产质数
class Prime
{
public:
	static std::vector<sint64> nk;
	sint64 n;
	suint64 i;
public:
	Prime() 
	{ 
		i = 0;
		n = 2; 
	}
	void Next()
	{
		if (++i < nk.size())
		{
			n = nk.at(i);
			return;
		}

		do
		{
			/*由于质数的分布，可能存在无穷对孪生素数，
			所以只能两个两个数字递增，使其接近下一个质数
			具体可以了解孪生素数猜想*/
			n += 2;
		} while (!IsPrime(n));
		return;
	}
};

//将Fraction<Polynomial>类型转换成字符串
std::string Out(Fraction<Polynomial> val);

/// <summary>
/// 化简任意正整数a的算术平方根的算法
/// 该算法由-- 安博 提出 特此鸣谢
/// 注意:请确保输入的参数为正整数
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
Polynomial_Exponential Simplest_radical_AnBo(Polynomial_Exponential val);

//化简任意纯数字根式的算法
Polynomial_Exponential Simplest_radical(Polynomial_Exponential val);
