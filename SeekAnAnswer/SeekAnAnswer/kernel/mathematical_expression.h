#pragma once
/*��ѧ���ʽ����
*���ڴ�����ѧ���ʽ��صĺ���*/

#include <string>

//�жϸ��ַ����������﷨�Ƿ���ȷ
bool ParenthesisSyntax(std::string val);

//ɾ����Χ������
std::string DeleteCircumjacentParentheses(std::string val);

//ɾ���ַ����еĿո�
std::string DeleteTheBlank(std::string val);

/// <summary>
/// �жϸ��ַ��Ƿ���������
/// </summary>
/// <param name="val"> �ַ������ַ��� </param>
/// <param name="i"> �ַ������ַ������±� </param>
/// <returns> bool ����������ʱ����true�����򷵻�false </returns>
bool CharacterInParentheses(std::string val, suint64 i);
