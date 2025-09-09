#include <stdio.h>
#include <stdlib.h>
#include <libcmine.h>

void testCalloc()
{
    int *a = p_calloc(sizeof(int));

    if(a != NULL) {
        printf("allocation done\n");
    }
    free(a);
}

void testprime()
{
    if(isPrime(5)) {
        printf("5 premier\n");
    } else {
        printf("5 non premier\n");
    }
}

int main()
{
    testCalloc();
    testprime();

    return 0;
}
