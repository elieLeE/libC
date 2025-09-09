#include "../shared/ftest.h"
#include "test.h"

int main()
{
    affResultTest("test_lecture\t\t", test_lecture_simple());
    affResultTest("test_lecture_matrice\t",test_lecture_matrice());

    return 0;
}
