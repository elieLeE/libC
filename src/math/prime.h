#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdbool.h>

#include "../vector/vector.h"

/*
 * p111
 */

typedef struct prime_factor_t {
    unsigned long prime;
    unsigned int iteration;
} prime_factor_t;

generic_vector_t(primes_factors, prime_factor_t);

bool is_prime(unsigned long n);

__attribute__((nonnull))
long get_all_primes_below_n(unsigned long lim, gv_t(uint64) *out);

__attribute__((nonnull))
void get_all_n_first_primes(long count_asked, gv_t(uint64) *out);

__attribute__((nonnull))
unsigned long
get_all_primes_factors_of_n(unsigned long n, const gv_t(uint64) *primes,
                            gv_t(primes_factors) *out);

/* WARNING: N is included.
 * Use 'get_all_proper_divisors_of_n_from_prime_factors' if you do not want it.
 */
__attribute__((nonnull))
void
get_all_divisors_of_n_from_prime_factors(const gv_t(primes_factors) *in,
                                         gv_t(uint64) *out);

__attribute__((nonnull))
void
get_all_proper_divisors_of_n_from_prime_factors(unsigned long n,
                                                const gv_t(primes_factors) *in,
                                                gv_t(uint64) *out);

/* In this method, the vector given contains N and its value will be removed
 * from the sum */
__attribute__((nonnull))
unsigned long
get_proper_divisors_sum_from_all_divisors(unsigned long n,
                                          const gv_t(uint64) *divisors);

__attribute__((nonnull))
unsigned long get_divisors_count(const gv_t(primes_factors) *primes_factors);


__attribute__((nonnull))
void get_all_phi_from_1_to_n(long n, gv_t(int64) *phi);

__attribute__((nonnull))
unsigned long
get_phi_from_primes_factors(unsigned long n,
                            const gv_t(primes_factors) *primes_factors);

#endif

