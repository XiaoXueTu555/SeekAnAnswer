#pragma once
#include <string>
#include <map>
#include <set>
#include "..\misinttypes\saint.h"
#include "fraction.h"
#include "seek_math.h"
/*����ʽ*/
class Monomial
{
	/*member variable*/
private:
	//����ϵ��
	Fraction<sint64> coefficient;
	//��ĸ����/ÿ����ĸ��ָ��
	std::map<sint8, Fraction<sint64>> factor;
private:
	//�Ƿ��ڴ���״̬
	bool error;

	/*member function*/
public: //���캯��
	Monomial();
	Monomial(std::string std_value);
	Monomial(Fraction<sint64> coefficient,
		std::map<sint8, Fraction<sint64>> factor = std::map<sint8, Fraction<sint64>>());
public:
	Fraction<sint64> GetDegree(); //��ô˵���ʽ�Ĵ���
	Fraction<sint64> GetCoefficient(); //��ô˵���ʽ��ϵ��
	std::set<sint8> GetMonomialSetFactor(); //��ôε���ʽ����ĸ����
	std::map<sint8, Fraction<sint64>>& GetFactor(); //��ô˵���ʽ����ĸ����
public:
	void SetCoefficient(sint64 a, sint64 b); //����ϵ��
	void SetCoefficientA(sint64 a); //����ϵ��a
	void SetCoefficientB(sint64 b); //����ϵ��b
	void Push(sint8 factor, Fraction<sint64> exponent); //�����ĸ����
	bool Substitute(sint8 character, Monomial val); //���룬��ĳ����ĸ�滻Ϊĳ������ʽ
	void DelOne(); //ɾ������ġ�1��
	void Clear_factor();
public:
	bool SimilarItems(Monomial b); //�ж��Ƿ�Ϊͬ����
	bool IsNumber(); //�ж��Ƿ�Ϊ�������֡�
	static bool IsValid(std::string val); //�ж��ַ����Ƿ����㵥��ʽ�﷨

/*��׼����*/
public:
	void SetValue(Monomial value);
	/*��׼�ַ�������*/
	void SetValue(std::string std_value);
/*�Ǳ�׼����*/
public:
	/*�Ǳ�׼�ַ�������*/
	void Input(std::string value);

/*��׼���*/
public:
	/*��׼�ַ������*/
	std::string GetValue();
	/*�Ǳ�׼�ַ������*/
	std::string Out();
public:
	bool operator==(Monomial b);
	bool operator!=(Monomial b);
public:
	Monomial operator+(Monomial b);
	Monomial operator-(Monomial b);
	Monomial operator*(Monomial b);
	Monomial operator/(Monomial b);
public:
	/*	��������¿ɽ�Monomialת��sint64����
	 *	������ʾ������㿴��Visual Studio 2022��ʾ��
	  ��δ�ҵ�operator Fraction<sint64>�ĺ������塱 
	  �벻Ҫ���ţ���һ����Visual Studio 2022��bug~~~
	*/
	operator Fraction<sint64>();
public:
	bool IsError(); //�жϴ˵���ʽ�Ƿ�Ϊ����״̬
};

/// <summary>
/// ��ȡ����ʽa����ĸ���ϵ�Ԫ�ظ���
/// </summary>
/// <param name=""></param>
/// <returns></returns>
sint64 GetLetterSize(Monomial& a);

//������������ʽ����ĸ���ϵĽ���
std::set<sint8> CommonFactor(Monomial a, Monomial b);
