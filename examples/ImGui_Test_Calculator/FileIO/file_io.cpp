#include "file_io.h"

//剔除字符串两端的空白字符
std::string trimString(const std::string& str) {
	std::string trimmedStr = str;
	size_t start = trimmedStr.find_first_not_of(" \t\n\r"); // 查找第一个非空白字符的位置
	size_t end = trimmedStr.find_last_not_of(" \t\n\r"); // 查找最后一个非空白字符的位置

	if (start == std::string::npos || end == std::string::npos) {
		// 字符串只包含空白字符
		return "";
	}

	trimmedStr = trimmedStr.substr(start, end - start + 1); // 提取非空白字符子串
	return trimmedStr;
}


bool TextMap::ReadLanguageFile(std::string file_name)
{
#ifdef FILE_IO_INI
	//报错的数量
	int error = 0;
#endif // FILE_IO_INI

#ifdef FILE_IO_INI
	//输出所读文件名
	std::cout << "\033[32;4mRead language file: \033[0m" <<
		file_name << std::endl;
#endif // FILE_IO_INI


	std::ifstream file;
	std::string line;

	//读文件
	file.open("Language/" + file_name);

	//如果打开文件失败则返回错误
	if (!file.is_open())
	{
#ifdef FILE_IO_INI
		//输出报错信息：打开文件失败
		std::cout << "\033[31;4m[ERROR]Failed to read language file. \033[0m"
			<< "The file is : " << file_name 
			<< std::endl;

		++error;
#endif // FILE_IO_INI
		return false;
	}
	
	//逐行读取字符串信息
	while (std::getline(file, line))
	{
		//每行文本的格式：key = Text

		if (line.size() > 0)
		{
			if (line.at(0) == '#')
				continue;
		}
		else
		{
			continue;
		}

		//分隔符
		std::size_t delimiter_pos = line.find('=');
		std::string key = trimString(line.substr(0, delimiter_pos));
		std::string value = trimString(line.substr(delimiter_pos + 1, line.size() - 1));

		//没找到该key
		if (this->text_map.find(key) == this->text_map.end())
		{
#ifdef FILE_IO_INI
			//输出报错
			std::cout << "\033[31;4m[ERROR]A key that does not exist was read. \033[0m"
				<<"The file is: "<< file_name << "\t" 
				<< "The key is: " << key << std::endl;

			++error;
#endif // FILE_IO_INI
		}
		//查找到该key
		else
		{
			//更改map内容
			this->text_map[key] = value;
		}
	}

#ifdef FILE_IO_INI
	if (error == 0)
	{
		std::cout << "\033[32;4mRead file finished, no error: \033[0m" 
			<< file_name << std::endl;
	}
	else
	{
		std::cout << "\033[31;4mRead file finished, \033[0m"
			<< error << " \033[31;4merror\033[0m " << "The file is: "
			<< file_name << std::endl;
	}
#endif // FILE_IO_INI


	//读取完毕
	return true;
}

bool TextMap::ReadKeyFile(std::string file_name)
{
	std::ifstream file;
	std::string line;

	//读文件
	file.open("Language/" + file_name);

	//如果打开文件失败则返回错误
	if (!file.is_open())
	{
		return false;
	}

	//逐行读取字符串信息
	while (std::getline(file, line))
	{
		if (line.size() > 0)
		{
			if (line.at(0) == '#')
				continue;
		}
		else
		{
			continue;
		}

		this->text_map.insert(std::pair<std::string, std::string>(trimString(line), ""));
	}

	//读取完毕
	return true;
}

const char* TextMap::ShowText(std::string key)
{
	static std::string result;

	result.clear();

	//如果是英文
	if (this->language == 0)
		result = key;
	//如果有的key对应的翻译映射是空的则默认英文
	else if(this->text_map.find(key)->second.empty())
	{
#ifdef FILE_IO_INI
		std::cout << "\033[31;4m[ERROR][ShowText]No mapping for this key was found!\033[0m"
			<< "The key is: " << key << std::endl;
#endif // FILE_IO_INI

		result = key;
	}
	else
		result = this->text_map.find(key)->second;

	return result.c_str();
}

TextMap::TextMap()
{
	this->language = 0;
}
