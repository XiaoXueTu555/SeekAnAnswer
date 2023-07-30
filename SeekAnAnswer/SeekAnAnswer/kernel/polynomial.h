#pragma once
#include <vector>
#include <map>
#include <set>
#include "monomial.h" //����ʽ

/*����ʽ��*/
class Polynomial
{
	/*member variable*/
public:
	std::vector<Monomial> list;//����ʽ
private:
	bool error;

	/*member function*/
public:
	Polynomial(std::string std_value);
	Polynomial(Monomial val);
	Polynomial();
/*��׼����*/
public:
	void SetValue(Polynomial value);
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
/*�Ǳ�׼���*/
public:
	/*�Ǳ�׼�ַ������*/
	std::string Out();

public:
	//�ϲ�ͬ����
	void Unite_like_terms();
public:
	//ȥ������ʽ�е�0
	void DeleteZero();
public:
	/*���һ������ʽ*/
	void Push(Monomial val);
public:
	//������ߴ���ĵ���ʽ
	Monomial GetDegree();
	//������ߴ���Ĵ���
	Fraction<sint64> GetTheDegree();
	//������ߴ����ĵ���ʽ��list���е��±�
	suint64 GetLocationOfDegree();
	//�����ض��ַ�����С����
	Fraction<sint64> GetDegreeOfMixOfchar(sint8 a);
public:
	//����val����ʽ�Ĺ���ʽ
	Monomial CommonFactor(Polynomial val);
	//����val����ʽ����С������
	Fraction<sint64> LeastCommonMultiple_of_coe(Polynomial val);
public:
	//����ߵĵ���ʽ�Ĵ����ĵ���ʽ�ƶ�����λ
	void Move();
public:
	//������е���ʽ
	void Clear();
public:
	bool Substitute(sint8 character, Polynomial val);
public:
	bool operator==(Polynomial val);
	bool operator!=(Polynomial val);
public:
	Polynomial operator=(Monomial val);
	Polynomial operator=(Polynomial val);
public:
	Polynomial operator+(Monomial val);
	Polynomial operator-(Monomial val);
	Polynomial operator*(Monomial val);
	Polynomial operator/(Monomial val);
public:
	Polynomial operator+(Polynomial val);
	Polynomial operator-(Polynomial val);
	Polynomial operator*(Polynomial val);
	Polynomial operator/(Polynomial val);
	Polynomial operator%(Polynomial val);
public:
	Polynomial operator+=(Polynomial val);
	Polynomial operator-=(Polynomial val);
	Polynomial operator*=(Polynomial val);
	Polynomial operator/=(Polynomial val);
	Polynomial operator%=(Polynomial val);
public:
	//ת��Ϊ����ʽ����ת��ǰʹ��Polynomial::monomial�����ж��Ƿ�Ϊ����ʽ
	operator Monomial();
public:
	//�ж��Ƿ�Ϊ����ʽ
	bool IsMonomial();
	//�ж��Ƿ�Ϊ������
	bool IsNumber();
	//�ж��ַ����Ƿ��������ʽ�﷨
	static bool IsValid(std::string val); 
public:
	bool IsError();
};

/// <summary>
/// �ж϶���ʽa�Ƿ�Ϊ����ʽb�ĸ�����ʽ
/// </summary>
/// <param name="a"> : ����ʽa</param>
/// <param name="b"> : ����ʽb</param>
/// <returns> true�����ǣ�false������ </returns>
bool IsParentPolynomial(Polynomial a, Polynomial b);

/// <summary>
/// �ж϶���ʽa�����ʽb����ĸ�����Ƿ���ȡ�
/// �ж϶���ʽa�����ʽb�Ƿ�Ϊ������ʽ���Ӷ���ʽ��
/// </summary>
/// <param name="a"> : ����ʽa </param>
/// <param name="b"> : ����ʽb </param>
/// <returns> true�����ǣ�false������ </returns>
bool IsLetterSetEquality(Polynomial a, Polynomial b);
