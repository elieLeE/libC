#include <assert.h>

#include "test_prime.h"
#include "../src/math/prime.h"
#include "../src/macros.h"

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

static void check_primes_number(gv_t(uint64) *primes_found,
                                unsigned long *primes_for_checking,
                                int primes_for_checking_count)
{
    assert(primes_found->len == primes_for_checking_count && "assert");

    gv_for_each_pos(pos, primes_found) {
        assert(primes_found->tab[pos] == primes_for_checking[pos]);
    }
}

static void test_get_all_primes_below_n(void)
{
    unsigned int lim = 100;
    gv_t(uint64) primes_found;
    unsigned long primes_for_checking[25];

    gv_init_size(&primes_found, 25);

    fill_25_first_primes(primes_for_checking);

    get_all_primes_below_n(lim, &primes_found);

    check_primes_number(&primes_found, primes_for_checking, 25);

    gv_wipe(&primes_found, NULL);
}

static void test_get_all_n_first_primes(void)
{
    unsigned int lim = 25;
    unsigned long primes_for_checking[25];
    gv_t(uint64) primes_found;

    gv_init_size(&primes_found, 25);

    fill_25_first_primes(primes_for_checking);

    get_all_n_first_primes(lim, &primes_found);

    check_primes_number(&primes_found, primes_for_checking, 25);

    gv_wipe(&primes_found, NULL);
}

static void test_get_all_primes_factors_of_n(void)
{
    gv_t(uint64) primes;
    gv_t(primes_factors) primes_factors;
    int res;

    gv_init_size(&primes, 200);
    gv_init(&primes_factors);

    get_all_n_first_primes(200, &primes);

    /* {{{ Test with 6 */

    res = get_all_primes_factors_of_n(6, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 2);
    ASSERT_EQUAL_INT((int)primes_factors.len, 2);

    ASSERT(primes_factors.tab[0].prime == 2,
           "expected: 2, obtained: %ld", primes_factors.tab[0].prime);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 3);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    /* }}} */
    /* {{{ Test with 4 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(4, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 1);
    ASSERT_EQUAL_INT((int)primes_factors.len, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 2);

    /* }}} */
    /* {{{ Test with 53 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(53, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 1);
    ASSERT_EQUAL_INT((int)primes_factors.len, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 53);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 1);

    /* }}} */
    /* {{{ Test with 63 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(63, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 2);
    ASSERT_EQUAL_INT((int)primes_factors.len, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 3);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 7);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    /* }}} */
    /* {{{ Test with 735 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(735, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 3);
    ASSERT_EQUAL_INT((int)primes_factors.len, 3);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 3);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 5);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[2].prime, 7);
    ASSERT_EQUAL_INT(primes_factors.tab[2].iteration, 2);

    /* }}} */
    /* {{{ Test with 845 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(845, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 2);
    ASSERT_EQUAL_INT((int)primes_factors.len, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 5);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 13);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 2);

    /* }}} */
    /* {{{ Test with 988 */

    gv_clear(&primes_factors, NULL);

    res = get_all_primes_factors_of_n(988, &primes, &primes_factors);

    ASSERT_EQUAL_INT(res, 3);
    ASSERT_EQUAL_INT((int)primes_factors.len, 3);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 13);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[2].prime, 19);
    ASSERT_EQUAL_INT(primes_factors.tab[2].iteration, 1);

    /* }}} */

    gv_wipe(&primes, NULL);
    gv_wipe(&primes_factors, NULL);
}

static void
get_and_check_divisors_number_of_n(const gv_t(uint64) *primes, unsigned int n,
                                   unsigned int expected_divisors_count)
{
    unsigned int obtained_divisors_count;
    gv_t(primes_factors) primes_factors;

    gv_init(&primes_factors);

    get_all_primes_factors_of_n(n, primes, &primes_factors);
    obtained_divisors_count = get_divisors_count(&primes_factors);

    ASSERT(obtained_divisors_count == expected_divisors_count,
           "failing for N = %d while getting divisors numbers; "
           "obtained: %d, expected: %d",
           n, obtained_divisors_count, expected_divisors_count);

    gv_wipe(&primes_factors, NULL);
}

static void test_get_divisors_count(void)
{
    gv_t(uint64) primes;

    gv_init_size(&primes, 200);

    get_all_n_first_primes(200, &primes);

    get_and_check_divisors_number_of_n(&primes, 12, 6);
    get_and_check_divisors_number_of_n(&primes, 96, 12);
    get_and_check_divisors_number_of_n(&primes, 117, 6);
    get_and_check_divisors_number_of_n(&primes, 169, 3);
    get_and_check_divisors_number_of_n(&primes, 174, 8);

    gv_wipe(&primes, NULL);
}

static void check_phi_value_from_primes_facrors(unsigned long n,
                                                const gv_t(uint64) *primes,
                                                unsigned int expected_res)
{
    unsigned int phi;
    gv_t(primes_factors) primes_factors;

    gv_init(&primes_factors);

    get_all_primes_factors_of_n(n, primes, &primes_factors);
    phi = get_phi_from_primes_factors(n, &primes_factors);

    ASSERT_EQUAL_INT(phi, expected_res);

    gv_wipe(&primes_factors, NULL);
}

static void test_get_phi_from_primes_factors(void)
{
    gv_t(uint64) primes;

    gv_init_size(&primes, 200);

    get_all_n_first_primes(200, &primes);

    check_phi_value_from_primes_facrors(10, &primes, 4);
    check_phi_value_from_primes_facrors(11, &primes, 10);
    check_phi_value_from_primes_facrors(32, &primes, 16);

    gv_wipe(&primes, NULL);
}

static void test_get_all_phi_from_1_to_n(void)
{
    gv_t(int64) phi;

    gv_init_size(&phi, 200);

    get_all_phi_from_1_to_n(200, &phi);

    /* 0 is also added to the tab in order not to have always sub 1 in
     * 'get_all_phi_from_1_to_n' method */
    ASSERT_EQUAL_INT((int)phi.len, 201);
    ASSERT_EQUAL_INT((int)phi.tab[10], 4);
    ASSERT_EQUAL_INT((int)phi.tab[11], 10);
    ASSERT_EQUAL_INT((int)phi.tab[32], 16);

    gv_wipe(&phi, NULL);
}

module_tests_t *get_all_tests_prime(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "PRIMES");
    ADD_TEST_TO_MODULE(module_tests, test_is_prime);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_primes_below_n);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_n_first_primes);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_primes_factors_of_n);
    ADD_TEST_TO_MODULE(module_tests, test_get_divisors_count);
    ADD_TEST_TO_MODULE(module_tests, test_get_phi_from_primes_factors);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_phi_from_1_to_n);

    return module_tests;

}
