//#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/polynomial.h"
using namespace std;

int main()
{
	Polynomial a, b, c;
	std::string input_a, input_b;
	input_a = "15ia + 15a + 16i + -8";
	input_b = "-15ib + 12b + -16i + -5";

	if (a.IsValid(input_a))
	{
		a.Input(input_a);
	}
	if (b.IsValid(input_b))
	{
		b.Input(input_b);
	}
	c = a * b;
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
	cout << "标准字符串输出：" << b.GetValue() << "\n";
	cout << "非标准字符串输出：" << b.Out() << "\n";
	cout << "标准字符串输出：" << c.GetValue() << "\n";
	cout << "非标准字符串输出：" << c.Out() << "\n";*/
}

#endif // DEBUG
