#pragma once
#include <string>
#include <map>
#include <set>
#include "..\misinttypes\saint.h"
#include "fraction.h"
#include "seek_math.h"
/*单项式*/
class Monomial
{
	/*member variable*/
private:
	//数字系数
	Fraction<sint64> coefficient;
	//字母因数/每个字母的指数
	std::map<sint8, Fraction<sint64>> factor;
private:
	//是否处于错误状态
	bool error;

	/*member function*/
public: //构造函数
	Monomial();
	Monomial(std::string std_value);
	Monomial(Fraction<sint64> coefficient,
		std::map<sint8, Fraction<sint64>> factor = std::map<sint8, Fraction<sint64>>());
public:
	Fraction<sint64> GetDegree(); //获得此单项式的次数
	Fraction<sint64> GetCoefficient(); //获得此单项式的系数
	std::set<sint8> GetMonomialSetFactor(); //获得次单项式的字母集合
	std::map<sint8, Fraction<sint64>>& GetFactor(); //获得此单项式的字母因数
public:
	void SetCoefficient(sint64 a, sint64 b); //设置系数
	void SetCoefficientA(sint64 a); //设置系数a
	void SetCoefficientB(sint64 b); //设置系数b
	void Push(sint8 factor, Fraction<sint64> exponent); //添加字母因数
	bool Substitute(sint8 character, Monomial val); //代入，将某个字母替换为某个单项式
	void DelOne(); //删除多余的“1”
	void Clear_factor();
public:
	bool SimilarItems(Monomial b); //判断是否为同类项
	bool IsNumber(); //判断是否为“纯数字”
	static bool IsValid(std::string val); //判断字符串是否满足单项式语法

/*标准输入*/
public:
	void SetValue(Monomial value);
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
	/*非标准字符串输出*/
	std::string Out();
public:
	bool operator==(Monomial b);
	bool operator!=(Monomial b);
public:
	Monomial operator+(Monomial b);
	Monomial operator-(Monomial b);
	Monomial operator*(Monomial b);
	Monomial operator/(Monomial b);
public:
	/*	特殊情况下可将Monomial转成sint64类型
	 *	贴心提示：如果你看到Visual Studio 2022提示您
	  “未找到operator Fraction<sint64>的函数定义” 
	  请不要惊慌，这一定是Visual Studio 2022的bug~~~
	*/
	operator Fraction<sint64>();
public:
	bool IsError(); //判断此单项式是否为错误状态
};

/// <summary>
/// 获取单项式a的字母集合的元素个数
/// </summary>
/// <param name=""></param>
/// <returns></returns>
sint64 GetLetterSize(Monomial& a);

//计算两个单项式的字母集合的交集
std::set<sint8> CommonFactor(Monomial a, Monomial b);
