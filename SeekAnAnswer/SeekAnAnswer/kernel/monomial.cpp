#include "monomial.h"
#include "../../High/Public/High_int.h"
#include "../../High/Public/High_float.h"
#include "../../High/Public/High_io.h"

#include <sstream>
#include <iostream>

//字母因数的迭代器
typedef std::map<sint8, Fraction<sint64>>::iterator FACTOR;

bool operator==(std::map<sint8, Fraction<sint64>> a, std::map<sint8, Fraction<sint64>> b)
{
	if (a.size() != b.size())
	{
		return false;
	}
	std::map<sint8, Fraction<sint64>>::iterator i = a.begin();
	std::map<sint8, Fraction<sint64>>::iterator j = b.begin();
	for (suint64 g = 0; g < a.size(); g++)
	{
		if (i->first != j->first) return false;
		if (i->second != j->second) return false;
	}
	return true;
}

Monomial::Monomial()
{
	this->coefficient.a = 0;
	this->coefficient.b = 1;
	this->error = false;
}

Monomial::Monomial(std::string std_value)
{
	this->SetValue(std_value);
	this->error = false;
}

Monomial::Monomial(Fraction<sint64> coefficient, std::map<sint8, Fraction<sint64>> factor)
{
	this->coefficient = coefficient;
	this->factor = factor;
	this->error = false;
}

Fraction<sint64> Monomial::GetDegree() //获得此单项式的次数
{
	Fraction<sint64> degree(0, 1);
	for (std::map<sint8, Fraction<sint64>>::iterator i = this->factor.begin(); i != this->factor.end(); i++)
	{
		degree += i->second;
	}
	return degree;
}

Fraction<sint64> Monomial::GetCoefficient() //获得此单项式的系数
{
	return this->coefficient;
}

std::map<sint8, Fraction<sint64>>& Monomial::GetFactor() //获得此单项式的字母因数
{
	return this->factor;
}

void Monomial::SetValue(Monomial value) //设置单项式的值
{
	this->coefficient = value.coefficient;
	this->factor = value.factor;
	this->DelOne();
}

void Monomial::SetValue(std::string std_value)
{
	std::string container; //容器
	Fraction<sint64> exponent; //指数(临时容器)
	sint8 letter = 0; //字母系数(临时容器)
	suint64 i = 0;

	/*清空自身的数据*/
	this->coefficient.a = 0;
	this->coefficient.b = 0;
	this->Clear_factor();

	//读取数字系数部分
	while (std_value.at(++i) != ')')
	{
		container.push_back(std_value.at(i));
	}

	//保存读取的数字系数
	this->coefficient.SetValue(container);

	container.clear();

	//读取字母因数部分
	for (i; i < std_value.size(); i++)
	{
		//以'^'为标志读取字母因数
		if (std_value.at(i) == '^')
		{
			letter = std_value.at(i - 1);

			i += 2; //定位到指数里

			//读取字母的指数部分
			while (std_value.at(i) != ')')
			{
				container.push_back(std_value.at(i));
				++i;
			}

			//保存读取的指数部分
			exponent.SetValue(container);

			//追加字母因数
			this->Push(letter, exponent);

			container.clear();
		}
	}

	this->DelOne();
	this->error = false;
}

void Monomial::SetCoefficient(sint64 a, sint64 b)
{
	this->coefficient.a = a;
	this->coefficient.b = b;
}

void Monomial::SetCoefficientA(sint64 a)
{
	this->coefficient.a = a;
}

void Monomial::SetCoefficientB(sint64 b)
{
	this->coefficient.b = b;
}

void Monomial::Push(sint8 factor, Fraction<sint64> exponent)
{
	Monomial temp;
	temp.coefficient.a = 1;
	temp.coefficient.b = 1;
	temp.factor.insert(std::pair<sint8, Fraction<sint64>>(factor, exponent));
	*this = *this * temp;
}

void Monomial::DelOne()
{
	for (FACTOR i = this->factor.begin(); i != this->factor.end();)
	{
		//如果指数部分等于0，则删除该字母项
		if (i->second.a == 0)
		{
			i = this->factor.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void Monomial::Clear_factor()
{
	this->factor.clear();
}

bool Monomial::SimilarItems(Monomial b) //判断是否为同类项
{
	if (this->factor.size() != b.factor.size())
	{
		return false;
	}
	FACTOR j = b.factor.begin();
	for (FACTOR i = this->factor.begin(); i != this->factor.end(); i++)
	{
		if (i->first != j->first)
		{
			return false;
		}
		if (i->second != j->second)
		{
			return false;
		}
		j++;
	}
	return true;
}

bool Monomial::IsNumber()
{
	if (this->factor.size() == 0) return true;
	return false;
}

bool Monomial::IsValid(std::string val)
{
	//如果字符串不满足括号语法规则
	if (!ParenthesisSyntax(val)) return false;
	//删除val外围的括号
	val = DeleteCircumjacentParentheses(val);

	if (val.size() == 0) return false;

	//排除不满足该字符集合的字符串
	for (suint64 i = 0; i < val.size(); i++)
	{
		if (!(val.at(i) >= '0' && val.at(i) <= '9' ||
			val.at(i) >= 'a' && val.at(i) <= 'z' ||
			val.at(i) >= 'A' && val.at(i) <= 'Z' ||
			val.at(i) == '^' ||
			val.at(i) == '(' || val.at(i) == ')' ||
			val.at(i) == '/' || val.at(i) == '-'))
		{
			return false;
		}
	}

	std::string container; //容器

	//获取数字系数部分
	while (!(val.at(0) >= 'a' && val.at(0) <= 'z' ||
		val.at(0) >= 'A' && val.at(0) <= 'Z'))
	{
		container.push_back(val.at(0));
		val.erase(val.begin());
		if (val.size() == 0) break;
	}

	/*经过上述判断，若container的元素数量为0，则必定存在字母因数
	若不为0，可能存在字母因数，也可能不存在*/

	//如果不存在数字系数或者数字系数满足条件则通过，否则不满足单项式
	if (!(container.size() == 0 || Fraction<sint64>::IsValid(container)))
		return false;

	//如果val的元素数量为0，说明不存在字母因数，满足单项式
	if (val.size() == 0) return true;

	container.clear();

	//执行到此处说明存在字母因数

	//遍历检查字母因数部分
	while(val.size() > 0)
	{
		container.push_back(val.at(0));
		val.erase(val.begin());
		//如果存在下一个元素，而且下一个元素不是字母因数
		if (val.size() > 0)
		{
			if (!(val.at(0) >= 'a' && val.at(0) <= 'z' ||
				val.at(0) >= 'A' && val.at(0) <= 'Z'))
				continue;
		}

		//检查点 说明此处val.at(0)是一个字母
		container.erase(container.begin());
		//如果删除字母元素后的长度为0,则该项没问题
		if (container.size() == 0) continue;
		//如果删除字母元素后，存在其他元素，若第一项不是次方符号则不符合单项式
		if (container.at(0) != '^') return false;
		
		//此时剩下的肯定是存在次方符号的情况
		container.erase(container.begin());
		//若删除次方符号后没有元素，则不满足单项式
		if (container.size() == 0) return false;
		//若剩下的项不是数字，则不满足单项式
		if (!(Fraction<sint64>::IsValid(container))) 
			return false;
		container.clear();
	}

	return true;
}

void Monomial::Input(std::string value)
{
	this->Clear_factor();
	this->coefficient.SetValue("0/1");

	if (value.empty()) return;

	value = DeleteCircumjacentParentheses(value);

	std::string container; //容器
	Fraction<sint64> exponent; //指数(临时容器)
	sint8 letter = 0; //字母系数(临时容器)
	suint64 i = 0;

	//若输入的数据存在数字系数
	if (value.at(0) == '(' 
		|| value.at(0) >= '0' && value.at(0) <= '9' || value.at(0) == '-')
	{
		//如果数字系数部分用小括号括起来
		if (value.at(0) == '(')
		{
			while (value.at(++i) != ')')
			{
				container.push_back(value.at(i));
			}

			if (container == "-") container.push_back('1');

			this->coefficient.Input(container);

			container.clear();

			//如果读取完毕
			if (i + 1 >= value.size()) return;
			else ++i; //定位到字母因数部分
		}
		else
		{
			while (value.at(i) >= '0' && value.at(i) <= '9' 
				|| value.at(i) == '-' || value.at(i) == '/')
			{
				container.push_back(value.at(i));

				if (++i >= value.size()) break;
			}

			if (container == "-") container.push_back('1');

			//保存系数
			this->coefficient.Input(container);

			container.clear();

			//如果读取完毕
			if (i >= value.size()) return;
		}
	}
	else this->coefficient.SetValue("1/1");

	container.clear();

	for (i; i < value.size(); i++)
	{
		if ((value.at(i) >= 'a' && value.at(i) <= 'z') ||
			(value.at(i) >= 'A' && value.at(i) <= 'Z'))
		{
			//保存字母
			letter = value.at(i);

			//默认为"1"
			exponent.SetValue("1/1");

			++i;

			//判断是否可继续读取指数
			if (i < value.size())
			{
				//如果存在指数标志
				if (value.at(i) == '^')
				{
					++i;

					//在范围内安全读取指数
					for (i; i < value.size(); i++)
					{
						if (value.at(i) >= '0' && value.at(i) <= '9'
							|| value.at(i) == '/' || value.at(i) == '-')
						{
							container.push_back(value.at(i));
						}
						else if (value.at(i) >= 'a' && value.at(i) <= 'z')
							break;
					}

					//保存指数
					exponent.Input(container);

					container.clear();
				}

				--i;
			}

			//追加字母因数
			this->Push(letter, exponent);
		}
	}

	this->DelOne();
	this->error = false;
}

std::string Monomial::GetValue()
{
	std::string result;
	result.push_back('(');
	result += this->coefficient.GetValue();
	result.push_back(')');

	for (FACTOR i = this->factor.begin(); i != this->factor.end(); i++)
	{
		result.push_back((sint8)i->first);
		result.push_back('^');
		result.push_back('(');
		result += i->second.GetValue();
		result.push_back(')');
	}

	return result;
}

std::string Monomial::Out()
{
	std::string result;

	//如果单项式等于0
	if (this->coefficient.a == 0)
	{
		result = this->coefficient.Out();
		return result;
	}

	//如果没有字母因数部分
	if (this->factor.size() == 0)
	{
		result += this->coefficient.Out();
		return result;
	}

	//如果数字等于-1
	if (this->coefficient == Fraction<sint64>(-1, 1))
	{
		result.push_back('-');
	}

	//如果数字系数不等于1
	else if (this->coefficient != Fraction<sint64>(1, 1))
	{
		if (this->coefficient.b != 1)
		{
			result.push_back('(');
		}

		result += this->coefficient.Out();

		if (this->coefficient.b != 1)
		{
			result.push_back(')');
		}
	}

	for (FACTOR i = this->factor.begin(); i != this->factor.end(); i++)
	{
		result.push_back((sint8)i->first);
		if (i->second != Fraction<sint64>(1, 1))
		{
			result.push_back('^');
			//说明指数不是整数
			if (i->second.b != 1)
			{
				result.push_back('(');
				result += i->second.Out();
				result.push_back(')');
			}
			else
				result += i->second.Out();
		}
	}

	return result;
}

bool Monomial::operator==(Monomial b)
{
	if (this->coefficient == b.coefficient &&
		this->coefficient == Fraction<sint64>())
		return true;

	if (this->coefficient == b.coefficient && this->factor == b.factor)
		return true;
	else
		return false;
}

bool Monomial::operator!=(Monomial b)
{
	if (this->coefficient == b.coefficient && this->factor == b.factor)
		return false;
	else
		return true;
}

/*加法：
注意：使用前请判断a和b是否为同类项，
函数内不会自动判断，传入的对象一律视为同类项*/
Monomial Monomial::operator+(Monomial b)
{
	Monomial result;
	result.coefficient = this->coefficient + b.coefficient;
	result.factor = this->factor;
	result.DelOne();
	return result;
}

/*减法：
注意：使用前请判断a和b是否为同类项，
函数内不会自动判断，传入的对象一律视为同类项*/
Monomial Monomial::operator-(Monomial b)
{
	Monomial result;
	result.coefficient = this->coefficient - b.coefficient;
	result.factor = this->factor;
	result.DelOne();
	return result;
}

Monomial Monomial::operator*(Monomial b)
{
	Monomial result;
	result.coefficient = this->coefficient * b.coefficient;

	//如果乘积后的系数为0则将0作为结果
	if (result.coefficient.a == 0)
	{
		/*这样做的目的是，可能存在0与任何数做乘积运算后，
		结果中存在某一个非零多项式的字母系数，后续其他地方
		在做判断0操作或者判断是否为纯数字的时候可能会失效，
		所以这里直接返回结果*/
		return result;
	}

	result.factor = this->factor;

	FACTOR pos;
	for (FACTOR i = b.factor.begin(); i != b.factor.end(); i++)
	{
		//未查询到该字母因数
		if (result.factor.find(i->first) == result.factor.end())
		{
			result.factor.insert(std::pair<sint8, Fraction<sint64>>(i->first, i->second));
		}
		else
		{
			result.factor.at(i->first) += i->second;
		}
	}
	result.DelOne();
	return result;
}

/*除法：
注意：如果除数为0，则返回错误，
得到该函数的返回值后，请调用Monomial::Error()函数，
判断该返回值是否处于错误状态*/
Monomial Monomial::operator/(Monomial b)
{
	Monomial result;
	if (*this == b)
	{
		result.coefficient.a = 1;
		result.coefficient.b = 1;
		return result;
	}
	if (b.coefficient.a == 0)
	{
		result.error = true;
		return result;
	}
	result.coefficient = this->coefficient / b.coefficient;
	result.factor = this->factor;

	for (FACTOR i = b.factor.begin(); i != b.factor.end(); i++)
	{
		//未查询到该字母因数
		if (result.factor.find(i->first) == result.factor.end())
		{
			result.factor.insert(std::pair<sint8, Fraction<sint64>>(i->first, Fraction<sint64>(0 - i->second.a, i->second.b)));
		}
		else
		{
			result.factor.find(i->first)->second -= i->second;
			if (result.factor.find(i->first)->second.a == 0)
			{
				result.factor.erase(i->first);
			}
		}
	}
	result.DelOne();
	return result;
}

Monomial::operator Fraction<sint64>()
{
	return this->coefficient;
}

/*判断该单项式是否处于错误状态，
如果当前处于错误状态，则返回true，
否则返回false*/
bool Monomial::IsError()
{
	return this->error;
}
