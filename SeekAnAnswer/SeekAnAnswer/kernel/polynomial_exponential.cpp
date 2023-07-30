#include "polynomial_exponential.h"
#include "../../High/Public/High_int.h"

Polynomial_Exponential::Polynomial_Exponential()
{
	this->coefficient = Fraction<sint64>(0, 1);
	this->exponential = Fraction<sint64>(1, 1);
	this->error = false;
}

Polynomial_Exponential::Polynomial_Exponential(Polynomial value)
{
	this->SetValue(value);
}

Polynomial_Exponential::Polynomial_Exponential(Fraction<sint64> coefficient, Polynomial number, Fraction<sint64> exponential)
{
	this->coefficient = coefficient;
	this->number = number;
	this->exponential = exponential;
	this->error = false;
}

void Polynomial_Exponential::simplify()
{
	//如果底数等于1，则指数赋值1
	if (this->number == Polynomial("(1/1)"))
	{
		this->exponential = Fraction<sint64>(1, 1);
	}

	//如果指数为1，则将底数的系数部分提到前面
	if (this->exponential == Fraction<sint64>(1, 1) &&
		(this->number != Polynomial()))
	{
		std::vector<sint64> a;
		for (suint64 i = 0; i < this->number.list.size(); i++)
		{
			a.push_back(this->number.list.at(i).GetCoefficient().a);
		}

		this->number /= Monomial(Fraction<sint64>(Fraction<sint64>::gcds(a), 1));
		this->coefficient *= Fraction<sint64>(Fraction<sint64>::gcds(a), 1);
	}
}

void Polynomial_Exponential::merge()
{
	if(this->exponential == Fraction<sint64>(1,1))
	{
		this->number *= Monomial(this->coefficient);
		this->coefficient.a = 1;
		this->coefficient.b = 1;
	}
}

bool Polynomial_Exponential::IsValid(std::string value)
{
	std::string container;
	suint64 start = 0;

	//如果字符串不满足括号语法规则
	if (!ParenthesisSyntax(value)) 
		return false;
	//删除val外围的括号
	value = DeleteCircumjacentParentheses(value);

	//字符串中存在乘号
	if (value.find('*') != std::string::npos)
	{
		start = value.find('*');
		//获取系数
		for (suint64 i = 0; i < start; i++)
			container.push_back(value.at(i));

		if (!Fraction<sint64>::IsValid(container))
			return false;

		value.clear();
		++start; //递增一位

		//如果乘号是最后一位
		if (start == value.size()) 
			return false;
	}

	//若字符串中存在次方符号
	if (value.find('^') != std::string::npos)
	{
		//说明底数外不存在次方符号
		if (CharacterInParentheses(value, value.rfind('^')))
		{
			if (Polynomial::IsValid(value))
				return true;
			else
				return false;
		}

		//说明底数外存在次方符号
		//且次方符号在所有次方符号的最右侧
		
		//获取底数
		for (suint64 i = start; i < value.rfind('^'); i++)
			container.push_back(value.at(i));
		//如果底数不满足多项式
		if (!Polynomial::IsValid(container))
			return false;

		container.clear();
		start = value.rfind('^') + 1;

		//说明这里^符号在结尾
		if (start >= value.size())
			return false;

		//获取次数
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		//如果次数部分不是分式
		if (!Fraction<sint64>::IsValid(container))
			return false;
		else
			return true;
	}

	//获取底数
	for (suint64 i = start; i < value.size(); i++)
		container.push_back(value.at(i));
	//如果底数部分不满足多项式
	if (!Polynomial::IsValid(container))
		return false;

	return true;
}

void Polynomial_Exponential::SetValue(std::string std_value)
{
	std::string container; //容器
	suint64 i = 1;
	suint64 temp_location = 0;

	//读取数字系数部分
	for (i; std_value.at(i) != ')'; i++)
	{
		container.push_back(std_value.at(i));
	}

	//获取数字系数
	this->coefficient.SetValue(container);

	container.clear(); //清空容器内容
	i += 3;            //定位到底数中

	temp_location = std_value.rfind("^"); //定位最后一个"^"
	//读取底数部分
	for (i; i < temp_location - 1; i++)
	{
		container.push_back(std_value.at(i));
	}

	//获取指数
	this->number.SetValue(container);

	container.clear(); //清空容器内容
	i += 3;            //定位到底数中

	//读取指数部分
	for (i; std_value.at(i) != ')'; i++)
	{
		container.push_back(std_value.at(i));
	}

	//获取指数
	this->exponential.SetValue(container);
	this->error = false;
}

void Polynomial_Exponential::SetValue(Polynomial value)
{
	this->coefficient = Fraction<sint64>(1, 1);
	this->number = value;
	this->exponential = Fraction<sint64>(1, 1);;
	this->error = false;
}

void Polynomial_Exponential::Input(std::string value)
{
	value = DeleteCircumjacentParentheses(value);

	this->coefficient.Input("1/1");
	this->number.Input("0");
	this->exponential.Input("1/1");

	std::string container;
	suint64 start = 0;
	suint64 end = 0;
	//字符串中存在乘号
	if (value.find('*') != std::string::npos)
	{
		start = value.find('*');
		//获取系数
		for (suint64 i = 0; i < start; i++)
			container.push_back(value.at(i));

		this->coefficient.Input(container);

		container.clear();
		++start; //递增一位
	}

	//若字符串中存在次方符号
	if (value.find('^') != std::string::npos)
	{
		//说明底数外不存在次方符号
		if (CharacterInParentheses(value, value.rfind('^')))
		{
			//剩下的结构必定为底数
			this->number.Input(value);
			return;
		}

		/*说明底数外存在次方符号，且次方符号在所有次方符号的最右侧
		或者底数外不存在次方符号，而从start到最后整体都是底数*/

		//判断从start到最后整体是否为底数
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		if (this->number.IsValid(container))
		{
			this->number.Input(container);
			return;
		}

		container.clear();

		//获取底数
		for (suint64 i = start; i < value.rfind('^'); i++)
			container.push_back(value.at(i));
		this->number.Input(container);

		container.clear();
		start = value.rfind('^') + 1;

		//获取次数
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		this->exponential.Input(container);

		//如果可以转成多项式则转换
		if (this->IsPolynomial())
		{
			*this = (Polynomial)*this;
		}

		return;
	}

	//获取底数
	for (suint64 i = start; i < value.size(); i++)
		container.push_back(value.at(i));
	this->number.Input(container);
}

std::string Polynomial_Exponential::GetValue()
{
	std::string value;
	//数字系数
	value.push_back('(');
	value += this->coefficient.GetValue();
	value.push_back(')');
	value.push_back('*');
	
	//底数
	value.push_back('(');
	value += this->number.GetValue();
	value.push_back(')');
	value.push_back('^');

	//指数
	value.push_back('(');
	value += this->exponential.GetValue();
	value.push_back(')');
	return value;
}

std::string Polynomial_Exponential::Out()
{
	std::string value;
	if (this->coefficient == Fraction<sint64>())
	{
		value.push_back('0');
		return value;
	}

	if (this->coefficient != Fraction<sint64>(1, 1))
	{
		value += this->coefficient.Out();
	}

	//当指数不等于0的时候显示底数以及指数
	if (this->exponential != Fraction<sint64>())
	{
		if (this->coefficient != Fraction<sint64>(1, 1))
		{
			//当系数不等于1的时候显示乘号
			value.push_back('\40');
			value.push_back('*');
			value.push_back('\40');
		}

		//如果系数不等于1，而且底数的项数大于1则外面套上括号。
		//如果指数不等于1，而且底数的项数大于1则外面套上括号
		//其余的情况外面不套括号
		if (this->number.list.size() > 1 &&
			(this->exponential != Fraction<sint64>(1, 1)
				|| this->coefficient != Fraction<sint64>(1, 1))
			)
		{
			value.push_back('(');
			value += this->number.Out();
			value.push_back(')');
		}
		else
		{
			value += this->number.Out();
		}

		if (this->exponential != Fraction<sint64>(1, 1))
		{
			value.push_back('^');
			if (this->exponential.b != 1)
			{
				value += "(";
			}
			value += this->exponential.Out();
			if (this->exponential.b != 1)
			{
				value += ")";
			}
		}
	}
	else
	{
		if (this->coefficient == Fraction<sint64>(1, 1))
		{
			value += this->coefficient.Out();
		}
	}

	return value;
}

bool Polynomial_Exponential::IsPolynomial()
{
	if (this->exponential.IsInteger())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//这里的Number指纯数字，不是特指可转化为Fraction<sint64>类型
bool Polynomial_Exponential::IsNumber()
{
	//当且仅当底数是纯数字的时候，整个类型是纯数字
	if (this->number.IsNumber()) 
		return true;

	return false;
}

Polynomial_Exponential::operator Polynomial()
{
	Polynomial result = this->number;
	for (sint64 i = 1; i < this->exponential.a; i++)
	{
		result *= this->number;
	}

	Monomial temp;
	temp.SetCoefficient(this->coefficient.a, this->coefficient.b);
	result *= (Polynomial)temp;

	return result;
}

Polynomial_Exponential Polynomial_Exponential::operator+(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	if (this->IsPolynomial() && val.IsPolynomial())
	{
		result.coefficient = Fraction<sint64>(1, 1);
		result.number = (Polynomial)*this + (Polynomial)val;
		result.exponential = Fraction<sint64>(1, 1);
		return result;
	}

	if (this->number == val.number && this->exponential == val.exponential)
	{
		result.coefficient = this->coefficient + val.coefficient;
		result.number = this->number;
		result.exponential = this->exponential;
		return result;
	}

	result.error = true;
	return result;
}

Polynomial_Exponential Polynomial_Exponential::operator-(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	if (this->IsPolynomial() && val.IsPolynomial())
	{
		result.coefficient = Fraction<sint64>(1, 1);
		result.number = (Polynomial)*this - (Polynomial)val;
		result.exponential = Fraction<sint64>(1, 1);
		return result;
	}

	if (this->number == val.number && this->exponential == val.exponential)
	{
		result.coefficient = this->coefficient - val.coefficient;
		result.number = this->number;
		result.exponential = this->exponential;
		return result;
	}

	result.error = true;
	return result;
}

Polynomial_Exponential Polynomial_Exponential::operator*(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	if (this->IsPolynomial() && val.IsPolynomial())
	{
		result.coefficient = Fraction<sint64>(1, 1);
		result.number = (Polynomial)*this * (Polynomial)val;
		result.exponential = Fraction<sint64>(1, 1);
		return result;
	}

	//底数相等可以相乘
	if (this->number == val.number)
	{
		result.coefficient = this->coefficient * val.coefficient;
		result.number = this->number;
		result.exponential = this->exponential + val.exponential;
		return result;
	}

	result.error = true;
	return result;
}

Polynomial_Exponential Polynomial_Exponential::operator/(Polynomial_Exponential val)
{
	Polynomial_Exponential result;
	if (this->IsPolynomial() && val.IsPolynomial())
	{
		result.coefficient = Fraction<sint64>(1, 1);
		result.number = (Polynomial)*this / (Polynomial)val;
		result.exponential = Fraction<sint64>(1, 1);
		return result;
	}

	//底数相等可以相除
	if (this->number == val.number)
	{
		result.coefficient = this->coefficient / val.coefficient;
		result.number = this->number;
		result.exponential = this->exponential - val.exponential;
	}

	result.error = true; //说明不可以进行相除运算
	return result;
}

Polynomial_Exponential Polynomial_Exponential::operator%(Polynomial_Exponential val)
{
	return *this - ((*this / val) * val);
}

bool Polynomial_Exponential::operator==(Polynomial_Exponential val)
{
	if (this->coefficient == val.coefficient
		&& this->number == val.number
		&& this->exponential == val.exponential)
	{
		return true;
	}
	return false;
}

bool Polynomial_Exponential::operator!=(Polynomial_Exponential val)
{
	return !(*this == val);
}

bool Polynomial_Exponential::Error()
{
	return this->error;
}
