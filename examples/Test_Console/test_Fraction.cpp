//#define DEBUG

#ifdef DEBUG

#include "SeekAnAnswer/kernel/fraction.h"
#include "SeekAnAnswer/kernel/monomial.h"
using namespace std;
int main()
{
	Fraction<sint64> a, b;
	a.Input("(100/4)");
	b.Input("3/2");
	//a -= b;
	cout << "��׼�ַ��������" << a.GetValue() << "\n";
	cout << "�Ǳ�׼�ַ��������" << a.Out() << "\n";
}

#endif // DEBUG
