#include "polynomial.h"
#include <iostream>
#include <math.h>

//字母因数的迭代器
typedef std::map<sint8, Fraction<sint64>>::iterator FACTOR;

std::set<sint8> S_set_intersection(std::set<sint8> a, std::set<sint8> b)
{
	std::set<sint8> intersection;
	for (std::set<sint8>::iterator i = a.begin(); i != a.end(); i++)
	{
		if (b.find(*i) != b.end())
			intersection.insert(*i);
	}
	return intersection;
}

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

	value = DeleteCircumjacentParentheses(value);

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
	this->DeleteZero();
	return;
}

void Polynomial::DeleteZero()
{
	/*最开始的时候下面这行被注释的代码是被添加在这个函数里的，
	因为可以提高这个函数的执行效率，逻辑上也说得清，貌似没问题，
	但是我最近发现了一个bug，如果一个多项式是一个单项式，而且等于0，
	也就是这个单项式的数字系数部分等于0，但是这个单项式有可能
	在字母系数部分携带了一些信息，这可能会带来额外的内存开销，
	或者引出一些其他的bug，所以这里以注释的方式保留这行代码，以警戒*/
	//if (this->list.size() <= 1) return;

	/*经过上述解释，我们需要处理数字系数部分为0，但是字母系数
	带有额外信息的情况，但是不能将单纯将上面的代码删掉，因为
	下面的代码会用到Push函数，而Push函数会调用DeleteZero，
	如果多项式是0的话直接递归死循环了，所以我们要加这一段代码*/
	if (this->list.size() <= 1
		&& this->list.at(0).GetCoefficient().a == 0)
	{
		this->list.at(0).Clear_factor();
		return;
	}

	for (suint64 i = 0; i < this->list.size(); i++)
	{
		if (this->list.at(i).GetCoefficient().a == 0)
		{
			this->list.erase(this->list.begin() + i);
		}
	}

	if (this->list.size() == 0)
	{
		this->Push(Monomial());
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

Fraction<sint64> Polynomial::GetTheDegree()
{
	Fraction<sint64> degree;
	Monomial MAX_degree = this->GetDegree();
	for (FACTOR it = MAX_degree.GetFactor().begin(); it != MAX_degree.GetFactor().end(); it++)
	{
		degree += it->second;
	}
	return degree;
}

/*该函数在保证返回最大次数所在单项式的下标之外，
还保证该函数返回单项式的字母集合元素数量最少*/
suint64 Polynomial::GetLocationOfDegree()
{
	//取初始次数
	Fraction<sint64> degree = this->list.at(0).GetDegree();

	//某单项式的字母集合元素个数
	sint64 element_quantity = GetLetterSize(this->list.at(0));

	//最高次单项式在表里的下标
	suint64 location = 0;

	Fraction<sint64> temp_degree;
	for (suint64 i = 1, degree_temp = 0; i < this->list.size(); i++)
	{
		//获取当前单项式的次数
		temp_degree = this->list.at(i).GetDegree();

		//如果当前单项式次数大于目前已知最大的次数则更新
		if (temp_degree > degree)
		{
			degree = temp_degree;
			element_quantity = GetLetterSize(this->list.at(i));
			location = i;
			continue;
		}
		//如果次数相等，但是字母集合元素个数更少，则更新
		else if (temp_degree == degree &&
			GetLetterSize(this->list.at(i)) < element_quantity)
		{
			degree = temp_degree;
			element_quantity = GetLetterSize(this->list.at(i));
			location = i;
		}
	}
	return location;
}

Fraction<sint64> Polynomial::GetDegreeOfMixOfchar(sint8 a)
{
	//GetTheDegree返回的是该多项式最大的次数，因此不存在某个单项式的字母的次数大于该次数
	//所以下面遍历判断比这个小的字母的次数就是最小的
	Fraction<sint64> degree = this->GetTheDegree();
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		//如果在这一项单项式中找到该元素
		if ((this->list.at(i).GetFactor().find(a)) != (this->list.at(i).GetFactor().end()))
		{
			if ((this->list.at(i).GetFactor().find(a))->second < degree)
			{
				degree = (this->list.at(i).GetFactor().find(a))->second;
			}
		}
	}
	return degree;
}

Monomial Polynomial::CommonFactor(Polynomial val)
{
	Monomial common_factor;

	std::vector<sint64> facs;
	//取出val的每一项系数
	for (suint64 i = 0; i < val.list.size(); i++)
	{
		facs.push_back(val.list.at(i).GetCoefficient().a);
	}
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		facs.push_back(this->list.at(i).GetCoefficient().a);
	}
	sint64 common_number = Fraction<sint64>::gcds(facs);

	common_factor.SetCoefficient(common_number, 1);

	std::set<sint8> a_set_intersection;
	std::set<sint8> last_set; //上一个单项式的字母集合
	std::set<sint8> set; //这个单项式的字母集合
	std::set<sint8> set_intersection; //相邻两个单项式的交集

	//获取初始的集合，否则下面求交集时结果始终为空集
	for (FACTOR j = this->list.at(0).GetFactor().begin(); j != this->list.at(0).GetFactor().end(); j++)
	{	
		set.insert(j->first);
	}

	//遍历多项式a，获取字母集合
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		//获取单项式的字母集合
		for (FACTOR j = this->list.at(i).GetFactor().begin(); j != this->list.at(i).GetFactor().end(); j++)
		{
			if (i % 2 == 0) last_set.insert(j->first);
			else
			{
				set.insert(j->first);
			}
		}

		if (i % 2 == 1)
		{
			set_intersection = S_set_intersection(
				S_set_intersection(last_set, set),
				set_intersection
			);

			last_set.clear();
		}
	}
	a_set_intersection = set_intersection;

	last_set.clear(); set.clear(); set_intersection.clear(); //初始化

	//获取初始的集合，否则下面求交集时结果始终为空集
	for (FACTOR j = val.list.at(0).GetFactor().begin(); j != val.list.at(0).GetFactor().end(); j++)
	{
		set.insert(j->first);
	}

	//遍历多项式a，获取字母集合
	for (suint64 i = 0; i < val.list.size(); i++)
	{
		//获取单项式的字母集合
		for (FACTOR j = val.list.at(i).GetFactor().begin(); j != val.list.at(i).GetFactor().end(); j++)
		{
			if (i % 2 == 0) last_set.insert(j->first);
			else
			{
				set.insert(j->first);
			}
		}

		if (i % 2 == 1)
		{
			set_intersection = S_set_intersection(
				S_set_intersection(last_set, set),
				set_intersection
			);

			last_set.clear();
		}
	}
	//计算两个多项式的字母交集
	set_intersection = S_set_intersection(a_set_intersection, set_intersection);

	for (std::set<sint8>::iterator it = set_intersection.begin(); it != set_intersection.end(); it++)
	{
		common_factor.Push(*it, //添加一个字母因数，次数为两个多项式间最小的次数
			(this->GetDegreeOfMixOfchar(*it) < val.GetDegreeOfMixOfchar(*it)
				? this->GetDegreeOfMixOfchar(*it) : val.GetDegreeOfMixOfchar(*it)));
	}

	return common_factor;
}

Fraction<sint64> Polynomial::LeastCommonMultiple_of_coe(Polynomial val)
{
	std::vector<sint64> a;
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		a.push_back(this->list.at(i).GetCoefficient().b);
	}
	for (suint64 i = 0; i < val.list.size(); i++)
	{
		a.push_back(val.list.at(i).GetCoefficient().b);
	}

	sint64 common_number = Fraction<sint64>::gcds(a);

	sint64 temp = 1;
	for (suint64 i = 0; i < a.size(); i++)
	{
		temp *= a.at(i);
	}
	return Fraction<sint64>(temp / common_number, 1);
}

void Polynomial::Clear()
{
	this->list.clear();
	this->Push(Monomial("(0/1)"));
}

bool Polynomial::Substitute(sint8 character, Polynomial val)
{
	//判断该多项式是否可以代换
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		//先判断是否该单项式是否存在该字符，存在就进行下一步的判断
		if (this->list.at(i).GetFactor().find(character) != this->list.at(i).GetFactor().end())
		{
			//如果存在代换字符的次数为非整数有理数，或者为负数，则无法代换
			if (this->list.at(i).GetFactor().find(character)->second.b != 1 ||
				this->list.at(i).GetFactor().find(character)->second.a < 0)
			{
				return false;
			}
		}
	}

	Polynomial temp; //临时变量
	sint64 exponent = 0;

	Fraction<sint64> coe_temp; //保存代换字符所在单项式的系数

	//遍历所有单项式
	for (suint64 i = 0; i < this->list.size(); i++)
	{
		//如果该项不含代换字符
		if (this->list.at(i).GetFactor().find(character) 
			== this->list.at(i).GetFactor().end())
			continue;

		//获取该项代换字符的次数，这里减去1是为了下面减少一次乘法运算
		exponent = this->list.at(i).GetFactor().find(character)->second.a - 1;
		temp = val; //赋值为val

		//进行次方运算
		for (sint64 j = 0; j < exponent; j++)
		{
			temp *= val;
		}

		//保存该项单项式的数字系数
		coe_temp = this->list.at(i).GetCoefficient();

		//删除该项单项式
		this->list.erase(this->list.begin() + i);

		*this += (Polynomial)(Monomial)coe_temp * temp;
	}
	return true;
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

	/*将被除数和除数最高的单项式的次数的单项式移动到首位
	且首位保证是字母集合元素最少的单项式*/
	this->Move(); val.Move();

	//被除数与除数相等
	if (*this == val)
	{
		Monomial temp;
		temp.SetCoefficient(1, 1);
		result = result + temp;
		return result;
	}

	/*该除法运算可进行因式分解的必要条件
	条件：多项式a是多项式b的父多项式，并且
	多项式a的次数大于等于多项式b的次数大于0
	*/
	if (IsParentPolynomial(*this, val) &&
		val.GetDegree().GetDegree() > Fraction<sint64>(0, 1) &&
		this->GetDegree().GetDegree() >= val.GetDegree().GetDegree())
	{
		//最初的余式是被除式
		remainder = *this;

		//清空商式的所有项
		result.list.clear();

		//如果不能进行因式分解算法则进行长除法
		if (!IsParentPolynomial(remainder, val) ||
			remainder.GetDegree().GetDegree() < val.GetDegree().GetDegree() ||
			(remainder.IsMonomial() && remainder.list.at(0) == Monomial("(0/1)")) ||
			(result.list.size() >= this->list.size())
			)
		{
			goto division_method;
		}

		/*如果“余式不是除式的子多项式”、“余式的次数小于除式的次数”、
		“余式等于0”、“商多项式的项数大于等于被除式的项数”
		其一满足，则因式分解结束，否则可进行因式分解*/
		while ( !( !IsParentPolynomial(remainder, val) ||
			remainder.GetDegree().GetDegree() < val.GetDegree().GetDegree() ||
			(remainder.IsMonomial() && remainder.list.at(0) == Monomial("(0/1)")) ||
			(result.list.size() >= this->list.size())
			) )
		{
			//余式的最高项的子多项式在除式中的位置（下标）
			sint64 location_of_sonPolynomial = -1;

			//得到下标
			for (suint64 i = 0; i < val.list.size(); i++)
			{
				//这里的remainder.list.at(0)就是余式的最高项
				if (IsParentPolynomial(remainder.list.at(0), val.list.at(i)))
				{
					location_of_sonPolynomial = i;
					break;
				}
			}

			//说明除式中不存在余式的最高项的子多项式，无法因式分解
			if (location_of_sonPolynomial == -1)
			{
				goto division_method;
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
	division_method:

		//最初的余式是被除式
		remainder = *this;

		//清空商式的所有项
		result.list.clear();

		//如果“余式为0或者商式的长度大于等于被除式的长度”条件满足则不能计算，否则能计算
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

	//防止某些运算提前结束导致长度为0
	if (result.list.size() == 0)
		result.Push(Monomial());

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

bool Polynomial::IsNumber()
{
	if (!this->IsMonomial()) 
		return false;
	else if (this->list.at(0).IsNumber()) 
		return true;
	return false;
}

bool Polynomial::IsValid(std::string val)
{
	//如果字符串不满足括号语法规则
	if (!ParenthesisSyntax(val)) return false;
	//删除val外围的括号
	val = DeleteCircumjacentParentheses(val);

	if (val.size() == 0) return false;

	std::string container; //容器
	for (suint64 i = 0; i < val.size(); i++)
	{
		if (val.at(i) == '\40') continue;
		if (val.at(i) == '+')
		{
			//如果这一项不是单项式则整体不满足多项式
			if (!Monomial::IsValid(container))
				return false;
			container.clear();
			continue;
		}

		container.push_back(val.at(i));
	}

	//如果这一项不是单项式则整体不满足多项式
	if (!Monomial::IsValid(container))
		return false;
	return true;
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
	if (this->list.size() <= 1)
	{
		return;
	}

	std::vector<Monomial> result;

	//按照次数从大到小排序
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
