#include "monomial.h"
#include "../../High/Public/High_int.h"
#include "../../High/Public/High_float.h"
#include "../../High/Public/High_io.h"

#include <sstream>
#include <iostream>

//��ĸ�����ĵ�����
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

Fraction<sint64> Monomial::GetDegree() //��ô˵���ʽ�Ĵ���
{
	Fraction<sint64> degree(0, 1);
	for (std::map<sint8, Fraction<sint64>>::iterator i = this->factor.begin(); i != this->factor.end(); i++)
	{
		degree += i->second;
	}
	return degree;
}

Fraction<sint64> Monomial::GetCoefficient() //��ô˵���ʽ��ϵ��
{
	return this->coefficient;
}

std::map<sint8, Fraction<sint64>>& Monomial::GetFactor() //��ô˵���ʽ����ĸ����
{
	return this->factor;
}

void Monomial::SetValue(Monomial value) //���õ���ʽ��ֵ
{
	this->coefficient = value.coefficient;
	this->factor = value.factor;
	this->DelOne();
}

void Monomial::SetValue(std::string std_value)
{
	std::string container; //����
	Fraction<sint64> exponent; //ָ��(��ʱ����)
	sint8 letter = 0; //��ĸϵ��(��ʱ����)
	suint64 i = 0;

	/*������������*/
	this->coefficient.a = 0;
	this->coefficient.b = 0;
	this->Clear_factor();

	//��ȡ����ϵ������
	while (std_value.at(++i) != ')')
	{
		container.push_back(std_value.at(i));
	}

	//�����ȡ������ϵ��
	this->coefficient.SetValue(container);

	container.clear();

	//��ȡ��ĸ��������
	for (i; i < std_value.size(); i++)
	{
		//��'^'Ϊ��־��ȡ��ĸ����
		if (std_value.at(i) == '^')
		{
			letter = std_value.at(i - 1);

			i += 2; //��λ��ָ����

			//��ȡ��ĸ��ָ������
			while (std_value.at(i) != ')')
			{
				container.push_back(std_value.at(i));
				++i;
			}

			//�����ȡ��ָ������
			exponent.SetValue(container);

			//׷����ĸ����
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
		//���ָ�����ֵ���0����ɾ������ĸ��
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

bool Monomial::SimilarItems(Monomial b) //�ж��Ƿ�Ϊͬ����
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
	//����ַ��������������﷨����
	if (!ParenthesisSyntax(val)) return false;
	//ɾ��val��Χ������
	val = DeleteCircumjacentParentheses(val);

	if (val.size() == 0) return false;

	//�ų���������ַ����ϵ��ַ���
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

	std::string container; //����

	//��ȡ����ϵ������
	while (!(val.at(0) >= 'a' && val.at(0) <= 'z' ||
		val.at(0) >= 'A' && val.at(0) <= 'Z'))
	{
		container.push_back(val.at(0));
		val.erase(val.begin());
		if (val.size() == 0) break;
	}

	/*���������жϣ���container��Ԫ������Ϊ0����ض�������ĸ����
	����Ϊ0�����ܴ�����ĸ������Ҳ���ܲ�����*/

	//�������������ϵ����������ϵ������������ͨ�����������㵥��ʽ
	if (!(container.size() == 0 || Fraction<sint64>::IsValid(container)))
		return false;

	//���val��Ԫ������Ϊ0��˵����������ĸ���������㵥��ʽ
	if (val.size() == 0) return true;

	container.clear();

	//ִ�е��˴�˵��������ĸ����

	//���������ĸ��������
	while(val.size() > 0)
	{
		container.push_back(val.at(0));
		val.erase(val.begin());
		//���������һ��Ԫ�أ�������һ��Ԫ�ز�����ĸ����
		if (val.size() > 0)
		{
			if (!(val.at(0) >= 'a' && val.at(0) <= 'z' ||
				val.at(0) >= 'A' && val.at(0) <= 'Z'))
				continue;
		}

		//���� ˵���˴�val.at(0)��һ����ĸ
		container.erase(container.begin());
		//���ɾ����ĸԪ�غ�ĳ���Ϊ0,�����û����
		if (container.size() == 0) continue;
		//���ɾ����ĸԪ�غ󣬴�������Ԫ�أ�����һ��Ǵη������򲻷��ϵ���ʽ
		if (container.at(0) != '^') return false;
		
		//��ʱʣ�µĿ϶��Ǵ��ڴη����ŵ����
		container.erase(container.begin());
		//��ɾ���η����ź�û��Ԫ�أ������㵥��ʽ
		if (container.size() == 0) return false;
		//��ʣ�µ�������֣������㵥��ʽ
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

	std::string container; //����
	Fraction<sint64> exponent; //ָ��(��ʱ����)
	sint8 letter = 0; //��ĸϵ��(��ʱ����)
	suint64 i = 0;

	//����������ݴ�������ϵ��
	if (value.at(0) == '(' 
		|| value.at(0) >= '0' && value.at(0) <= '9' || value.at(0) == '-')
	{
		//�������ϵ��������С����������
		if (value.at(0) == '(')
		{
			while (value.at(++i) != ')')
			{
				container.push_back(value.at(i));
			}

			if (container == "-") container.push_back('1');

			this->coefficient.Input(container);

			container.clear();

			//�����ȡ���
			if (i + 1 >= value.size()) return;
			else ++i; //��λ����ĸ��������
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

			//����ϵ��
			this->coefficient.Input(container);

			container.clear();

			//�����ȡ���
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
			//������ĸ
			letter = value.at(i);

			//Ĭ��Ϊ"1"
			exponent.SetValue("1/1");

			++i;

			//�ж��Ƿ�ɼ�����ȡָ��
			if (i < value.size())
			{
				//�������ָ����־
				if (value.at(i) == '^')
				{
					++i;

					//�ڷ�Χ�ڰ�ȫ��ȡָ��
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

					//����ָ��
					exponent.Input(container);

					container.clear();
				}

				--i;
			}

			//׷����ĸ����
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

	//�������ʽ����0
	if (this->coefficient.a == 0)
	{
		result = this->coefficient.Out();
		return result;
	}

	//���û����ĸ��������
	if (this->factor.size() == 0)
	{
		result += this->coefficient.Out();
		return result;
	}

	//������ֵ���-1
	if (this->coefficient == Fraction<sint64>(-1, 1))
	{
		result.push_back('-');
	}

	//�������ϵ��������1
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
			//˵��ָ����������
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

/*�ӷ���
ע�⣺ʹ��ǰ���ж�a��b�Ƿ�Ϊͬ���
�����ڲ����Զ��жϣ�����Ķ���һ����Ϊͬ����*/
Monomial Monomial::operator+(Monomial b)
{
	Monomial result;
	result.coefficient = this->coefficient + b.coefficient;
	result.factor = this->factor;
	result.DelOne();
	return result;
}

/*������
ע�⣺ʹ��ǰ���ж�a��b�Ƿ�Ϊͬ���
�����ڲ����Զ��жϣ�����Ķ���һ����Ϊͬ����*/
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

	//����˻����ϵ��Ϊ0��0��Ϊ���
	if (result.coefficient.a == 0)
	{
		/*��������Ŀ���ǣ����ܴ���0���κ������˻������
		����д���ĳһ���������ʽ����ĸϵ�������������ط�
		�����ж�0���������ж��Ƿ�Ϊ�����ֵ�ʱ����ܻ�ʧЧ��
		��������ֱ�ӷ��ؽ��*/
		return result;
	}

	result.factor = this->factor;

	FACTOR pos;
	for (FACTOR i = b.factor.begin(); i != b.factor.end(); i++)
	{
		//δ��ѯ������ĸ����
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

/*������
ע�⣺�������Ϊ0���򷵻ش���
�õ��ú����ķ���ֵ�������Monomial::Error()������
�жϸ÷���ֵ�Ƿ��ڴ���״̬*/
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
		//δ��ѯ������ĸ����
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

/*�жϸõ���ʽ�Ƿ��ڴ���״̬��
�����ǰ���ڴ���״̬���򷵻�true��
���򷵻�false*/
bool Monomial::IsError()
{
	return this->error;
}
