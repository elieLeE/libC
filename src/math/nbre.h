#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

unsigned int count_digits_in_nber(unsigned int n);
unsigned int get_digits_from_number(unsigned int n, unsigned char *out);
int get_digits_iterations_from_number(unsigned int n, unsigned char *out);

bool are_permutation_nbers(unsigned int n, unsigned int n2);
bool is_nber_a_palindrome(unsigned int n);

/* The two methods below supposes that all the digits are positives */
unsigned long
build_number_from_digits(const unsigned char digits[], unsigned int first_idx,
                         unsigned int last_idx);
unsigned long build_number_from_digits_rev(const unsigned char digits[],
                                           int first_idx, int last_idx);

bool is_a_increasing_number(unsigned char digits[], unsigned int max_idx);
bool is_a_decreasing_number(unsigned char digits[], unsigned int max_idx);
bool is_a_bouncy_number(unsigned char digits[], unsigned int max_idx);

unsigned long get_triangle_nber_from_index(unsigned long idx);
unsigned long get_next_triangle_nber(unsigned long previous_n,
                                     unsigned long idx);

#endif
