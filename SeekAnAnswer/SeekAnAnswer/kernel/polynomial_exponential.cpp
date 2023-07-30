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
	//�����������1����ָ����ֵ1
	if (this->number == Polynomial("(1/1)"))
	{
		this->exponential = Fraction<sint64>(1, 1);
	}

	//���ָ��Ϊ1���򽫵�����ϵ�������ᵽǰ��
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

	//����ַ��������������﷨����
	if (!ParenthesisSyntax(value)) 
		return false;
	//ɾ��val��Χ������
	value = DeleteCircumjacentParentheses(value);

	//�ַ����д��ڳ˺�
	if (value.find('*') != std::string::npos)
	{
		start = value.find('*');
		//��ȡϵ��
		for (suint64 i = 0; i < start; i++)
			container.push_back(value.at(i));

		if (!Fraction<sint64>::IsValid(container))
			return false;

		value.clear();
		++start; //����һλ

		//����˺������һλ
		if (start == value.size()) 
			return false;
	}

	//���ַ����д��ڴη�����
	if (value.find('^') != std::string::npos)
	{
		//˵�������ⲻ���ڴη�����
		if (CharacterInParentheses(value, value.rfind('^')))
		{
			if (Polynomial::IsValid(value))
				return true;
			else
				return false;
		}

		//˵����������ڴη�����
		//�Ҵη����������дη����ŵ����Ҳ�
		
		//��ȡ����
		for (suint64 i = start; i < value.rfind('^'); i++)
			container.push_back(value.at(i));
		//����������������ʽ
		if (!Polynomial::IsValid(container))
			return false;

		container.clear();
		start = value.rfind('^') + 1;

		//˵������^�����ڽ�β
		if (start >= value.size())
			return false;

		//��ȡ����
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		//����������ֲ��Ƿ�ʽ
		if (!Fraction<sint64>::IsValid(container))
			return false;
		else
			return true;
	}

	//��ȡ����
	for (suint64 i = start; i < value.size(); i++)
		container.push_back(value.at(i));
	//����������ֲ��������ʽ
	if (!Polynomial::IsValid(container))
		return false;

	return true;
}

void Polynomial_Exponential::SetValue(std::string std_value)
{
	std::string container; //����
	suint64 i = 1;
	suint64 temp_location = 0;

	//��ȡ����ϵ������
	for (i; std_value.at(i) != ')'; i++)
	{
		container.push_back(std_value.at(i));
	}

	//��ȡ����ϵ��
	this->coefficient.SetValue(container);

	container.clear(); //�����������
	i += 3;            //��λ��������

	temp_location = std_value.rfind("^"); //��λ���һ��"^"
	//��ȡ��������
	for (i; i < temp_location - 1; i++)
	{
		container.push_back(std_value.at(i));
	}

	//��ȡָ��
	this->number.SetValue(container);

	container.clear(); //�����������
	i += 3;            //��λ��������

	//��ȡָ������
	for (i; std_value.at(i) != ')'; i++)
	{
		container.push_back(std_value.at(i));
	}

	//��ȡָ��
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
	//�ַ����д��ڳ˺�
	if (value.find('*') != std::string::npos)
	{
		start = value.find('*');
		//��ȡϵ��
		for (suint64 i = 0; i < start; i++)
			container.push_back(value.at(i));

		this->coefficient.Input(container);

		container.clear();
		++start; //����һλ
	}

	//���ַ����д��ڴη�����
	if (value.find('^') != std::string::npos)
	{
		//˵�������ⲻ���ڴη�����
		if (CharacterInParentheses(value, value.rfind('^')))
		{
			//ʣ�µĽṹ�ض�Ϊ����
			this->number.Input(value);
			return;
		}

		/*˵����������ڴη����ţ��Ҵη����������дη����ŵ����Ҳ�
		���ߵ����ⲻ���ڴη����ţ�����start��������嶼�ǵ���*/

		//�жϴ�start����������Ƿ�Ϊ����
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		if (this->number.IsValid(container))
		{
			this->number.Input(container);
			return;
		}

		container.clear();

		//��ȡ����
		for (suint64 i = start; i < value.rfind('^'); i++)
			container.push_back(value.at(i));
		this->number.Input(container);

		container.clear();
		start = value.rfind('^') + 1;

		//��ȡ����
		for (suint64 i = start; i < value.size(); i++)
			container.push_back(value.at(i));
		this->exponential.Input(container);

		//�������ת�ɶ���ʽ��ת��
		if (this->IsPolynomial())
		{
			*this = (Polynomial)*this;
		}

		return;
	}

	//��ȡ����
	for (suint64 i = start; i < value.size(); i++)
		container.push_back(value.at(i));
	this->number.Input(container);
}

std::string Polynomial_Exponential::GetValue()
{
	std::string value;
	//����ϵ��
	value.push_back('(');
	value += this->coefficient.GetValue();
	value.push_back(')');
	value.push_back('*');
	
	//����
	value.push_back('(');
	value += this->number.GetValue();
	value.push_back(')');
	value.push_back('^');

	//ָ��
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

	//��ָ��������0��ʱ����ʾ�����Լ�ָ��
	if (this->exponential != Fraction<sint64>())
	{
		if (this->coefficient != Fraction<sint64>(1, 1))
		{
			//��ϵ��������1��ʱ����ʾ�˺�
			value.push_back('\40');
			value.push_back('*');
			value.push_back('\40');
		}

		//���ϵ��������1�����ҵ�������������1�������������š�
		//���ָ��������1�����ҵ�������������1��������������
		//�����������治������
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

//�����Numberָ�����֣�������ָ��ת��ΪFraction<sint64>����
bool Polynomial_Exponential::IsNumber()
{
	//���ҽ��������Ǵ����ֵ�ʱ�����������Ǵ�����
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

	//������ȿ������
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

	//������ȿ������
	if (this->number == val.number)
	{
		result.coefficient = this->coefficient / val.coefficient;
		result.number = this->number;
		result.exponential = this->exponential - val.exponential;
	}

	result.error = true; //˵�������Խ����������
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
