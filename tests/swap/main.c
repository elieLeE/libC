#include "../../src/swap.h"
#include "../shared/ftest.h"

int main(){
    int a = 5;
    int b = 13;

    echangeGenerique(&a, &b, sizeof(a)); 

    affResultTest("testEchangeGenerique", ((a==13) && (b = 5)));

    return 0;
}
