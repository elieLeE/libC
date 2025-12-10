#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

#include "../vector/vector.h"

__attribute__((const)) unsigned int get_count_digits_of_n(unsigned long n);
void get_digits_from_number(unsigned long n, gv_t(uint8) *out);
int get_digits_iterations_from_number(unsigned long n, unsigned char *out);

__attribute__((const))
bool are_permutation_nbers(unsigned long n, unsigned long n2);
__attribute__((const)) bool is_nber_a_palindrome(unsigned long n);

/* The two methods below supposes that all the digits are positives */
unsigned long
build_number_from_digits(const unsigned char digits[], unsigned int first_idx,
                         unsigned int last_idx);
unsigned long build_number_from_digits_rev(const unsigned char digits[],
                                           int first_idx, int last_idx);

bool is_a_increasing_number(unsigned char digits[], unsigned int max_idx);
bool is_a_decreasing_number(unsigned char digits[], unsigned int max_idx);
bool is_a_bouncy_number(unsigned char digits[], unsigned int max_idx);

#endif
