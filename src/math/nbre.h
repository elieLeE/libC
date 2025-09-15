#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

unsigned int count_digits_in_nber(unsigned int n);
unsigned int get_digits_from_number(unsigned int n, unsigned char *out);
int get_digits_iterations_from_number(unsigned int n, unsigned char *out);

bool are_permutation_nbers(unsigned int n, unsigned int n2);
bool is_nber_a_palindrome(unsigned int n);

unsigned long build_number_from_digits(const unsigned char digits[],
                                      int first_idx, int last_idx);
unsigned long build_number_from_digits_rev(const unsigned char digits[],
                                           int first_idx, int last_idx);

#endif
