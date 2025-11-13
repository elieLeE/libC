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
    ASSERT_EQUAL(vector.size, 8);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static void test_vector_add_and_remove_elem(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[8] = {11, 10, 4, 12};

    gv_init(&vector);

    gv_add(&vector, 1);
    gv_add(&vector, 2);
    gv_insert_elem_at_pos(&vector, 10, 1);
    gv_remove(&vector, 2);
    gv_add(&vector, 3);
    gv_add(&vector, 4);
    gv_add(&vector, 5);
    gv_remove(&vector, 2);
    gv_remove(&vector, 0);
    gv_insert_elem_at_pos(&vector, 11, 0);
    gv_insert_elem_at_pos(&vector, 12, 4);
    gv_remove(&vector, 3);

    ASSERT_EQUAL(vector.len, 4);
    ASSERT_EQUAL(vector.size, 6);

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

    gv_insert_elem_sorted(&vector, 3, cmp_elem);
    gv_insert_elem_sorted(&vector, 1, cmp_elem);
    gv_insert_elem_sorted(&vector, 5, cmp_elem);
    gv_insert_elem_sorted(&vector, 4, cmp_elem);
    gv_insert_elem_sorted(&vector, 2, cmp_elem);

    ASSERT_EQUAL(vector.len, 5);
    ASSERT_EQUAL(vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static void test_sort_vector_simple(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_add(&vector, 3);
    gv_add(&vector, 1);
    gv_add(&vector, 5);
    gv_add(&vector, 4);
    gv_add(&vector, 2);

    gv_sort(&vector, cmp_elem);

    ASSERT_EQUAL(vector.len, 5);
    ASSERT_EQUAL(vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static void
check_vector_sorting(const gv_t(int32) *vector, bool is_increasing_vector)
{
    int previous_val = vector->tab[0];

    for (int i = 1; i < vector->len; i++) {
        int current_data = vector->tab[i];

        if (is_increasing_vector) {
            ASSERT(current_data >= previous_val,
                   "vector seems not having well sorted - "
                   "current : %d, previous: %d", current_data, previous_val);
        } else {
            ASSERT(current_data <= previous_val,
                   "vector seems not having well sorted - "
                   "current : %d, previous: %d", current_data, previous_val);
        }

        previous_val = current_data;
    }
}

static void test_sort_vector_increasing(void)
{
    gv_t(int32) vector;

    gv_init_size(&vector, 1000);

    for (int i = 0; i < 1000; i++) {
        int tmp = rand() % 10000;

        gv_add(&vector, tmp);
    }

    gv_sort(&vector, cmp_elem);

    check_vector_sorting(&vector, true);

    gv_wipe(&vector, NULL);
}

static int cmp_elem_decreasing_order(const void *d1, const void *d2)
{
    return - cmp_elem(d1, d2);
}

static void test_sort_vector_decreasing(void)
{
    gv_t(int32) vector;

    gv_init_size(&vector, 1000);

    for (int i = 0; i < 1000; i++) {
        int tmp = rand() % 10000;

        gv_add(&vector, tmp);
    }

    gv_sort(&vector, cmp_elem_decreasing_order);

    check_vector_sorting(&vector, false);

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
    ASSERT_EQUAL(vector.size, 5);

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
    ASSERT_EQUAL(vector->size, 5);

    gv_for_each_pos(pos, vector) {
        ASSERT_EQUAL(vector->tab[pos], tab[idx_tab]);
        idx_tab++;
    }
    gv_delete(vector, NULL);
}

static void test_vector_init_size(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[8] = {11, 1, 10, 2, 3, 4, 5, 12};

    gv_init_size(&vector, 10);

    gv_add(&vector, 1);
    gv_add(&vector, 2);
    gv_insert_elem_at_pos(&vector, 10, 1);
    gv_add(&vector, 3);
    gv_add(&vector, 4);
    gv_add(&vector, 5);
    gv_insert_elem_at_pos(&vector, 11, 0);
    gv_insert_elem_at_pos(&vector, 12, 7);

    ASSERT_EQUAL(vector.len, 8);
    ASSERT_EQUAL(vector.size, 10);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
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
    module_tests_t *module_tests = RETHROW_P(module_tests_new());

    set_module_name(module_tests, "VECTOR");
    ADD_TEST_TO_MODULE(module_tests, test_fill_vector);
    ADD_TEST_TO_MODULE(module_tests, test_vector_add_and_remove_elem);
    ADD_TEST_TO_MODULE(module_tests, test_fill_vector_sorting);
    ADD_TEST_TO_MODULE(module_tests, test_sort_vector_simple);
    ADD_TEST_TO_MODULE(module_tests, test_sort_vector_increasing);
    ADD_TEST_TO_MODULE(module_tests, test_sort_vector_decreasing);
    ADD_TEST_TO_MODULE(module_tests, test_fill_pointer_vector);
    ADD_TEST_TO_MODULE(module_tests, test_new_and_delete_vector);
    ADD_TEST_TO_MODULE(module_tests, test_vector_init_size);
    ADD_TEST_TO_MODULE(module_tests, test_reset_vector);

    return module_tests;
}
