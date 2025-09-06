#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test.h"

int main(){
    srand(time(NULL));

    testMyCalloc();
    testMyRealloc();
    testAllocTab2D();

    return 0;
}
