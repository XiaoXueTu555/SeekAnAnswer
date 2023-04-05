//#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/polynomial.h"
using namespace std;

int main()
{
	Polynomial a, b, c;
	a.Input("15ia + 15a + 16i -8");
	b.Input("-15ib + 12b -16i -5");
	c = a * b;
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
	cout << "标准字符串输出：" << b.GetValue() << "\n";
	cout << "非标准字符串输出：" << b.Out() << "\n";
	cout << "标准字符串输出：" << c.GetValue() << "\n";
	cout << "非标准字符串输出：" << c.Out() << "\n";
}

#endif // DEBUG
