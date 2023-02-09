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
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
}

#endif // DEBUG