#include "calculs.h"

unsigned long sum_1_to_n(unsigned long n)
{
    return n * (n + 1) / 2;
}

unsigned long sum_carre_1_to_n(unsigned long n)
{
    return (n * (n + 1) * (2 * n + 1)) / 6;
}

long get_next_closest_power_of_two(long n)
{
    n--;

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;

    n++;

    return n;
}

