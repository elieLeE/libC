#include <stdio.h>
#include <stdlib.h>
#include "../../src/liste/liste.h"
#include "test.h"

void visuElement(void *data){
    printf("%d, ", *((int *)data));
}

void removeElement(void *data){
    free(data);
}

int main(){
    liste l = creerListe((void(*)(void *))visuElement, (void(*)(void *))removeElement);
    
    test1(&l);
    test2(&l);
    test3(&l);
    test4(&l);

    return 0;
}
