#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../mem/alloc.h"

/*
 * p111
 * */

bool isPrime(unsigned long n);
void remplissageTabPrimeBool(bool *tab, unsigned int lim);
void remplissageTabPrime(unsigned int* tabPrime, unsigned int lim);

#endif

