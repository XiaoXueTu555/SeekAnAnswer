#pragma once
/*��ѧ���ʽ
���ڴ�����ѧ���ʽ��ص�ģ��*/

#include <string>

//�жϸ��ַ����������﷨�Ƿ���ȷ
inline bool ParenthesisSyntax(std::string val);

//ɾ����Χ������
inline std::string DeleteCircumjacentParentheses(std::string val);


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
