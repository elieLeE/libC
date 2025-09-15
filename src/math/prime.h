#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdbool.h>

/*
 * p111
 */

typedef struct prime_factor_t {
    unsigned int prime;
    unsigned int iteration;
} prime_factor_t;

/* We can say that I should use long long here but if I do, I will have to
 * use long long everywhere (especially in the method "remplissage_tab_prime"
 * for example. But, it does not seem really necessary for now. */
bool is_prime(unsigned long n);
unsigned int
get_all_primes_below_n(unsigned int lim, unsigned int size_tab_out,
                       unsigned int *out);
unsigned int get_all_primes_factors_of_n(unsigned long n,
                                         const unsigned int primes[],
                                         unsigned int primes_tab_size,
                                         unsigned int size_tab_out,
                                         prime_factor_t out[]);

unsigned int get_phi(unsigned int n, const unsigned int primes[],
                     unsigned int idx_max, bool stop_on_firt_divisor);

#endif

