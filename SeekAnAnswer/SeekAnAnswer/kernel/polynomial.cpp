#include "polynomial.h"
#include <iostream>

//字母因数的迭代器
typedef std::map<sint8, Fraction<sint64>>::iterator FACTOR;

Polynomial::Polynomial(std::string std_value)
{
	this->SetValue(std_value);
}

Polynomial::Polynomial(Monomial val)
{
	*this = val;
}

Polynomial::Polynomial()
{
	this->list.push_back(Monomial());
	this->error = false;
}

void Polynomial::SetValue(Polynomial value)
{
	this->list.clear();

	*this = value;
}

void Polynomial::SetValue(std::string std_value)
{
	this->list.clear();

	std::string container; //容器

	for (suint64 i = 0; i < std_value.size(); i++)
	{
		if (std_value.at(i) == '+')
		{
			this->Push(Monomial(container));
			container.clear();
		}
		else if(std_value.at(i) == 32){}
		else
		{
			container.push_back(std_value.at(i));
		}
	}
	this->Push(Monomial(container));

	this->DeleteZero();

	this->error = false;
}

void Polynomial::Input(std::string value)
{
	this->list.clear();

	std::string container; //容器
	Monomial temp; //临时变量
	for (suint64 i = 0; i < value.size(); i++)
	{
		if (value.at(i) == '+')
		{
			temp.Input(container);
			this->Push(temp);
			container.clear();
		}
		else if (value.at(i) == '-')
		{
			if (!container.empty())
			{
				temp.Input(container);
				this->Push(temp);
				container.clear();
			}

			container.push_back('-');
		}
		else if (value.at(i) == 32) {}
		else
		{
			container.push_back(value.at(i));
		}
	}

	temp.Input(container);
	this->Push(temp);

	this->DeleteZero();
}

void Polynomial::Unite_like_terms()
{
	if (this->list.size() <= 1)
	{
		return;
	}
	this->DeleteZero();
	for (suint64 i = 0; i < this->list.size() - 1; i++)
	{
		for (suint64 j = i + 1; j < this->list.size(); j++)
		{
			if (this->list.at(i).SimilarItems(this->list.at(j)))
			{
				this->list.at(i) = this->list.at(i) + this->list.at(j);
				this->list.erase(this->list.begin() + j);
			}
		}
	}
	return;
}

void Polynomial::DeleteZero()
{
	if (this->list.size() <= 1) return;

	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i).GetCoefficient().a == 0)
		{
			this->list.erase(this->list.begin() + i);
		}
	}
}

void Polynomial::Push(Monomial val)
{
	Monomial temp;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i).SimilarItems(val))
		{
			temp = this->list.at(i);
			this->list.at(i) = temp + val;
			return;
		}
	}
	this->list.push_back(val);

	this->DeleteZero();
}

Monomial Polynomial::GetDegree()
{
	return this->list.at(GetLocationOfDegree());
}

suint64 Polynomial::GetLocationOfDegree()
{
	Fraction<sint64> degree(0, 1);
	suint64 count = 0;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i).GetDegree() > degree)
		{
			degree = this->list.at(i).GetDegree();
			count = i;
		}
	}
	return count;
}

void Polynomial::Clear()
{
	this->list.clear();
	this->Push(Monomial("(0/1)"));
}

Polynomial::operator Monomial()
{
	return this->list.at(0);
}

bool Polynomial::operator==(Polynomial val)
{
	this->Move();
	val.Move();
	if (this->list.size() != val.list.size())
	{
		return false;
	}
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i) != val.list.at(i))
		{
			return false;
		}
	}
	return true;
}

bool Polynomial::operator!=(Polynomial val)
{
	if (*this == val) return false;
	return true;
}

Polynomial Polynomial::operator=(Monomial val)
{
	this->list.clear();
	this->Push(val);
	this->error = val.IsError();
	return *this;
}

Polynomial Polynomial::operator=(Polynomial val)
{
	this->list = val.list;
	this->error = val.error;
	return *this;
}

/*单项式运算*/
Polynomial Polynomial::operator+(Monomial val)
{
	Polynomial result = *this;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (result.list.at(i).SimilarItems(val))
		{
			result.list.at(i) = result.list.at(i) + val;
			return result;
		}
	}
	result.list.push_back(Monomial(val));
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

Polynomial Polynomial::operator-(Monomial val)
{
	Polynomial result = *this;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (result.list.at(i).SimilarItems(val))
		{
			result.list.at(i) = result.list.at(i) - val;
			return result;
		}
	}
	result.list.push_back(Monomial(
		Fraction<sint64>(0 - val.GetCoefficient().a, 
		val.GetCoefficient().b), 
		val.GetFactor())
	);
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

Polynomial Polynomial::operator*(Monomial val)
{
	Polynomial result;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		result.list.push_back(this->list.at(i) * val);
	}
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

Polynomial Polynomial::operator/(Monomial val)
{
	Polynomial result = *this;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		result.list.at(i) = result.list.at(i) / val;
	}
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

/*多项式运算*/
Polynomial Polynomial::operator+(Polynomial val)
{
	Polynomial result = *this;
	for (suint64 i = 0; i < val.list.size(); i++)
	{
		result = result + val.list.at(i);
	}
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

Polynomial Polynomial::operator-(Polynomial val)
{
	Polynomial b = val;
	//0 - val
	for (suint64 i = 0; i < b.list.size(); i++)
	{
		b.list.at(i).SetCoefficientA(0 - b.list.at(i).GetCoefficient().a);
	}
	Polynomial result = *this + b;
	return result;
}

Polynomial Polynomial::operator*(Polynomial val)
{
	Polynomial result; 
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		for (suint64 j = 0; j < val.list.size(); j++)
		{
			result = result + (this->list.at(i) * val.list.at(j));
		}
	}
	result.DeleteZero();
	result.Unite_like_terms();
	return result;
}

Polynomial Polynomial::operator/(Polynomial val)
{
	Polynomial result; //商
	Polynomial remainder; //余数

	//若除数为0，则置为错误状态
	if (val.IsMonomial() && val.list.at(0).GetCoefficient().a == 0)
	{
		result.error = true;
		return result;
	}

	//清空商式的所有项
	result.list.clear();

	//将被除数和除数最高的单项式的次数的单项式移动到首位
	this->Move(); val.Move();

	//被除数与除数相等
	if (*this == val)
	{
		Monomial temp;
		temp.SetCoefficient(1, 1);
		result = result + temp;
		return result;
	}

	//最初的余式是被除式
	remainder = *this;

	/*如果该除法运算可进行因式分解
	条件：多项式a和多项式b互为父多项式（子多项式）并且
	多项式a的次数大于等于多项式b的次数大于0
	*/
	if (IsLetterSetEquality(*this, val) &&
		val.GetDegree().GetDegree() > Fraction<sint64>(0, 1) &&
		this->GetDegree().GetDegree() >= val.GetDegree().GetDegree())
	{
		while ( !( !IsLetterSetEquality(remainder, val) ||
			remainder.GetDegree().GetDegree() < val.GetDegree().GetDegree() ||
			(remainder.IsMonomial() && remainder.list.at(0) == Monomial("(0/1)"))
			) )
		{
			//余式的最高项的子多项式在除式中的位置（下标）
			sint64 location_of_sonPolynomial = -1;

			//得到下标
			for (suint64 i = 0; i < val.list.size(); i++)
			{
				if (IsParentPolynomial(remainder.list.at(0), val.list.at(i)))
				{
					location_of_sonPolynomial = i;
					break;
				}
			}

			//计算商式
			result.list.push_back(remainder.list.at(0) /
				val.list.at(location_of_sonPolynomial));

			//计算余式
			remainder -= (val * result.list.at(result.list.size() - 1));

			remainder.Move();
		}
	}

	//一般的两个多项式相除
	else
	{
		//如果“余式为0或者商式的长度大于等于被除式的长度”条件的否定满足则计算
		while (!((remainder.IsMonomial() && remainder.list.at(0) == Monomial("(0/1)")) ||
			result.list.size() >= this->list.size()))
		{
			//计算商式
			result.list.push_back(remainder.list.at(0) / val.list.at(0));

			//计算余式
			remainder -= (val * result.list.at(result.list.size() - 1));

			remainder.Move();
		}
	}

	result.DeleteZero();
	result.Unite_like_terms();

	if (result.list.size() == 0) result.SetValue("(0/1)");

	return result;
}

Polynomial Polynomial::operator%(Polynomial val)
{
	return *this - ((*this / val) * val);
}

Polynomial Polynomial::operator+=(Polynomial val)
{
	*this = *this + val;
	return *this;
}
Polynomial Polynomial::operator-=(Polynomial val)
{
	*this = *this - val;
	return *this;
}
Polynomial Polynomial::operator*=(Polynomial val)
{
	*this = *this * val;
	return *this;
}
Polynomial Polynomial::operator/=(Polynomial val)
{
	*this = *this / val;
	return *this;
}
Polynomial Polynomial::operator%=(Polynomial val)
{
	*this = *this % val;
	return *this;
}

bool Polynomial::IsMonomial()
{
	if (this->list.size() == 1)
	{
		return true;
	}
	return false;
}

bool Polynomial::IsError()
{
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i).IsError())
		{
			this->error = true;
			return this->error;
		}
	}

	if (this->list.size() == 0)
	{
		this->error = true;
		return this->error;
	}

	return this->error;
}

std::string Polynomial::GetValue()
{
	std::string value;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		value += this->list.at(i).GetValue();
		if (i != this->list.size() - 1)
		{
			value.push_back('\40');
			value.push_back('+');
			value.push_back('\40');
		}
	}
	return value;
}

std::string Polynomial::Out()
{
	std::string value;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		value += this->list.at(i).Out();
		if (i != this->list.size() - 1)
		{
			value.push_back('\40');
			value.push_back('+');
			value.push_back('\40');
		}
	}
	return value;
}

void Polynomial::Move()
{
	if (this->list.size() == 0)
	{
		return;
	}
	std::vector<Monomial> result;
	while (this->list.size() != 1)
	{
		result.push_back(this->list.at(this->GetLocationOfDegree()));
		this->list.erase(this->list.begin() + this->GetLocationOfDegree());
	}
	result.push_back(this->list.at(0));

	for (suint64 i = 0; i < result.size(); i++)
	{
		if (result.at(i).GetDegree().a == 0)
		{
			result.push_back(result.at(i));
			result.erase(result.begin() + i);
		}
	}

	this->list = result;
	return;
}

bool IsParentPolynomial(Polynomial a, Polynomial b)
{
	std::set<sint8> D; //多项式a的字母集合
	std::set<sint8> E; //多项式b的字母集合

	//取得多项式a的所有字母
	for (suint64 i = 0; i < a.list.size(); i++)
	{
		for (FACTOR j = a.list.at(i).GetFactor().begin(); j != a.list.at(i).GetFactor().end(); j++)
		{
			D.insert(j->first);
		}
	}

	//取得多项式b的所有字母
	for (suint64 i = 0; i < b.list.size(); i++)
	{
		for (FACTOR j = b.list.at(i).GetFactor().begin(); j != b.list.at(i).GetFactor().end(); j++)
		{
			E.insert(j->first);
		}
	}

	if (E.size() > D.size()) return false;

	for (std::set<sint8>::iterator it = E.begin(); it != E.end(); it++)
	{
		//如果E集合中存在D集合中没有的元素，则a不是b的父多项式
		if (D.find(*it) == D.end()) return false;
	}

	return true;
}

bool IsLetterSetEquality(Polynomial a, Polynomial b)
{
	std::set<sint8> D; //多项式a的字母集合
	std::set<sint8> E; //多项式b的字母集合

	//取得多项式a的所有字母
	for (suint64 i = 0; i < a.list.size(); i++)
	{
		for (FACTOR j = a.list.at(i).GetFactor().begin(); j != a.list.at(i).GetFactor().end(); j++)
		{
			D.insert(j->first);
		}
	}

	//取得多项式b的所有字母
	for (suint64 i = 0; i < b.list.size(); i++)
	{
		for (FACTOR j = b.list.at(i).GetFactor().begin(); j != b.list.at(i).GetFactor().end(); j++)
		{
			E.insert(j->first);
		}
	}

	if (D == E) return true;

	return false;
}
