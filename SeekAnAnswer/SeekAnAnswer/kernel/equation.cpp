#include "equation.h"
#include "polynomial.h"
#include "polynomial_exponential.h"
#include "../../High/Public/High_int.h"

/*	初始化Prime类中的质数表
 *	这里使用了C++11中的列表初始化，如果编译器不支持，
	可以使用其他初始化方法代替。 */
std::vector<sint64> Prime::nk = { 
	2,  3,  5,  7,  11, 13, 17, 
	19, 23, 29, 31, 37, 41, 43, 
	47, 53, 59, 61, 67, 71, 73, 
	79, 83, 89, 97 
};

void Equation::SetValue(std::string value)
{
	this->coefficient_left.clear();
	this->coefficient_right.clear();
	this->exponent_left.clear();
	this->exponent_right.clear();

	std::string container;
	suint64 i = 1;

	//从当前位置遍历到等号之前
	for (i; value.at(i) != '='; i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		//读取未知数系数
		for (i; value.at(i) != '*'; i++)
		{
			if (value.at(i) == '(' && container.size() >= 1)
			{
				if (container.at(container.size() - 1) == '(')
					continue;
			}
			container.push_back(value.at(i));
		}
		container.erase(container.size() - 1);

		//保存未知数系数
		this->coefficient_left.push_back(Polynomial(container));
		container.clear();

		i += 4; //定位到指数

		//读取未知数指数
		for (i; value.at(i) != ')'; i++)
		{
			container.push_back(value.at(i));
		}
		//保存未知数指数
		this->exponent_left.push_back(Fraction<sint64>(container));
		container.clear();
	}

	i += 2; //定位到等号右侧第一个未知数系数内

	for (i; i != value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		//读取未知数系数
		for (i; value.at(i) != '*'; i++)
		{
			if (value.at(i) == '(' && container.size() >= 1)
			{
				if (container.at(container.size() - 1) == '(')
					continue;
			}
			container.push_back(value.at(i));
		}
		container.erase(container.size() - 1);

		//保存未知数系数
		this->coefficient_right.push_back(Polynomial(container));
		container.clear();

		i += 4; //定位到指数

		//读取未知数指数
		for (i; value.at(i) != ')'; i++)
		{
			container.push_back(value.at(i));
		}
		//保存未知数指数
		this->exponent_right.push_back(Fraction<sint64>(container));
		container.clear();
	}

	this->DeleteZero();
	this->ShiftItem();
	this->Unite_like_terms();
}

void Equation::Input(std::string value)
{
	this->coefficient_left.clear();
	this->coefficient_right.clear();
	this->exponent_left.clear();
	this->exponent_right.clear();

	std::string container;
	std::string temp_val;
	Polynomial temp;
	suint64 i = 0;
	suint64 start = 0; //记录分离每一项的起始位置

	//从当前位置遍历到等号之前
	for (i; i < value.find('='); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			//判断是否存在加号在等号之前
			(value.find('+', i) != std::string::npos &&
				value.find('+', i) < value.find('='))
			//如果存在则返回该项的位置，否则返回等号的位置
			? value.find('+', i) : value.find('=');

		//安全性判断
		if (node_location < value.size())
		{
			//如果节点外围存在括号则不是有效节点
			if (CharacterInParentheses(value, node_location))
				continue;
		}

		//获取起始位置
		i = start;
		//记录下一项的起始位置
		start = node_location + 1;

		//读取这一项
		for (i; i < node_location; i++)
		{
			if (value.at(i) == '\40') continue;

			container.push_back(value.at(i));
		}

		//分离这一项

		//如果没找到X标志，说明这是一个常数项
		if (container.find('X') == std::string::npos)
		{
			temp.Input(container);
			this->coefficient_left.push_back(temp);
			this->exponent_left.push_back(Fraction<sint64>(0, 1));

			if (value.at(i) == '=') break;

			container.clear();
			temp_val.clear();
			temp.Clear();
			continue;
		}

		//提取多项式a
		for (suint64 j = 0;
			j < ((container.find('*') != std::string::npos)
				? container.find('*')
				: container.find('X'));
			j++)
		{
			temp_val.push_back(container.at(j));
		}

		/******处理多项式a*******/
		  //说明多项式a为1
		if (temp_val.empty())
		{
			temp.Input("1");
			this->coefficient_left.push_back(temp);
		}
		//如果多项式a的首位和末尾没找到括号，说明多项式a外围没有括号
		else if (temp_val.at(0) != '(' || temp_val.at(temp_val.size() - 1) != ')')
		{
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//如果多项式a的首位和末尾有重复括号，说明多项式a外围有括号
		else if (temp_val.at(1) == '(' || temp_val.at(temp_val.size() - 2) == ')')
		{
			temp_val.erase(0);
			temp_val.erase(temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//从右往左的第一个左括号不是最左面的括号说明多项式a外围没有括号
		else if (temp_val.rfind('(') != 0)
		{
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//最后一种情况多项式a外围有括号
		else
		{
			temp_val.erase(temp_val.begin() + 0);
			temp_val.erase(temp_val.begin() + temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}

		temp_val.clear(); //清空多项式a

		//提取指数n
		//如果X是最后一位，则指数为1
		if (container.find('X') == container.size() - 1)
		{
			this->exponent_left.push_back(Fraction<sint64>(1, 1));
		}
		//指定指数大小
		else
		{
			for (suint64 i = container.rfind('^') + 1; i < container.size(); i++)
				temp_val.push_back(container.at(i));
			this->exponent_left.push_back(Fraction<sint64>(temp_val));
		}

		container.clear();
		temp_val.clear();
		temp.Clear();
	}

	container.clear();
	temp_val.clear();
	temp.Clear();
	i = value.find('=') + 1;
	start = value.find('=') + 1; //归零起始位置

	//从当前位置遍历到最后
	for (i; i < value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			//判断是否存在加号在结尾之前
			(value.find('+', i) != std::string::npos)
			//如果存在则返回该项的位置，否则返回等号的位置
			? value.find('+', i) : value.size();

		//安全性判断
		if (node_location < value.size())
		{
			//如果节点外围存在括号则不是有效节点
			if (CharacterInParentheses(value, node_location))
				continue;
		}

		//获取起始位置
		i = start;
		//记录下一项的起始位置
		start = node_location + 1;

		//读取这一项
		for (i; i < node_location; i++)
		{
			if (value.at(i) == '\40') continue;

			container.push_back(value.at(i));
		}

		//分离这一项

		//如果没找到X标志，说明这是一个常数项
		if (container.find('X') == std::string::npos)
		{
			temp.Input(container);
			this->coefficient_right.push_back(temp);
			this->exponent_right.push_back(Fraction<sint64>(0, 1));

			if (i == value.size()) break;

			container.clear();
			temp_val.clear();
			temp.Clear();
			continue;
		}

		//提取多项式a
		for (suint64 j = 0;
			j < ((container.find('*') != std::string::npos)
				? container.find('*')
				: container.find('X'));
			j++)
		{
			temp_val.push_back(container.at(j));
		}

		/******处理多项式a*******/
		  //说明多项式a为1
		if (temp_val.empty())
		{
			temp.Input("1");
			this->coefficient_right.push_back(temp);
		}
		//如果多项式a的首位和末尾没找到括号，说明多项式a外围没有括号
		else if (temp_val.at(0) != '(' || temp_val.at(temp_val.size() - 1) != ')')
		{
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//如果多项式a的首位和末尾有重复括号，说明多项式a外围有括号
		else if (temp_val.at(1) == '(' || temp_val.at(temp_val.size() - 2) == ')')
		{
			temp_val.erase(0);
			temp_val.erase(temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//从右往左的第一个左括号不是最左面的括号说明多项式a外围没有括号
		else if (temp_val.rfind('(') != 0)
		{
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//最后一种情况多项式a外围有括号
		else
		{
			temp_val.erase(temp_val.begin() + 0);
			temp_val.erase(temp_val.begin() + temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}

		temp_val.clear(); //清空多项式a

		//提取指数n
		//如果X是最后一位，则指数为1
		if (container.find('X') == container.size() - 1)
		{
			this->exponent_right.push_back(Fraction<sint64>(1, 1));
		}
		//指定指数大小
		else
		{
			for (suint64 i = container.rfind('^') + 1; i < container.size(); i++)
				temp_val.push_back(container.at(i));
			this->exponent_right.push_back(Fraction<sint64>(temp_val));
		}

		container.clear();
		temp_val.clear();
		temp.Clear();
	}

	this->DeleteZero();
	this->ShiftItem();
	this->Unite_like_terms();
}

std::string Equation::GetValue()
{
	std::string result;
	for (suint64 i = 0; i < this->coefficient_left.size(); i++)
	{
		result.push_back('(');
		result += this->coefficient_left.at(i).GetValue();
		result.push_back(')');
		result.push_back('*');
		result.push_back('X');
		result.push_back('^');
		result.push_back('(');
		result += this->exponent_left.at(i).GetValue();
		result.push_back(')');
		if (this->coefficient_left.size() != i + 1)
		{
			result.push_back('\40');
			result.push_back('+');
			result.push_back('\40');
		}
	}
	result.push_back('\40');
	result.push_back('=');
	result.push_back('\40');

	if (this->coefficient_right.size() == 0)
	{
		result.push_back('0');
	}

	for (suint64 i = 0; i < this->coefficient_right.size(); i++)
	{
		result.push_back('(');
		result += this->coefficient_right.at(i).GetValue();
		result.push_back(')');
		result.push_back('*');
		result.push_back('X');
		result.push_back('^');
		result.push_back('(');
		result += this->exponent_right.at(i).GetValue();
		result.push_back(')');
		if (this->coefficient_right.size() != i + 1)
		{
			result.push_back('\40');
			result.push_back('+');
			result.push_back('\40');
		}
	}
	return result;
}

std::string Equation::Out()
{
	std::string result;
	//等号左侧
	for (suint64 i = 0; i < this->coefficient_left.size(); i++)
	{
		/*系数*/
		if (this->exponent_left.at(i) != Fraction<sint64>(0, 1)
			&& this->coefficient_left.at(i) == Polynomial("(1/1)"))
		{
			result.push_back('X');
			if (this->exponent_left.at(i) != Fraction<sint64>(1, 1))
			{
				result.push_back('^');
				result += this->exponent_left.at(i).Out();
			}
			goto add;
		}
		else
		{
			if (this->coefficient_left.at(i).list.size() > 1) result.push_back('(');

			result += this->coefficient_left.at(i).Out();

			if (this->coefficient_left.at(i).list.size() > 1) result.push_back(')');
		}

		/*未知数*/
		if (this->exponent_left.at(i) != Fraction<sint64>(0, 1))
		{
			result.push_back('X');
			if (this->exponent_left.at(i) != Fraction<sint64>(1, 1))
			{
				result.push_back('^');
				result += this->exponent_left.at(i).Out();
			}
		}

		add:
		if (i < this->coefficient_left.size() - 1)
		{
			result.push_back('\40');
			result.push_back('+');
			result.push_back('\40');
		}
	}

	//等号
	result.push_back('\40');
	result.push_back('=');
	result.push_back('\40');

	//等号右侧
	for (suint64 i = 0; i < this->coefficient_right.size(); i++)
	{
		/*系数*/
		if (this->exponent_right.at(i) != Fraction<sint64>(0, 1)
			&& this->coefficient_right.at(i) == Polynomial("(1/1)"))
		{
			result.push_back('X');
			if (this->exponent_right.at(i) != Fraction<sint64>(1, 1))
			{
				result.push_back('^');
				result += this->exponent_right.at(i).Out();
			}
			continue;
		}
		else
		{
			if (this->coefficient_right.at(i).list.size() > 1) result.push_back('(');

			result += this->coefficient_right.at(i).Out();

			if (this->coefficient_right.at(i).list.size() > 1) result.push_back(')');
		}

		/*未知数*/
		if (this->exponent_right.at(i) != Fraction<sint64>(0, 1))
		{
			result.push_back('X');
			if (this->exponent_right.at(i) != Fraction<sint64>(1, 1))
			{
				result.push_back('^');
				result += this->exponent_right.at(i).Out();
			}
		}

		if (i < this->coefficient_right.size() - 1)
		{
			result.push_back('\40');
			result.push_back('+');
			result.push_back('\40');
		}
	}

	if (this->coefficient_right.size() == 0)
	{
		result.push_back('0');
	}

	return result;
}

Fraction<sint64> Equation::GetTheHighestDegreeTermOfTheUnknown()
{
	Fraction<sint64> result = this->exponent_left.at(0);
	for (suint64 i = 0; i < this->exponent_left.size(); i++)
	{
		if (this->exponent_left.at(i) > result)
		{
			result = this->exponent_left.at(i);
		}
	}
	return result;
}

sint64 Equation::FindDegree(Fraction<sint64> degree)
{
	for (suint64 i = 0; i < this->exponent_left.size(); i++)
	{
		if (this->exponent_left.at(i) == degree)
		{
			return i;
		}
	}
	return -1;
}

void Equation::DeleteZero()
{
	for (suint64 i = 0; i < this->coefficient_left.size(); i++)
	{
		if (this->coefficient_left.at(i).list.size() == 1 && 
			this->coefficient_left.at(i).list.at(0).GetCoefficient().a == 0)
		{
			this->coefficient_left.erase(this->coefficient_left.begin() + i);
			this->exponent_left.erase(this->exponent_left.begin() + i);
		}
	}
	for (suint64 i = 0; i < this->coefficient_right.size(); i++)
	{
		if (this->coefficient_right.at(i).list.size() == 1 && 
			this->coefficient_right.at(i).list.at(0).GetCoefficient().a == 0)
		{
			this->coefficient_right.erase(this->coefficient_right.begin() + i);
			this->exponent_right.erase(this->exponent_right.begin() + i);
		}
	}
}

void Equation::ShiftItem()
{
	const Monomial Negative = (std::string)"(-1/1)";
	Polynomial temp;
	while (this->coefficient_right.size() != 0)
	{
		temp = this->coefficient_right.at(0);
		temp *= Negative;

		this->coefficient_left.push_back(temp);
		this->exponent_left.push_back(this->exponent_right.at(0));

		this->coefficient_right.erase(this->coefficient_right.begin());
		this->exponent_right.erase(this->exponent_right.begin());
	}
	this->DeleteZero();
	this->Unite_like_terms();
	return;
}

void Equation::Unite_like_terms()
{
	for (suint64 i = 0; i < this->coefficient_left.size() - 1; i++)
	{
		for (suint64 j = i + 1; j < this->coefficient_left.size(); j++)
		{
			if (this->exponent_left.at(i) == this->exponent_left.at(j))
			{
				this->coefficient_left.at(i) += this->coefficient_left.at(j);

				this->coefficient_left.erase(this->coefficient_left.begin() + j);
				this->exponent_left.erase(this->exponent_left.begin() + j);
			}
		}
	}
}

bool Equation::IsPurelyNumericalEquation()
{
	for (suint64 i = 0; i < this->coefficient_left.size(); i++)
	{
		if (!this->coefficient_left.at(i).IsNumber())
			return false;
	}
	return true;
}

sint64 Equation::NumberOfRoot()
{
	if (this->GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(1, 1))
	{
		return 1;
	}

	if (this->GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(2, 1))
	{
		this->Unite_like_terms();
		this->ShiftItem();
		Polynomial a, b, c;

		//如果没找到常数项，则c等于0
		if (this->FindDegree(Fraction<sint64>(0, 1)) != -1)
		{
			c = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(0, 1)));
		}

		//如果没找到一次项，则b等于0
		if (this->FindDegree(Fraction<sint64>(1, 1)) != -1)
		{
			b = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(1, 1)));
		}

		//二次项作为最大的一项，所以肯定存在
		a = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(2, 1)));

		Polynomial temp = (b * b) - (Polynomial(Monomial("(4/1)")) * a * c);

		if (!temp.IsNumber()) 
			return -1; //该方程可解，但不是纯数字方程

		//如果判别式大于0
		if ((Fraction<sint64>)(Monomial)temp > Fraction<sint64>(0, 1))
		{
			return 2;
		}
		else if ((Fraction<sint64>)(Monomial)temp == Fraction<sint64>(0, 1))
		{
			return 1;
		}
		else if ((Fraction<sint64>)(Monomial)temp < Fraction<sint64>(0, 1))
		{
			return 0;
		}
	}

	return -2; //该方程无法使用SeekAnAnswer求解
}

void Equation::linear_equation_with_one_unknown()
{
	Monomial Negative("(-1/1)");
	Fraction<sint64> degree; //0

	this->root1.a.Clear();
	this->root1.b.Clear();

	if (this->FindDegree(degree) == -1)
	{
		this->root1.a.Push(Monomial("0/1"));
		this->root1.b.Push(Monomial("1/1"));
	}
	this->root1.a = (this->coefficient_left.at(this->FindDegree(degree)) * Negative);

	degree.a = 1; degree.b = 1;
	
	this->root1.b = this->coefficient_left.at(this->FindDegree(degree));

	if (this->root1.a.IsNumber() && this->root1.a == Polynomial("(0/1)"))
	{
		this->root1.b = Polynomial("(1/1)");
		return;
	}

	//最小公倍数
	Fraction<sint64> least_common_multiple = 
		this->root1.a.LeastCommonMultiple_of_coe(this->root1.b);

	this->root1.a *= Monomial(least_common_multiple);
	this->root1.b *= Monomial(least_common_multiple);

	//求分子与分母的公因式
	Monomial common_factor = this->root1.a.CommonFactor(this->root1.b);
	//分别除以公因式以化简
	this->root1.a /= common_factor;
	this->root1.b /= common_factor;

	//如果分母的分式的分子是-1，则化简
	if (root1.b.IsNumber() && root1.b.list.at(0).GetCoefficient().a == -1)
	{
		root1.a *= Monomial("(-1/1)");
		this->root1.b.list.at(0).SetCoefficientA(1);
	}
}

void Equation::quadratic_equation_in_one_unknown()
{
	Polynomial a, b, c; //未知数的系数
	Polynomial_Exponential Discriminant; //根的判别式

	this->Unite_like_terms();
	this->ShiftItem();

	this->root2.clear();
	this->root2.push_back(Fraction<Polynomial_Exponential_Sum>()); //X1
	this->root2.push_back(Fraction<Polynomial_Exponential_Sum>()); //X2

	//如果找到了常数项，则赋值 //没找到常数项，则c = 0
	if (this->FindDegree(Fraction<sint64>(0, 1)) != -1)
	{
		c = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(0, 1)));
	}

	//如果找到了一次项，则赋值 //如果没找到一次项，则b等于0
	if (this->FindDegree(Fraction<sint64>(1, 1)) != -1)
	{
		b = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(1, 1)));
	}

	//二次项作为最大的一项，所以肯定存在
	a = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(2, 1)));

	//计算根的判别式 并在可化简时化简
	Discriminant = Simplest_radical(Polynomial_Exponential(
		Fraction<sint64>(1, 1),
		(b * b) + (Polynomial("(-4/1)") * a * c),
		Fraction<sint64>(1, 2))
	);

	/*计算X1*/
	//-b
	this->root2.at(0).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//根的判别式
	this->root2.at(0).a.Push(Discriminant);

	//2a
	this->root2.at(0).b.Push(Polynomial_Exponential(
		Polynomial("(2/1)") * a
	));

	this->root2.at(0).a.DeleteZero();

	//////////////////////////////////////////////
	/*计算X2*/
	//-b
	this->root2.at(1).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//根的判别式的相反数
	Discriminant.coefficient.a = 0 - Discriminant.coefficient.a;

	//负根号判别式 化简判别式
	this->root2.at(1).a.Push(Discriminant);

	//2a
	this->root2.at(1).b.Push(Polynomial_Exponential(
		Polynomial("(2/1)") * a
	));

	this->root2.at(1).a.DeleteZero();

	for (suint64 i = 0; i < 2; i++)
	{
		if (this->root2.at(i).a.IsPolynomial()
			&& this->root2.at(i).b.IsPolynomial())
		{
			//如果分母能整除分子，则进行化简运算
			if (((Polynomial)this->root2.at(i).a % (Polynomial)this->root2.at(i).b) == Polynomial())
			{
				this->root2.at(i).a = (Polynomial_Exponential_Sum)
					(Polynomial_Exponential)
					((Polynomial)this->root2.at(i).a / (Polynomial)this->root2.at(i).b);

				this->root2.at(i).b = Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
					Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1)));
			}
			//如果分子不为0，则约分
			if ((Polynomial)this->root2.at(i).a != Polynomial("(0/1)"))
			{
				//求最小公倍数
				Fraction<sint64> LeastCommonMultiple =
					((Polynomial)this->root2.at(i).a).LeastCommonMultiple_of_coe((Polynomial)this->root2.at(i).b);

				this->root2.at(i).a = 
					(Polynomial_Exponential)(((Polynomial)this->root2.at(i).a) * Monomial(LeastCommonMultiple));

				this->root2.at(i).b =
					(Polynomial_Exponential)(((Polynomial)this->root2.at(i).b) * Monomial(LeastCommonMultiple));

				//求最大公约数
				Monomial common_factor =
					((Polynomial)this->root2.at(i).a).CommonFactor((Polynomial)this->root2.at(i).b);

				this->root2.at(i).a = (Polynomial_Exponential)(((Polynomial)this->root2.at(i).a) / common_factor);
				this->root2.at(i).b = (Polynomial_Exponential)(((Polynomial)this->root2.at(i).b) / common_factor);
			}
		}
		else if (this->root2.at(i).a.IsPolynomial_Exponential()
			&& this->root2.at(i).b.IsPolynomial_Exponential())
		{
			if (!(this->root2.at(i).a.list.at(0) / this->root2.at(i).b.list.at(0)).Error())
			{
				this->root2.at(i).a = (Polynomial_Exponential_Sum)
					(this->root2.at(i).a.list.at(i) / this->root2.at(i).b.list.at(i));

				this->root2.at(i).b = Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
					Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1)));
			}
		}

		//提出系数，为下面化简做准备
		this->root2.at(i).a.Split();
		this->root2.at(i).b.Split();

		//如果系数不为0
		if (this->root2.at(i).a.list.at(0).coefficient != Fraction<sint64>(0, 1))
		{
			std::vector<sint64> a; Fraction<sint64> temp_a(1, 1);
			std::vector<sint64> b;
			//获取公倍数
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				a.push_back(this->root2.at(i).a.list.at(j).coefficient.b);
				temp_a.a *= a.at(a.size() - 1);
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				a.push_back(this->root2.at(i).b.list.at(j).coefficient.b);
				temp_a.a *= a.at(a.size() - 1);
			}

			//乘以最小公倍数
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				//分子乘以公倍数
				this->root2.at(i).a.list.at(j).coefficient =
					this->root2.at(i).a.list.at(j).coefficient * 
					(temp_a / Fraction<sint64>(Fraction<sint64>::gcds(a), 1));
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				//分母乘以公倍数
				this->root2.at(i).b.list.at(j).coefficient =
					this->root2.at(i).b.list.at(j).coefficient * 
					(temp_a / Fraction<sint64>(Fraction<sint64>::gcds(a), 1));
			}


			//获取公因数
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				b.push_back(this->root2.at(i).a.list.at(j).coefficient.a);
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				b.push_back(this->root2.at(i).b.list.at(j).coefficient.a);
			}

			//除以最大公约数
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				//分子除以公约数
				this->root2.at(i).a.list.at(j).coefficient =
					this->root2.at(i).a.list.at(j).coefficient /
					Fraction<sint64>(Fraction<sint64>::gcds(b), 1);
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				//分母除以公约数
				this->root2.at(i).b.list.at(j).coefficient =
					this->root2.at(i).b.list.at(j).coefficient /
					Fraction<sint64>(Fraction<sint64>::gcds(b), 1);
			}
		}

		//将系数合并回底数
		this->root2.at(i).a.Merge();
		this->root2.at(i).b.Merge();
	}
	return;
}

bool Equation::IsValid(std::string val)
{
	if (val.find('=') == std::string::npos) return false;

	std::string left, right;
	suint64 start = 0; //记录分离每一项的起始位置

	//从当前遍历到等号之前
	for (suint64 i = 0; i < val.find('='); i++)
	{
		if (val.at(i) == '\40') continue;
		left.push_back(val.at(i));
	}

	//从等号遍历到最后
	for (suint64 i = val.find('=') + 1; i < val.size(); i++)
	{
		if (val.at(i) == '\40') continue;
		right.push_back(val.at(i));
	}
	val.clear();

	std::string container; //容器

	here:

	start = 0; //归零起始位置

	for (suint64 i = 0; i < left.size(); i++)
	{
		if (left.at(i) == '\40') continue;
		if (left.at(i) == '+') continue;

		val.clear();
		container.clear();

		suint64 node_location =
			//判断是否存在加号在结尾之前
			(left.find('+', i) != std::string::npos)
			//如果存在则返回该项的位置，否则返回结尾的位置
			? left.find('+', i) : left.size();

		//安全性判断
		if (node_location < left.size())
		{
			//如果节点外围存在括号则不是有效节点
			if (CharacterInParentheses(left, node_location))
				continue;
		}

		//获取起始位置
		i = start; 
		//记录下一项的起始位置
		start = node_location + 1;

		//读取这一项
		for (i; i < node_location; i++)
		{
			if (left.at(i) == '\40') continue;

			container.push_back(left.at(i));
		}

		//分离这一项

		//如果没找到X标志，说明这是一个常数项
		if (container.find('X') == std::string::npos)
		{
			if (!Polynomial::IsValid(container))
				return false;
			continue;
		}

		//提取多项式a
		for (suint64 j = 0;
			j < ((container.find('*') != std::string::npos)
				? container.find('*')
				: container.find('X'));
			j++)
		{
			val.push_back(container.at(j));
		}

		if (!Polynomial::IsValid(val) && val.size() > 0) 
			return false;

		val.clear();

		//提取未知数
		for (suint64 j = container.find('X'); j < container.size(); j++)
		{
			val.push_back(container.at(j));
		}

		if (!Monomial::IsValid(val)) 
			return false;
	}

	container.clear();

	if (left != right)
	{
		left = right;
		goto here;
	}

	return true;
}

std::string Out(Fraction<Polynomial> val)
{
	std::string value;

	//如果val的值为0
	if (val.a.list.size() == 1 && val.a.list.at(0) == Monomial())
	{
		value += val.a.Out();
		return value;
	}

	//如果分子是多项式并且b不等于1
	if (val.a.list.size() > 1 && 
		!(val.b.IsNumber() && val.b.list.at(0).GetCoefficient().a == 1 && 
			val.b.list.at(0).GetCoefficient().b == 1)
		)
	{
		value.push_back('(');
	}

	value += val.a.Out();

	//如果分子是多项式并且b不等于1
	if (val.a.list.size() > 1 &&
		!(val.b.IsNumber() && val.b.list.at(0).GetCoefficient().a == 1 &&
			val.b.list.at(0).GetCoefficient().b == 1)
		)
	{
		value.push_back(')');
	}

	if (val.b != Polynomial(Monomial("(1/1)")))
	{
		value.push_back('\40');
		value.push_back('/');
		value.push_back('\40');

		//如果分母是多项式
		if (val.b.list.size() > 1)
		{
			value.push_back('(');
		}

		value += val.b.Out();

		//如果分母是多项式
		if (val.b.list.size() > 1)
		{
			value.push_back(')');
		}
	}
	return value;
}

Polynomial_Exponential Simplest_radical_AnBo(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	sint64 a = 0; //自然数
	sint64 aj = 0; //待分解的剩余因子
	std::vector<sint64> k; //分解出的k因子序列
	Prime ki; //用于分解的k素数
	sint64 g = 1; //所有不相等的k因子的乘积
	//如果a不是自然数
	if (!(val.number.IsNumber() && val.number.list.at(0).GetCoefficient().IsInteger()
		&& val.number.list.at(0).GetCoefficient().a >= 0))
	{
		result.error = true;
		return result;
	}

	//如果指数不是1/2
	if (val.exponential != Fraction<sint64>(1, 2))
	{
		result.error = true;
		return result;
	}

	/*开始化简*/
	//赋值
	a = ((Monomial)(val.number)).GetCoefficient().a;

	//如果a = 0
	if (a == 0)
	{
		result.exponential.b = 1;
		result.number.list.at(0).SetCoefficient(0, 1);
		return result;
	}

	//如果a = 1
	if (a == 1)
	{
		result.coefficient.a = 1;
		result.coefficient.b = 1;
		result.exponential.b = 1;
		result.number.list.at(0).SetCoefficient(1, 1);
		return result;
	}

	//如果a是素数
	if (IsPrime(a))
	{
		result.coefficient = val.coefficient;
		result.number.list.at(0).SetValue(Fraction<sint64>(a, 1));
		result.exponential.b = 2;
		return result;
	}

	aj = a;
	while (!IsPrime(aj))
	{
		if (aj % ki.n != 0) ki.Next();
		else
		{
			k.push_back(ki.n); //记录分解因子
			aj /= ki.n; //分解出新的aj
		}
	}

	//将剩余的aj插入进k因子序列
	for (suint64 i = 0; i < k.size(); i++)
	{
		if (aj == k.at(i)) 
		{
			k.insert(k.begin() + i, aj);
			aj = 0;
			break;
		}
	}

	if (aj != 0)
	{
		k.push_back(aj);
	}

	result.coefficient.a = 1;

	//处理k因子序列
	while (k.size() >= 1)
	{
		if (k.size() == 1)
		{
			g *= k.at(0);
			k.erase(k.begin());
		}
		else if (k.at(0) == k.at(1))
		{
			result.coefficient.a *= k.at(0);
			k.erase(k.begin());
			k.erase(k.begin());
		}
		else
		{
			g *= k.at(0);
			k.erase(k.begin());
		}
	}

	result.coefficient *= val.coefficient;
	//如果g=1，说明化简结果为整数
	if (g == 1)
	{
		result.number.list.at(0).SetCoefficientA(1);
		return result;
	}
	result.number.list.at(0).SetCoefficientA(g);
	result.exponential.b = 2;
	return result;
}

Polynomial_Exponential Simplest_radical(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	Fraction<sint64> a;
	//如果val不是纯数字，则无法化简
	if (!val.IsNumber())
	{
		return val;
	}

	//赋值（提取）a
	a = ((Monomial)val.number).GetCoefficient();

	//如果a小于0，则无法做化简操作
	if (a < Fraction<sint64>(0, 1))
	{
		return val;
	}

	//如果a是整数，则直接化简
	if (a.IsInteger())
	{
		return Simplest_radical_AnBo(val);
	}

	result.coefficient = val.coefficient * Fraction<sint64>(1, a.b);
	result.number = Fraction<sint64>(a.a * a.b, 1);
	result.exponential.b = 2;
	return Simplest_radical_AnBo(result);
}
