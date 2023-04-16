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
	Fraction<Polynomial> root1; //һԪһ�η��̵ĸ�
	//һԪ���η��̵ĸ�
	std::vector<Fraction<Polynomial_Exponential_Sum>> root2;
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
	void linear_equation_with_one_unknown();
	//���һԪ���η���
	void quadratic_equation_in_one_unknown();
};

//�ж�����n�Ƿ�Ϊ����
inline bool IsPrime(sint64 n) {
	if (n <= 3) {
		return n > 1;
	}
	// ֻ��6x-1��6x+1�������п���������
	if (n % 6 != 1 && n % 6 != 5) {
		return false;
	}
	// �ж���Щ���ܷ�С��sqrt(n)����������
	sint64 sqrt_n = (sint64)sqrt(n);
	for (sint64 i = 5; i <= sqrt_n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}
	return true;
}

//��������
class Prime
{
public:
	std::vector<sint64> nk;
	sint64 n;
	suint64 i;
public:
	Prime() 
	{ 
		i = 0;
		n = 2; 
		nk.push_back(2);
		nk.push_back(3);
		nk.push_back(5);
		nk.push_back(7);
		nk.push_back(11);
		nk.push_back(13);
		nk.push_back(17);
	}
	void Next()
	{
		if (++i < nk.size())
		{
			n = nk.at(i);
			return;
		}
		while (!IsPrime(n))
		{
			n += 2;
		}
		return;
	}
};

//��Fraction<Polynomial>����ת�����ַ���
std::string Out(Fraction<Polynomial> val);

/// <summary>
/// ��������������a������ƽ�������㷨
/// ���㷨��-- ���� ��� �ش���л
/// ע��:��ȷ������Ĳ���Ϊ������
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
Polynomial_Exponential Simplest_radical_AnBo(Polynomial_Exponential val);

//�������ⴿ���ָ�ʽ���㷨
Polynomial_Exponential Simplest_radical(Polynomial_Exponential val);
