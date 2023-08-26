#include "file_io.h"

//�޳��ַ������˵Ŀհ��ַ�
std::string trimString(const std::string& str) {
	std::string trimmedStr = str;
	size_t start = trimmedStr.find_first_not_of(" \t\n\r"); // ���ҵ�һ���ǿհ��ַ���λ��
	size_t end = trimmedStr.find_last_not_of(" \t\n\r"); // �������һ���ǿհ��ַ���λ��

	if (start == std::string::npos || end == std::string::npos) {
		// �ַ���ֻ�����հ��ַ�
		return "";
	}

	trimmedStr = trimmedStr.substr(start, end - start + 1); // ��ȡ�ǿհ��ַ��Ӵ�
	return trimmedStr;
}


bool TextMap::ReadLanguageFile(std::string file_name)
{
#ifdef FILE_IO_INI
	//���������
	int error = 0;
#endif // FILE_IO_INI

#ifdef FILE_IO_INI
	//��������ļ���
	std::cout << "\033[32;4mRead language file: \033[0m" <<
		file_name << std::endl;
#endif // FILE_IO_INI


	std::ifstream file;
	std::string line;

	//���ļ�
	file.open("Language/" + file_name);

	//������ļ�ʧ���򷵻ش���
	if (!file.is_open())
	{
#ifdef FILE_IO_INI
		//���������Ϣ�����ļ�ʧ��
		std::cout << "\033[31;4m[ERROR]Failed to read language file. \033[0m"
			<< "The file is : " << file_name 
			<< std::endl;

		++error;
#endif // FILE_IO_INI
		return false;
	}
	
	//���ж�ȡ�ַ�����Ϣ
	while (std::getline(file, line))
	{
		//ÿ���ı��ĸ�ʽ��key = Text

		if (line.size() > 0)
		{
			if (line.at(0) == '#')
				continue;
		}
		else
		{
			continue;
		}

		//�ָ���
		std::size_t delimiter_pos = line.find('=');
		std::string key = trimString(line.substr(0, delimiter_pos));
		std::string value = trimString(line.substr(delimiter_pos + 1, line.size() - 1));

		//û�ҵ���key
		if (this->text_map.find(key) == this->text_map.end())
		{
#ifdef FILE_IO_INI
			//�������
			std::cout << "\033[31;4m[ERROR]A key that does not exist was read. \033[0m"
				<<"The file is: "<< file_name << "\t" 
				<< "The key is: " << key << std::endl;

			++error;
#endif // FILE_IO_INI
		}
		//���ҵ���key
		else
		{
			//����map����
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


	//��ȡ���
	return true;
}

bool TextMap::ReadKeyFile(std::string file_name)
{
	std::ifstream file;
	std::string line;

	//���ļ�
	file.open("Language/" + file_name);

	//������ļ�ʧ���򷵻ش���
	if (!file.is_open())
	{
		return false;
	}

	//���ж�ȡ�ַ�����Ϣ
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

	//��ȡ���
	return true;
}

const char* TextMap::ShowText(std::string key)
{
	static std::string result;

	result.clear();

	//�����Ӣ��
	if (this->language == 0)
		result = key;
	//����е�key��Ӧ�ķ���ӳ���ǿյ���Ĭ��Ӣ��
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
