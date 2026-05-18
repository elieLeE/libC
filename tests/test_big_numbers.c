#include "test_big_numbers.h"

#include <limits.h>

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

/* {{{ Helpers tests */

static void test_bn_get_digits_count(void)
{
    big_number_t bn;

    bn_init(&bn);

    bn_init_with_args(&bn, 0, 10000000);

    bn_set_from_ul(1768928368, &bn);
    ASSERT_EQUAL_INT(bn_get_digits_count(&bn), 10);

    bn_set_limit(&bn, 100);
    bn_set_from_ul(154, &bn);
    ASSERT_EQUAL_INT(bn_get_digits_count(&bn), 3);

    bn_wipe(&bn);
}

static void test_bn_cmp(void)
{
    big_number_t bn, bn2;

    bn_init(&bn);
    bn_init(&bn2);

    bn_init_with_args(&bn, 0, 10000000);
    bn_init_with_args(&bn2, 0, 10000000);

    bn_set_from_ul(1768928368, &bn);
    bn_set_from_ul(1768928367, &bn2);
    assert((bn_cmp(&bn, &bn2) > 0));

    bn_set_from_ul(1768928368, &bn);
    bn_set_from_ul(2768928367, &bn2);
    assert((bn_cmp(&bn, &bn2) < 0));

    bn_set_from_ul(31768928368, &bn);
    bn_set_from_ul(2768928367, &bn2);
    assert((bn_cmp(&bn, &bn2) > 0));

    bn_set_from_ul(99, &bn);
    bn_set_from_ul(27689283678888, &bn2);
    assert((bn_cmp(&bn, &bn2) < 0));

    bn_set_from_ul(27689283678888, &bn);
    bn_set_from_ul(27689283678888, &bn2);
    assert((bn_cmp(&bn, &bn2) == 0));

    bn_set_from_ul(2, &bn);
    bn_set_from_ul(7, &bn2);
    assert((bn_cmp(&bn, &bn2) < 0));

    bn_wipe(&bn);
    bn_wipe(&bn2);
}

static void test_bn_cmp_ul(void)
{
    big_number_t bn;

    bn_init(&bn);

    bn_init_with_args(&bn, 0, 10000000);

    bn_set_from_ul(1768928368, &bn);
    assert((bn_cmp_ul(&bn, 1768928367) > 0));

    bn_set_from_ul(1768928368, &bn);
    assert((bn_cmp_ul(&bn, 2768928367) < 0));

    bn_set_from_ul(31768928368, &bn);
    assert((bn_cmp_ul(&bn, 2768928367) > 0));

    bn_set_from_ul(27689283678888, &bn);
    assert((bn_cmp_ul(&bn, 99) > 0));

    bn_set_from_ul(27689283678888, &bn);
    assert((bn_cmp_ul(&bn, 27689283678888) == 0));

    bn_set_from_ul(2, &bn);
    assert((bn_cmp_ul(&bn, 7) < 0));

    bn_wipe(&bn);
}

/* }}} */
/* {{{ Setting tests */

static void test_bn_set_from_bn(void)
{
    big_number_t bn, bn2;

    bn_init(&bn);
    bn_init(&bn2);

    /* {{{ Test with 100000000000000002 then reset to -6575901 */
    /* {{{ set bn to 100000000000000002 */

    bn_set_from_ul(100000000000000002, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);

    check_bn_value_str(&bn, "100000000000000002");

    /* }}} */
    /* {{{ set bn to -6575901 */

    bn_set_from_l(-6575901, &bn2);

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

    bn_set_from_ul(100000000000000002, &bn);

    /* the first part is in reality : 0000002. So, mathematically, it is 2.
     * Same fot he others parts. */
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 0L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1000L);

    check_bn_value_str(&bn, "100000000000000002");

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_ul(100000006050900972, &bn);

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

    bn_set_from_l(-6575901, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6575901L);
    ASSERT(!bn.positive_number, "this big number is a negative one");

    check_bn_value_str(&bn, "-6575901");

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_l(-100000006050900972, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6050900972L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be a negative number");

    check_bn_value_str(&bn, "-100000006050900972");

    /* }}} */

    bn_wipe(&bn);
}

/* }}} */
/* {{{ Adding tests */
/* {{{ test_bn_pos_bn_add_* */

static void test_bn_pos_bn_add_pos_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 100);
    bn_init_with_args(&bn2, 0, 100);
    bn_init(&bn3);

    /* {{{ longest pos BN1 + lowest pos BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = 5 */

    bn_set_from_ul(9, &bn1);
    bn_set_from_ul(5, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 5L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 14L);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = 14 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 13L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 1L);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = 1 */

    bn_set_from_ul(99999999, &bn1);
    bn_set_from_ul(1, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 99L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 5L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 0L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 0L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 0L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 0L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[4], 1L);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 16L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 35L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 33L);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 75L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[5], 70L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 79L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 66L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[3], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 47L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 83L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 76L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[5], 70L);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 + longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = 5 and bn2 = 9 */

    bn_set_from_ul(9, &bn1);
    bn_set_from_ul(5, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 5L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 14 and bn2 = 99 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 13L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 1 and bn2 = 99999999 */

    bn_set_from_ul(99999999, &bn1);
    bn_set_from_ul(1, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 99L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 5L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 0L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 0L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 0L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[3], 0L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[4], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 16L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 35L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 33L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 75L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[5], 70L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 79L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 66L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[3], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 47L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 83L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[3], 76L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[5], 70L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 + pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = 5 and bn2 = 9 */

    bn_set_from_ul(5, &bn1);
    bn_set_from_ul(9, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 5L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 9L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 14L);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 14 and bn2 = 99 */

    bn_set_from_ul(14, &bn1);
    bn_set_from_ul(99, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 14L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 99L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 13L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[1], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 16L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[1], 35L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[2], 33L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 67L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 16L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[3], 75L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[5], 70L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 98L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 79L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 66L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[3], 1L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 6L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[1], 47L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[2], 83L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[3], 76L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[4], 97L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[5], 70L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_pos_bn_add_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 10000000);
    bn_init_with_args(&res, 0, 10000000);

    /* {{{ pos BN + ul => BN
     * Add a unsigned long to a positive big number */

    bn_set_from_ul(1768928368, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 8928368L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    check_bn_value_str(&bn, "1768928368");

    bn_add_ul(&bn, 78865, &bn);
    /* Only the first part has been modified */
    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 9007233L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    bn_add_ul(&bn, 787363765, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6370998L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 255L);

    bn_add_ul(&bn, 5693637787363765, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3734763L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 9364034L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 56L);

    bn_set_from_l(2, &bn);
    bn_add_ul(&bn, ULONG_MAX -1, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 9551616L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 4407370L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 184467L);

    check_bn_value_str(&bn, "18446744073709551616");

    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a positive big number with result in a
     * another big_number_t */

    bn_set_from_ul(1768928368, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 8928368L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    check_bn_value_str(&bn, "1768928368");

    bn_add_ul(&bn, 78865, &res);
    /* Only the first part has been modified */
    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 9007233L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 176L);

    bn_set_from_bn(&res, &bn);
    bn_add_ul(&bn, 787363765, &res);
    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 6370998L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 255L);

    bn_set_from_bn(&res, &bn);
    bn_add_ul(&bn, 5693637787363765, &res);
    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 3734763L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 9364034L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 56L);

    check_bn_value_str(&res, "5693640343734763");

    bn_set_from_l(2, &bn);
    bn_add_ul(&bn, ULONG_MAX -1, &res);
    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 9551616L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 4407370L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 184467L);

    check_bn_value_str(&res, "18446744073709551616");

    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_pos_bn_add_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 10000000);
    bn_init_with_args(&res, 0, 10000000);

    /* {{{ pos BN + l => BN
     * Add a long to a positive big number */

    bn_set_from_l(1768928368, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 8928368L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    check_bn_value_str(&bn, "1768928368");

    bn_add_l(&bn, 78865, &bn);
    /* Only the first part has been modified */
    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 9007233L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    bn_add_l(&bn, -787363765, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 1643468L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 98L);
    ASSERT(bn.positive_number, "bn should be positive");
    check_bn_value_str(&bn, "981643468");

    bn_add_l(&bn, -5693637787363765, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 5720297L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 9363680L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 56L);
    ASSERT(!bn.positive_number, "bn should be negative");
    check_bn_value_str(&bn, "-5693636805720297");

    bn_add_l(&bn, LONG_MAX, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 9055510L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 7840004L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 92176L);

    check_bn_value_str(&bn, "9217678400049055510");

    /* {{{ bn = LONG_MAX - 1 and n = - (LONG_MAX - 1) */

    bn_set_from_l(LONG_MAX - 1, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 92233L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_add_l(&bn, - (LONG_MAX - 1), &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a positive big number with result in a
     * another big_number_t */

    bn_set_from_l(-1768928368, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 8928368L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 176L);

    check_bn_value_str(&bn, "-1768928368");

    bn_add_l(&bn, -78865, &res);
    /* Only the first part has been modified */
    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 9007233L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 176L);
    ASSERT(!res.positive_number, "bn should be negative");

    bn_set_from_bn(&res, &bn);
    bn_add_l(&bn, 787363765, &res);
    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 1643468L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 98L);
    ASSERT(!res.positive_number, "bn should be negative");

    bn_set_from_bn(&res, &bn);
    bn_add_l(&bn, 5693637787363765, &res);
    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 5720297L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 9363680L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 56L);
    ASSERT(res.positive_number, "bn should be positive");

    check_bn_value_str(&res, "5693636805720297");

    bn_set_from_bn(&res, &bn);
    bn_add_l(&bn, - (LONG_MAX -1), &res);
    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 9055509L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 7840004L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 92176L);
    ASSERT(!res.positive_number, "bn should be negative");

    check_bn_value_str(&res, "-9217678400049055509");

    /* {{{ bn = LONG_MAX - 1 and n = - (LONG_MAX - 1) */

    bn_set_from_l(LONG_MAX - 1, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 92233L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_add_l(&bn, - (LONG_MAX - 1), &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 0L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ test_bn_neg_bn_add_* */

static void test_bn_neg_bn_add_neg_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 10000000);
    bn_init_with_args(&bn2, 0, 10000000);
    bn_init(&bn3);

    /* {{{ longest pos BN1 + lowest pos BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = -5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 5L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 14L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = -14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 113L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -99999999 and bn2 = -1 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9999999L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 9L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 1L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 0L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 10L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 92233L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 92233L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9551612L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 4407370L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 184467L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 + longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 5L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -14 and bn2 = -99 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 99L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 14L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 113L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -1 and bn2 = -99999999 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9999999L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 9L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 1L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 0L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 10L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 92233L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 92233L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 9551612L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 4407370L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 184467L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 + pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-5, &bn1);
    bn_set_from_l(-9, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 5L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 9L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 14L);
    ASSERT(!bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -14 and bn2 = -99 */

    bn_set_from_l(14, &bn1);
    bn_set_from_l(99, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 14L);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 99L);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 113L);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -1 and bn2 = -99999999 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_LONG(bn1.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 9999999L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 9L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_LONG(bn2.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 1L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 0L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[1], 10L);
    ASSERT(!bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_LONG(bn1.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn1.parts.tab[2], 92233L);
    ASSERT(!bn1.positive_number, "bn should be positive");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_LONG(bn2.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[0], 4775806L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[1], 7203685L);
    ASSERT_EQUAL_LONG(bn2.parts.tab[2], 92233L);
    ASSERT(!bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_LONG(bn3.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[0], 9551612L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[1], 4407370L);
    ASSERT_EQUAL_LONG(bn3.parts.tab[2], 184467L);
    ASSERT(!bn3.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_neg_bn_add_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ neg BN + ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = -5577 and n = 5577 */

    bn_set_from_l(-5577, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 77L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 55L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 5577, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");
    check_bn_value_str(&bn, "0");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 4, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 99L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 100, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 400, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 15367, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 315367 , &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 22L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -873672 and n = 873672 */

    bn_set_from_l(-873672, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 72L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 87L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 873672, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");
    check_bn_value_str(&bn, "0");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 4, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 1L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 3, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 99L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 100, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 2L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 400, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 3L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 15367, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 1L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 315367 , &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 22L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 315367 and n = -543628 */

    bn_set_from_l(-315367 , &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 67L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 53L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 31L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 543628, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 22L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ test_bn_pos_bn_sub_* */

static void test_bn_pos_bn_sub_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ pos BN - ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = 3332 and n = 3332 */

    bn_set_from_l(3332, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 32L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 33L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3332, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 4 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 4, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 3 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 99L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 102 and n = 100 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 100, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 400 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 400, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 15367 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 15367, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 543628 and n = 315367 */

    bn_set_from_l(543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 315367 , &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 22L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = LONG_MAX and n = LONG_MAX */

    bn_set_from_l(LONG_MAX, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 10L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 7L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 58L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 77L);
    ASSERT_EQUAL_LONG(bn.parts.tab[3], 54L);
    ASSERT_EQUAL_LONG(bn.parts.tab[4], 68L);
    ASSERT_EQUAL_LONG(bn.parts.tab[5], 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[6], 72L);
    ASSERT_EQUAL_LONG(bn.parts.tab[7], 33L);
    ASSERT_EQUAL_LONG(bn.parts.tab[8], 22L);
    ASSERT_EQUAL_LONG(bn.parts.tab[9], 9L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, LONG_MAX, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 4 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 4, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 1L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 3 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 99L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 102 and n = 100 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 100, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 2L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 400 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 400, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 3L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 15367 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 15367, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 1L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 543628 and n = 315367 */

    bn_set_from_l(543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 315367 , &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 22L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 315367 and n = -543628 */

    bn_set_from_l(315367 , &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 67L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 53L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 31L);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 543628, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 22L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ test_bn_neg_bn_sub_* */

static void test_bn_neg_bn_add_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ neg BN + ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -100, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 4, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 99L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 100, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 400, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 3L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -315367 , &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 95L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 89L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 85L);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 1) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 1), &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 10L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 58L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 77L);
    ASSERT_EQUAL_LONG(bn.parts.tab[3], 54L);
    ASSERT_EQUAL_LONG(bn.parts.tab[4], 68L);
    ASSERT_EQUAL_LONG(bn.parts.tab[5], 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[6], 72L);
    ASSERT_EQUAL_LONG(bn.parts.tab[7], 33L);
    ASSERT_EQUAL_LONG(bn.parts.tab[8], 22L);
    ASSERT_EQUAL_LONG(bn.parts.tab[9], 9L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 0L);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 4, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 1L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 3, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 99L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 100, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 2L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 1L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 3L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 400, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 2L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 97L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 3L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 2L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 65L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 52L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 1L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -15367 and n = 15367 => 0 */

    bn_set_from_l(-15367, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 67L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 53L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 1L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 0L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_LONG(bn.parts.len, 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 28L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 36L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 54L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 315367 , &res);

    ASSERT_EQUAL_LONG(res.parts.len, 3L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 82L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 22L);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 315367 and n = -543628 */

    bn_set_from_l(-6789544, &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 4L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 44L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 95L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 78L);
    ASSERT_EQUAL_LONG(bn.parts.tab[3], 6L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -822890, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 4L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 34L);
    ASSERT_EQUAL_LONG(res.parts.tab[1], 24L);
    ASSERT_EQUAL_LONG(res.parts.tab[2], 61L);
    ASSERT_EQUAL_LONG(res.parts.tab[3], 7L);
    ASSERT(!res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 1) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 1), &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 10L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 6L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 58L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 77L);
    ASSERT_EQUAL_LONG(bn.parts.tab[3], 54L);
    ASSERT_EQUAL_LONG(bn.parts.tab[4], 68L);
    ASSERT_EQUAL_LONG(bn.parts.tab[5], 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[6], 72L);
    ASSERT_EQUAL_LONG(bn.parts.tab[7], 33L);
    ASSERT_EQUAL_LONG(bn.parts.tab[8], 22L);
    ASSERT_EQUAL_LONG(bn.parts.tab[9], 9L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 0L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 2) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 2), &bn);
    ASSERT_EQUAL_LONG(bn.parts.len, 10L);
    ASSERT_EQUAL_LONG(bn.parts.tab[0], 5L);
    ASSERT_EQUAL_LONG(bn.parts.tab[1], 58L);
    ASSERT_EQUAL_LONG(bn.parts.tab[2], 77L);
    ASSERT_EQUAL_LONG(bn.parts.tab[3], 54L);
    ASSERT_EQUAL_LONG(bn.parts.tab[4], 68L);
    ASSERT_EQUAL_LONG(bn.parts.tab[5], 3L);
    ASSERT_EQUAL_LONG(bn.parts.tab[6], 72L);
    ASSERT_EQUAL_LONG(bn.parts.tab[7], 33L);
    ASSERT_EQUAL_LONG(bn.parts.tab[8], 22L);
    ASSERT_EQUAL_LONG(bn.parts.tab[9], 9L);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &res);

    ASSERT_EQUAL_LONG(res.parts.len, 1L);
    ASSERT_EQUAL_LONG(res.parts.tab[0], 1L);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */

module_tests_t *get_all_tests_big_numbers(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "BIG_NUMBERS");

    ADD_TEST_TO_MODULE(module_tests, test_bn_get_digits_count);

    ADD_TEST_TO_MODULE(module_tests, test_bn_cmp);
    ADD_TEST_TO_MODULE(module_tests, test_bn_cmp_ul);

    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_l);
    ADD_TEST_TO_MODULE(module_tests, test_bn_set_from_bn);

    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_pos_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_neg_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_sub_ul);

    return module_tests;
}
