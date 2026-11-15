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
    return (*key1) - (*key2);
}

generic_map_t(test, int, int, hash_method, cmp_method);

void test_map_init_and_wipe(void)
{
    gm_t(test) map;

    gm_init_size(&map, 7);

    ASSERT_INT_EQUAL(map.size, 7);
    ASSERT_INT_EQUAL(map.count, 0);
    for (int i = 0; i < map.size; i++) {
        ASSERT_INT_EQUAL(map.tabs_len[i], 0);
        ASSERT_INT_EQUAL(map.tabs_size[i], 0);
    }

    gm_wipe(&map, NULL, NULL);
}

void test_map_put_and_get()
{
    gm_t(test) map;
    int keys[] = {6, 98, 43, 678, 0, 65, 445, 8, 41654, 9};
    int values[] = {8765, 9, 51, 976654, 4, 876, 9, 8, 0, 8};
    int val, val2;
    int *val_p, *val_p2;

    gm_init_size(&map, 7);

    for (int i = 0; i < 10; i++) {
        int64_t full_pos = gm_put(test, &map, keys[i], values[i], 0);

        ASSERT_INT_GT_OR_EQUAL(full_pos, 0);
        assert(!(full_pos & QHASH_COLLISION));

        ASSERT_INT_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i], val);

        ASSERT_INT_EQUAL(gm_get_p(test, &map, keys[i], &val_p), 0);
        ASSERT_INT_EQUAL(values[i], *val_p);

        gm_get_from_fpos(test, &map, full_pos, &val2);
        ASSERT_INT_EQUAL(values[i], val2);

        gm_get_p_from_fpos(test, &map, full_pos, &val_p2);
        ASSERT_INT_EQUAL(values[i], *val_p2);
    }

    ASSERT_INT_EQUAL(gm_get(test, &map, 7, &val), -1);
    ASSERT_INT_EQUAL(gm_get_p(test, &map, 8756, &val_p), -1);

    gm_wipe(&map, NULL, NULL);
}

void test_map_add(void)
{
    gm_t(test) map;
    int keys[] = {9, 86, 67, 78698, 6809, 8, 890, 98, 220, 10};
    int values[] = {7, 769878, 545678, 9, 56, 876, -789, 98, 0, 8};
    int val;

    gm_init_size(&map, 7);

    for (int i = 0; i < 10; i++) {
        int64_t full_pos = gm_add(test, &map, keys[i], values[i]);

        ASSERT_INT_GT_OR_EQUAL(full_pos, 0);
        assert(!(full_pos & QHASH_COLLISION));

        ASSERT_INT_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i], val);
    }

    for (int i = 0; i < 10; i++) {
        int64_t full_pos = gm_add(test, &map, keys[i], values[i] + 1);

        ASSERT_INT_GT_OR_EQUAL(full_pos, 0);
        assert(full_pos & QHASH_COLLISION);

        ASSERT_INT_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i], val);
    }

    gm_wipe(&map, NULL, NULL);
}

void test_map_set(void)
{
    gm_t(test) map;
    int keys[] = {9, 86, 67, 78698, 6809, 8, 890, 98, 220, 10};
    int values[] = {7, 769878, 545678, 9, 56, 876, -789, 98, 0, 8};
    int val;

    gm_init_size(&map, 7);

    for (int i = 0; i < 10; i++) {
        int64_t full_pos = gm_set(test, &map, keys[i], values[i]);

        ASSERT_INT_GT_OR_EQUAL(full_pos, 0);
        assert(!(full_pos & QHASH_COLLISION));

        ASSERT_INT_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i], val);
    }

    for (int i = 0; i < 10; i++) {
        int64_t full_pos = gm_set(test, &map, keys[i], values[i] + 1);

        ASSERT_INT_GT_OR_EQUAL(full_pos, 0);
        /* value are reset by force */
        assert(!(full_pos & QHASH_COLLISION));

        ASSERT_INT_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i] + 1, val);
    }

    gm_wipe(&map, NULL, NULL);
}

void test_map_reserve()
{
    gm_t(test) map;
    int keys[] = {6, 98, 43, 678, 0, 65, 445, 8, 41654, 9};
    int values[] = {8765, 9, 51, 976654, 4, 876, 9, 8, 0, 8};

    gm_init_size(&map, 7);

    for (int i = 0; i < 10; i++) {
        int *val_p;
        int64_t fpos = gm_reserve(test, &map, keys[i], 0);

        ASSERT_INT_GT_OR_EQUAL(fpos, 0);
        gm_get_p_from_fpos(test, &map, fpos, &val_p);
        *val_p = values[i];
    }

    for (int i = 0; i < 10; i++) {
        int val;

        ASSERT_INT_GT_OR_EQUAL(gm_get(test, &map, keys[i], &val), 0);
        ASSERT_INT_EQUAL(values[i], val);
    }

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
    ADD_TEST_TO_MODULE(module_tests, test_map_put_and_get);
    ADD_TEST_TO_MODULE(module_tests, test_map_add);
    ADD_TEST_TO_MODULE(module_tests, test_map_set);
    ADD_TEST_TO_MODULE(module_tests, test_map_reserve);

    return module_tests;
}
