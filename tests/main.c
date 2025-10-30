#include <time.h>
#include <stdlib.h>

#include "../src/macros.h"

#include "test_liste.h"
#include "test_io.h"

static void test_swap(void)
{
    int a = 5;
    int b = 13;

    SWAP(int, &a, &b);

    ASSERT_EQUAL(a, 13);
    ASSERT_EQUAL(b, 5);
}

static void run_without_modules_tests(void)
{
    BEGIN_TEST_MODULE("WITHOUT MODULE");

    CALL_TEST_FUNC(test_swap);

    END_TEST_MODULE();
}

int main()
{
    srand(time(NULL));

    run_without_modules_tests();
    run_tests_liste();
    run_tests_io();

    return 0;
}
