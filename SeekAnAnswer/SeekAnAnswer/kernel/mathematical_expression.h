#pragma once
/*��ѧ���ʽ
���ڴ�����ѧ���ʽ��ص�ģ��*/

#include <string>

//�жϸ��ַ����������﷨�Ƿ���ȷ
inline bool ParenthesisSyntax(std::string val);

//ɾ����Χ������
inline std::string DeleteCircumjacentParentheses(std::string val);

//ɾ���ַ����еĿո�
inline std::string DeleteTheBlank(std::string val);

/// <summary>
/// �жϸ��ַ��Ƿ���������
/// </summary>
/// <param name="val"> �ַ������ַ��� </param>
/// <param name="i"> �ַ������ַ������±� </param>
/// <returns> bool ����������ʱ����true�����򷵻�false </returns>
inline bool CharacterInParentheses(std::string val, suint64 i);


bool ParenthesisSyntax(std::string val)
{
	//�����ŵ������������ŵ�����
	sint64 left = 0, right = 0;

	//ͳ���������������ŵ�����
	for (suint64 i = 0; i < val.size(); i++)
	{
		if (val.at(i) == '(')
		{
			++left;
		}
		else if (val.at(i) == ')')
		{
			++right;
		}
	}

	if (left != right)
		return false;

	//���������ŵ������ַ�ɾ��
	for (suint64 i = 0; i < val.size();)
	{
		if (val.at(i) != '(' && val.at(i) != ')')
		{
			val.erase(val.begin() + i);
		}
		else
		{
			++i;
		}
	}

	if (val.size() == 0) return true;
	if (val.at(0) == ')') return false;

	//���Ŷ�����,��Ϊ�����űض���������ֻ����������
	std::vector<bool> nums;

	for (suint64 i = 0; i < val.size(); i++)
	{
		//������������򴴽�������
		if (val.at(i) == '(')
		{
			nums.push_back(false);
		}
		//������������űض���������
		else
		{
			//�ж��ڴ˴�ǰ�Ƿ��Ѿ��������Ŷ�Ӧ
			bool up_to_standard = true;
			for (sint64 i = nums.size() - 1; i >= 0; i--)
			{
				//���û�ж�Ӧ��������
				if (!nums.at(i))
				{
					//û���������Ŷ�Ӧ
					up_to_standard = false;
					nums.at(i) = true;
					break;
				}
			}
			//����Ѿ����������������������﷨
			if (up_to_standard) return false;
		}
	}

	return true;
}

std::string DeleteCircumjacentParentheses(std::string val)
{
	val = DeleteTheBlank(val);

	//��������������﷨��ȥ����
	if (!ParenthesisSyntax(val))
		return val;

	if (val.size() == 0) return val;

	while (val.at(0) == '(' && val.at(val.size() - 1) == ')')
	{
		val.erase(val.begin());
		val.erase(val.begin() + val.size() - 1);

		//���ɾ�������������﷨
		if (!ParenthesisSyntax(val))
		{
			val.insert(val.begin(), '(');
			val.push_back(')');
			return val; //˵���Ѿ�����Χ������ȥ��
		}
	}
	return val;
}

std::string DeleteTheBlank(std::string val)
{
	for (suint64 i = 0; i < val.size();)
	{
		if (val.at(i) == '\40')
		{
			val.erase(val.begin() + i);
		}
		else
		{
			++i;
		}
	}
	return val;
}

bool CharacterInParentheses(std::string val, suint64 i)
{
	sint8 temp_char = 0;

	if(val.size() == 0)
		return false;

	//�ַ����ﲻ���ڸ�Ԫ��
	if (val.size() <= i)
		return false;

	//����������Ѱ�ҵ�i�ַ�ʱֹͣѰ��
	while (val.rfind(val.at(i)) != i)
	{
		//ɾ����һ��
		val.erase(val.begin() + val.rfind(val.at(i)));
	}

	//������������֮��ָ����i�ַ��ض������е�i�ַ������Ҳ�
	//��Ϊ����Ĳ�����ı�i�ַ���λ�ã����Դ˴���¼�ַ�
	temp_char = val.at(i);

	//����������Ѱ�ҵ�i�ַ�ʱֹͣѰ��
	while (val.find(temp_char) != val.rfind(temp_char))
	{
		//ɾ����һ��
		val.erase(val.begin() + val.find(temp_char));
	}

	//���������ַ�����ɾ�������޹��ַ�
	for(suint64 j = 0; j < val.size();)
	{
		if (!(val.at(j) == '(' || val.at(j) == ')' || val.at(j) == temp_char))
		{
			val.erase(val.begin() + j);
		}
		else
		{
			++j;
		}
	}

	//ɾ���޹صģ�����ģ�����
	for (suint64 j = 0; j < val.size();)
	{
		if (j + 1 < val.size())
		{
			if (val.at(j) == '(' && val.at(j + 1) == ')')
			{
				val.erase(val.begin() + j);
				val.erase(val.begin() + j);
				j = 0;
			}
			else
				++j;
		}
		else
			break;
	}

	//������������Ź�������ַ�����������
	if (!ParenthesisSyntax(val))
		return false;

	//�������������ʣ�µ����ֻ��"(^)"�Լ�Ƕ���˺ܶ�����ţ�����"^"
	//��ˣ����ֻ��һ��Ԫ������������
	if (val.size() == 1)
		return false;

	//������������
	return true;
}
