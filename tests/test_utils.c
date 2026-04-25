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

module_tests_t *get_all_tests_utils(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "UTILS");
    ADD_TEST_TO_MODULE(module_tests, test_swap);

    return module_tests;
}

