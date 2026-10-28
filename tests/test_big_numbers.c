#include "test_big_numbers.h"

#include <limits.h>
#include <gmp.h>

#include "../src/math/big_numbers.h"
#include "../src/macros.h"
#include "../src/mem/mem.h"


static bool cmp_bn_mpz(const big_number_t *bn, const mpz_t *mpz)
{
    bool res = true;
    char *gmp_str, *bn_str;

    gmp_str = mpz_get_str(NULL, 10, *mpz);
    bn_str = bn_to_str(bn);

    if (strcmp(gmp_str, bn_str) != 0) {
        res = false;
        logger_error("\nbig numbers string expected: %s\n,"
                     "                  obtained: %s\n", gmp_str, bn_str);
    }

    p_free((void **)&gmp_str);
    p_free((void **)&bn_str);

    return res;
}

static bool
check_bn_value_str(const big_number_t *bn, const char *expected_str)
{
    char *bn_str;
    bool res = true;

    bn_str = bn_to_str(bn);

    if (strcmp(bn_str, expected_str) != 0) {
        res = false;
        logger_error("\nbig numbers string expected: %s\n,"
                     "                  obtained: %s\n", expected_str, bn_str);
    }

    p_free((void **)&bn_str);

    return res;
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

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);

    assert(check_bn_value_str(&bn, "100000000000000002"));

    /* }}} */
    /* {{{ set bn to -6575901 */

    bn_set_from_l(-6575901, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 6575901);
    ASSERT(!bn2.positive_number, "this big number is a negative one");

    assert(check_bn_value_str(&bn2, "-6575901"));

    /* }}} */
    /* reset bn from bn2 */

    bn_set_from_bn(&bn2, &bn);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 6575901);
    ASSERT(!bn2.positive_number, "this big number is a negative one");

    assert(check_bn_value_str(&bn, "-6575901"));

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
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1000);

    assert(check_bn_value_str(&bn, "100000000000000002"));

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_ul(100000006050900972, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 900972);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 605);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1000);

    assert(check_bn_value_str(&bn, "100000006050900972"));

    /* }}} */

    bn_wipe(&bn);
}

static void test_bn_set_from_l(void)
{
    big_number_t bn;

    bn_init(&bn);

    /* {{{ Test with -6575901 */

    bn_set_from_l(-6575901, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6575901);
    ASSERT(!bn.positive_number, "this big number is a negative one");

    assert(check_bn_value_str(&bn, "-6575901"));

    /* }}} */
    /* {{{ Test with 10005228970000272820072000070002 */

    bn_set_from_l(-100000006050900972, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6050900972);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be a negative number");

    assert(check_bn_value_str(&bn, "-100000006050900972"));

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

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "5"));

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = 14 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "14"));

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = 1 */

    bn_set_from_ul(99999999, &bn1);
    bn_set_from_ul(1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "1"));

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 33);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "166799"));

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "1667998"));

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 + longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = 5 */

    bn_set_from_ul(9, &bn1);
    bn_set_from_ul(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = 14 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99"));
    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = 1 */

    bn_set_from_ul(99999999, &bn1);
    bn_set_from_ul(1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99999999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "166799"));
    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 33);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "709775166799"));
    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 + pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = 5 and bn2 = 9 */

    bn_set_from_ul(5, &bn1);
    bn_set_from_ul(9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "5"));
    assert(check_bn_value_str(&bn2, "9"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 14);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 14 and bn2 = 99 */

    bn_set_from_ul(14, &bn1);
    bn_set_from_ul(99, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "14"));
    assert(check_bn_value_str(&bn2, "99"));
    ASSERT_EQUAL_INT(bn3.parts.len, 2);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "166799"));
    assert(check_bn_value_str(&bn2, "166799"));
    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 33);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "709775166799"));
    assert(check_bn_value_str(&bn2, "1667998"));
    ASSERT_EQUAL_INT(bn3.parts.len, 6);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN + pos BN => BN
     * Test adding a big number with itself to itself */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn1, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 33);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 50);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 95);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 41);
    ASSERT_EQUAL_INT(bn1.parts.tab[6], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_pos_bn_add_neg_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 100);
    bn_init_with_args(&bn2, 0, 100);
    bn_init(&bn3);

    /* {{{ longest pos BN1 + lowest neg BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = -5 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-5"));

    /* }}} */
    /* {{{ bn1 = 100 and bn2 = -14 */

    bn_set_from_l(100, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 86);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-14"));

    /* }}} */
    /* {{{ bn1 = 100000001 and bn2 = -99999999 */

    bn_set_from_l(100000001, &bn1);
    bn_set_from_l(-99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 2);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-99999999"));

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = -166799 */

    bn_set_from_l(166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-166799"));

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = -1667998 */

    bn_set_from_l(709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-1667998"));

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 + longest neg BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = -11 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-11, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 11);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 2);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = -1114 */

    bn_set_from_l(99, &bn1);
    bn_set_from_l(-1114, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 11);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99"));
    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 15);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 10);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = -100000001 */

    bn_set_from_l(99999999, &bn1);
    bn_set_from_l(-100000001, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99999999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 2);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = -166799 */

    bn_set_from_l(166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "166799"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 1667998 and bn2 = -709775166799 */

    bn_set_from_l(1667998, &bn1);
    bn_set_from_l(-709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "1667998"));
    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 + neg BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = 9 and bn2 = -5 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "9"));
    assert(check_bn_value_str(&bn2, "-5"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 4);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 198 and bn2 = -199 */

    bn_set_from_l(198, &bn1);
    bn_set_from_l(-199, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "198"));
    assert(check_bn_value_str(&bn2, "-199"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 1);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = -100000001 */

    bn_set_from_l(99999999, &bn1);
    bn_set_from_l(-100000001, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "99999999"));
    assert(check_bn_value_str(&bn2, "-100000001"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 2);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = -166799 */

    bn_set_from_l(166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "166799"));
    assert(check_bn_value_str(&bn2, "-166799"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = -1667998 */

    bn_set_from_l(709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "709775166799"));
    assert(check_bn_value_str(&bn2, "-1667998"));
    ASSERT_EQUAL_INT(bn3.parts.len, 6);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[5], 70);
    ASSERT(bn3.positive_number, "bn should be positive");

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

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 8928368);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);

    assert(check_bn_value_str(&bn, "1768928368"));

    bn_add_ul(&bn, 78865, &bn);
    /* Only the first part has been modified */
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9007233);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);

    bn_add_ul(&bn, 787363765, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6370998);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 255);

    bn_add_ul(&bn, 5693637787363765, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3734763);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 9364034);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 56);

    bn_set_from_l(2, &bn);
    bn_add_ul(&bn, ULONG_MAX -1, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9551616);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 4407370);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 184467);

    assert(check_bn_value_str(&bn, "18446744073709551616"));

    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a positive big number with result in a
     * another big_number_t */

    bn_set_from_ul(1768928368, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 8928368);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);
    assert(check_bn_value_str(&bn, "1768928368"));

    bn_add_ul(&bn, 78865, &res);
    /* Only the first part has been modified */
    assert(check_bn_value_str(&bn, "1768928368"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9007233);
    ASSERT_EQUAL_INT(res.parts.tab[1], 176);
    assert(check_bn_value_str(&res, "1769007233"));

    bn_set_from_bn(&res, &bn);
    assert(check_bn_value_str(&bn, "1769007233"));
    bn_add_ul(&bn, 787363765, &res);
    assert(check_bn_value_str(&bn, "1769007233"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 6370998);
    ASSERT_EQUAL_INT(res.parts.tab[1], 255);
    assert(check_bn_value_str(&res, "2556370998"));

    bn_set_from_bn(&res, &bn);
    bn_add_ul(&bn, 5693637787363765, &res);
    assert(check_bn_value_str(&bn, "2556370998"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 3734763);
    ASSERT_EQUAL_INT(res.parts.tab[1], 9364034);
    ASSERT_EQUAL_INT(res.parts.tab[2], 56);
    assert(check_bn_value_str(&res, "5693640343734763"));

    bn_set_from_l(2, &bn);
    bn_add_ul(&bn, ULONG_MAX -1, &res);
    assert(check_bn_value_str(&bn, "2"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9551616);
    ASSERT_EQUAL_INT(res.parts.tab[1], 4407370);
    ASSERT_EQUAL_INT(res.parts.tab[2], 184467);
    assert(check_bn_value_str(&res, "18446744073709551616"));

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

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 8928368);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);

    assert(check_bn_value_str(&bn, "1768928368"));

    bn_add_l(&bn, 78865, &bn);
    /* Only the first part has been modified */
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9007233);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);

    bn_add_l(&bn, -787363765, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1643468);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 98);
    ASSERT(bn.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn, "981643468"));

    bn_add_l(&bn, -5693637787363765, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5720297);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 9363680);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 56);
    ASSERT(!bn.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn, "-5693636805720297"));

    bn_add_l(&bn, LONG_MAX, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9055510);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 7840004);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 92176);

    assert(check_bn_value_str(&bn, "9217678400049055510"));

    /* {{{ bn = LONG_MAX - 1 and n = - (LONG_MAX - 1) */

    bn_set_from_l(LONG_MAX - 1, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 92233);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_add_l(&bn, - (LONG_MAX - 1), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a positive big number with result in a
     * another big_number_t */

    bn_set_from_l(-1768928368, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 8928368);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 176);
    assert(check_bn_value_str(&bn, "-1768928368"));

    bn_add_l(&bn, -78865, &res);
    /* Only the first part has been modified */
    assert(check_bn_value_str(&bn, "-1768928368"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9007233);
    ASSERT_EQUAL_INT(res.parts.tab[1], 176);
    ASSERT(!res.positive_number, "bn should be negative");
    assert(check_bn_value_str(&res, "-1769007233"));

    bn_set_from_bn(&res, &bn);
    assert(check_bn_value_str(&bn, "-1769007233"));
    bn_add_l(&bn, 787363765, &res);
    assert(check_bn_value_str(&bn, "-1769007233"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1643468);
    ASSERT_EQUAL_INT(res.parts.tab[1], 98);
    ASSERT(!res.positive_number, "bn should be negative");
    assert(check_bn_value_str(&res, "-981643468"));

    bn_set_from_bn(&res, &bn);
    assert(check_bn_value_str(&bn, "-981643468"));
    bn_add_l(&bn, 5693637787363765, &res);
    assert(check_bn_value_str(&bn, "-981643468"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 5720297);
    ASSERT_EQUAL_INT(res.parts.tab[1], 9363680);
    ASSERT_EQUAL_INT(res.parts.tab[2], 56);
    ASSERT(res.positive_number, "bn should be positive");
    assert(check_bn_value_str(&res, "5693636805720297"));

    bn_set_from_bn(&res, &bn);
    assert(check_bn_value_str(&bn, "5693636805720297"));
    bn_add_l(&bn, - (LONG_MAX -1), &res);
    assert(check_bn_value_str(&bn, "5693636805720297"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9055509);
    ASSERT_EQUAL_INT(res.parts.tab[1], 7840004);
    ASSERT_EQUAL_INT(res.parts.tab[2], 92176);
    ASSERT(!res.positive_number, "bn should be negative");
    assert(check_bn_value_str(&res, "-9217678400049055509"));

    /* {{{ bn = LONG_MAX - 1 and n = - (LONG_MAX - 1) */

    bn_set_from_l(LONG_MAX - 1, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 92233);
    ASSERT(bn.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn, "9223372036854775806"));

    bn_add_l(&bn, - (LONG_MAX - 1), &res);

    assert(check_bn_value_str(&bn, "9223372036854775806"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
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

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "-5"));

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = -14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 113);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "-14"));

    /* }}} */
    /* {{{ bn1 = -99999999 and bn2 = -1 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9999999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 10);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "-1"));

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 92233);
    ASSERT(!bn1.positive_number, "bn should be negative");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 92233);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9551612);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 4407370);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 184467);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "-9223372036854775806"));

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 + longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -14 and bn2 = -99 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-99"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 113);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -1 and bn2 = -99999999 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9999999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-99999999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 10);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 92233);
    ASSERT(!bn1.positive_number, "bn should be negative");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 92233);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-9223372036854775806"));
    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9551612);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 4407370);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 184467);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 + pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-5, &bn1);
    bn_set_from_l(-9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-5"));
    assert(check_bn_value_str(&bn2, "-9"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 14);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -14 and bn2 = -99 */

    bn_set_from_l(14, &bn1);
    bn_set_from_l(99, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT(bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "14"));
    assert(check_bn_value_str(&bn2, "99"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 113);
    ASSERT(bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99999999 and bn2 = -1 */

    bn_set_from_l(-99999999, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9999999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-99999999"));
    assert(check_bn_value_str(&bn2, "-1"));
    ASSERT_EQUAL_INT(bn3.parts.len, 2);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 10);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 92233);
    ASSERT(!bn1.positive_number, "bn should be negative");

    bn_set_from_l(-(LONG_MAX - 1), &bn2);
    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4775806);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 7203685);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 92233);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-9223372036854775806"));
    assert(check_bn_value_str(&bn2, "-9223372036854775806"));
    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 9551612);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 4407370);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 184467);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN + neg BN => BN
     * Test adding a big number with itself to itself */
    /* {{{ bn1 = 709775166799 */

    bn_set_from_l(-709775166799, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5166799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 70977);
    ASSERT(!bn1.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn1, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 333598);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 141955);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_neg_bn_add_pos_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 100);
    bn_init_with_args(&bn2, 0, 100);
    bn_init(&bn3);

    /* {{{ longest neg BN1 + lowest pos BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "5"));

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 85);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "14"));

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 2);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "99999999"));

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "166799"));

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = 1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");
    assert(check_bn_value_str(&bn2, "1667998"));

    /* }}} */
    /* }}} */
    /* {{{ lowest neg BN1 + longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -9 and bn2 = 51 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(51, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 51);
    ASSERT(bn2.positive_number, "bn should be  positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 42);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-99"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 85);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-100000001"));
    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-166799"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -1667998 and bn2 = 709775166799 */

    bn_set_from_l(-1667998, &bn1);
    bn_set_from_ul(709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "-1667998"));
    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN1 + pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-9"));
    assert(check_bn_value_str(&bn2, "5"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 4);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-99"));
    assert(check_bn_value_str(&bn2, "14"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 85);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-100000001"));
    assert(check_bn_value_str(&bn2, "99999999"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 2);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-166799"));
    assert(check_bn_value_str(&bn2, "166799"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = 1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "-709775166799"));
    assert(check_bn_value_str(&bn2, "1667998"));
    ASSERT_EQUAL_INT(bn3.parts.len, 6);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 88);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 49);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[5], 70);
    ASSERT(!bn3.positive_number, "bn should be negative");

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

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 77);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 55);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn, "0"));

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 4, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 99);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 100, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 400, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 15367, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 65);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 52);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 315367 , &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 61);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 82);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 22);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -873672 and n = 873672 */

    bn_set_from_l(-873672, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 72);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 87);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 873672, &res);

    assert(check_bn_value_str(&bn, "-873672"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");
    assert(check_bn_value_str(&res, "0"));

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 4, &res);

    assert(check_bn_value_str(&bn, "-3"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 3, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 99);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 100, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 2);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 400, &res);

    assert(check_bn_value_str(&bn, "-3"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 97);
    ASSERT_EQUAL_INT(res.parts.tab[1], 3);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 15367, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 65);
    ASSERT_EQUAL_INT(res.parts.tab[1], 52);
    ASSERT_EQUAL_INT(res.parts.tab[2], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 315367 , &res);

    assert(check_bn_value_str(&bn, "-543628"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 61);
    ASSERT_EQUAL_INT(res.parts.tab[1], 82);
    ASSERT_EQUAL_INT(res.parts.tab[2], 22);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -315367 and n = 543628 */

    bn_set_from_l(-315367 , &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 67);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 53);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 31);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_ul(&bn, 543628, &res);

    assert(check_bn_value_str(&bn, "-315367"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 61);
    ASSERT_EQUAL_INT(res.parts.tab[1], 82);
    ASSERT_EQUAL_INT(res.parts.tab[2], 22);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_neg_bn_add_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ neg BN + ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -100, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 4, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 99);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 100, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 400, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 65);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 52);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -315367 , &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 95);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 89);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 85);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 1) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 1), &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 10);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 58);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 77);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 54);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 68);
    ASSERT_EQUAL_INT(bn.parts.tab[5], 3);
    ASSERT_EQUAL_INT(bn.parts.tab[6], 72);
    ASSERT_EQUAL_INT(bn.parts.tab[7], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[8], 22);
    ASSERT_EQUAL_INT(bn.parts.tab[9], 9);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 4, &res);

    assert(check_bn_value_str(&bn, "-3"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 3 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 3, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 99);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 100 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 100, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 2);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 400 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 400, &res);

    assert(check_bn_value_str(&bn, "-3"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 97);
    ASSERT_EQUAL_INT(res.parts.tab[1], 3);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -102 and n = 15367 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &res);

    assert(check_bn_value_str(&bn, "-102"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 65);
    ASSERT_EQUAL_INT(res.parts.tab[1], 52);
    ASSERT_EQUAL_INT(res.parts.tab[2], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -15367 and n = 15367 => 0 */

    bn_set_from_l(-15367, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 67);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 53);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 15367, &res);

    assert(check_bn_value_str(&bn, "-15367"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -543628 and n = 315367 */

    bn_set_from_l(-543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, 315367 , &res);

    assert(check_bn_value_str(&bn, "-543628"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 61);
    ASSERT_EQUAL_INT(res.parts.tab[1], 82);
    ASSERT_EQUAL_INT(res.parts.tab[2], 22);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -6789544 and n = -822890 */

    bn_set_from_l(-6789544, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 4);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 78);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, -822890, &res);

    assert(check_bn_value_str(&bn, "-6789544"));
    ASSERT_EQUAL_INT(res.parts.len, 4);
    ASSERT_EQUAL_INT(res.parts.tab[0], 34);
    ASSERT_EQUAL_INT(res.parts.tab[1], 24);
    ASSERT_EQUAL_INT(res.parts.tab[2], 61);
    ASSERT_EQUAL_INT(res.parts.tab[3], 7);
    ASSERT(!res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 1) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 1), &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 10);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 58);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 77);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 54);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 68);
    ASSERT_EQUAL_INT(bn.parts.tab[5], 3);
    ASSERT_EQUAL_INT(bn.parts.tab[6], 72);
    ASSERT_EQUAL_INT(bn.parts.tab[7], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[8], 22);
    ASSERT_EQUAL_INT(bn.parts.tab[9], 9);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &res);

    assert(check_bn_value_str(&bn, "-9223372036854775806"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = - (LONG_MAX - 2) and n = LONG_MAX - 1*/

    bn_set_from_l(- (LONG_MAX - 2), &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 10);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 58);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 77);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 54);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 68);
    ASSERT_EQUAL_INT(bn.parts.tab[5], 3);
    ASSERT_EQUAL_INT(bn.parts.tab[6], 72);
    ASSERT_EQUAL_INT(bn.parts.tab[7], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[8], 22);
    ASSERT_EQUAL_INT(bn.parts.tab[9], 9);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_add_l(&bn, LONG_MAX - 1, &res);

    assert(check_bn_value_str(&bn, "-9223372036854775805"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ test_bn_pos_bn_sub_* */

static void test_bn_pos_bn_sub_pos_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 1000);
    bn_init_with_args(&bn2, 0, 1000);
    bn_init(&bn3);

    /* {{{ longest pos BN1 - lowest pos BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = 5 */

    bn_set_from_ul(9, &bn1);
    bn_set_from_ul(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "5"));

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = 14 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 85);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "14"));

    /* }}} */
    /* {{{ bn1 = 1000000 and bn2 = 1 */

    bn_set_from_ul(1000000, &bn1);
    bn_set_from_ul(1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 999);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "1"));

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "166799"));

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 801);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 498);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 773);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "1667998"));

    /* }}} */
    /* {{{ bn1 = ULONG_MAX and bn2 = ULONG_MAX - 1 */

    bn_set_from_ul(ULONG_MAX, &bn1);
    bn_set_from_ul(ULONG_MAX - 1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 615);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 551);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 709);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 744);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 446);
    ASSERT_EQUAL_INT(bn1.parts.tab[6], 18);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 7);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 614);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 551);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 709);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 744);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 446);
    ASSERT_EQUAL_INT(bn2.parts.tab[6], 18);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "18446744073709551614"));

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 - longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = 5 and bn2 = 9 */

    bn_set_from_ul(5, &bn1);
    bn_set_from_ul(9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "5"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 999 and bn2 = 1000 */

    bn_set_from_ul(999, &bn1);
    bn_set_from_ul(1000, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 999999 and bn2 = 1000000  */

    bn_set_from_ul(999999, &bn1);
    bn_set_from_ul(1000000 , &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 999);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "999999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "166799"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 1667998 and bn2 = 709775166799 */

    bn_set_from_ul(1667998, &bn1);
    bn_set_from_ul(709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 709);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "1667998"));
    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 801);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 498);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 773);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 709);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = ULONG_MAX - 1 and bn2 = ULONG_MAX */

    bn_set_from_ul(ULONG_MAX - 1, &bn1);
    bn_set_from_ul(ULONG_MAX, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 614);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 551);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 709);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 744);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 446);
    ASSERT_EQUAL_INT(bn1.parts.tab[6], 18);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 615);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 551);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 709);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 73);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 744);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 446);
    ASSERT_EQUAL_INT(bn2.parts.tab[6], 18);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "18446744073709551614"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 - pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = 5 and bn2 = 9 */

    bn_set_from_ul(5, &bn1);
    bn_set_from_ul(9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "5"));
    assert(check_bn_value_str(&bn2, "9"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 4);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = 14 */

    bn_set_from_ul(99, &bn1);
    bn_set_from_ul(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "99"));
    assert(check_bn_value_str(&bn2, "14"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 85);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 10000000 and bn2 = 10 */

    bn_set_from_ul(10000000, &bn1);
    bn_set_from_ul(10, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 10);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 10);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "10000000"));
    assert(check_bn_value_str(&bn2, "10"));
    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 990);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 999);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 9);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = 166799 */

    bn_set_from_ul(166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "166799"));
    assert(check_bn_value_str(&bn2, "166799"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = 1667998 */

    bn_set_from_ul(709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_add_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "709775166799"));
    assert(check_bn_value_str(&bn2, "1667998"));
    ASSERT_EQUAL_INT(bn3.parts.len, 4);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 797);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 834);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 776);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 709);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN - pos BN => BN
     * Test adding a big number with itself to itself */
    /* {{{ bn = 709775166799 */

    bn_set_from_l(709775166799, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(bn1.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn1, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_pos_bn_sub_neg_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 100);
    bn_init_with_args(&bn2, 0, 100);
    bn_init(&bn3);

    /* {{{ longest pos BN1 - lowest neg BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = -5 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-5"));

    /* }}} */
    /* {{{ bn1 = 100 and bn2 = -14 */

    bn_set_from_l(100, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-14"));

    /* }}} */
    /* {{{ bn1 = 100000001 and bn2 = -99999999 */

    bn_set_from_l(100000001, &bn1);
    bn_set_from_l(-99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 2);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-99999999"));

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = -166799 */

    bn_set_from_l(166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 33);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-166799"));

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = -1667998 */

    bn_set_from_l(709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");
    assert(check_bn_value_str(&bn2, "-1667998"));

    /* }}} */
    /* }}} */
    /* {{{ lowest pos BN1 - longest neg BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = 9 and bn2 = -11 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-11, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 11);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "9"));
    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 20);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99 and bn2 = -1114 */

    bn_set_from_l(99, &bn1);
    bn_set_from_l(-1114, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 11);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99"));
    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 12);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = -100000001 */

    bn_set_from_l(99999999, &bn1);
    bn_set_from_l(-100000001, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "99999999"));
    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 2);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 1667998 and bn2 = -709775166799 */

    bn_set_from_l(1667998, &bn1);
    bn_set_from_l(-709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    assert(check_bn_value_str(&bn1, "1667998"));
    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN1 - neg BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = 9 and bn2 = -5 */

    bn_set_from_l(9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "9"));
    assert(check_bn_value_str(&bn2, "-5"));
    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 14);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 198 and bn2 = -199 */

    bn_set_from_l(198, &bn1);
    bn_set_from_l(-199, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "198"));
    assert(check_bn_value_str(&bn2, "-199"));
    ASSERT_EQUAL_INT(bn3.parts.len, 2);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 3);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 99999999 and bn2 = -100000001 */

    bn_set_from_l(99999999, &bn1);
    bn_set_from_l(-100000001, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 99);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 5);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "99999999"));
    assert(check_bn_value_str(&bn2, "-100000001"));
    ASSERT_EQUAL_INT(bn3.parts.len, 5);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 2);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 166799 and bn2 = -166799 */

    bn_set_from_l(166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "166799"));
    assert(check_bn_value_str(&bn2, "-166799"));
    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 33);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 709775166799 and bn2 = -1667998 */

    bn_set_from_l(709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    assert(check_bn_value_str(&bn1, "709775166799"));
    assert(check_bn_value_str(&bn2, "-1667998"));
    ASSERT_EQUAL_INT(bn3.parts.len, 6);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[5], 70);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_pos_bn_sub_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ pos BN - ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = 3332 and n = 3332 */

    bn_set_from_l(3332, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 32);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 33);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3332, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 4 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 4, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 3 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 99);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 102 and n = 100 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 100, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 400 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 400, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 15367 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 15367, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 65);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 52);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 543628 and n = 315367 */

    bn_set_from_l(543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 315367 , &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 61);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 82);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 22);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN + ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = LONG_MAX and n = LONG_MAX */

    bn_set_from_l(LONG_MAX, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 10);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 7);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 58);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 77);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 54);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 68);
    ASSERT_EQUAL_INT(bn.parts.tab[5], 3);
    ASSERT_EQUAL_INT(bn.parts.tab[6], 72);
    ASSERT_EQUAL_INT(bn.parts.tab[7], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[8], 22);
    ASSERT_EQUAL_INT(bn.parts.tab[9], 9);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, LONG_MAX, &res);

    assert(check_bn_value_str(&bn, "9223372036854775807"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 4 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 4, &res);

    assert(check_bn_value_str(&bn, "3"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 3 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 3, &res);

    assert(check_bn_value_str(&bn, "102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 99);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 102 and n = 100 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 100, &res);

    assert(check_bn_value_str(&bn, "102"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 2);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3 and n = 400 */

    bn_set_from_l(3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 400, &res);

    assert(check_bn_value_str(&bn, "3"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 97);
    ASSERT_EQUAL_INT(res.parts.tab[1], 3);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 102 and n = 15367 */

    bn_set_from_l(102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 15367, &res);

    assert(check_bn_value_str(&bn, "102"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 65);
    ASSERT_EQUAL_INT(res.parts.tab[1], 52);
    ASSERT_EQUAL_INT(res.parts.tab[2], 1);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 543628 and n = 315367 */

    bn_set_from_l(543628, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 28);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 36);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 54);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 315367 , &res);

    assert(check_bn_value_str(&bn, "543628"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 61);
    ASSERT_EQUAL_INT(res.parts.tab[1], 82);
    ASSERT_EQUAL_INT(res.parts.tab[2], 22);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 315367 and n = -543628 */

    bn_set_from_l(315367 , &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 67);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 53);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 31);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_ul(&bn, 543628, &res);

    assert(check_bn_value_str(&bn, "315367"));
    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 61);
    ASSERT_EQUAL_INT(res.parts.tab[1], 82);
    ASSERT_EQUAL_INT(res.parts.tab[2], 22);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_pos_bn_sub_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 10000);
    bn_init_with_args(&res, 0, 10000);

    /* {{{ pos BN - l => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = 9999 and n = 9999 */

    bn_set_from_l(9999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9999);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_l(&bn, 9999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 3333 and n = 4000 */

    bn_set_from_l(3333, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3333);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_l(&bn, 40000, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6667);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 10002 and n = -3776 */

    bn_set_from_l(10002, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 2);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_l(&bn, -3776, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3778);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN - l => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = LONG_MAX and n = LONG_MAX - 77654 */

    bn_set_from_l(LONG_MAX, &bn);
    ASSERT_EQUAL_INT(bn.parts.len, 5);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5807);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 5477);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 368);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 3372);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 922);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_sub_l(&bn, LONG_MAX - 77654, &res);

    assert(check_bn_value_str(&bn, "9223372036854775807"));
    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 7654);
    ASSERT_EQUAL_INT(res.parts.tab[1], 7);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ test_bn_neg_bn_sub_* */

static void test_bn_neg_bn_sub_neg_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 1000);
    bn_init_with_args(&bn2, 0, 1000);
    bn_init(&bn3);

    /* {{{ longest neg BN1 - lowest neg BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = -5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(-5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 4);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = -14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 85);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -1000000 and bn2 = -1 */

    bn_set_from_l(-1000000, &bn1);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 999);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = -166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = -1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 801);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 498);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 773);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 1) and bn2 = -(LONG_MAX - 2) */

    bn_set_from_l(-(LONG_MAX - 1), &bn1);
    bn_set_from_l(-(LONG_MAX - 2), &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 806);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 854);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 36);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 372);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 223);
    ASSERT_EQUAL_INT(bn1.parts.tab[6], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 7);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 805);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 775);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 854);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 36);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 372);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 223);
    ASSERT_EQUAL_INT(bn2.parts.tab[6], 9);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ lowest neg BN1 - longest neg BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-5, &bn1);
    bn_set_from_l(-9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 4);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = 999 and bn2 = 1000 */

    bn_set_from_l(-999, &bn1);
    bn_set_from_l(-1000, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -999999 and bn2 = -1000000  */

    bn_set_from_l(-999999, &bn1);
    bn_set_from_l(-1000000 , &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 999);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 999);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = -166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -1667998 and bn2 = -709775166799 */

    bn_set_from_l(-1667998, &bn1);
    bn_set_from_l(-709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 709);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 801);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 498);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 773);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 709);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -(LONG_MAX - 2) and bn2 = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 2), &bn1);
    bn_set_from_l(-(LONG_MAX - 1), &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 7);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 805);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 854);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 36);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 372);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 223);
    ASSERT_EQUAL_INT(bn1.parts.tab[6], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 7);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 806);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 775);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 854);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 36);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 372);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 223);
    ASSERT_EQUAL_INT(bn2.parts.tab[6], 9);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN1 - neg BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = -5 and bn2 = -9 */

    bn_set_from_l(-5, &bn1);
    bn_set_from_l(-9, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 5);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 9);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 4);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = -14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(-14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 85);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -10000000 and bn2 = -10 */

    bn_set_from_l(-10000000, &bn1);
    bn_set_from_l(-10, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 10);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 10);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 990);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 999);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 9);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = -166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_l(-166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT(bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = -1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_l(-1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 998);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 667);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_add_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 4);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 797);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 834);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 776);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 709);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN - neg BN => BN
     * Test adding a big number with itself to itself */
    /* {{{ bn = -709775166799 */

    bn_set_from_l(-709775166799, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 799);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 166);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 775);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 709);
    ASSERT(!bn1.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn1, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT(bn1.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_neg_bn_sub_pos_bn(void)
{
    big_number_t bn1, bn2, bn3;

    bn_init_with_args(&bn1, 0, 100);
    bn_init_with_args(&bn2, 0, 100);
    bn_init(&bn3);

    /* {{{ longest neg BN1 - lowest pos BN2 => BN1
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the longest one in the addition (the first one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 14);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 2);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 1);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 2);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 33);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = 1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn1);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ lowest neg BN1 - longest pos BN2 => BN2
     * Test with 2 positives BNs and result has to be set in the same
     * variable than the lowest one in the addition (the second one if both
     * have same length) */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -9 and bn2 = 51 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(51, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 51);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 60);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 9);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 10);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 33);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -1667998 and bn2 = 709775166799 */

    bn_set_from_l(-1667998, &bn1);
    bn_set_from_ul(709775166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 4);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 1);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn2.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn2.parts.tab[5], 70);
    ASSERT(!bn2.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN1 - pos BN2 => BN3
     * Test with 2 positives BNs and result in another variable */
    /* {{{ bn1 = -9 and bn2 = 5 */

    bn_set_from_l(-9, &bn1);
    bn_set_from_l(5, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 9);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 1);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 14);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -99 and bn2 = 14 */

    bn_set_from_l(-99, &bn1);
    bn_set_from_l(14, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 14);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 2);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 13);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 1);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -100000001 and bn2 = 99999999 */

    bn_set_from_l(-100000001, &bn1);
    bn_set_from_l(99999999, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 5);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 1);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 1);
    ASSERT(!bn1.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 99);
    ASSERT(bn2.positive_number, "bn should be negative");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 5);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 0);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 2);
    ASSERT(!bn3.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn1 = -166799 and bn2 = 166799 */

    bn_set_from_l(-166799, &bn1);
    bn_set_from_ul(166799, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 3);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 16);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 3);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 33);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn1 = -709775166799 and bn2 = 1667998 */

    bn_set_from_l(-709775166799, &bn1);
    bn_set_from_ul(1667998, &bn2);

    ASSERT_EQUAL_INT(bn1.parts.len, 6);
    ASSERT_EQUAL_INT(bn1.parts.tab[0], 99);
    ASSERT_EQUAL_INT(bn1.parts.tab[1], 67);
    ASSERT_EQUAL_INT(bn1.parts.tab[2], 16);
    ASSERT_EQUAL_INT(bn1.parts.tab[3], 75);
    ASSERT_EQUAL_INT(bn1.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn1.parts.tab[5], 70);
    ASSERT(!bn1.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 4);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 98);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 79);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 66);
    ASSERT_EQUAL_INT(bn2.parts.tab[3], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_sub_bn(&bn1, &bn2, &bn3);

    ASSERT_EQUAL_INT(bn3.parts.len, 6);
    ASSERT_EQUAL_INT(bn3.parts.tab[0], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[1], 47);
    ASSERT_EQUAL_INT(bn3.parts.tab[2], 83);
    ASSERT_EQUAL_INT(bn3.parts.tab[3], 76);
    ASSERT_EQUAL_INT(bn3.parts.tab[4], 97);
    ASSERT_EQUAL_INT(bn3.parts.tab[5], 70);
    ASSERT(!bn3.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn1);
    bn_wipe(&bn2);
    bn_wipe(&bn3);
}

static void test_bn_neg_bn_sub_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 10000);
    bn_init_with_args(&res, 0, 10000);

    /* {{{ neg BN - ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = -5577 and n = 5577 */

    bn_set_from_l(-5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5577);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1154);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -31008 and n = 49999 */

    bn_set_from_l(-31008, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1008);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 49999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1007);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 8);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -10255 and n = 3499 */

    bn_set_from_l(-10255, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 255);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 3499, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3754);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -(LONG_MAX - 1) and n = LONG_MAX */

    bn_set_from_l(-(LONG_MAX - 1), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 5);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5806);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 5477);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 368);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 3372);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 922);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, LONG_MAX - 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 5);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1612);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 955);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 737);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 6744);
    ASSERT_EQUAL_INT(bn.parts.tab[4], 1844);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN - ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -873672 and n = 873672 */

    bn_set_from_l(-873672, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3672);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 87);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 873672, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 7344);
    ASSERT_EQUAL_INT(res.parts.tab[1], 174);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 4, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 7);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -99999999 and n = 1 */

    bn_set_from_l(-99999999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9999);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 9999);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT_EQUAL_INT(res.parts.tab[1], 0);
    ASSERT_EQUAL_INT(res.parts.tab[2], 1);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 6673849 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 102);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_ul(&bn, 6673849, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 3951);
    ASSERT_EQUAL_INT(res.parts.tab[1], 667);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_neg_bn_sub_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100000);
    bn_init_with_args(&res, 0, 100000);

    /* {{{ neg BN - ul => BN
     * Add a unsigned long to a negative big number */
    /* {{{ bn = -5577 and n = 5577 */

    bn_set_from_l(-5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5577);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, 5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 11154);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -5577 and n = -5577 */

    bn_set_from_l(-5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 5577);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, -5577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -64578975577 and n = -64578975577 */

    bn_set_from_l(-64578975577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 75577);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 45789);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 6);

    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, -64578975577, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -131008 and n = -4449999 */

    bn_set_from_l(-131008, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 31008);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 1);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, -4449999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 18991);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 43);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -(LONG_MAX - 1) and n = -(LONG_MAX - 10) */

    bn_set_from_l(-(LONG_MAX - 1), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 4);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 75806);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 68547);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 37203);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 9223);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, -(LONG_MAX - 10), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ neg BN - ul => BN2
     * Add a unsigned long to a negative big number with result in a
     * another big_number_t */
    /* {{{ bn = -873672 and n = -873672 */

    bn_set_from_l(-873672, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 73672);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 8);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, -873672, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -3 and n = 4 */

    bn_set_from_l(-3, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 3);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, 4, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 7);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -999999990000099999 and n = 11000000001 */

    bn_set_from_l(-999999990000099999, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 4);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 99999);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 0);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 99999);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 999);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, 11000000001, &res);

    ASSERT_EQUAL_INT(res.parts.len, 4);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT_EQUAL_INT(res.parts.tab[1], 10001);
    ASSERT_EQUAL_INT(res.parts.tab[2], 0);
    ASSERT_EQUAL_INT(res.parts.tab[3], 1000);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -102 and n = 6673849 */

    bn_set_from_l(-102, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 102);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_sub_l(&bn, 6673849, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 73951);
    ASSERT_EQUAL_INT(res.parts.tab[1], 66);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* }}} */
/* {{{ Multiplying tests */

static void test_bn_mul_bn_same_sign(void)
{
    big_number_t bn, bn2, res;

    bn_init_with_args(&bn, 0, 10000);
    bn_init_with_args(&bn2, 0, 10000);
    bn_init_with_args(&res, 0, 10000);

    /* {{{ longest pos BN * shortest pos BN2 => BN
     * Multiply a positive big number by an another positive big number */
    /* {{{ bn = 56637 and bn2 = 0 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(0, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 5);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and bn2 = 1 */

    bn_set_from_l(9544, &bn);
    bn_set_from_l(1, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 56637 and bn2 = 10000 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(10000, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 6637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 5);
    ASSERT(bn.positive_number, "bn should be positive");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(bn2.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT_EQUAL_INT(bn.parts.tab[1],  6637);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 5);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 99637 and bn2 = 99637  */

    bn_set_from_l(99637, &bn);
    bn_set_from_l(99637, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 9);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1769);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 2753);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 99);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = 66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(66666, &bn2);

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 6);
    assert(check_bn_value_str(&bn, "119323903127539186103622"));

    /* }}} */
    /* {{{ bn = ULONG_MAX and bn2 = ULONG_MAX */

    bn_set_from_ul(ULONG_MAX, &bn);
    bn_set_from_ul(ULONG_MAX, &bn2);

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn, "340282366920938463426481119284349108225"));

    /* }}} */
    /* }}} */
    /* {{{ longest pos BN * shortest pos BN2 => BN2
     * Multiply a positive big number by an another positive big number */
    /* {{{ bn = 56637 and bn2 = 0 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(0, &bn2);

    bn_mul_bn(&bn, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = 66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(66666, &bn2);

    bn_mul_bn(&bn, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    assert(check_bn_value_str(&bn2, "119323903127539186103622"));

    /* }}} */
    /* }}} */
    /* {{{ pos BN * pos BN2 => BN3
     * Multiply a positive big number by an another positive big number */
    /* {{{ bn = 56637 and bn2 = 0 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(0, &bn2);

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and bn2 = 1 */

    bn_set_from_l(9544, &bn);
    bn_set_from_l(1, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9544);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = 66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(66666, &bn2);

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 6);
    assert(check_bn_value_str(&res, "119323903127539186103622"));

    /* }}} */
    /* }}} */
    /* {{{ pos BN * pos BN => BN
     * Multiply a positive big number by itself */
    /* {{{ bn = ULONG_MAX */

    bn_set_from_ul(ULONG_MAX, &bn);
    assert(check_bn_value_str(&bn, "18446744073709551615"));

    bn_mul_bn(&bn, &bn, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn, "340282366920938463426481119284349108225"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg BN2 => BN
     * Multiply a positive big number by an another positive big number */
    /* {{{ bn = -69637 and bn2 = -15765 */

    bn_set_from_l(-69637 , &bn);
    bn_set_from_l(-15765, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5765);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 7305);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 9782);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 10);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg BN2 => BN2
     * Multiply a negative big number by an another negative big number */
    /* {{{ bn = -69637 and bn2 = -15765 */

    bn_set_from_l(-69637 , &bn);
    bn_set_from_l(-15765, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5765);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_mul_bn(&bn, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 3);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 7305);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 9782);
    ASSERT_EQUAL_INT(bn2.parts.tab[2], 10);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg BN2 => BN3
     * Multiply a negative big number by an another negative big number */
    /* {{{ bn = -69637 and bn2 = -15765 */

    bn_set_from_l(-69637 , &bn);
    bn_set_from_l(-15765, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9637);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    ASSERT_EQUAL_INT(bn2.parts.len, 2);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 5765);
    ASSERT_EQUAL_INT(bn2.parts.tab[1], 1);
    ASSERT(!bn2.positive_number, "bn should be negative");

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 7305);
    ASSERT_EQUAL_INT(res.parts.tab[1], 9782);
    ASSERT_EQUAL_INT(res.parts.tab[2], 10);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg BN => BN
     * Multiply a positive big number by itself */
    /* {{{ bn = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn);
    assert(check_bn_value_str(&bn, "-9223372036854775806"));

    bn_mul_bn(&bn, &bn, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn, "85070591730234615828950163710522949636"));

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&bn2);
    bn_wipe(&res);
}

static void test_bn_mul_bn_different_sign(void)
{
    big_number_t bn, bn2, res;

    return;

    bn_init_with_args(&bn, 0, 10000);
    bn_init_with_args(&bn2, 0, 10000);
    bn_init_with_args(&res, 0, 10000);

    /* {{{ pos BN * neg BN2 => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 9544 and bn2 = -1 */

    bn_set_from_l(9544, &bn);
    bn_set_from_l(-1, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 99637 and bn2 = -99637  */

    bn_set_from_l(99637, &bn);
    bn_set_from_l(-99637, &bn2);

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1769);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 2753);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 99);
    ASSERT(!bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = -66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(-66666, &bn2);

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 6);
    assert(check_bn_value_str(&bn, "-119323903127539186103622"));

    /* }}} */
    /* {{{ bn = ULONG_MAX and n = -(LONG_MAX - 1) */

    bn_set_from_ul(ULONG_MAX, &bn);
    bn_set_from_ul(-(LONG_MAX - 1), &bn2);

    bn_mul_bn(&bn, &bn2, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn,
                              "-170141183460469231704017187605319778305"));

    /* }}} */
    /* }}} */
    /* {{{ pos BN * neg BN2 => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 56637 and bn2 = 0 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(0, &bn2);

    bn_mul_bn(&bn, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 1);
    ASSERT_EQUAL_INT(bn2.parts.tab[0], 0);
    ASSERT(bn2.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = 66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(66666, &bn2);

    bn_mul_bn(&bn, &bn2, &bn2);

    ASSERT_EQUAL_INT(bn2.parts.len, 6);
    assert(check_bn_value_str(&bn2, "119323903127539186103622"));

    /* }}} */
    /* }}} */
    /* {{{ pos BN * neg BN2 => BN3
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 56637 and bn2 = 0 */

    bn_set_from_l(56637, &bn);
    bn_set_from_l(0, &bn2);

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and bn2 = 1 */

    bn_set_from_l(9544, &bn);
    bn_set_from_l(1, &bn2);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 9544);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 9544);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1789876445677544567 and bn2 = 66666 */

    bn_set_from_l(1789876445677544567, &bn);
    bn_set_from_l(66666, &bn2);

    bn_mul_bn(&bn, &bn2, &res);

    ASSERT_EQUAL_INT(res.parts.len, 6);
    assert(check_bn_value_str(&res, "119323903127539186103622"));

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&bn2);
    bn_wipe(&res);
}

static void test_bn_mul_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ pos BN * ul => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = 0 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = 1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 637 and n = 5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 5, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 85);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 31);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 6637 and n = 1789 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 4);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 93);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 87);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 11);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 37 and n = 1789876445677544567 */

    bn_set_from_l(37, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn, "66225428490069148979"));

    /* }}} */
    /* {{{ bn = 66 and n = 1789876445677544567 */

    bn_set_from_l(66, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 11);
    assert(check_bn_value_str(&bn, "118131845414717941422"));

    /* }}} */
    /* {{{ bn = 6637 and n = 1789876445677544567 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 12);
    assert(check_bn_value_str(&bn, "11879409969961863291179"));

    /* }}} */
    /* {{{ bn = 11879409969961863291179 and n = 1789876445677544567 */

    bn_mul_ul(&bn, 1789876445677544567, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 21);
    assert(check_bn_value_str(&bn,
                              "21262676093781726337504762048790070474493"));

    /* }}} */
    /* {{{ bn = ULONG_MAX and n = ULONG_MAX */

    bn_set_from_ul(ULONG_MAX, &bn);

    bn_mul_ul(&bn, ULONG_MAX, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 20);
    assert(check_bn_value_str(&bn, "340282366920938463426481119284349108225"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * ul => BN
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = 0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = 1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -6637 and n = 9 */

    bn_set_from_l(-6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 9, &res);

    ASSERT_EQUAL_INT(res.parts.len, 3);
    ASSERT_EQUAL_INT(res.parts.tab[0], 33);
    ASSERT_EQUAL_INT(res.parts.tab[1], 97);
    ASSERT_EQUAL_INT(res.parts.tab[2], 5);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN * ul => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = 0 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = 1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 637 and n = 5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 5, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 85);
    ASSERT_EQUAL_INT(res.parts.tab[1], 31);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 6637 and n = 1789 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789, &res);

    ASSERT_EQUAL_INT(res.parts.len, 4);
    ASSERT_EQUAL_INT(res.parts.tab[0], 93);
    ASSERT_EQUAL_INT(res.parts.tab[1], 35);
    ASSERT_EQUAL_INT(res.parts.tab[2], 87);
    ASSERT_EQUAL_INT(res.parts.tab[3], 11);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 37 and n = 1789876445677544567 */

    bn_set_from_l(37, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &res);

    ASSERT_EQUAL_INT(res.parts.len, 10);
    assert(check_bn_value_str(&res, "66225428490069148979"));

    /* }}} */
    /* {{{ bn = 66 and n = 1789876445677544567 */

    bn_set_from_l(66, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &res);

    ASSERT_EQUAL_INT(res.parts.len, 11);
    assert(check_bn_value_str(&res, "118131845414717941422"));

    /* }}} */
    /* {{{ bn = 6637 and n = 1789876445677544567 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 1789876445677544567, &res);

    ASSERT_EQUAL_INT(res.parts.len, 12);
    assert(check_bn_value_str(&res, "11879409969961863291179"));

    /* }}} */
    /* {{{ bn = 11879409969961863291179 and n = 1789876445677544567 */

    bn_set_from_bn(&res, &bn);
    assert(check_bn_value_str(&bn, "11879409969961863291179"));

    bn_mul_ul(&bn, 1789876445677544567, &res);

    ASSERT_EQUAL_INT(res.parts.len, 21);
    assert(check_bn_value_str(&res,
                              "21262676093781726337504762048790070474493"));

    /* }}} */
    /* {{{ bn = ULONG_MAX and n = ULONG_MAX */

    bn_set_from_ul(ULONG_MAX, &bn);

    bn_mul_ul(&bn, ULONG_MAX, &res);

    ASSERT_EQUAL_INT(res.parts.len, 20);
    assert(check_bn_value_str(&res,
                              "340282366920938463426481119284349108225"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * ul => BN2
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = 0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = 1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_mul_l(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100);
    bn_init_with_args(&res, 0, 100);

    /* {{{ pos BN * pos l => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = 0 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = 1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 637 and n = 5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 5, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 85);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 31);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 6637 and n = 1789 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 1789, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 4);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 93);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 35);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 87);
    ASSERT_EQUAL_INT(bn.parts.tab[3], 11);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 37 and n = LONG_MAX */

    bn_set_from_l(37, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, LONG_MAX, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 11);
    assert(check_bn_value_str(&bn, "341264765363626704859"));

    /* }}} */
    /* {{{ bn = LONG_MAX and n = LONG_MAX */

    bn_set_from_ul(LONG_MAX, &bn);

    bn_mul_l(&bn, LONG_MAX, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 19);
    assert(check_bn_value_str(&bn, "85070591730234615847396907784232501249"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * pos l => BN
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = 0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = 1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = -6637 and n = 9 */

    bn_set_from_l(-6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, 9, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 97);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 5);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN * neg l => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = -0; 0 is a positive number, just check that */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = -1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 637 and n = -5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -5, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 85);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 31);
    ASSERT(!bn.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 37 and n = -(LONG_MAX - 1)*/

    bn_set_from_l(37, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -(LONG_MAX - 1), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 11);
    assert(check_bn_value_str(&bn, "-341264765363626704822"));

    /* }}} */
    /* {{{ bn = LONG_MAX and n = -(LONG_MAX - 1) */

    bn_set_from_ul(LONG_MAX, &bn);

    bn_mul_l(&bn, -(LONG_MAX - 1), &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 19);
    assert(check_bn_value_str(&bn, "-85070591730234615838173535747377725442"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg l => BN
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = -0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, -0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = -1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, -1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -6637 and n = -9 */

    bn_set_from_l(-6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, -9, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 3);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 33);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 97);
    ASSERT_EQUAL_INT(bn.parts.tab[2], 5);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN * pos l => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = 0 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = 1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 637 and n = 5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 5, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 85);
    ASSERT_EQUAL_INT(res.parts.tab[1], 31);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 6637 and n = 1789 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, 1789, &res);

    ASSERT_EQUAL_INT(res.parts.len, 4);
    ASSERT_EQUAL_INT(res.parts.tab[0], 93);
    ASSERT_EQUAL_INT(res.parts.tab[1], 35);
    ASSERT_EQUAL_INT(res.parts.tab[2], 87);
    ASSERT_EQUAL_INT(res.parts.tab[3], 11);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = LONG_MAX and n = 37 */

    bn_set_from_l(LONG_MAX, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 10);
    assert(check_bn_value_str(&bn, "9223372036854775807"));
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_ul(&bn, 37, &res);

    ASSERT_EQUAL_INT(res.parts.len, 11);
    assert(check_bn_value_str(&res, "341264765363626704859"));

    /* }}} */
    /* {{{ bn = LONG_MAX and n = LONG_MAX */

    bn_set_from_ul(LONG_MAX, &bn);

    bn_mul_ul(&bn, LONG_MAX, &res);

    ASSERT_EQUAL_INT(res.parts.len, 19);
    assert(check_bn_value_str(&res, "85070591730234615847396907784232501249"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * pos l => BN2
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = 0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = 1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_ul(&bn, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */
    /* {{{ pos BN * neg l => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 637 and n = -0 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 9544 and n = -1 */

    bn_set_from_l(9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ bn = 637 and n = -5 */

    bn_set_from_l(637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -5, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 85);
    ASSERT_EQUAL_INT(res.parts.tab[1], 31);
    ASSERT(!res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 6637 and n = -1789 */

    bn_set_from_l(6637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 66);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -1789, &res);

    ASSERT_EQUAL_INT(res.parts.len, 4);
    ASSERT_EQUAL_INT(res.parts.tab[0], 93);
    ASSERT_EQUAL_INT(res.parts.tab[1], 35);
    ASSERT_EQUAL_INT(res.parts.tab[2], 87);
    ASSERT_EQUAL_INT(res.parts.tab[3], 11);
    ASSERT(!res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 37 and n = -178987645677544567 */

    bn_set_from_l(37, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT(bn.positive_number, "bn should be positive");

    bn_mul_l(&bn, -178987645677544567 , &res);

    ASSERT_EQUAL_INT(res.parts.len, 10);
    assert(check_bn_value_str(&res, "-6622542890069148979"));

    /* }}} */
    /* {{{ bn = LONG_MAX and n = -(LONG_MAX - 1) */

    bn_set_from_ul(LONG_MAX, &bn);

    bn_mul_l(&bn, -(LONG_MAX - 1), &res);

    ASSERT_EQUAL_INT(res.parts.len, 19);
    assert(check_bn_value_str(&res,
                              "-85070591730234615838173535747377725442"));

    /* }}} */
    /* }}} */
    /* {{{ neg BN * neg l => BN2
     * Multiply a negative big number by a positive unsigned long */
    /* {{{ bn = -637 and n = -0 */

    bn_set_from_l(-637, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 37);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 6);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, -0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -9544 and n = -1 */

    bn_set_from_l(-9544, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 2);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 44);
    ASSERT_EQUAL_INT(bn.parts.tab[1], 95);
    ASSERT(!bn.positive_number, "bn should be negative");

    bn_mul_l(&bn, -1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -(LONG_MAX - 1) and n = -(LONG_MAX - 1) */

    bn_set_from_l(-(LONG_MAX - 1), &bn);

    bn_mul_l(&bn, -(LONG_MAX - 1), &res);

    ASSERT_EQUAL_INT(res.parts.len, 19);
    assert(check_bn_value_str(&res, "85070591730234615828950163710522949636"));

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

/* }}} */
/* {{{ Factoriel tests */

static bool
check_infinity_res_fact(const big_number_t *bn, uint32_t n)
{
    bool res = true;
    mpz_t x;

    mpz_init(x);

    mpz_fac_ui(x, n);

    res = cmp_bn_mpz(bn, &x);

    mpz_clear(x);

    return res;
}

static void test_bn_get_fact_n(void)
{
    big_number_t res;

    bn_init_multiplying(&res);

    /* {{{ 0! */

    bn_get_fact_n(0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ 1! */

    bn_get_fact_n(0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ 6! */

    bn_get_fact_n(6, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 720);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ 20! */

    bn_get_fact_n(20, &res);
    assert(check_bn_value_str(&res, "2432902008176640000"));

    /* }}} */
    /* {{{ 50! */

    bn_get_fact_n(50, &res);
    assert(check_infinity_res_fact(&res, 50));

    /* }}} */
    /* {{{ 100! */

    bn_get_fact_n(100, &res);
    assert(check_infinity_res_fact(&res, 100));

    /* }}} */

  bn_wipe(&res);
}

/* }}} */
/* {{{ Powering tests */

static bool
check_infinity_res_pow(const big_number_t *bn, int64_t n, int32_t exp)
{
    bool res = true;
    mpz_t x;

    mpz_init(x);

    mpz_set_si(x, n);
    mpz_pow_ui(x, x, exp);

    res = cmp_bn_mpz(bn, &x);

    mpz_clear(x);

    return res;
}

static void test_bn_pow_ul(void)
{
    big_number_t bn, res;

    bn_init_with_args(&bn, 0, 100000);
    bn_init_with_args(&res, 0, 100000);

    /* {{{ pos BN ^ ul => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 0 and exp = 0 */

    bn_set_from_l(0, &bn);

    bn_pow_ul(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 65637 and exp = 0 */

    bn_set_from_l(65637, &bn);

    bn_pow_ul(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 19544 and exp = 1 */

    bn_set_from_l(19544, &bn);

    bn_pow_ul(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 19544);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 0 and exp = 5788 */

    bn_set_from_l(0, &bn);

    bn_pow_ul(&bn, 5788, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 0);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 1 and exp = 9823 */

    bn_set_from_l(1, &bn);

    bn_pow_ul(&bn, 9823, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 19544 and exp = 9 */

    bn_set_from_l(19544, &bn);

    bn_pow_ul(&bn, 9, &bn);
    assert(check_infinity_res_pow(&bn, 19544, 9));
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ pos BN ^ ul => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = 0 and exp = 0 */

    bn_set_from_l(0, &bn);

    bn_pow_ul(&bn, 0, &res);

    assert(check_bn_value_str(&bn, "0"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 19544 and exp = 0 */

    bn_set_from_l(19544, &bn);

    bn_pow_ul(&bn, 0, &res);

    assert(check_bn_value_str(&bn, "19544"));
    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 19544 and exp = 1 */

    bn_set_from_l(19544, &bn);

    bn_pow_ul(&bn, 1, &res);

    assert(check_bn_value_str(&bn, "19544"));
    assert(check_bn_value_str(&res, "19544"));

    /* }}} */
    /* {{{ bn = 0 and exp = 5788 */

    bn_set_from_l(0, &bn);

    bn_pow_ul(&bn, 5788, &res);

    assert(check_bn_value_str(&bn, "0"));
    assert(check_bn_value_str(&res, "0"));

    /* }}} */
    /* {{{ bn = 1 and exp = 9823 */

    bn_set_from_l(1, &bn);

    bn_pow_ul(&bn, 9823, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 8632 and exp = 9 */

    bn_set_from_l(8632, &bn);

    bn_pow_ul(&bn, 9, &res);
    assert(check_infinity_res_pow(&res, 8632, 9));
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN ^ ul => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = -65637 and exp = 0 */

    bn_set_from_l(-19544, &bn);
    assert(check_bn_value_str(&bn, "-19544"));

    bn_pow_ul(&bn, 0, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 1);
    ASSERT(bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = 19544 and exp = 1 */

    bn_set_from_l(-19544, &bn);
    assert(check_bn_value_str(&bn, "-19544"));

    bn_pow_ul(&bn, 1, &bn);

    ASSERT_EQUAL_INT(bn.parts.len, 1);
    ASSERT_EQUAL_INT(bn.parts.tab[0], 19544);
    ASSERT(!bn.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -1 and exp = 9823 */

    bn_set_from_l(-1, &bn);
    assert(check_bn_value_str(&bn, "-1"));

    bn_pow_ul(&bn, 9823, &bn);

    assert(check_bn_value_str(&bn, "-1"));

    /* }}} */
    /* {{{ bn = -1 and exp = 9824 */

    bn_set_from_l(-1, &bn);
    assert(check_bn_value_str(&bn, "-1"));

    bn_pow_ul(&bn, 9824, &bn);

    assert(check_bn_value_str(&bn, "1"));

    /* }}} */
    /* {{{ bn = -4332 and exp = 99 */

    bn_set_from_l(-4332, &bn);

    bn_pow_ul(&bn, 99, &bn);
    assert(check_infinity_res_pow(&bn, -4332, 99));
    ASSERT(!bn.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */
    /* {{{ neg BN ^ ul => BN2
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ bn = -65637 and exp = 0 */

    bn_set_from_l(-65637, &bn);
    assert(check_bn_value_str(&bn, "-65637"));

    bn_pow_ul(&bn, 0, &res);

    assert(check_bn_value_str(&bn, "-65637"));
    assert(check_bn_value_str(&res, "1"));

    /* }}} */
    /* {{{ bn = -19544 and exp = 1 */

    bn_set_from_l(-19544, &bn);
    assert(check_bn_value_str(&bn, "-19544"));

    bn_pow_ul(&bn, 1, &res);

    assert(check_bn_value_str(&bn, "-19544"));
    assert(check_bn_value_str(&res, "-19544"));

    /* }}} */
    /* {{{ bn = -1 and exp = 9823 */

    bn_set_from_l(-1, &bn);
    assert(check_bn_value_str(&bn, "-1"));

    bn_pow_ul(&bn, 9823, &res);

    assert(check_bn_value_str(&bn, "-1"));
    assert(check_bn_value_str(&res, "-1"));

    /* }}} */
    /* {{{ bn = -1 and exp = 98 */

    bn_set_from_l(-1, &bn);
    assert(check_bn_value_str(&bn, "-1"));

    bn_pow_ul(&bn, 98, &res);

    assert(check_bn_value_str(&bn, "-1"));
    assert(check_bn_value_str(&res, "1"));

    /* }}} */
    /* {{{ bn = -99999 and exp = 77 */

    bn_set_from_l(-99999, &bn);

    bn_pow_ul(&bn, 77, &res);

    assert(check_bn_value_str(&bn, "-99999"));
    assert(check_infinity_res_pow(&res, -99999, 77));
    ASSERT(!res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ bn = -99999 and exp = 78 */

    bn_set_from_l(-99999, &bn);

    bn_pow_ul(&bn, 78, &res);

    assert(check_bn_value_str(&bn, "-99999"));
    assert(check_infinity_res_pow(&res, -99999, 78));
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&bn);
    bn_wipe(&res);
}

static void test_bn_ul_pow_ul(void)
{
    big_number_t res;

    bn_init_with_args(&res, 0, 100);

    /* {{{ pos ul ^ ul => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ n = 0 and exp = 0 */

    bn_ul_pow_ul(0, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 637 and exp = 0 */

    bn_ul_pow_ul(637, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 9544 and exp = 1 */

    bn_ul_pow_ul(9544, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 0 and exp = 57788 */

    bn_ul_pow_ul(0, 57788, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 0);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 1 and exp = 7658 */

    bn_ul_pow_ul(1, 7658, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 637 and exp = 5 */

    bn_ul_pow_ul(637, 5, &res);

    ASSERT_EQUAL_INT(res.parts.len, 8);
    ASSERT_EQUAL_INT(res.parts.tab[0], 57);
    ASSERT_EQUAL_INT(res.parts.tab[1], 69);
    ASSERT_EQUAL_INT(res.parts.tab[2], 62);
    ASSERT_EQUAL_INT(res.parts.tab[3], 82);
    ASSERT_EQUAL_INT(res.parts.tab[4], 10);
    ASSERT_EQUAL_INT(res.parts.tab[5], 88);
    ASSERT_EQUAL_INT(res.parts.tab[6], 4);
    ASSERT_EQUAL_INT(res.parts.tab[7], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 6637 and n = 17 */

    bn_ul_pow_ul(6637, 17, &res);
    assert(check_bn_value_str(&res, "940851106307814976946784947592094489558"
                              "49731301966729691184867117"));

    /* }}} */
    /* {{{ n = 6637 and n = 1789 */

    bn_set_limit(&res, 1000000000);

    /* this result has been checked with the library gmpz */
    bn_ul_pow_ul(6637, 1789, &res);

    ASSERT_EQUAL_INT(res.parts.len, 760);
    ASSERT_EQUAL_INT(res.parts.tab[0], 562990877);
    ASSERT_EQUAL_INT(res.parts.tab[1], 330069634);
    ASSERT_EQUAL_INT(res.parts.tab[758], 300279261);
    ASSERT_EQUAL_INT(res.parts.tab[759], 3217968);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* }}} */

    bn_wipe(&res);
}

static void test_bn_l_pow_ul(void)
{
    big_number_t res;

    bn_init_with_args(&res, 0, 100);

    /* {{{ l ^ ul => BN
     * Multiply a positive big number by a positive unsigned long */
    /* {{{ n = -637 and exp = 0 */

    bn_l_pow_ul(-637, 0, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = 9544 and exp = 1 */

    bn_l_pow_ul(-9544, 1, &res);

    ASSERT_EQUAL_INT(res.parts.len, 2);
    ASSERT_EQUAL_INT(res.parts.tab[0], 44);
    ASSERT_EQUAL_INT(res.parts.tab[1], 95);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ n = -1 and exp = 7658 */

    bn_l_pow_ul(-1, 7658, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(res.positive_number, "bn should be positive");

    /* }}} */
    /* {{{ n = -1 and exp = 7657 */

    bn_l_pow_ul(-1, 7657, &res);

    ASSERT_EQUAL_INT(res.parts.len, 1);
    ASSERT_EQUAL_INT(res.parts.tab[0], 1);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ n = -637 and exp = 5 */

    bn_l_pow_ul(-637, 5, &res);

    ASSERT_EQUAL_INT(res.parts.len, 8);
    ASSERT_EQUAL_INT(res.parts.tab[0], 57);
    ASSERT_EQUAL_INT(res.parts.tab[1], 69);
    ASSERT_EQUAL_INT(res.parts.tab[2], 62);
    ASSERT_EQUAL_INT(res.parts.tab[3], 82);
    ASSERT_EQUAL_INT(res.parts.tab[4], 10);
    ASSERT_EQUAL_INT(res.parts.tab[5], 88);
    ASSERT_EQUAL_INT(res.parts.tab[6], 4);
    ASSERT_EQUAL_INT(res.parts.tab[7], 1);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* {{{ n = -6637 and n = 17 */

    bn_l_pow_ul(-6637, 17, &res);
    assert(check_bn_value_str(&res, "-94085110630781497694678494759209448955"
                              "849731301966729691184867117"));

    /* }}} */
    /* {{{ n = 6637 and n = 1789 */

    bn_set_limit(&res, 1000000000);

    /* this result has been checked with the library gmpz */
    bn_l_pow_ul(-6637, 1789, &res);

    ASSERT_EQUAL_INT(res.parts.len, 760);
    ASSERT_EQUAL_INT(res.parts.tab[0], 562990877);
    ASSERT_EQUAL_INT(res.parts.tab[1], 330069634);
    ASSERT_EQUAL_INT(res.parts.tab[758], 300279261);
    ASSERT_EQUAL_INT(res.parts.tab[759], 3217968);
    ASSERT(!res.positive_number, "bn should be negative");

    /* }}} */
    /* }}} */

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
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_neg_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_add_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_pos_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_neg_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_add_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_sub_pos_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_sub_neg_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_sub_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_pos_bn_sub_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_sub_pos_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_sub_neg_bn);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_sub_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_neg_bn_sub_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_mul_bn_same_sign);
    ADD_TEST_TO_MODULE(module_tests, test_bn_mul_bn_different_sign);
    ADD_TEST_TO_MODULE(module_tests, test_bn_mul_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_mul_l);

    ADD_TEST_TO_MODULE(module_tests, test_bn_get_fact_n);

    ADD_TEST_TO_MODULE(module_tests, test_bn_pow_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_ul_pow_ul);
    ADD_TEST_TO_MODULE(module_tests, test_bn_l_pow_ul);

    return module_tests;
}
