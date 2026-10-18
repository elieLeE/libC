#include "utils.h"

int64_t get_next_closest_power_of_two(int64_t n)
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

