#include "equation.h"
#include "polynomial.h"
#include "polynomial_exponential.h"
#include "../../High/Public/High_int.h"

/*	��ʼ��Prime���е�������
 *	����ʹ����C++11�е��б��ʼ���������������֧�֣�
	����ʹ��������ʼ���������档 */
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
	suint64 start = 0; //��¼����ÿһ�����ʼλ��

	//�ӵ�ǰλ�ñ������Ⱥ�֮ǰ
	for (i; i < value.find('='); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			//�ж��Ƿ���ڼӺ��ڵȺ�֮ǰ
			(value.find('+', i) != std::string::npos &&
				value.find('+', i) < value.find('='))
			//��������򷵻ظ����λ�ã����򷵻صȺŵ�λ��
			? value.find('+', i) : value.find('=');

		//��ȫ���ж�
		if (node_location < value.size())
		{
			//����ڵ���Χ��������������Ч�ڵ�
			if (CharacterInParentheses(value, node_location))
				continue;
		}

		//��ȡ��ʼλ��
		i = start;
		//��¼��һ�����ʼλ��
		start = node_location + 1;

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
	start = value.find('=') + 1; //������ʼλ��

	//�ӵ�ǰλ�ñ��������
	for (i; i < value.size(); i++)
	{
		if (value.at(i) == '\40') continue;
		if (value.at(i) == '+') continue;

		suint64 node_location =
			//�ж��Ƿ���ڼӺ��ڽ�β֮ǰ
			(value.find('+', i) != std::string::npos)
			//��������򷵻ظ����λ�ã����򷵻صȺŵ�λ��
			? value.find('+', i) : value.size();

		//��ȫ���ж�
		if (node_location < value.size())
		{
			//����ڵ���Χ��������������Ч�ڵ�
			if (CharacterInParentheses(value, node_location))
				continue;
		}

		//��ȡ��ʼλ��
		i = start;
		//��¼��һ�����ʼλ��
		start = node_location + 1;

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

		Polynomial temp = (b * b) - (Polynomial(Monomial("(4/1)")) * a * c);

		if (!temp.IsNumber()) 
			return -1; //�÷��̿ɽ⣬�����Ǵ����ַ���

		//����б�ʽ����0
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

	return -2; //�÷����޷�ʹ��SeekAnAnswer���
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

	//��С������
	Fraction<sint64> least_common_multiple = 
		this->root1.a.LeastCommonMultiple_of_coe(this->root1.b);

	this->root1.a *= Monomial(least_common_multiple);
	this->root1.b *= Monomial(least_common_multiple);

	//��������ĸ�Ĺ���ʽ
	Monomial common_factor = this->root1.a.CommonFactor(this->root1.b);
	//�ֱ���Թ���ʽ�Ի���
	this->root1.a /= common_factor;
	this->root1.b /= common_factor;

	//�����ĸ�ķ�ʽ�ķ�����-1���򻯼�
	if (root1.b.IsNumber() && root1.b.list.at(0).GetCoefficient().a == -1)
	{
		root1.a *= Monomial("(-1/1)");
		this->root1.b.list.at(0).SetCoefficientA(1);
	}
}

void Equation::quadratic_equation_in_one_unknown()
{
	Polynomial a, b, c; //δ֪����ϵ��
	Polynomial_Exponential Discriminant; //�����б�ʽ

	this->Unite_like_terms();
	this->ShiftItem();

	this->root2.clear();
	this->root2.push_back(Fraction<Polynomial_Exponential_Sum>()); //X1
	this->root2.push_back(Fraction<Polynomial_Exponential_Sum>()); //X2

	//����ҵ��˳������ֵ //û�ҵ��������c = 0
	if (this->FindDegree(Fraction<sint64>(0, 1)) != -1)
	{
		c = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(0, 1)));
	}

	//����ҵ���һ�����ֵ //���û�ҵ�һ�����b����0
	if (this->FindDegree(Fraction<sint64>(1, 1)) != -1)
	{
		b = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(1, 1)));
	}

	//��������Ϊ����һ����Կ϶�����
	a = this->coefficient_left.at(this->FindDegree(Fraction<sint64>(2, 1)));

	//��������б�ʽ ���ڿɻ���ʱ����
	Discriminant = Simplest_radical(Polynomial_Exponential(
		Fraction<sint64>(1, 1),
		(b * b) + (Polynomial("(-4/1)") * a * c),
		Fraction<sint64>(1, 2))
	);

	/*����X1*/
	//-b
	this->root2.at(0).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//�����б�ʽ
	this->root2.at(0).a.Push(Discriminant);

	//2a
	this->root2.at(0).b.Push(Polynomial_Exponential(
		Polynomial("(2/1)") * a
	));

	this->root2.at(0).a.DeleteZero();

	//////////////////////////////////////////////
	/*����X2*/
	//-b
	this->root2.at(1).a.Push(Polynomial_Exponential(
		(Polynomial_Exponential)(Polynomial("(-1/1)") * b)
	));

	//�����б�ʽ���෴��
	Discriminant.coefficient.a = 0 - Discriminant.coefficient.a;

	//�������б�ʽ �����б�ʽ
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
			//�����ĸ���������ӣ�����л�������
			if (((Polynomial)this->root2.at(i).a % (Polynomial)this->root2.at(i).b) == Polynomial())
			{
				this->root2.at(i).a = (Polynomial_Exponential_Sum)
					(Polynomial_Exponential)
					((Polynomial)this->root2.at(i).a / (Polynomial)this->root2.at(i).b);

				this->root2.at(i).b = Polynomial_Exponential_Sum(Polynomial_Exponential(Fraction<sint64>(1, 1),
					Polynomial(Monomial("(1/1)")), Fraction<sint64>(1, 1)));
			}
			//������Ӳ�Ϊ0����Լ��
			if ((Polynomial)this->root2.at(i).a != Polynomial("(0/1)"))
			{
				//����С������
				Fraction<sint64> LeastCommonMultiple =
					((Polynomial)this->root2.at(i).a).LeastCommonMultiple_of_coe((Polynomial)this->root2.at(i).b);

				this->root2.at(i).a = 
					(Polynomial_Exponential)(((Polynomial)this->root2.at(i).a) * Monomial(LeastCommonMultiple));

				this->root2.at(i).b =
					(Polynomial_Exponential)(((Polynomial)this->root2.at(i).b) * Monomial(LeastCommonMultiple));

				//�����Լ��
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

		//���ϵ����Ϊ���滯����׼��
		this->root2.at(i).a.Split();
		this->root2.at(i).b.Split();

		//���ϵ����Ϊ0
		if (this->root2.at(i).a.list.at(0).coefficient != Fraction<sint64>(0, 1))
		{
			std::vector<sint64> a; Fraction<sint64> temp_a(1, 1);
			std::vector<sint64> b;
			//��ȡ������
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

			//������С������
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				//���ӳ��Թ�����
				this->root2.at(i).a.list.at(j).coefficient =
					this->root2.at(i).a.list.at(j).coefficient * 
					(temp_a / Fraction<sint64>(Fraction<sint64>::gcds(a), 1));
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				//��ĸ���Թ�����
				this->root2.at(i).b.list.at(j).coefficient =
					this->root2.at(i).b.list.at(j).coefficient * 
					(temp_a / Fraction<sint64>(Fraction<sint64>::gcds(a), 1));
			}


			//��ȡ������
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				b.push_back(this->root2.at(i).a.list.at(j).coefficient.a);
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				b.push_back(this->root2.at(i).b.list.at(j).coefficient.a);
			}

			//�������Լ��
			for (suint64 j = 0; j < this->root2.at(i).a.list.size(); j++)
			{
				//���ӳ��Թ�Լ��
				this->root2.at(i).a.list.at(j).coefficient =
					this->root2.at(i).a.list.at(j).coefficient /
					Fraction<sint64>(Fraction<sint64>::gcds(b), 1);
			}
			for (suint64 j = 0; j < this->root2.at(i).b.list.size(); j++)
			{
				//��ĸ���Թ�Լ��
				this->root2.at(i).b.list.at(j).coefficient =
					this->root2.at(i).b.list.at(j).coefficient /
					Fraction<sint64>(Fraction<sint64>::gcds(b), 1);
			}
		}

		//��ϵ���ϲ��ص���
		this->root2.at(i).a.Merge();
		this->root2.at(i).b.Merge();
	}
	return;
}

bool Equation::IsValid(std::string val)
{
	if (val.find('=') == std::string::npos) return false;

	std::string left, right;
	suint64 start = 0; //��¼����ÿһ�����ʼλ��

	//�ӵ�ǰ�������Ⱥ�֮ǰ
	for (suint64 i = 0; i < val.find('='); i++)
	{
		if (val.at(i) == '\40') continue;
		left.push_back(val.at(i));
	}

	//�ӵȺű��������
	for (suint64 i = val.find('=') + 1; i < val.size(); i++)
	{
		if (val.at(i) == '\40') continue;
		right.push_back(val.at(i));
	}
	val.clear();

	std::string container; //����

	here:

	start = 0; //������ʼλ��

	for (suint64 i = 0; i < left.size(); i++)
	{
		if (left.at(i) == '\40') continue;
		if (left.at(i) == '+') continue;

		val.clear();
		container.clear();

		suint64 node_location =
			//�ж��Ƿ���ڼӺ��ڽ�β֮ǰ
			(left.find('+', i) != std::string::npos)
			//��������򷵻ظ����λ�ã����򷵻ؽ�β��λ��
			? left.find('+', i) : left.size();

		//��ȫ���ж�
		if (node_location < left.size())
		{
			//����ڵ���Χ��������������Ч�ڵ�
			if (CharacterInParentheses(left, node_location))
				continue;
		}

		//��ȡ��ʼλ��
		i = start; 
		//��¼��һ�����ʼλ��
		start = node_location + 1;

		//��ȡ��һ��
		for (i; i < node_location; i++)
		{
			if (left.at(i) == '\40') continue;

			container.push_back(left.at(i));
		}

		//������һ��

		//���û�ҵ�X��־��˵������һ��������
		if (container.find('X') == std::string::npos)
		{
			if (!Polynomial::IsValid(container))
				return false;
			continue;
		}

		//��ȡ����ʽa
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

		//��ȡδ֪��
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

	//���val��ֵΪ0
	if (val.a.list.size() == 1 && val.a.list.at(0) == Monomial())
	{
		value += val.a.Out();
		return value;
	}

	//��������Ƕ���ʽ����b������1
	if (val.a.list.size() > 1 && 
		!(val.b.IsNumber() && val.b.list.at(0).GetCoefficient().a == 1 && 
			val.b.list.at(0).GetCoefficient().b == 1)
		)
	{
		value.push_back('(');
	}

	value += val.a.Out();

	//��������Ƕ���ʽ����b������1
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

		//�����ĸ�Ƕ���ʽ
		if (val.b.list.size() > 1)
		{
			value.push_back('(');
		}

		value += val.b.Out();

		//�����ĸ�Ƕ���ʽ
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
	sint64 a = 0; //��Ȼ��
	sint64 aj = 0; //���ֽ��ʣ������
	std::vector<sint64> k; //�ֽ����k��������
	Prime ki; //���ڷֽ��k����
	sint64 g = 1; //���в���ȵ�k���ӵĳ˻�
	//���a������Ȼ��
	if (!(val.number.IsNumber() && val.number.list.at(0).GetCoefficient().IsInteger()
		&& val.number.list.at(0).GetCoefficient().a >= 0))
	{
		result.error = true;
		return result;
	}

	//���ָ������1/2
	if (val.exponential != Fraction<sint64>(1, 2))
	{
		result.error = true;
		return result;
	}

	/*��ʼ����*/
	//��ֵ
	a = ((Monomial)(val.number)).GetCoefficient().a;

	//���a = 0
	if (a == 0)
	{
		result.exponential.b = 1;
		result.number.list.at(0).SetCoefficient(0, 1);
		return result;
	}

	//���a = 1
	if (a == 1)
	{
		result.coefficient.a = 1;
		result.coefficient.b = 1;
		result.exponential.b = 1;
		result.number.list.at(0).SetCoefficient(1, 1);
		return result;
	}

	//���a������
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
			k.push_back(ki.n); //��¼�ֽ�����
			aj /= ki.n; //�ֽ���µ�aj
		}
	}

	//��ʣ���aj�����k��������
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

	//����k��������
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
	//���g=1��˵��������Ϊ����
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
	//���val���Ǵ����֣����޷�����
	if (!val.IsNumber())
	{
		return val;
	}

	//��ֵ����ȡ��a
	a = ((Monomial)val.number).GetCoefficient();

	//���aС��0�����޷����������
	if (a < Fraction<sint64>(0, 1))
	{
		return val;
	}

	//���a����������ֱ�ӻ���
	if (a.IsInteger())
	{
		return Simplest_radical_AnBo(val);
	}

	result.coefficient = val.coefficient * Fraction<sint64>(1, a.b);
	result.number = Fraction<sint64>(a.a * a.b, 1);
	result.exponential.b = 2;
	return Simplest_radical_AnBo(result);
}
