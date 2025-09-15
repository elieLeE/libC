#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdbool.h>

/*
 * p111
 */

typedef struct prime_factor_t {
    unsigned long prime;
    unsigned int iteration;
} prime_factor_t;

bool is_prime(unsigned long n);
unsigned int
get_all_primes_below_n(unsigned long lim, unsigned int size_tab_out,
                       unsigned long *out);
unsigned int get_all_primes_factors_of_n(unsigned long n,
                                         const unsigned long primes[],
                                         unsigned int primes_tab_size,
                                         unsigned int size_tab_out,
                                         prime_factor_t out[]);

unsigned int get_phi(unsigned long n, const unsigned long primes[],
                     unsigned int idx_max, bool stop_on_firt_divisor);

#endif

