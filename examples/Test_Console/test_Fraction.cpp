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
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
}

#endif // DEBUG
