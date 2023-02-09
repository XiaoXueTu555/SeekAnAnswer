#pragma once
#include <math.h>
#include <thread>
#include"High_int.h"
#include"High_float.h"
//pow function
High_int pow(High_int a, High_int n);
High_float pow(High_float a, High_int n);

//abs function
High_int abs(High_int a);
High_float abs(High_float a);


High_int pow(High_int a, High_int n) 
{
    if (n == (High_int)0) return 1;
    High_int ans = 1;
    const High_int one = 1;
    const High_int two = 2;
    while (n != one)
    {
        if (n.Data().at(n.Data().size() - 1) % 2 == 0)
        {
            a = a * a;
            n = n / two;
        }
        else
        {
            ans = ans * a;
            n = n - one;
        }
        //std::cout << n << "\n";
    }
    return ans * a;
}

High_float pow(High_float a, High_int n)
{
    if (n == (High_int)0) return (High_float)1;
    High_float result = 1, base = a;
    while (n != (High_int)0) {
        if (n % (High_int)2) result *= base;
        base *= base;
        n /= 2;
    }
    return result;
}

High_int abs(High_int a)
{
    a.Sign() = true;
    return a;
}

High_float abs(High_float a)
{
    a.Sign() = true;
    return a;
}



High_float sqrt(High_float a)
{
    if (a == (High_float)0)
    {
        return (High_float)0;
    }
    High_float guess = (High_float)0.5 * a , constant = 2, last;
    int64_t size = 32;
    guess.setprecision(size);
    constant.setprecision(size);
    int64_t i = 0;
    while(true)
    {
        guess = (guess * guess + a) / (constant * guess);
        //std::cout << "µÚ" << ++i << "´Îµü´ú: " << guess << "\n";
        if (guess == last)
        {
            break;
        }
        last = guess;
    }
    return guess;
}