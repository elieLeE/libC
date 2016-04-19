#include <stdio.h>
#include <stdlib.h>
#include "../../liste.h"

void visuElement(void *data){
    printf("%d, ", *((int *)data));
}

int main(){
    liste* l = creerListe();

    int *a = my_calloc(sizeof(int));
    int *b = my_calloc(sizeof(int));
    int *c = my_calloc(sizeof(int));

    *a = 1;
    *b = 2;
    *c = 3;

    addElementFirst(&l, a);
    addElementFirst(&l, b);
    //addElementLast(l, &a, sizeof(int));

    //removeLastElement(l);
    visuListe(l, (void(*)(void *))visuElement, "\b\b \n");

    freeListe(&l);

    return 0;
}
