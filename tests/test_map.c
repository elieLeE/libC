#include <time.h>

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


void test_map_pos(void)
{
    srand(time(NULL));

    for (int i = 0; i < 10; i++) {
        int32_t pos1 = rand() % 1031;
        int32_t pos2 = rand() % 1000;
        int32_t pos1_b, pos2_b;
        int64_t full_pos;

        full_pos = __gm_get_fpos_from_2pos(pos1, pos2, 0);
        __gm_get_2pos_from_fpos(full_pos, &pos1_b, &pos2_b);

        ASSERT_INT_EQUAL(pos1, pos1_b);
        ASSERT_INT_EQUAL(pos2, pos2_b);
        assert(!(full_pos & QHASH_COLLISION));

        full_pos = __gm_get_fpos_from_2pos(pos1, pos2, QHASH_COLLISION);
        __gm_get_2pos_from_fpos(full_pos, &pos1_b, &pos2_b);

        ASSERT_INT_EQUAL(pos1, pos1_b);
        ASSERT_INT_EQUAL(pos2, pos2_b);
        assert(full_pos & QHASH_COLLISION);
    }
}

module_tests_t *get_all_tests_map(void)
{
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "MAP");
    ADD_TEST_TO_MODULE(module_tests, test_map_init_and_wipe);
    ADD_TEST_TO_MODULE(module_tests, test_map_pos);

    return module_tests;
}
