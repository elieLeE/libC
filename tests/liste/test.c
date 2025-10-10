#include "test.h"
#include "../../src/mem/mem.h"
#include "../../src/macros.h"

static int *get_new_int(int val)
{
    int *p = p_calloc(sizeof(int));

    *p = val;

    return p;
}

static void check_list_data(const generic_liste_t *l,
                            const int expected_vals[],
                            unsigned int val_count)
{
    unsigned int i = 0;

    ASSERT_EQUAL(l->nbre_elem, val_count);

    gl_for_each(elem, l->first) {
        ASSERT(i < val_count, "i: %d, val_count: %d", i, val_count);
        ASSERT_EQUAL((*(int *) (elem->data)), expected_vals[i]);
        i++;
    }
}

static void remove_element(void *data)
{
    p_free(&data);
}

static int cmp_elem_increasing(void const *d1, void const *d2)
{
    int const *a = d1;
    int const *b = d2;

    return (*a - *b);
}

static int cmp_elem_decreasing(void const *d1, void const *d2)
{
    int const *a = d1;
    int const *b = d2;

    return (*b - *a);
}

void test_get_elem_data(void)
{
    int *a;
    generic_liste_t l;
    generic_elem_liste_t *elem;

    gl_init(&l);

    ASSERT(gl_is_empty(&l), "list should be empty");

    a = p_calloc(sizeof(int));
    *a = 1;

    gl_add_elem_first(&l, a);
    elem = gl_get_elem_data(&l, a, cmp_elem_increasing);

    ASSERT((elem != NULL), "elem has not been found");

    gl_free(&l, remove_element);

    ASSERT(gl_is_empty(&l), "list should be empty");
}

void test_add_and_remove_element(void)
{
    generic_liste_t l;
    int expected_vals[3] = {6, 1, 3};

    gl_init(&l);

    gl_add_elem_last(&l, get_new_int(1));
    gl_add_elem_first(&l, get_new_int(2));
    gl_add_elem_last(&l, get_new_int(3));
    gl_remove_first_elem(&l, remove_element);
    gl_add_elem_last(&l, get_new_int(4));
    gl_add_elem_first(&l, get_new_int(5));
    gl_remove_last_elem(&l, remove_element);
    gl_remove_first_elem(&l, remove_element);
    gl_add_elem_first(&l, get_new_int(6));

    check_list_data(&l, expected_vals, 3);

    gl_free(&l, remove_element);
}

void test_add_element_trie(void)
{
    generic_liste_t l;
    int expected_vals[6] = {1, 2, 3, 4, 5, 6};

    gl_init(&l);

    gl_add_elem_sorted(&l, &expected_vals[2], cmp_elem_increasing);
    gl_add_elem_sorted(&l, &expected_vals[4], cmp_elem_increasing);
    gl_add_elem_sorted(&l, &expected_vals[5], cmp_elem_increasing);
    gl_add_elem_sorted(&l, &expected_vals[1], cmp_elem_increasing);
    gl_add_elem_sorted(&l, &expected_vals[3], cmp_elem_increasing);
    gl_add_elem_sorted(&l, &expected_vals[0], cmp_elem_increasing);

    check_list_data(&l, expected_vals, 6);

    gl_free(&l, NULL);

    ASSERT_EQUAL(l.nbre_elem, 0);
    ASSERT(l.first == NULL, "first elem of the list should be NULL");
    ASSERT(l.end == NULL, "last elem of the list should be NULL");

    for (int i = 0, val = 6; i < 6; i++, val--) {
        expected_vals[i] = val;
    }

    gl_add_elem_sorted(&l, &expected_vals[1], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &expected_vals[3], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &expected_vals[0], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &expected_vals[5], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &expected_vals[2], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &expected_vals[4], cmp_elem_decreasing);

    check_list_data(&l, expected_vals, 6);

    gl_free(&l, NULL);
}

/* TODO => not call for now */
void test_remove_element(void)
{
    generic_liste_t l;
    int expected_vals[5] = {1, 3, 5, 6};

    gl_init(&l);

    gl_add_elem_sorted(&l, get_new_int(1), cmp_elem_increasing);
    gl_add_elem_sorted(&l, get_new_int(2), cmp_elem_increasing);
    gl_add_elem_sorted(&l, get_new_int(3), cmp_elem_increasing);
    gl_add_elem_sorted(&l, get_new_int(4), cmp_elem_increasing);
    gl_add_elem_sorted(&l, get_new_int(5), cmp_elem_increasing);
    gl_add_elem_sorted(&l, get_new_int(6), cmp_elem_increasing);

    gl_remove_elem_n(&l, 2, remove_element);
    //gl_remove_elem_&n(l, 3);

    check_list_data(&l, expected_vals, 6);

    gl_free(&l, remove_element);
}

void test_triage_liste(void)
{
    generic_liste_t l;
    int tab[6] = {1, 2, 3, 4, 5, 6};

    gl_init(&l);

    gl_add_elem_sorted(&l, &tab[4], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &tab[0], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &tab[1], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &tab[5], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &tab[3], cmp_elem_decreasing);
    gl_add_elem_sorted(&l, &tab[2], cmp_elem_decreasing);

    check_list_data(&l, tab, 6);

    gl_free(&l, NULL);
}
