#include "sum.h"

unsigned long sum1_to_n(unsigned long n)
{
    return n * (n + 1) / 2;
}

unsigned long sum_carre(unsigned long n)
{
    return (n * (n + 1) * (2 * n + 1)) / 6;
}
