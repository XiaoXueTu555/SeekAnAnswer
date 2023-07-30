#pragma once
/*数学表达式处理
*用于处理数学表达式相关的函数*/

#include <string>

//判断该字符串的括号语法是否正确
bool ParenthesisSyntax(std::string val);

//删除外围的括号
std::string DeleteCircumjacentParentheses(std::string val);

//删除字符串中的空格
std::string DeleteTheBlank(std::string val);

/// <summary>
/// 判断该字符是否在括号里
/// </summary>
/// <param name="val"> 字符所在字符串 </param>
/// <param name="i"> 字符所在字符串的下标 </param>
/// <returns> bool 当在括号里时返回true，否则返回false </returns>
bool CharacterInParentheses(std::string val, suint64 i);
