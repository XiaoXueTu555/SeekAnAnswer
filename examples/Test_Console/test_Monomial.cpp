//#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/monomial.h"
using namespace std;
int main()
{
	Monomial a, b, c;
	a.Input("-a^-1");
	b.Input("3a");
	//c = a * b;
	cout << "��׼�ַ��������" << a.GetValue() << "\n";
	cout << "�Ǳ�׼�ַ��������" << a.Out() << "\n";
}

#endif // DEBUG