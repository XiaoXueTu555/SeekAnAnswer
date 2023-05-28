#pragma once
/*数学表达式
用于处理数学表达式相关的模块*/

#include <string>

//判断该字符串的括号语法是否正确
inline bool ParenthesisSyntax(std::string val);

//删除外围的括号
inline std::string DeleteCircumjacentParentheses(std::string val);

//删除字符串中的空格
inline std::string DeleteTheBlank(std::string val);

/// <summary>
/// 判断该字符是否在括号里
/// </summary>
/// <param name="val"> 字符所在字符串 </param>
/// <param name="i"> 字符所在字符串的下标 </param>
/// <returns> bool 当在括号里时返回true，否则返回false </returns>
inline bool CharacterInParentheses(std::string val, suint64 i);


bool ParenthesisSyntax(std::string val)
{
	//左括号的数量，右括号的数量
	sint64 left = 0, right = 0;

	//统计左括号与右括号的数量
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

	//将除了括号的所有字符删除
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

	//括号对数组,因为左括号必定存在所以只保存右括号
	std::vector<bool> nums;

	for (suint64 i = 0; i < val.size(); i++)
	{
		//如果是左括号则创建布尔对
		if (val.at(i) == '(')
		{
			nums.push_back(false);
		}
		//如果不是左括号必定是右括号
		else
		{
			//判断在此处前是否已经满足括号对应
			bool up_to_standard = true;
			for (sint64 i = nums.size() - 1; i >= 0; i--)
			{
				//如果没有对应的右括号
				if (!nums.at(i))
				{
					//没有满足括号对应
					up_to_standard = false;
					nums.at(i) = true;
					break;
				}
			}
			//如果已经满足条件，则不满足括号语法
			if (up_to_standard) return false;
		}
	}

	return true;
}

std::string DeleteCircumjacentParentheses(std::string val)
{
	val = DeleteTheBlank(val);

	//如果不符合括号语法则不去括号
	if (!ParenthesisSyntax(val))
		return val;

	if (val.size() == 0) return val;

	while (val.at(0) == '(' && val.at(val.size() - 1) == ')')
	{
		val.erase(val.begin());
		val.erase(val.begin() + val.size() - 1);

		//如果删除后不满足括号语法
		if (!ParenthesisSyntax(val))
		{
			val.insert(val.begin(), '(');
			val.push_back(')');
			return val; //说明已经将外围的括号去除
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

	//字符串里不存在该元素
	if (val.size() <= i)
		return false;

	//当从右向左寻找到i字符时停止寻找
	while (val.rfind(val.at(i)) != i)
	{
		//删除这一项
		val.erase(val.begin() + val.rfind(val.at(i)));
	}

	//经过上述处理之后，指定的i字符必定在所有的i字符的最右侧
	//因为下面的操作会改变i字符的位置，所以此处记录字符
	temp_char = val.at(i);

	//当从左向右寻找到i字符时停止寻找
	while (val.find(temp_char) != val.rfind(temp_char))
	{
		//删除这一项
		val.erase(val.begin() + val.find(temp_char));
	}

	//遍历整个字符串，删除所有无关字符
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

	//删除无关的（多余的）括号
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

	//如果不符合括号规则，则该字符不在括号里
	if (!ParenthesisSyntax(val))
		return false;

	//按照上述处理后，剩下的情况只有"(^)"以及嵌套了很多层括号，或者"^"
	//因此，如果只有一个元素则在括号外
	if (val.size() == 1)
		return false;

	//否则在括号里
	return true;
}
