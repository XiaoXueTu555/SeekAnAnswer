#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#define FILE_IO_INI

/// <summary>
/// TextMap 文本映射
/// Key   : 字符串的含义（意思）
/// value : 不同语言的字符串
/// </summary>
class TextMap
{
public:
	std::map<std::string, std::string> text_map;
	int language;
public:
	TextMap();
public:
	/// <summary>
	/// 读取语言文件
	/// </summary>
	/// <param name="file_name"></param>
	/// <param name="language_text"></param>
	/// <returns> 读取是否成功 </returns>
	bool ReadLanguageFile(std::string file_name);

	/// <summary>
	/// 读取key_list文件
	/// </summary>
	/// <param name="file_name"></param>
	/// <param name="language_text"></param>
	/// <returns> 读取是否成功 </returns>
	bool ReadKeyFile(std::string file_name);

	/// <summary>
	/// 根据语言设置与key输出不同的字符串
	/// </summary>
	/// <param name="key"></param>
	/// <param name="language_text"></param>
	/// <returns></returns>
	const char* ShowText(std::string key);
};
