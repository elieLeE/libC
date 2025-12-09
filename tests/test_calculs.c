#include "test_calculs.h"

#include "../src/macros.h"
#include "../src/math/calculs.h"

static void test_get_next_closest_power_of_two(void)
{
    ASSERT_EQUAL_LONG(get_next_closest_power_of_two(3), 4L);
    ASSERT_EQUAL_LONG(get_next_closest_power_of_two(12), 16L);
    ASSERT_EQUAL_LONG(get_next_closest_power_of_two(55), 64L);
    ASSERT_EQUAL_LONG(get_next_closest_power_of_two(91), 128L);
}

module_tests_t *get_all_tests_calculs(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "CALCULS");
    ADD_TEST_TO_MODULE(module_tests, test_get_next_closest_power_of_two);

    return module_tests;
}
