#include <assert.h>

#include "test_prime.h"
#include "../src/math/prime.h"
#include "../src/macros.h"

static int cmp_long(const void *_d1, const void *_d2)
{
    const long *d1 = _d1;
    const long *d2 = _d2;

    return *d1 - *d2;
}

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

    get_all_n_first_primes(1000, &primes);

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
    /* {{{ Test with 17640 */

    gv_clear(&primes_factors, NULL);

    get_all_primes_factors_of_n(17640, &primes, &primes_factors);

    ASSERT_EQUAL_INT((int)primes_factors.len, 4);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 3);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 3);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[2].prime, 5);
    ASSERT_EQUAL_INT(primes_factors.tab[2].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[3].prime, 7);
    ASSERT_EQUAL_INT(primes_factors.tab[3].iteration, 2);

    /* }}} */

    gv_wipe(&primes, NULL);
    gv_wipe(&primes_factors, NULL);
}

static void
check_obtained_divisors_of_n(long n, const gv_t(uint64) *obtained_divisors,
                             const gv_t(uint64) *expected_divisors)
{
    gv_for_each_pos(pos, obtained_divisors) {
        ASSERT(gv_contains(expected_divisors, obtained_divisors->tab[pos],
                       GV_DICHOTOMY_SEARCH, cmp_long),
               "%ld is not in the expected divisors of %ld",
               obtained_divisors->tab[pos], n);
    }
}

static void test_get_all_divisors_of_n(void)
{
    gv_t(uint64) primes;
    gv_t(primes_factors) primes_factors;
    gv_t(uint64) obtained_divisors;
    gv_t(uint64) expected_divisors;

    gv_init_size(&primes, 200);
    gv_init(&primes_factors);
    gv_init(&obtained_divisors);
    gv_init(&expected_divisors);

    get_all_n_first_primes(100, &primes);

    /* {{{ Test with 220 */
    /* {{{ set expected_divisors */

    gv_add(&expected_divisors, 1);
    gv_add(&expected_divisors, 2);
    gv_add(&expected_divisors, 4);
    gv_add(&expected_divisors, 5);
    gv_add(&expected_divisors, 10);
    gv_add(&expected_divisors, 11);
    gv_add(&expected_divisors, 20);
    gv_add(&expected_divisors, 22);
    gv_add(&expected_divisors, 44);
    gv_add(&expected_divisors, 55);
    gv_add(&expected_divisors, 110);
    gv_add(&expected_divisors, 220);

    /* }}} */

    get_all_primes_factors_of_n(220, &primes, &primes_factors);

    ASSERT_EQUAL_INT((int)primes_factors.len, 3);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 5);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[2].prime, 11);
    ASSERT_EQUAL_INT(primes_factors.tab[2].iteration, 1);

    get_all_divisors_of_n_from_prime_factors(&primes_factors,
                                             &obtained_divisors);
    check_obtained_divisors_of_n(220, &obtained_divisors, &expected_divisors);

    /* }}} */
    /* {{{ Test with 284 */

    gv_fast_clear(&primes_factors);
    gv_fast_clear(&obtained_divisors);
    gv_fast_clear(&expected_divisors);

    /* {{{ set expected_divisors */

    gv_add(&expected_divisors, 1);
    gv_add(&expected_divisors, 2);
    gv_add(&expected_divisors, 4);
    gv_add(&expected_divisors, 71);
    gv_add(&expected_divisors, 142);
    gv_add(&expected_divisors, 284);

    /* }}} */

    get_all_primes_factors_of_n(284, &primes, &primes_factors);

    ASSERT_EQUAL_INT((int)primes_factors.len, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 71);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 1);

    get_all_divisors_of_n_from_prime_factors(&primes_factors,
                                             &obtained_divisors);
    check_obtained_divisors_of_n(284, &obtained_divisors, &expected_divisors);

    /* }}} */
    /* {{{ Test with 17640 */

    gv_fast_clear(&primes_factors);
    gv_fast_clear(&obtained_divisors);
    gv_fast_clear(&expected_divisors);

    /* {{{ set expected_divisors */

    gv_add(&expected_divisors, 1);
    gv_add(&expected_divisors, 2);
    gv_add(&expected_divisors, 3);
    gv_add(&expected_divisors, 4);
    gv_add(&expected_divisors, 5);
    gv_add(&expected_divisors, 6);
    gv_add(&expected_divisors, 7);
    gv_add(&expected_divisors, 8);
    gv_add(&expected_divisors, 9);
    gv_add(&expected_divisors, 10);
    gv_add(&expected_divisors, 12);
    gv_add(&expected_divisors, 14);
    gv_add(&expected_divisors, 15);
    gv_add(&expected_divisors, 18);
    gv_add(&expected_divisors, 20);
    gv_add(&expected_divisors, 21);
    gv_add(&expected_divisors, 24);
    gv_add(&expected_divisors, 28);
    gv_add(&expected_divisors, 30);
    gv_add(&expected_divisors, 35);
    gv_add(&expected_divisors, 36);
    gv_add(&expected_divisors, 40);
    gv_add(&expected_divisors, 42);
    gv_add(&expected_divisors, 45);
    gv_add(&expected_divisors, 49);
    gv_add(&expected_divisors, 56);
    gv_add(&expected_divisors, 60);
    gv_add(&expected_divisors, 63);
    gv_add(&expected_divisors, 70);
    gv_add(&expected_divisors, 72);
    gv_add(&expected_divisors, 84);
    gv_add(&expected_divisors, 90);
    gv_add(&expected_divisors, 98);
    gv_add(&expected_divisors, 105);
    gv_add(&expected_divisors, 120);
    gv_add(&expected_divisors, 126);
    gv_add(&expected_divisors, 140);
    gv_add(&expected_divisors, 147);
    gv_add(&expected_divisors, 168);
    gv_add(&expected_divisors, 180);
    gv_add(&expected_divisors, 196);
    gv_add(&expected_divisors, 210);
    gv_add(&expected_divisors, 245);
    gv_add(&expected_divisors, 252);
    gv_add(&expected_divisors, 280);
    gv_add(&expected_divisors, 294);
    gv_add(&expected_divisors, 315);
    gv_add(&expected_divisors, 360);
    gv_add(&expected_divisors, 392);
    gv_add(&expected_divisors, 420);
    gv_add(&expected_divisors, 441);
    gv_add(&expected_divisors, 490);
    gv_add(&expected_divisors, 504);
    gv_add(&expected_divisors, 588);
    gv_add(&expected_divisors, 630);
    gv_add(&expected_divisors, 735);
    gv_add(&expected_divisors, 840);
    gv_add(&expected_divisors, 882);
    gv_add(&expected_divisors, 980);
    gv_add(&expected_divisors, 1176);
    gv_add(&expected_divisors, 1260);
    gv_add(&expected_divisors, 1470);
    gv_add(&expected_divisors, 1764);
    gv_add(&expected_divisors, 1960);
    gv_add(&expected_divisors, 2205);
    gv_add(&expected_divisors, 2520);
    gv_add(&expected_divisors, 2940);
    gv_add(&expected_divisors, 3528);
    gv_add(&expected_divisors, 4410);
    gv_add(&expected_divisors, 5880);
    gv_add(&expected_divisors, 8820);
    gv_add(&expected_divisors, 17640);

    /* }}} */

    get_all_primes_factors_of_n(17640, &primes, &primes_factors);

    ASSERT_EQUAL_INT((int)primes_factors.len, 4);

    ASSERT_EQUAL_INT((int)primes_factors.tab[0].prime, 2);
    ASSERT_EQUAL_INT(primes_factors.tab[0].iteration, 3);

    ASSERT_EQUAL_INT((int)primes_factors.tab[1].prime, 3);
    ASSERT_EQUAL_INT(primes_factors.tab[1].iteration, 2);

    ASSERT_EQUAL_INT((int)primes_factors.tab[2].prime, 5);
    ASSERT_EQUAL_INT(primes_factors.tab[2].iteration, 1);

    ASSERT_EQUAL_INT((int)primes_factors.tab[3].prime, 7);
    ASSERT_EQUAL_INT(primes_factors.tab[3].iteration, 2);

    get_all_divisors_of_n_from_prime_factors(&primes_factors,
                                             &obtained_divisors);
    check_obtained_divisors_of_n(17640, &obtained_divisors,
                                 &expected_divisors);

    /* }}} */

    gv_wipe(&primes, NULL);
    gv_wipe(&primes_factors, NULL);
    gv_wipe(&obtained_divisors, NULL);
    gv_wipe(&expected_divisors, NULL);
}

static void
get_and_check_divisors_count_of_n(const gv_t(uint64) *primes, unsigned int n,
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

    get_and_check_divisors_count_of_n(&primes, 12, 6);
    get_and_check_divisors_count_of_n(&primes, 96, 12);
    get_and_check_divisors_count_of_n(&primes, 117, 6);
    get_and_check_divisors_count_of_n(&primes, 169, 3);
    get_and_check_divisors_count_of_n(&primes, 174, 8);

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
    ADD_TEST_TO_MODULE(module_tests, test_get_all_divisors_of_n);
    ADD_TEST_TO_MODULE(module_tests, test_get_divisors_count);
    ADD_TEST_TO_MODULE(module_tests, test_get_phi_from_primes_factors);
    ADD_TEST_TO_MODULE(module_tests, test_get_all_phi_from_1_to_n);

    return module_tests;

}
