#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#define FILE_IO_INI

/// <summary>
/// TextMap �ı�ӳ��
/// Key   : �ַ����ĺ��壨��˼��
/// value : ��ͬ���Ե��ַ���
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
	/// ��ȡ�����ļ�
	/// </summary>
	/// <param name="file_name"></param>
	/// <param name="language_text"></param>
	/// <returns> ��ȡ�Ƿ�ɹ� </returns>
	bool ReadLanguageFile(std::string file_name);

	/// <summary>
	/// ��ȡkey_list�ļ�
	/// </summary>
	/// <param name="file_name"></param>
	/// <param name="language_text"></param>
	/// <returns> ��ȡ�Ƿ�ɹ� </returns>
	bool ReadKeyFile(std::string file_name);

	/// <summary>
	/// ��������������key�����ͬ���ַ���
	/// </summary>
	/// <param name="key"></param>
	/// <param name="language_text"></param>
	/// <returns></returns>
	const char* ShowText(std::string key);
};
