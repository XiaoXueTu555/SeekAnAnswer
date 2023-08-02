# SeekAnAnswer

| [介绍](#介绍)     | [用法](#用法) | [工作原理](#工作原理) | [演示](#演示) |
| ----------------- | ------------- | --------------------- | ------------- |
| [许可证](#许可证) |               |                       |               |

### 介绍

SeekAnAnswer 是一个**用于C++的小型数学库**。它支持**多项式计算**以及**多项式系数一元方程**求解。它是快速的，可移植的，无依赖的或独立的（不依赖任何第三方库）。

SeekAnAnswer 旨在**实现快速开发**，使程序员在需要的地方上写简洁的代码实现数学上的计算操作。该库内部支持将一些组合结构直接读取的功能，不需要程序员对用户输入的文本数据做过多的结构化处理，这**有利于实现这种简单易用的特性**，但作为牺牲，**该库对复杂的组合结构不支持直接读取**，这主要是由于处理这种组合结构的拆分是一个很棘手的问题，而且每个结构所支持的表示方法多种多样，几乎没有一个通用的方法来实现组合结构的有效拆分，所以**在复杂的组合结构输入中，不建议使用SeekAnAnswer。如果你有能力实现这种复杂结构的拆分，且拆分后的结构可以使用SeekAnAnswer 来处理，那也是一个有效的解决方案！**

SeekAnAnswer 特别适合集成在对多项式计算有需求的项目、实现科学计算器的项目，甚至在小型嵌入式项目中！

- **快速开发**
- **易上手和快速入门**
- **无额外依赖**（只用到STL）
- **方便移植，无需配置**
- **小型C++库**
- **内存占用低**
- **毫秒级别的速度**
- **C++11或者以下！**

### 用法

SeekAnAnswer 的使用非常简单，**不需要额外的配置**就可以在大多数IDE中轻松地编译 SeekAnAnswer 的源文件。你可以将这些文件添加到你的项目中以使用 SeekAnAnswer。

在编译通过之后，你可以包含 SeekAnAnswer 的头文件，来使用 SeekAnAnswer 的所有功能。

```c++
/*包含一下小巧的SeekAnAnswer*/
#include "SeekAnAnswer/kernel/Fraction.h"
#include "SeekAnAnswer/kernel/monomial.h"
#include "SeekAnAnswer/kernel/polynomial.h"
#include "SeekAnAnswer/kernel/polynomial_exponential.h"
#include "SeekAnAnswer/kernel/polynomial_exponential_sum.h"
#include "SeekAnAnswer/kernel/equation.h"
#include "SeekAnAnswer/kernel/arithmetic_expression.h"
```

如果你想使用 SeekAnAnswer 中的 **Polynomial 来实现多项式计算**，你只需要写**简洁的几行代码即可实现**。下面是计算两个多项式乘积（ (a^2 + 2ab + b^2) * (a + b) ）的例子。

```c++
//创建两个多项式
Polynomial pol1, pol2;
//使用非标准格式输入来对这两个多项式赋值
pol1.Input("a^2 + 2ab + b^2"); pol2.Input("a + b");
//计算pol1乘以pol2的值，并将结果保存在pol3中
Polynomial pol3 = pol1 * pol2;
//打印结果
std::cout << "pol1 + pol2 = (" << pol1.Out() << ") * ("
		<< pol2.Out() << ") = " << pol3.Out() << std::endl;
```

![1690949527493](typora-user-images\1690949527493.png)



如果你想使用 SeekAnAnswer 中的 **Equation 来实现方程的求解**，你只需要写**简洁的几行代码即可实现**。下面是对方程：“aX^2 + bX^2 + c = 0” 求解的例子。

```c++
//首先要创建一个Equation对象来储存这个等式
Equation equation;

//对等式进行非标准格式输入
equation.Input("aX^2 + bX + c = 0");

//我们需要创建两个字符串来保存方程的根(因为二次方程可能有两个根)
std::string equ_Out1;
std::string equ_Out2;

//因为上面的输入是一个多项式系数一元二次方程，所以调用这个函数来求根
equation.quadratic_equation_in_one_unknown();
//获取第一个根的输出
equ_Out1 = equation.root2.at(0).Out();
//获取第二个根的输出
equ_Out2 = equation.root2.at(1).Out();

//输出根的结果
std::cout << "你输入的方程是: " << equation.Out() << std::endl;
std::cout << "根的情况：" << std::endl
	<< equ_Out1 << std::endl << equ_Out2 << std::endl;
```

![1690950102603](typora-user-images\1690950102603.png)

事实上 SeekAnAnswer 的使用如同上面的例子一样简单！而且**很多类都有类似的函数与功能，也十分符合直觉与逻辑**，这一点从上面的例子也可以看出。当然，SeekAnAnswer 还有许多的功能来帮助你实现更多的数学运算，或是获取一些必要的数学信息。

***如果你想学习更多关于 SeekAnAnswer 的使用，可以打开使用 Visual Studio 2022 打开这个解决方案，并将项目“SeekAnAnswer”设为启动项目，然后打开项目中“源文件”目录下的“test.cpp”文件来学习使用 SeekAnAnswer 的更多功能。***

### 工作原理

SeekAnAnswer 使用不同的思路来实现类似 **作业帮智能计算器** 或者 **微软数学求解器（Microsoft Math Solver）**的效果。

作业帮智能计算器主要是在表达式以及快速搜索路径这两点做主要文章，实现对中学生常见表达式的化简求值以及呈现运算过程。

SeekAnAnswer 主要从中学生常见的几种方程以及表达式下手，寻找这些方程与表达式的共同点，**用固定的复合结构去表示方程根的结果，或者表达式化简后的结果**。这样的实现有利于输出结果的稳定性，在相似的方程或者表达式计算中，结果的输出非常稳定，不会出现太大的出入。

同时你也不必担心输出的结果过于格式化，因为SeekAnAnswer 内部有很多的化简机制，在能化简的地方都尽量进行化简，所以在不同的方程类型或者是表达式类型中，即使计算的过程相同，输出的结果也是不同的。这样的设计有利于提高结果的可读性。

当然这样的实现也有弊端，**如果是复杂的复合结构化简求值，SeekAnAnswer 将无能为力**。

因为**SeekAnAnswer只能对所支持的固定结构进行计算，所以并不能完全替代作业帮智能计算器**（虽然过于复杂的式子即使是这类计算器也无法计算），但是可以在大部分的计算场景使用 SeekAnAnswer 达到类似的效果。

得益于 SeekAnAnswer 的固定结构计算，**SeekAnAnswer并不需要太多的算力**，所以**非常适合使用在嵌入式，这类芯片主频不高且想实现中学阶段科学计算器的场景**！

SeekAnAnswer 在大部分支持的结构中，支持复合结构的直接输入（太复杂的不支持），得益于这一点，你可以直接将表达式或方程输入给 SeekAnAnswer 来处理，不需要做过多的结构化处理！

这里提到由特性产生的效果，都会在下面的演示中得到展示。

### 演示

这个解决方案包含一个项目"ImGui_Test_Calculator"。

是的，这个项目使用了著名的 Dear ImGui ，同时也是我最喜欢的Gui之一，所以使用这个Gui来进行 SeekAnAnswer 的效果演示

![1690953926354](typora-user-images\1690953926354.png)

![1690953965402](typora-user-images\1690953965402.png)

![1690954073584](typora-user-images\1690954073584.png)

![1690954087552](typora-user-images\1690954087552.png)

![1690954106673](typora-user-images\1690954106673.png)

![1690954142972](typora-user-images\1690954142972.png)

![1690954169611](typora-user-images\1690954169611.png)

![1690954721141](typora-user-images\1690954721141.png)

### 许可证

 SeekAnAnswer 根据 GPL3.0 许可证获得许可 

