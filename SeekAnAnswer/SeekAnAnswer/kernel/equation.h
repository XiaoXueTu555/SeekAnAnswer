#pragma once
#include <vector>
#include "polynomial.h"
#include "polynomial_exponential.h"
#include "polynomial_exponential_sum.h"
#include "fraction.h"
#include "Fraction_PES.h"
#include "../misinttypes/saint.h"
class Equation
{
public:
	//δ֪����ϵ��
	std::vector<Polynomial> coefficient_left;
	std::vector<Polynomial> coefficient_right;
	//δ֪����ָ��
	std::vector<Fraction<sint64>> exponent_left;
	std::vector<Fraction<sint64>> exponent_right;
public:
	void SetValue(std::string value); //��׼����
	void Input(std::string value); //�Ǳ�׼����
	std::string GetValue();
	std::string Out();
public:
	void DeleteZero();
public:
	Fraction<sint64> GetTheHighestDegreeTermOfTheUnknown();
	sint64 FindDegree(Fraction<sint64> degree);
public:
	//����
	void ShiftItem();
	//�ϲ�ͬ����
	void Unite_like_terms();
public:
	//�Ƿ�Ϊ�����ַ��̣�δ֪��ϵ��Ϊ�����֣�
	bool IsPurelyNumericalEquation();
	//�÷��̸�������
	sint64 NumberOfRoot();
public:
	//���һԪһ�η���
	Fraction<Polynomial> linear_equation_with_one_unknown();
	//���һԪ���η���
	std::vector<Fraction<Polynomial_Exponential_Sum>> quadratic_equation_in_one_unknown();
};

//��Fraction<Polynomial>����ת�����ַ���
std::string Out(Fraction<Polynomial> val);
