#ifndef _SHARED_NBRE_H_
#define _SHARED_NBRE_H_

#include <stdbool.h>
#include <stdio.h>

int nbre_digits(int n);
unsigned int get_digits_from_number(unsigned int n, unsigned char *tab);
bool is_nber_a_palindrome(unsigned int n);

#endif
