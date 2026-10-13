#include "calculs.h"

uint64_t sum_1_to_n(uint64_t n)
{
    return n * (n + 1) / 2;
}

uint64_t sum_carre_1_to_n(uint64_t n)
{
    return (n * (n + 1) * (2 * n + 1)) / 6;
}

uint64_t get_triangle_nber_from_index(uint64_t idx)
{
    return idx * (idx + 1) / 2;
}

uint64_t get_next_triangle_nber(uint64_t previous_n, uint64_t idx)
{
    return previous_n + idx;
}

