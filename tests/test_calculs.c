#include "test_calculs.h"

#include "../src/macros.h"
#include "../src/math/calculs.h"

module_tests_t *get_all_tests_calculs(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "CALCULS");

    return module_tests;
}
