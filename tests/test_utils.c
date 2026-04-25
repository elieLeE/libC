#include <assert.h>

#include "test_utils.h"

#include "../src/macros.h"
#include "../src/utils.h"

#include <limits.h>

static void test_swap(void)
{
    int a = 5;
    int b = 13;

    SWAP(int, &a, &b);

    ASSERT_EQUAL_INT(a, 13);
    ASSERT_EQUAL_INT(b, 5);
}

static void test_g_cmp_int32(void)
{
    int n, n2;

    n = 10;
    n2 = INT_MAX;
    assert(g_cmp_int32(&n, &n2) < 0);
}

static void test_g_cmp_uint32(void)
{
    unsigned int n, n2;

    n = 10;
    n2 = UINT_MAX;
    assert(g_cmp_uint32(&n, &n2) < 0);
}

static void test_g_cmp_int64(void)
{
    long n, n2;

    n = 10;
    n2 = LONG_MAX;
    assert(g_cmp_int64(&n, &n2) < 0);

    n = 10;
    n2 = 11;
    assert(g_cmp_int64(&n, &n2) < 0);

    n = 10000;
    n2 = 11;
    assert(g_cmp_int64(&n, &n2) > 0);

    n = 11;
    n2 = 11;
    assert(g_cmp_int64(&n, &n2) == 0);
}

static void test_g_cmp_uint64(void)
{
    unsigned long n, n2;

    n = 10;
    n2 = ULONG_MAX;
    assert(g_cmp_uint64(&n, &n2) < 0);
}

module_tests_t *get_all_tests_utils(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "UTILS");
    ADD_TEST_TO_MODULE(module_tests, test_swap);
    ADD_TEST_TO_MODULE(module_tests, test_g_cmp_int32);
    ADD_TEST_TO_MODULE(module_tests, test_g_cmp_uint32);
    ADD_TEST_TO_MODULE(module_tests, test_g_cmp_int64);
    ADD_TEST_TO_MODULE(module_tests, test_g_cmp_uint64);

    return module_tests;
}

