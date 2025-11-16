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

__attribute__((nonnull))
unsigned long get_divisors_count(const gv_t(primes_factors) *primes_factors);

unsigned int get_phi(unsigned long n, const unsigned long primes[],
                     unsigned int idx_max, bool stop_on_firt_divisor);

__attribute__((nonnull))
void get_all_phi_from_1_to_n(long n, gv_t(int64) *phi);

__attribute__((nonnull))
unsigned long
get_phi_from_primes_factors(unsigned long n,
                            const gv_t(primes_factors) *primes_factors);

#endif

