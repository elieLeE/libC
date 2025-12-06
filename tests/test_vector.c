#include <time.h>

#include "test_vector.h"
#include "../src/vector/vector.h"
#include "../src/macros.h"
#include "../src/utils.h"

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

    ASSERT_EQUAL_INT((int)vector.len, 8);
    ASSERT_EQUAL_INT((int)vector.size, 8);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
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

    ASSERT_EQUAL_INT((int)vector.len, 4);
    ASSERT_EQUAL_INT((int)vector.size, 6);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static void test_fill_vector_sorting(void)
{
    int idx_tab = 0;
    gv_t(int32) vector;
    int32_t tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_insert_elem_sorted(&vector, 3, g_cmp_int32);
    gv_insert_elem_sorted(&vector, 1, g_cmp_int32);
    gv_insert_elem_sorted(&vector, 5, g_cmp_int32);
    gv_insert_elem_sorted(&vector, 4, g_cmp_int32);
    gv_insert_elem_sorted(&vector, 2, g_cmp_int32);

    ASSERT_EQUAL_INT((int)vector.len, 5);
    ASSERT_EQUAL_INT((int)vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
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

    gv_sort(&vector, g_cmp_int32);

    ASSERT_EQUAL_INT((int)vector.len, 5);
    ASSERT_EQUAL_INT((int)vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
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

    gv_sort(&vector, g_cmp_int32);

    check_vector_sorting(&vector, true);

    gv_wipe(&vector, NULL);
}

static void test_sort_vector_decreasing(void)
{
    gv_t(int32) vector;

    gv_init_size(&vector, 1000);

    for (int i = 0; i < 1000; i++) {
        int tmp = rand() % 10000;

        gv_add(&vector, tmp);
    }

    gv_sort(&vector, g_cmp_int32_rev);

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

    ASSERT_EQUAL_INT((int)vector.len, 5);
    ASSERT_EQUAL_INT((int)vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(*(vector.tab[pos]), tab[idx_tab]);
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

    ASSERT_EQUAL_INT((int)vector->len, 5);
    ASSERT_EQUAL_INT((int)vector->size, 5);

    gv_for_each_pos(pos, vector) {
        ASSERT_EQUAL_INT(vector->tab[pos], tab[idx_tab]);
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

    ASSERT_EQUAL_INT((int)vector.len, 8);
    ASSERT_EQUAL_INT((int)vector.size, 10);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }

    gv_wipe(&vector, NULL);
}

static void test_vector_shuffle(void)
{
    gv_t(int32) vector;
    int expected_tab[10] = {5, 8, 1, 9, 6, 4, 10, 3, 7, 2};

    gv_init_size(&vector, 10);

    /* set the seed to 0 in order to have always the same number from 'rand'
     * method and so be able to check the shuffle algorithm */
    srand(0);

    for (int i = 1; i <= 10; i++) {
        gv_add(&vector, i);
    }
    gv_shuffle(&vector);

    /* reset the seed */
    srand(time(NULL));

    /* As the seed of the rand is always set to 0 in this test, the shuffle of
     * the tab will be always the same */
    for (int i = 0; i < 10; i++) {
        ASSERT_EQUAL_INT(vector.tab[i], expected_tab[i]);
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

    ASSERT_EQUAL_INT((int)vector.len, 3);
    ASSERT_EQUAL_INT((int)vector.size, 5);

    gv_for_each_pos(pos, &vector) {
        ASSERT_EQUAL_INT(vector.tab[pos], tab[idx_tab]);
        idx_tab++;
    }
    gv_wipe(&vector, NULL);
}

static void check_pos_found_elem(gv_t(int32) *vector, long elem,
                                 gv_algo_search_t algo, long pos_expected)
{
    long pos_obtained = gv_find(vector, elem, algo, g_cmp_int32);
    ASSERT_EQUAL_LONG(pos_expected, pos_obtained);
}

static void test_vector_find_and_contains_sequential_algo(void)
{
    gv_t(int32) vector;

    gv_init(&vector);

    gv_add(&vector, 1);
    gv_add(&vector, 5);
    gv_add(&vector, 4);
    gv_add(&vector, 2);
    gv_add(&vector, 3);

    ASSERT((!(gv_contains(&vector, 10, GV_SEQUENTIAL_SEARCH, g_cmp_int32))),
           "elem '10' has not been found");
    ASSERT(gv_contains(&vector, 2, GV_SEQUENTIAL_SEARCH, g_cmp_int32),
           "elem '2' has not been found");
    check_pos_found_elem(&vector, 1, GV_SEQUENTIAL_SEARCH, 0);
    check_pos_found_elem(&vector, 2, GV_SEQUENTIAL_SEARCH, 3);
    check_pos_found_elem(&vector, 3, GV_SEQUENTIAL_SEARCH, 4);
    check_pos_found_elem(&vector, 4, GV_SEQUENTIAL_SEARCH, 2);
    check_pos_found_elem(&vector, 5, GV_SEQUENTIAL_SEARCH, 1);

    gv_clear(&vector, NULL);

    gv_add(&vector, 6);
    gv_add(&vector, 7);
    gv_add(&vector, 8);

    for (int i = 0; i < 5; i++) {
        ASSERT((!(gv_contains(&vector, i, GV_SEQUENTIAL_SEARCH, g_cmp_int32))),
               "elem %d should not be present", i);
    }
    for (int i = 6; i <= 8; i++) {
        check_pos_found_elem(&vector, i, GV_SEQUENTIAL_SEARCH, i - 6);
    }

    gv_wipe(&vector, NULL);
}

static void test_vector_find_and_contains_dichotomy_algo(void)
{
    gv_t(int32) vector;

    gv_init(&vector);

    check_pos_found_elem(&vector, 1, GV_DICHOTOMY_SEARCH, -1);

    gv_insert_elem_sorted(&vector, 0,   g_cmp_int32);
    gv_insert_elem_sorted(&vector, 3,   g_cmp_int32);
    gv_insert_elem_sorted(&vector, 9,   g_cmp_int32);
    gv_insert_elem_sorted(&vector, 13,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 21,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 65,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 66,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 71,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 99,  g_cmp_int32);
    gv_insert_elem_sorted(&vector, 100, g_cmp_int32);
    gv_insert_elem_sorted(&vector, 111, g_cmp_int32);

    check_pos_found_elem(&vector, 0,   GV_DICHOTOMY_SEARCH, 0);
    check_pos_found_elem(&vector, 3,   GV_DICHOTOMY_SEARCH, 1);
    check_pos_found_elem(&vector, 9,   GV_DICHOTOMY_SEARCH, 2);
    check_pos_found_elem(&vector, 13,  GV_DICHOTOMY_SEARCH, 3);
    check_pos_found_elem(&vector, 21,  GV_DICHOTOMY_SEARCH, 4);
    check_pos_found_elem(&vector, 65,  GV_DICHOTOMY_SEARCH, 5);
    check_pos_found_elem(&vector, 66,  GV_DICHOTOMY_SEARCH, 6);
    check_pos_found_elem(&vector, 71,  GV_DICHOTOMY_SEARCH, 7);
    check_pos_found_elem(&vector, 99,  GV_DICHOTOMY_SEARCH, 8);
    check_pos_found_elem(&vector, 100, GV_DICHOTOMY_SEARCH, 9);
    check_pos_found_elem(&vector, 111, GV_DICHOTOMY_SEARCH, 10);

    check_pos_found_elem(&vector, -1, GV_DICHOTOMY_SEARCH, -1);
    check_pos_found_elem(&vector, 200, GV_DICHOTOMY_SEARCH, -1);
    check_pos_found_elem(&vector, 10, GV_DICHOTOMY_SEARCH, -1);
    check_pos_found_elem(&vector, 70, GV_DICHOTOMY_SEARCH, -1);

    gv_wipe(&vector, NULL);
}

static int cmp_elem_p(const void *_d1, const void *_d2)
{
    const int *d1 = *(int **)_d1;
    const int *d2 = _d2;

    return ((*d1) - (*d2));
}

static int cmp_elem_p2(const void *_d1, const void *_d2)
{
    const int *d1 = *(int **)_d1;
    const int *d2 = *(int **)_d2;

    return d1 == d2 ? 0: -1;
}

static void test_vector_find_with_pointer(void)
{
    gv_t(int_p) vector;
    int *elem;
    int tab[5] = {1, 2, 3, 4, 5};

    gv_init(&vector);

    gv_add(&vector, &tab[1]);
    gv_add(&vector, &tab[0]);
    gv_add(&vector, &tab[3]);
    gv_add(&vector, &tab[2]);
    gv_add(&vector, &tab[4]);

    elem = p_calloc(sizeof(int));

    for (int i = 1; i <= 5; i++) {
        /* all depends on what the suer looks for: the value or 
         * the memory space */
        ASSERT(gv_contains(&vector, i, GV_SEQUENTIAL_SEARCH, cmp_elem_p),
               "elem %d has not been fond", i);

        ASSERT(gv_contains(&vector, &tab[i - 1], GV_SEQUENTIAL_SEARCH,
                           cmp_elem_p2),
               "pointer elem %d has not been fond", i);
    }

    for (int i = 1; i <= 5; i++) {
        *elem = i;

        ASSERT(gv_contains(&vector, (*elem), GV_SEQUENTIAL_SEARCH, cmp_elem_p),
               "elem %d has not been fond", i);

        ASSERT((!(gv_contains(&vector, elem, GV_SEQUENTIAL_SEARCH,
                              cmp_elem_p2))),
               "elem %d has not been fond", i);
    }

    gv_wipe(&vector, NULL);
    p_free((void **)&elem);
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
    ADD_TEST_TO_MODULE(module_tests, test_vector_shuffle);
    ADD_TEST_TO_MODULE(module_tests,
                       test_vector_find_and_contains_sequential_algo);
    ADD_TEST_TO_MODULE(module_tests,
                       test_vector_find_and_contains_dichotomy_algo);
    ADD_TEST_TO_MODULE(module_tests, test_vector_find_with_pointer);

    return module_tests;
}
