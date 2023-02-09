//#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/polynomial.h"
using namespace std;

int main()
{
	Polynomial a, b, c;
	a.Input("a^7 + 3a^6 + 4a + 3");
	b.Input("a + b");
	c = a * b;
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
	cout << "标准字符串输出：" << b.GetValue() << "\n";
	cout << "非标准字符串输出：" << b.Out() << "\n";
	cout << "标准字符串输出：" << c.GetValue() << "\n";
	cout << "非标准字符串输出：" << c.Out() << "\n";
}

#endif // DEBUG
