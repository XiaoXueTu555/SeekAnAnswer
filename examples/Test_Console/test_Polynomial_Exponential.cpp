//#define DEBUG
#ifdef DEBUG

#include"SeekAnAnswer/kernel/polynomial_exponential.h"
using namespace std;
int main()
{
	Polynomial_Exponential a;
	a.SetValue("(1/1)*((1/1)a^(1/1) + (1/1)b^(1/1))^(2/1)");
	cout << "��׼�ַ��������" << a.GetValue() << "\n";
	cout << "�Ǳ�׼�ַ��������" << a.Out() << "\n";

	if (a.IsPolynomial())
	{
		cout << "�����" << ((Polynomial)a).Out() << "\n";
	}
}

#endif // DEBUG
