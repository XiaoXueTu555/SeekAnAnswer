//#define DEBUG

#ifdef DEBUG

#include "SeekAnAnswer/kernel/fraction.h"
#include "SeekAnAnswer/kernel/monomial.h"
using namespace std;
int main()
{
	Fraction<sint64> a, b;

	std::string input_a, input_b;
	input_a = "(100/4)";
	input_b = "3/2";

	if (a.IsValid(input_a))
	{
		a.Input(input_a);
	}
	if (b.IsValid(input_b))
	{
		b.Input(input_b);
	}
	//a -= b;
	cout << "��׼�ַ��������" << a.GetValue() << "\n";
	cout << "�Ǳ�׼�ַ��������" << a.Out() << "\n";
}

#endif // DEBUG
