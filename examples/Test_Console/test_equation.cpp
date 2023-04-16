#define DEBUG
#ifdef DEBUG

#include "SeekAnAnswer/kernel/equation.h"
#include <Windows.h>
using namespace std;
int main()
{
	Equation E;
	E.Input("X^2 = 1");
	cout << "方程：" << E.GetValue() << endl;
	cout << "方程：" << E.Out() << endl;
	E.quadratic_equation_in_one_unknown();
	cout << "X1 = " << E.root2.at(0).Out() << endl;
	cout << "X2 = " << E.root2.at(1).Out() << endl;
}

#endif // DEBUG
