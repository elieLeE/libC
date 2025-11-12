#include <assert.h>

#include "test_prime.h"
#include "../src/math/prime.h"
#include "../src/mem/mem.h"
#include "../src/macros.h"
#include "../libc.h"

static void test_is_prime(void)
{
    assert(is_prime(2));
    assert(!is_prime(4));
    assert(is_prime(5));
    assert(is_prime(79));
    assert(!is_prime(323));
}

static void fill_25_first_primes(unsigned long *primes)
{
    /* {{{ 25 first primes or all primes belox 100 */

    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;
    primes[3] = 7;
    primes[4] = 11;
    primes[5] = 13;
    primes[6] = 17;
    primes[7] = 19;
    primes[8] = 23;
    primes[9] = 29;
    primes[10] = 31;
    primes[11] = 37;
    primes[12] = 41;
    primes[13] = 43;
    primes[14] = 47;
    primes[15] = 53;
    primes[16] = 59;
    primes[17] = 61;
    primes[18] = 67;
    primes[19] = 71;
    primes[20] = 73;
    primes[21] = 79;
    primes[22] = 83;
    primes[23] = 89;
    primes[24] = 97;

    /* }}} */
}

static void
check_primes_number(unsigned long *primes_found,
                    unsigned int primes_found_count,
                    unsigned long *primes_for_checking,
                    unsigned int primes_for_checking_count)
{
    assert(primes_found_count == primes_for_checking_count && "assert");

    for (unsigned int i = 0; i < primes_found_count; i++) {
        assert(primes_found[i] == primes_for_checking[i]);
    }
}

static void test_get_all_primes_below_n(void)
{
    unsigned int lim = 100;
    unsigned long *primes_found;
    unsigned long primes_for_checking[25];
    unsigned int primes_count;

    primes_found = p_calloc(sizeof(long) * lim);

    fill_25_first_primes(primes_for_checking);

    primes_count = get_all_primes_below_n(lim, lim, primes_found);

    check_primes_number(primes_found, primes_count, primes_for_checking, 25);

    p_free((void **)&primes_found);
}

static void test_get_all_n_first_primes(void)
{
    unsigned int lim = 25;
    unsigned long *primes_found;
    unsigned long primes_for_checking[25];

    primes_found = p_calloc(sizeof(long) * lim);

    fill_25_first_primes(primes_for_checking);

    ASSERT(get_all_n_first_primes(lim, lim, primes_found) == 0,
           "get_all_n_first_primes has failed");

    check_primes_number(primes_found, 25, primes_for_checking, 25);

    p_free((void **)&primes_found);
}

module_tests_t *get_all_tests_prime(void)
{
    module_tests_t *module_tests = RETHROW_P(p_calloc(sizeof(module_tests_t)));

    set_module_name(module_tests, "PRIMES");
    ADD_TEST_TO_MODULE(module_tests, test_is_prime);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_primes_below_n);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_n_first_primes);

    return module_tests;

}
