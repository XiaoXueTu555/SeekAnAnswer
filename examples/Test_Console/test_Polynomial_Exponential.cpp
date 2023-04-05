//#define DEBUG
#ifdef DEBUG

#include"SeekAnAnswer/kernel/polynomial_exponential.h"
using namespace std;
int main()
{
	Polynomial_Exponential a;
	a.SetValue("(1/1)*((1/1)a^(1/1) + (1/1)b^(1/1))^(2/1)");
	cout << "标准字符串输出：" << a.GetValue() << "\n";
	cout << "非标准字符串输出：" << a.Out() << "\n";

	if (a.IsPolynomial())
	{
		cout << "结果：" << ((Polynomial)a).Out() << "\n";
	}
}

#endif // DEBUG
