#include "test_map.h"
#include "../src/containers/map.h"
#include "../src/macros.h"

uint32_t hash_method(const int *key)
{
    return *key;
}

int cmp_method(const int *key1, const int *key2)
{
    (void)key1;
    (void)key2;
    return 0;
}

generic_map_t(test, int, int, hash_method, cmp_method);

void test_map_init_and_wipe()
{
    gm_t(test) map;

    gm_init(&map);

    gm_wipe(&map, NULL, NULL);
}

module_tests_t *get_all_tests_map(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "MAP");
    ADD_TEST_TO_MODULE(module_tests, test_map_init_and_wipe);

    return module_tests;
}
