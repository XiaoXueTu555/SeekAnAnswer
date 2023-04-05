#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/equation.h"
#include <Windows.h>
using namespace std;
int main()
{
	Equation E;
	E.Input("aX^2 + bX + c = 0");
	cout << "方程：" << E.GetValue() << endl;
	cout << "方程：" << E.Out() << endl;
	vector<Fraction<Polynomial_Exponential_Sum>> result =  E.quadratic_equation_in_one_unknown();
	cout << "X1 = " << result.at(0).Out() << endl;
	cout << "X2 = " << result.at(1).Out() << endl;
}

#endif // DEBUG
