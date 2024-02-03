/*
* 这是一个示例项目
* 在这里，你可以学习到SeekAnAnswer的用法（简称SAA），并使用SAA来计算一些例子
* 简单介绍一下SAA中，各个类的作用
 
* Fraction 分式，这是一个模板类。顾名思义，表示两个数的比，最常见的类型是Fraction<sint64>
 
* Monomial 单项式 这个类用来表示数学的单项式，你可以对两个单项式进行一些数学运算
 
* Polynomial 多项式 这个类用来表示数学的多项式，你可以对两个多项式间进行一些数学运算，
 而且你可以将Monomial转成Polynomial或在特殊情况下，将Polynomial转成Monomial，从而进行一些特殊操作

* Polynomial_Exponential 多项式的次方 用来表示一个数字系数乘以多项式次方的特殊结构，在SAA其他的功能实现中
 应用广泛

* Polynomial_Exponential_Sum 多项式的次方的和 用来表示许多polynomial_exponential的和，用户层一般用不到
 这个类，但在SAA的其他的功能实现中存在应用。

* Equation 等式 用来表示一个数学中的等式，允许至多存在一个未知数（X），在输入时，请注意这个未知数是大写的'X',
 支持求解一元一次方程和一元二次方程，且方程的系数允许为多项式（Polynomial）结构

* Arithmetic_Expression 算术表达式 用来表示一个数学中的表达式，支持化简表达式（精确值）、求表达式的近似值
*/

/*首先包含一下小巧的SeekAnAnswer*/
#include "SeekAnAnswer/kernel/Fraction.h"
#include "SeekAnAnswer/kernel/monomial.h"
#include "SeekAnAnswer/kernel/polynomial.h"
#include "SeekAnAnswer/kernel/polynomial_exponential.h"
#include "SeekAnAnswer/kernel/polynomial_exponential_sum.h"
#include "SeekAnAnswer/kernel/equation.h"
#include "SeekAnAnswer/kernel/arithmetic_expression.h"
#include "SeekAnAnswer/kernel/rational_function.h"

int main()
{
	/*首先让我们来学习使用 Fraction 分式*/

	//让我们利用Fraction计算 二分之一 加 三分之一的值（等于六分之五）
	//因为是两个整数的比，所以创建两个Fraction<sint64>类型的对象
	Fraction<sint64> frac1, frac2;

	/*Input这个函数可以对一个Fraction<sint64>对象赋值，
	而且对于用户来说，推荐使用Input函数进行输入。
	同样可以进行输入的函数还有Fraction<sint64>::SetValue，
	但是对于用户来说并不推荐这种输入方式，因为这种方式的输入非常繁琐，
	需要严格按照Fraction的标准输入格式进行输入，如果输入的格式不对，可能会导致
	内存的泄露，几乎在所有的输入场所下都推荐使用Input和IsValid(针对Input输入方式的安全格式检查)
	除非你对效率有特殊的要求，请参考SeekAnAnswer 各个类的“标准输入函数”的输入标准*/
	frac1.Input("4212/260"); frac2.Input("0/3");
	/*如果你担心在使用Input进行非标准格式输入的时候出错，导致SAA内存泄露，SAA十分贴心的为您提供
	了一个检查函数，当你在使用Input进行非标准格式输入之前，你可以检查你输入的内容是否为合规的格式，
	如果为合规的格式你就可以放心的使用Input函数来进行非标准格式输入了！这个函数是Fraction::IsValid，
	它的输出是一个bool值，若为true，则格式合规，则为false，则输入不合规*/

	//进行相加运算，将结果保存在frac3中
	Fraction<sint64> frac3 = frac1 + frac2;

	//利用Fraction<sint64>::Out函数（非标准格式输出函数）进行输出
	std::cout << "frac1 + frac2 = " << frac1.Out() << " + "
		<< frac2.Out() << " = " << frac3.Out() << std::endl;
	/*其他的运算请自行摸索，用法大同小异，但请注意不要进行除以0操作！*/

	/*其实 Fraction<sint64> 也有一个别名 Rational_number 译为 ‘有理数’，
	这两个类型本质上是等价的，只是在某些地方上为了强调‘有理数’或者为了考虑可读性而设计的*/
	std::cout << "————————————————————————————————" << std::endl;



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////



	/*接下来我们学习使用 Monomial 单项式*/

	//让我们利用Monomial计算 2ab * 5bc的值（等于 10ab^2c）
	//首先创建两个Monomial对象
	Monomial mon1, mon2;

	/*用Input函数来对Monomial对象进行非标准格式输入*/
	mon1.Input("2ab"); mon2.Input("5bc");
	/*若你担心使用Input函数进行非标准格式输入时，导致SAA内存泄漏，
	你依然可以向上面Fraction一样，使用Monomial::IsValid函数来对你即将输入的
	字符串进行安全格式检查*/

	//计算mon1乘以mon2的值，并将结果保存在mon3中
	Monomial mon3 = mon1 * mon2;

	//利用Monomial::Out函数（非标准格式输出函数）进行输出
	std::cout << "mon1 + mon2 = " << mon1.Out() << " * "
		<< mon2.Out() << " = " << mon3.Out() << std::endl;
	/*其他的运算请自行摸索，用法大同小异，但请注意不要进行除以0操作！*/

	std::cout << "————————————————————————————————" << std::endl;



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////



	/*接下来我们学习使用 Polynomial 多项式*/

	//让我们利用Polynomial计算 (a^2 + 2ab + b^2) * (a + b)的值（等于 a^3 + 3a^2b + 3ab^2 + b^3）
	//首先创建两个Polynomial对象
	Polynomial pol1, pol2;

	/*用Input函数来对Polynomial对象进行非标准格式输入，
	其实这里可以看出，SAA绝大多数的类都有Input函数来进行非标准格式的输入，
	以及SetValue函数来进行标准格式的输入，但是Polynomial_Exponential_Sum是个怪家伙，
	这两个输入都没有！这就是用户几乎接触不到这个类型的原因*/
	pol1.Input("a^2 + 2ab + b^2"); pol2.Input("a + b");
	/*我觉得经过Fraction和Monomial的例子，你已经聪明的猜到，Polynomial也有类似的
	安全格式检查，所以不会担心你的输入可能会导致SAA的内存泄漏了（笑）*/

	//计算pol1乘以pol2的值，并将结果保存在pol3中
	Polynomial pol3 = pol1 * pol2;

	//利用Monomial::Out函数（非标准格式输出函数）进行输出
	std::cout << "pol1 * pol2 = (" << pol1.Out() << ") * ("
		<< pol2.Out() << ") = " << pol3.Out() << std::endl;
	/*其他的运算请自行摸索，用法大同小异，但请注意不要进行除以0操作！*/

	std::cout << "————————————————————————————————" << std::endl;



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////



						/*Extra*/
	/*我们可以来学习一下额外的扩展 Rational_function*/
	/*Rational_function 通常被翻译为'有理函数'，该结构主要是由
	两个多项式的比组成的，而且本质上Rational_function在SeekAnAnswer中
	等价于 Fraction <Polynomial> 
	但是为什么要单独搞出来一个 Rational_function 呢？
	这主要是因为 Polynomial 本身不满足域的定义（四则运算不封闭），
	在某些多项式除法中，得出的结果不一定是 Polynomial ，而
	Fraction <Polynomial> 作为Polynomial的拓展，是满足域的定义的，
	Fraction <Polynomial> 在许多地方上都有应用，因此这值得单独拎出来
	做进一步的优化。也是出于应用层面的考虑，在使用了Fraction <Polynomial>
	的地方，可以使用 Rational_function 作为类型名*/
	Rational_function RF1("a", "b");
	//同上类似，你可以使用SetValue和Input来进行简单的输入操作
	RF1.SetValue("(c) / (d)"); 
	RF1.Input("(a) / (b + c)");

	Rational_function RF2("b", "a + c");
	Rational_function RF3("c", "a + b");
	Rational_function RF4("-4");

	Rational_function RF5 = RF1 + RF2 + RF3 + RF4;

	//std::cout << "RF1 / RF2 = (" << RF1.Out() << ") / ("
		//<< RF2.Out() << ") = " << RF3.Out() << std::endl;

	std::cout << "RF5.a = " << RF5.a.Out() << std::endl;

	std::cout << "————————————————————————————————" << std::endl;



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////



	/*接下来我们学习使用Equation 多项式*/
	/*让我们利用 Equation 求解 aX ^ 2 + bX + c = 0的两个实数解，
	这里的a、b、c都可以替换为任意的多项式（Polynomial），而从数学角度来说Polynomial
	也可以是数字，或是许多数字的和，所以Equation既可以求解这种纯字母系数的方程也支持
	以数字为系数的方程，所以Equation非常有意思，甚至可以用于数学和物理作用上（开个玩笑，
	小朋友的作业要自己写哦！）*/

	//首先要创建一个Equation对象来储存这个等式
	Equation equation;
	//这里在创建一个字符串str来对Equation对象进行输入
	std::string equ_str = "aX^2 + bX + c = 0"; //这个方程的根就是一元二次方程的求根公式

	//我们需要创建两个字符串来保存方程的根(因为二次方程可能有两个根)
	std::string equ_Out1;
	std::string equ_Out2;

	/*我觉得你已经猜到了使用Input函数来进行Equation的非标准格式输入了（qwq）
	但是这里要注意一点，使用Input函数来对Equation进行输入的时候，一定要确保
	这个输入中至少且至多含有一个未知数（大写的X），不过也不用过于担心这一点，
	因为Equation也有安全格式检查函数IsValid，如果输入不合规，则会返回false，
	所以我推荐在所有的情况下使用IsValid在进行Input函数输入之前，对字符串进行检查。
	但是你可能会有疑问，为什么这个演示里的所有Input输入都没有进行安全格式检查呢？
	因为大部分看这个演示项目的人都是第一次接触SAA，如果上来就进行迷惑的操作，学习者
	可能不理解，且无兴趣继续学习SAA，所以这里强调这一点。
	Equation是一个很常用且重要的模块，如果不熟悉SAA的朋友，可能导致Equation的输入格式是错误的，
	即使这里仅仅只是一个演示，而且我也对SAA非常熟悉，但这里还是使用IsValid来强调这一点。*/
	if (equation.IsValid(equ_str))
	{
		equation.Input(equ_str);
	}
	else
	{
		//虽然这个演示不会运行到这个分支中，但是你可以尝试更换其他的错误输入，来运行这个分支。
		std::cout << "Equation输入有误，请重新输入！" << std::endl;

		goto Arithmetic;
	}

	/*虽然对于这个演示来说，这里输入的方程事实上的确是一个一元二次方程，但是对于用户输入来说，
	我们不能获取到用户所想的东西（悲），所以如果用户正确的输入了一个方程，我们需要立刻知道这个
	方程对于SAA来说是否可解，然后来做下一步的打算，而SAA也提供了许多函数来获取这个方程的一些信息，
	下面是一些演示*/

	//首先要获取这个方程未知数的最高次数，来判断这个方程是几次方程，从而调动对应的函数来进行求解

	//获取这个方程未知数的最高次数，如果为1，则是一元一次方程
	if (equation.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(1, 1))
	{
		//应用一元一次方程的求根公式来求这个方程的根
		equation.linear_equation_with_one_unknown();
		/*获取这个方程的根，这里要注意不是使用Equation::Out，Equation::Out是用来输出
		这个方程本身的，而不是输出这个方程的根。SAA提供了Out函数来输出一元一次方程的根*/
		equ_Out1 = Out(equation.root1);
		//从数学角度来说，一元一次方程只有一个根，不存在第二个根，所以第二个字符串的输出就是描述这句话
		equ_Out2 = "这个方程只有一个根";
	}
	//获取这个方程未知数的最高次数，如果为1，则是一元二次方程
	else if (equation.GetTheHighestDegreeTermOfTheUnknown() == Fraction<sint64>(2, 1))
	{
		//如果该方程是纯数字方程，这个函数可以判断方程的系数是否为纯数字也就是Fraction<sint64>
		if (equation.IsPurelyNumericalEquation())
		{
			/*这个函数可以获取这个方程根的数量，如果为正数或者为0时，含义为根的数量，若返回值为-1则
			意味着这个方程的系数不是纯数字的，但是该方程可解。
			若返回值为-2，则SAA不能求解这个方程*/
			//这里大于0意味着根的数量大于0，而且是可解的
			if (equation.NumberOfRoot() > 0)
			{
				//应用一元二次方程的求根公式来求这个方程的两个根
				equation.quadratic_equation_in_one_unknown();
				//使用Equation::root2::Out函数来输出这个根
				equ_Out1 = equation.root2.at(0).Out();
				/*因为有两个根（如果根只有一个可以视为有两个相等的根），
				所以这里要不同的下标来访问不同的根*/
				equ_Out2 = equation.root2.at(1).Out();
			}
			else if(equation.NumberOfRoot() == -2)
			{
				equ_Out1 = equ_Out2 = "SeekAnAnswer目前不支持求解这个方程";
			}
			/*这个时候你可以会有一些疑问，不是说好的返回值为负数时候这个返回值的意义不一样吗，
			为什么这里直接将0,-1这两种情况都视为“这个方程没有实数根”呢？
			通常来说，你的说辞是正确的，的确当返回值为负数的时候，这个返回值的意义不一样，但是我们在此之前
			判断了这个方程是一个纯数字的方程，而纯数字的方程要么不能被SAA求解，要么根的数量是0或者正整数的，
			所以这里的返回值不可能是-1，而直接写else if(equation.NumberOfRoot() == -1)，可能会导致编译器
			给出警告，这对强迫症人士来说非常不友好，所以这里就直接else了（悲）*/
			else
			{
				equ_Out1 = equ_Out2 = "这个方程没有实数根";
			}
		}
		//如果程序走进这个分支，说明系数不是纯数字，而是一个多项式类型
		else
		{
			equation.quadratic_equation_in_one_unknown();
			equ_Out1 = equation.root2.at(0).Out();
			equ_Out2 = equation.root2.at(1).Out();
		}
	}
	//如果既不是一次方程也不是二次方程，很遗憾，目前SAA并不支持求解
	else
	{
		equ_Out1 = equ_Out2 =
			"SeekAnAnswer目前不支持求解这个方程";
	}

	//别忘了，最后输出根的结果。
	std::cout << "你输入的方程是: " << equation.Out() << std::endl;
	std::cout << "根的情况：" << std::endl
		<< equ_Out1 << std::endl << equ_Out2 << std::endl;
	/*Equation需要注意的内容还是挺多的，但是这些都是在数学上需要注意的，
	或是使用Equation进行开发的时候自然而然想要获取的一些信息，可以消化一下。*/

	std::cout << "————————————————————————————————" << std::endl;



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////



	//如果方程输入有误则直接执行Arithmetic演示
	Arithmetic:

	/*现在我们来学习如何使用Arithmetic_Expression*/
	/*或许善于发现的你早就发现了这个演示中貌似缺少Polynomial_Exponential的使用演示。
	事实上Arithmetic_Expression只能支持两个Polynomial_Exponential之间的运算，
	只不过Arithmetic_Expression提供了十分方便的输入方式，你可以直接输入一长串
	数学表达式，然后交给Arithmetic_Expression进行处理，这就是为什么SAA会推荐在
	简单的场景下使用Arithmetic_Expression*/

	//首先，我们要创建一个Arithmetic_Expression对象
	Arithmetic_Expression arithmetic;

	/*然后在创建一个字符串str用于输入。这里的例子是2倍根号2减去根号2，
	这里要注意的是，Arithmetic_Expression不支持减法操作，但是可以通过
	加上一个负数的方式实现减法，这是出于某种问题考虑的。但是不得不承认这的确是一个缺陷*/
	std::string arith_str = "(-1*2^(1/2)) + (2*2^(1/2))";

	//创建两个字符串用于保存化简后的输出以及近似值的输出
	std::string arith_Out1, arith_Out2;

	//进行安全格式检查
	if (arithmetic.IsValid(arith_str))
	{
		//使用Input函数输入
		arithmetic.Input(arith_str);

		//化简该表达式
		arithmetic.Calculate();
	}
	else
	{
		arith_Out1 = arith_Out2 = "Arithmetic_Expression输入有误";
	}

	//最后输出化简后的结果，以及输出近似值
	std::cout << "化简后的表达式: " << arithmetic.Out() << std::endl;
	std::cout << "近似值的输出：" << arithmetic.Approximate_Out() << std::endl;

	/*经过上面的演示，你已经学会了SAA的使用，更多的技巧可以访问GitHub仓库：
	 https://github.com/XiaoXueTu555/SeekAnAnswer 来查看更多的功能使用（如果我有时间来写的话）
	 所以这里更推荐自己摸索哦！如果想要学习更多关于SeekAnAnswer的技巧，你也可以将“ImGui_Test_Calculator”设为启动项目，
	 来随时使用和测试SeekAnAnswer的几乎每一个模块，也可以学习其中的功能使用。

	* 如果你对SeekAnAnswer中的某个功能感兴趣，你可以尝试阅读SeekAnAnswer的源代码学习该功能是如何实现的，
	 以满足你的好奇心。

	* 如果在使用SSA中遇到问题，或是发现bug，可以到GitHub上发issue，当我有时间的时候就会回复。
	 如果你觉得SeekAnAnswer还不错的话，可以到GitHub仓库点一个Star哦~*/

	return 0;
}
