#pragma once
#include <string>
#include <map>
#include "..\misinttypes\saint.h"
#include "fraction.h"
/*����ʽ*/
class Monomial
{
private:
	//����ϵ��
	Fraction<sint64> coefficient;
	//��ĸ����/ÿ����ĸ��ָ��
	std::map<sint8, Fraction<sint64>> factor;
public: //���캯��
	Monomial();
	Monomial(std::string std_value);
	Monomial(Fraction<sint64> coefficient, std::map<sint8, Fraction<sint64>> factor);
public:
	Fraction<sint64> GetDegree(); //��ô˵���ʽ�Ĵ���
	Fraction<sint64> GetCoefficient(); //��ô˵���ʽ��ϵ��
	std::map<sint8, Fraction<sint64>>& GetFactor(); //��ô˵���ʽ����ĸ����
public:
	void SetCoefficient(sint64 a, sint64 b); //����ϵ��
	void SetCoefficientA(sint64 a); //����ϵ��a
	void SetCoefficientB(sint64 b); //����ϵ��b
	void Push(sint8 factor, Fraction<sint64> exponent); //�����ĸ����
	void DelOne(); //ɾ������ġ�1��
	void Clear_factor();
public:
	bool SimilarItems(Monomial b); //�ж��Ƿ�Ϊͬ����
	bool IsNumber(); //�ж��Ƿ�Ϊ�������֡�

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
private:
	//�Ƿ��ڴ���״̬
	bool error;
public:
	bool IsError(); //�жϴ˵���ʽ�Ƿ�Ϊ����״̬
};

