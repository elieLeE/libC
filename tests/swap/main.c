#include "../../src/macros.h"
#include "../shared/ftest.h"

int main(){
    int a = 5;
    int b = 13;

    SWAP(int, &a, &b);

    affResultTest("testEchangeGenerique", ((a==13) && (b = 5)));

    return 0;
}
