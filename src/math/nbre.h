#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

int count_digits_in_nber(int n);
unsigned int get_digits_from_number(unsigned int n, unsigned char *tab);
unsigned int get_digits_iterations_from_number(unsigned int n,
                                               unsigned char *tab);

bool is_nber_a_palindrome(unsigned int n);

#endif
