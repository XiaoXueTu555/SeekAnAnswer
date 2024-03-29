#pragma once
#include <vector>
#include <map>
#include <set>
#include "monomial.h" //单项式

/*多项式类*/
class Polynomial
{
	/*member variable*/
public:
	std::vector<Monomial> list;//多项式
private:
	bool error;

	/*member function*/
public:
	Polynomial(std::string std_value);
	Polynomial(Monomial val);
	Polynomial();
/*标准输入*/
public:
	void SetValue(Polynomial value);
	/*标准字符串输入*/
	void SetValue(std::string std_value);

/*非标准输入*/
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
	//合并同类项
	void Unite_like_terms();
public:
	//去除多项式中的0
	void DeleteZero();
public:
	/*添加一个单项式*/
	void Push(Monomial val);
public:
	//返回最高次项的单项式
	Monomial GetDegree();
	//返回最高次项的次数
	Fraction<sint64> GetTheDegree();
	//返回最高次数的单项式在list表中的下标
	suint64 GetLocationOfDegree();
	//返回特定字符的最小次数
	Fraction<sint64> GetDegreeOfMixOfchar(sint8 a);
public:
	//求与val多项式的公因式
	Monomial CommonFactor(Polynomial val);
	//求与val多项式的最小公倍数
	Fraction<sint64> LeastCommonMultiple_of_coe(Polynomial val);
public:
	/*将最高的单项式的次数的单项式移动到首位，
	且保证第一项是所有单项式中字母集合的元素最少的单项式*/
	void Move();
public:
	//清空所有单项式
	void Clear();
public:
	bool Substitute(sint8 character, Polynomial val);
public:
	bool operator==(Polynomial val);
	bool operator!=(Polynomial val);
public:
	Polynomial operator=(Monomial val);
	Polynomial operator=(Polynomial val);
public:
	Polynomial operator+(Monomial val);
	Polynomial operator-(Monomial val);
	Polynomial operator*(Monomial val);
	Polynomial operator/(Monomial val);
public:
	Polynomial operator+(Polynomial val);
	Polynomial operator-(Polynomial val);
	Polynomial operator*(Polynomial val);
	Polynomial operator/(Polynomial val);
	Polynomial operator%(Polynomial val);
public:
	Polynomial operator+=(Polynomial val);
	Polynomial operator-=(Polynomial val);
	Polynomial operator*=(Polynomial val);
	Polynomial operator/=(Polynomial val);
	Polynomial operator%=(Polynomial val);
public:
	//转换为单项式，请转换前使用Polynomial::monomial函数判断是否为单项式
	operator Monomial();
public:
	//判断是否为单项式
	bool IsMonomial();
	//判断是否为纯数字
	bool IsNumber();
	//判断字符串是否满足多项式语法
	static bool IsValid(std::string val); 
public:
	bool IsError();
};

/// <summary>
/// 判断多项式a是否为多项式b的父多项式
/// </summary>
/// <param name="a"> : 多项式a</param>
/// <param name="b"> : 多项式b</param>
/// <returns> true代表是，false代表不是 </returns>
bool IsParentPolynomial(Polynomial a, Polynomial b);

/// <summary>
/// 判断多项式a与多项式b的字母集合是否相等。
/// 判断多项式a与多项式b是否互为父多项式（子多项式）
/// </summary>
/// <param name="a"> : 多项式a </param>
/// <param name="b"> : 多项式b </param>
/// <returns> true代表是，false代表不是 </returns>
bool IsLetterSetEquality(Polynomial a, Polynomial b);
