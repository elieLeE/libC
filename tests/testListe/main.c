#include <stdio.h>
#include <stdlib.h>
#include "../../src/liste/liste.h"
#include "test.h"

void visuElement(void const *data){
    printf("%d, ", *((int *)data));
}

void removeElement(void *data){
    free(data);
}

int comparElement(void const *d1, void const *d2){
    int const *a = d1;
    int const *b = d2;

    return (*a - *b);
}

int main(){
    liste l = creerListe((void(*)(void const*))visuElement, (int(*)(void const*, void const*))comparElement, (void(*)(void *))removeElement);
    
    testAddAndRemoveElement(&l);

    /*
     * simplie l'initilisation de la liste
     * et permet de tester avec et sans liberation explicaites des datas
     * */
    l = creerListe((void(*)(void const*))visuElement, (int(*)(void const*, void const*))comparElement, NULL);

    testAddElementTrieC(&l);
    testAddElementTrieD(&l);

    /*l = creerListe((void(*)(void *))visuElement, (int(*)(void*, void*))comparElement, (void(*)(void *))removeElement);
    testRemoveElement(&l);*/

    return 0;
}
