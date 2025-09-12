#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdbool.h>

/*
 * p111
 */

bool is_prime(unsigned long n);
void remplissage_tab_prime_bool(bool *tab, unsigned int lim);
unsigned int remplissage_tab_prime(unsigned int *tabPrime, unsigned int lim);

unsigned int get_phi(unsigned int n, unsigned int primes_nber[],
                     unsigned int idx_max, bool stop_on_firt_divisor);

#endif

