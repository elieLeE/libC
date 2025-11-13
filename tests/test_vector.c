#include "test_vector.h"
#include "../src/vector/vector.h"
#include "../src/macros.h"

static void test_fill_vector(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[8] = {11, 1, 10, 2, 3, 4, 5, 12};

    gv_init(&vector);

    gv_add(&vector, 1);
    gv_add(&vector, 2);
    gv_insert_elem_at_pos(&vector, 10, 1);
    gv_add(&vector, 3);
    gv_add(&vector, 4);
    gv_add(&vector, 5);
    gv_insert_elem_at_pos(&vector, 11, 0);
    gv_insert_elem_at_pos(&vector, 12, 7);

    ASSERT_EQUAL(vector.len, 8);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static int cmp_elem(const void *_d1, const void *_d2)
{
    const int *d1 = _d1;
    const int *d2 = _d2;

    return (*d1) - (*d2);
}

static void test_fill_vector_sorting(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_insert_elem_sorted(&vector, &tab[2], cmp_elem);
    gv_insert_elem_sorted(&vector, &tab[0], cmp_elem);
    gv_insert_elem_sorted(&vector, &tab[4], cmp_elem);
    gv_insert_elem_sorted(&vector, &tab[3], cmp_elem);
    gv_insert_elem_sorted(&vector, &tab[1], cmp_elem);

    ASSERT_EQUAL(vector.len, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

generic_vector_t(int_p, int *);

static void free_elem(void **elem)
{
    p_free(elem);
}

static void add_new_elem_and_set(gv_t(int_p) *vec, int val)
{
    int **a_p = NULL;

    a_p = gv_grow1(vec);
    *a_p = p_calloc(sizeof(int));
    **a_p = val;
}

static void test_fill_pointer_vector(void)
{
    int idx_tab = 0;
    gv_t(int_p) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    add_new_elem_and_set(&vector, 1);
    add_new_elem_and_set(&vector, 2);
    add_new_elem_and_set(&vector, 3);
    add_new_elem_and_set(&vector, 4);
    add_new_elem_and_set(&vector, 5);

    ASSERT_EQUAL(vector.len, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(*(vector.tab[pos]), tab[idx_tab]);
        idx_tab++;
    }
    gv_wipe(&vector, free_elem);
}

static void test_new_and_delete_vector(void)
{
    int idx_tab = 0;
    gv_t(int32) *vector;
    int32_t tab[5]= {1, 2, 3, 4, 5};

    vector = gv_new(int32);

    gv_add(vector, 1);
    gv_add(vector, 2);
    gv_add(vector, 3);
    gv_add(vector, 4);
    gv_add(vector, 5);

    ASSERT_EQUAL(vector->len, 5);

    gv_for_each_pos(pos, vector) {
        ASSERT_EQUAL(vector->tab[pos], tab[idx_tab]);
        idx_tab++;
    }
    gv_delete(vector, NULL);
}

static void test_reset_vector(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[5] = {6, 7, 8};

    gv_init(&vector);

    gv_add(&vector, 1);
    gv_add(&vector, 2);
    gv_add(&vector, 3);
    gv_add(&vector, 4);
    gv_add(&vector, 5);

    gv_clear(&vector, NULL);

    gv_add(&vector, 6);
    gv_add(&vector, 7);
    gv_add(&vector, 8);

    ASSERT_EQUAL(vector.len, 3);
    ASSERT_EQUAL(vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }
    gv_wipe(&vector, NULL);
}

module_tests_t *get_all_tests_vector(void)
{
    module_tests_t *module_tests = RETHROW_P(p_calloc(sizeof(module_tests_t)));

    set_module_name(module_tests, "VECTOR");
    ADD_TEST_TO_MODULE(module_tests, test_fill_vector);
    ADD_TEST_TO_MODULE(module_tests, test_fill_vector_sorting);
    ADD_TEST_TO_MODULE(module_tests, test_fill_pointer_vector);
    ADD_TEST_TO_MODULE(module_tests, test_new_and_delete_vector);
    ADD_TEST_TO_MODULE(module_tests, test_reset_vector);

    return module_tests;
}
