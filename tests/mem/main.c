#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../src/mem/alloc.h"
#include "test.h"

int main(){
    srand(time(NULL));
    
    testMyCalloc();
    testMyRealloc();
    testAllocTab2D();

    return 0;
}
