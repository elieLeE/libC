#ifndef _PRIME_H_
#define _PRIME_H_

#include <stdbool.h>

#include "../containers/vector.h"

typedef struct prime_factor_t {
    uint64_t prime;
    int32_t iteration;
} prime_factor_t;

generic_vector_t(primes_factors, prime_factor_t);

bool is_prime(uint64_t n);
bool is_prime_from_smallest_primes(uint64_t n, const gv_t(uint64) *primes);

__attribute__((nonnull))
int64_t get_all_primes_below_n(uint64_t lim, gv_t(uint64) *out);

__attribute__((nonnull))
void get_all_n_first_primes(int64_t count_asked, gv_t(uint64) *out);

__attribute__((nonnull))
int64_t
get_all_primes_factors_of_n(uint64_t n, const gv_t(uint64) *primes,
                            gv_t(primes_factors) *out);

/* WARNING: N is included.
 * Use 'get_all_proper_divisors_of_n_from_prime_factors' if you do not want it.
 */
__attribute__((nonnull))
void
get_all_divisors_of_n_from_prime_factors(const gv_t(primes_factors) *in,
                                         gv_t(uint64) *out);

__attribute__((nonnull))
int
get_all_proper_divisors_of_n_from_prime_factors(uint64_t n,
                                                const gv_t(primes_factors) *in,
                                                gv_t(uint64) *out);

/* Warning, this method will create a vector for the prime factors of N and
 * an other one for all the divisors of N. This vector will then be wiped.
 * These allocations (of the array inside the vector) and deallocations could
 * decrease a little the performance of your program.
 * An other faster way would be use the methods 'get_all_primes_factors_of_n',
 * 'get_all_divisors_of_n_from_prime_factors' and
 * 'get_proper_divisors_sum_from_all_divisors' reusing the vectors filled by
 * these methods */
__attribute__((nonnull))
uint64_t
get_proper_divisors_sum_of_n(uint64_t n, const gv_t(uint64) *primes);

__attribute__((nonnull))
uint64_t get_divisors_count(const gv_t(primes_factors) *primes_factors);

__attribute__((nonnull))
void get_all_phi_from_1_to_n(int64_t, gv_t(int64) *phi);

__attribute__((nonnull))
uint64_t
get_phi_from_primes_factors(uint64_t n,
                            const gv_t(primes_factors) *primes_factors);

#endif

