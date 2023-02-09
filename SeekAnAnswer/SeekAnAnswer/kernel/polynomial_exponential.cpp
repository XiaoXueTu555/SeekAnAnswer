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
			//当指数不等于0的时候显示乘号
			value.push_back('\40');
			value.push_back('*');
			value.push_back('\40');
		}


		if (this->number.list.size() > 1)
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
			value.push_back('\40');
			value += this->exponential.Out();
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
	if (this->exponential.b == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
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
	}

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
	}

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
	}

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

	//底数相等可以相乘
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
