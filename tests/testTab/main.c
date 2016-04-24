#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fonctionTest.h"
#include "../../src/tab/tab.h"
#include "test.h"

#define TAILLE_TAB 10

void visuElem(void const *e){
    printf("%d, ", *((unsigned int*)e));
}

int comparElement(void const *d1, void const *d2){
    const int *a = d1;
    const int *b = d2;

    return (*a - *b);
}

int main(){
    clock_t deb, fin;

    srand(time(NULL));

    deb = clock();
    affResultatTest("testTrieFusion", testTrieFusion(comparElement));
    fin = clock();
    printf("trieFusion1\t : %lf\n", ((double)(fin-deb))/CLOCKS_PER_SEC);

    deb = clock();
    testQSort(comparElement);
    fin = clock();
    printf("qsort\t\t : %lf\n", ((double)(fin-deb))/CLOCKS_PER_SEC);

    testSearch(comparElement, visuElem);

    return 0;
}
