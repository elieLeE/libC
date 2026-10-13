#ifndef _SHARED_SUM_H_
#define _SHARED_SUM_H_

#include <stdint.h>

__attribute__((const)) uint64_t sum_1_to_n(uint64_t n);
__attribute__((const)) uint64_t sum_carre_1_to_n(uint64_t n);

__attribute__((const)) uint64_t get_triangle_nber_from_index(uint64_t idx);
__attribute__((const))
uint64_t get_next_triangle_nber(uint64_t previous_n, uint64_t idx);

#endif
