#include "equation.h"
#include "polynomial.h"
#include "polynomial_exponential.h"
#include "../../High/Public/High_int.h"

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

	//从当前位置遍历到等号之前
	for (i; i < value.find('='); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			((value.find('X', i) != std::string::npos)
				&& (value.find('+', value.find('X', i)) != std::string::npos)
				&& (value.find('+', value.find('X', i)) < value.find('=')))
			? value.find('+', value.find("X", i)) //如果有X，且后面有“+”则返回该符号下标
			: (value.find('X', i) != std::string::npos) ? value.find('=') //有X后面没有“+”则返回“=”下标
			: (value.find('+', i) != std::string::npos) ? value.find('+', i) : value.find('=');

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
			for (suint64 i = container.find('^') + 1; i < container.size(); i++)
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

	//从当前位置遍历到最后
	for (i; i < value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			((value.find('X', i) != std::string::npos)
				&& (value.find('+', value.find('X', i)) != std::string::npos))
			? value.find('+', value.find("X", i)) //如果有X，且后面有“+”则返回该符号下标
			: (value.find('X', i) != std::string::npos) ? value.size() //有X后面没有“+”则返回结尾
			: (value.find('+', i) != std::string::npos) ? value.find('+', i) : value.size();

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
			for (suint64 i = container.find('^') + 1; i < container.size(); i++)
				temp_val.push_back(container.at(i));
			this->exponent_right.push_back(Fraction<sint64>(temp_val));
		}

		container.clear();
		temp_val.clear();
		temp.Clear();
	}
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
			this->coefficient_right.erase(this->coefficient_right.begin() + i);
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
			}
		}
	}
}

Fraction<Polynomial> Equation::linear_equation_with_one_unknown()
{
	const Monomial Negative = (std::string)"-1/1";
	Fraction<Polynomial> result;
	Fraction<sint64> degree; //0
	if (this->FindDegree(degree) == -1)
	{
		result.a.Push(Monomial("0/1"));
		result.b.Push(Monomial("1/1"));
		return result;
	}
	result.a = (this->coefficient_left.at(this->FindDegree(degree)) * Negative);

	degree.a = 1; degree.b = 1;

	result.b = this->coefficient_left.at(this->FindDegree(degree));
	return result;
}

std::vector<Fraction<Polynomial_Exponential_Sum>> Equation::quadratic_equation_in_one_unknown()
{
	std::vector<Fraction<Polynomial_Exponential_Sum>> result;
	Polynomial a, b, c;

	result.push_back(Fraction<Polynomial_Exponential_Sum>()); //X1
	result.push_back(Fraction<Polynomial_Exponential_Sum>()); //X2

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

	/*计算X1*/
	//-b
	result.at(0).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//根号判别式
	result.at(0).a.Push(Polynomial_Exponential(
		Fraction<sint64>(1, 1),
		(b * b) + (Polynomial("(-4/1)") * a * c),
		Fraction<sint64>(1, 2)
	));

	//2a
	result.at(0).b.Push(Polynomial_Exponential(
		Polynomial("(2/1)") * a
	));

	result.at(0).a.DeleteZero();

	//////////////////////////////////////////////
	/*计算X2*/
	//-b
	result.at(1).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//负根号判别式
	result.at(1).a.Push(Polynomial_Exponential(
		Fraction<sint64>(-1, 1),
		(b * b) + (Polynomial("(-4/1)") * a * c),
		Fraction<sint64>(1, 2)
	));

	//2a
	result.at(1).b.Push(Polynomial_Exponential(
		Polynomial("(2/1)") * a
	));

	result.at(1).a.DeleteZero();

	for (suint64 i = 0; i < 2; i++)
	{
		if (result.at(i).a.IsPolynomial()
			&& result.at(i).b.IsPolynomial())
		{
			//如果余数相除余数等于0，则进行化简运算
			if (((Polynomial)result.at(i).a % (Polynomial)result.at(i).b) == Polynomial())
			{
				result.at(i).a = (Polynomial_Exponential_Sum)
					(Polynomial_Exponential)
					((Polynomial)result.at(i).a / (Polynomial)result.at(i).b);

				result.at(i).b = Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
					Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1)));
			}
		}
		else if (result.at(i).a.IsPolynomial_Exponential()
			&& result.at(i).b.IsPolynomial_Exponential())
		{
			if (!(result.at(i).a.list.at(i) / result.at(i).b.list.at(i)).Error())
			{
				result.at(i).a = (Polynomial_Exponential_Sum)
					(result.at(i).a.list.at(i) / result.at(i).b.list.at(i));

				result.at(i).b = Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
					Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1)));
			}
		}
	}

	return result;
}
