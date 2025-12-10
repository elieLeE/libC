#include "test_big_numbers.h"

#include "../src/math/big_numbers.h"
#include "../src/macros.h"
#include "../src/mem/mem.h"

static void
check_bn_value_str(const big_number_t *bn, const char *expected_str)
{
    char *bn_str;

    bn_str = bn_to_str(bn);

    ASSERT(strcmp(bn_str, expected_str) == 0,
           "\nbig numbers string expected: %s\n,"
           "                  obtained: %s\n", expected_str, bn_str);

    p_free((void **)&bn_str);
}

/* {{{ setting tests */

static void test_bn_set_from_bn(void)
{
    big_number_t bn, bn2;

    bn_init(&bn);
    bn_init(&bn2);

    /* {{{ Test with 100000000000000002 then reset to -6575901 */
    /* {{{ set bn to 100000000000000002 */

    bn_set_from_ul(&bn, 100000000000000002);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);

    check_bn_value_str(&bn, "100000000000000002");

    /* }}} */
    /* {{{ set bn to -6575901 */

    bn_set_from_l(&bn2, -6575901);

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 6575901L);
    ASSERT(!bn2.positive_number, "this big number is a negative one");

    check_bn_value_str(&bn2, "-6575901");

    /* }}} */
    /* reset bn from bn2 */

    bn_set_from_bn(&bn2, &bn);

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 6575901L);
    ASSERT(!bn2.positive_number, "this big number is a negative one");

    check_bn_value_str(&bn, "-6575901");

    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&bn2);
}

static void test_bn_set_from_ul(void)
{
    big_number_t bn;

    bn_init_with_args(&bn, 0, 10000000);

    /* {{{ Test with 100000000000000002 */

    bn_set_from_ul(&bn, 100000000000000002);

    /* the first part is in reality : 0000002. So, mathematically, it is 2.
     * Same fot he others parts. */
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 0L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1000L);

    check_bn_value_str(&bn, "100000000000000002");

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_ul(&bn, 100000006050900972);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 900972L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 605L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1000L);

    check_bn_value_str(&bn, "100000006050900972");

    /* }}} */

    bn_wipe(&bn);
}

static void test_bn_set_from_l(void)
{
    big_number_t bn;

    bn_init(&bn);

    /* {{{ Test with -6575901 */

    bn_set_from_l(&bn, -6575901);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6575901L);
    ASSERT(!bn.positive_number, "this big number is a negative one");

    check_bn_value_str(&bn, "-6575901");

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_l(&bn, -100000006050900972);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6050900972L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be a negative number");

    check_bn_value_str(&bn, "-100000006050900972");

    /* }}} */

    bn_wipe(&bn);
}

/* }}} */

module_tests_t *get_all_tests_big_numbers(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "BIG_NUMBERS");
    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_l);
    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_bn);

    return module_tests;
}
