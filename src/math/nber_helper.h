#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

#include "../containers/vector.h"

__attribute__((const)) uint32_t get_count_digits_of_n(uint64_t n);
void get_digits_from_number(uint64_t n, gv_t(uint8) *out);
uint32_t get_digits_iterations_from_number(uint64_t n, uint32_t *out);

__attribute__((const))
bool are_permutation_nbers(uint64_t n, uint64_t n2);
__attribute__((const)) bool is_nber_a_palindrome(uint64_t n);

/* The two methods below supposes that all the digits are positives */
uint64_t
build_number_from_digits(const uint8_t digits[],
                         uint32_t first_idx, uint32_t last_idx);
uint64_t build_number_from_digits_rev(const uint8_t digits[],
                                      uint32_t first_idx, uint32_t last_idx);

bool is_a_increasing_number(const uint8_t digits[], uint32_t max_idx);
bool is_a_decreasing_number(const uint8_t digits[], uint32_t max_idx);
bool is_a_bouncy_number(const uint8_t digits[], uint32_t max_idx);

#endif
