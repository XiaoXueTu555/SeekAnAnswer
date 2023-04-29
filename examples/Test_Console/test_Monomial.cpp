//#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/monomial.h"
using namespace std;
int main()
{
	Monomial a, b, c;

	std::string input_a, input_b;
	input_a = "(-3)a^(-9)";
	input_b = "-3ab^(1)c^(-1)";

	if (a.IsValid(input_a))
	{
		a.Input(input_a);
	}
	if (b.IsValid(input_b))
	{
		b.Input(input_b);
	}
	//c = a * b;
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";
	cout << "标准字符串输出：" << b.GetValue() << "\n";
	cout << "非标准字符串输出：" << b.Out() << "\n";
}

#endif // DEBUG