#pragma once
/*数学表达式
用于处理数学表达式相关的模块*/

#include <string>

//判断该字符串的括号语法是否正确
inline bool ParenthesisSyntax(std::string val);

//删除外围的括号
inline std::string DeleteCircumjacentParentheses(std::string val);


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
