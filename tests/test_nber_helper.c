#include <stdio.h>

#include "test_nber_helper.h"
#include "../src/macros.h"
#include "../src/math/nber_helper.h"

void test_is_nber_a_palindrome(void)
{
#define CHECK(_n, _res_expected)                                              \
    if (_res_expected) {                                                      \
        ASSERT(is_nber_a_palindrome(_n), "%ld is a palindrome", _n);          \
    } else {                                                                  \
        ASSERT(!is_nber_a_palindrome(_n), "%ld is not a palindrome", _n);     \
    }

    CHECK(5L, true);
    CHECK(595L, true);
    CHECK(156464651L, true);
    CHECK(153464351L, true);

    CHECK(153264351L, false);

#undef CHECK
}

module_tests_t *get_all_tests_nber_helper(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "NBER_HELPER");
    ADD_TEST_TO_MODULE(module_tests, test_is_nber_a_palindrome);

    return module_tests;
}
