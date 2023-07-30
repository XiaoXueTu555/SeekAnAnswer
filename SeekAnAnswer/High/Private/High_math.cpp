#include "..\Public\High_math.h"

#define ITERATIONS 10

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
    High_float guess = (High_float)0.5 * a, constant = 2, last;
    while (true)
    {
        guess = (guess * guess + a) / (constant * guess);
        if (guess == last)
        {
            break;
        }
        last = guess;
    }
    return guess;
}

High_float Rooting(High_float a, High_int n)
{
    if (a == (High_float)0)
    {
        return (High_float)0;
    }

    High_float guess = (High_float)0.5 * a;
    High_int one = 1;

    for (int64_t i = 0; i < ITERATIONS; i++)
    {
        guess = (a + (((High_float)(n - one)) * pow(guess, n))) 
            / ((High_float)n * pow(guess, n - one));
    }
    return guess;
}
