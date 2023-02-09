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

	//�ӵ�ǰλ�ñ������Ⱥ�֮ǰ
	for (i; value.at(i) != '='; i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		//��ȡδ֪��ϵ��
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

		//����δ֪��ϵ��
		this->coefficient_left.push_back(Polynomial(container));
		container.clear();

		i += 4; //��λ��ָ��

		//��ȡδ֪��ָ��
		for (i; value.at(i) != ')'; i++)
		{
			container.push_back(value.at(i));
		}
		//����δ֪��ָ��
		this->exponent_left.push_back(Fraction<sint64>(container));
		container.clear();
	}

	i += 2; //��λ���Ⱥ��Ҳ��һ��δ֪��ϵ����

	for (i; i != value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		//��ȡδ֪��ϵ��
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

		//����δ֪��ϵ��
		this->coefficient_right.push_back(Polynomial(container));
		container.clear();

		i += 4; //��λ��ָ��

		//��ȡδ֪��ָ��
		for (i; value.at(i) != ')'; i++)
		{
			container.push_back(value.at(i));
		}
		//����δ֪��ָ��
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

	//�ӵ�ǰλ�ñ������Ⱥ�֮ǰ
	for (i; i < value.find('='); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			((value.find('X', i) != std::string::npos)
				&& (value.find('+', value.find('X', i)) != std::string::npos)
				&& (value.find('+', value.find('X', i)) < value.find('=')))
			? value.find('+', value.find("X", i)) //�����X���Һ����С�+���򷵻ظ÷����±�
			: (value.find('X', i) != std::string::npos) ? value.find('=') //��X����û�С�+���򷵻ء�=���±�
			: (value.find('+', i) != std::string::npos) ? value.find('+', i) : value.find('=');

		//��ȡ��һ��
		for (i; i < node_location; i++)
		{
			if (value.at(i) == '\40') continue;

			container.push_back(value.at(i));
		}

		//������һ��

		//���û�ҵ�X��־��˵������һ��������
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

		//��ȡ����ʽa
		for (suint64 j = 0;
			j < ((container.find('*') != std::string::npos)
				? container.find('*')
				: container.find('X'));
			j++)
		{
			temp_val.push_back(container.at(j));
		}

		/******�������ʽa*******/
		  //˵������ʽaΪ1
		if (temp_val.empty())
		{
			temp.Input("1");
			this->coefficient_left.push_back(temp);
		}
		//�������ʽa����λ��ĩβû�ҵ����ţ�˵������ʽa��Χû������
		else if (temp_val.at(0) != '(' || temp_val.at(temp_val.size() - 1) != ')')
		{
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//�������ʽa����λ��ĩβ���ظ����ţ�˵������ʽa��Χ������
		else if (temp_val.at(1) == '(' || temp_val.at(temp_val.size() - 2) == ')')
		{
			temp_val.erase(0);
			temp_val.erase(temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//��������ĵ�һ�������Ų��������������˵������ʽa��Χû������
		else if (temp_val.rfind('(') != 0)
		{
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}
		//���һ���������ʽa��Χ������
		else
		{
			temp_val.erase(temp_val.begin() + 0);
			temp_val.erase(temp_val.begin() + temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_left.push_back(temp);
		}

		temp_val.clear(); //��ն���ʽa

		//��ȡָ��n
		//���X�����һλ����ָ��Ϊ1
		if (container.find('X') == container.size() - 1)
		{
			this->exponent_left.push_back(Fraction<sint64>(1, 1));
		}
		//ָ��ָ����С
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

	//�ӵ�ǰλ�ñ��������
	for (i; i < value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			((value.find('X', i) != std::string::npos)
				&& (value.find('+', value.find('X', i)) != std::string::npos))
			? value.find('+', value.find("X", i)) //�����X���Һ����С�+���򷵻ظ÷����±�
			: (value.find('X', i) != std::string::npos) ? value.size() //��X����û�С�+���򷵻ؽ�β
			: (value.find('+', i) != std::string::npos) ? value.find('+', i) : value.size();

		//��ȡ��һ��
		for (i; i < node_location; i++)
		{
			if (value.at(i) == '\40') continue;

			container.push_back(value.at(i));
		}

		//������һ��

		//���û�ҵ�X��־��˵������һ��������
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

		//��ȡ����ʽa
		for (suint64 j = 0;
			j < ((container.find('*') != std::string::npos)
				? container.find('*')
				: container.find('X'));
			j++)
		{
			temp_val.push_back(container.at(j));
		}

		/******�������ʽa*******/
		  //˵������ʽaΪ1
		if (temp_val.empty())
		{
			temp.Input("1");
			this->coefficient_right.push_back(temp);
		}
		//�������ʽa����λ��ĩβû�ҵ����ţ�˵������ʽa��Χû������
		else if (temp_val.at(0) != '(' || temp_val.at(temp_val.size() - 1) != ')')
		{
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//�������ʽa����λ��ĩβ���ظ����ţ�˵������ʽa��Χ������
		else if (temp_val.at(1) == '(' || temp_val.at(temp_val.size() - 2) == ')')
		{
			temp_val.erase(0);
			temp_val.erase(temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//��������ĵ�һ�������Ų��������������˵������ʽa��Χû������
		else if (temp_val.rfind('(') != 0)
		{
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}
		//���һ���������ʽa��Χ������
		else
		{
			temp_val.erase(temp_val.begin() + 0);
			temp_val.erase(temp_val.begin() + temp_val.size() - 1);
			temp.Input(temp_val);
			this->coefficient_right.push_back(temp);
		}

		temp_val.clear(); //��ն���ʽa

		//��ȡָ��n
		//���X�����һλ����ָ��Ϊ1
		if (container.find('X') == container.size() - 1)
		{
			this->exponent_right.push_back(Fraction<sint64>(1, 1));
		}
		//ָ��ָ����С
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
	//�Ⱥ����
	for (suint64 i = 0; i < this->coefficient_left.size(); i++)
	{
		/*ϵ��*/
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

		/*δ֪��*/
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

	//�Ⱥ�
	result.push_back('\40');
	result.push_back('=');
	result.push_back('\40');

	//�Ⱥ��Ҳ�
	for (suint64 i = 0; i < this->coefficient_right.size(); i++)
	{
		/*ϵ��*/
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

		/*δ֪��*/
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

	//���û�ҵ��������c����0
	if (this->FindDegree(Fraction<sint64>(0, 1)) != -1)
	{
		c = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(0, 1)));
	}

	//���û�ҵ�һ�����b����0
	if (this->FindDegree(Fraction<sint64>(1, 1)) != -1)
	{
		b = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(1, 1)));
	}

	//��������Ϊ����һ����Կ϶�����
	a = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(2, 1)));

	/*����X1*/
	//-b
	result.at(0).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//�����б�ʽ
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
	/*����X2*/
	//-b
	result.at(1).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//�������б�ʽ
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
			//������������������0������л�������
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
