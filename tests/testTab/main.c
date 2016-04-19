#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../tab.h"

#define TAILLE_TAB 10

void visuElem(void *e){
    printf("%d, ", *((unsigned int*)e));
}

void randomTabRemplissage(unsigned int* tab){
    unsigned int min = 0;
    unsigned int max = TAILLE_TAB;
    unsigned int i;

    for(i=0; i<TAILLE_TAB; i++){
	tab[i] = rand()%(max - min)+min;
    }
}


int main(){
    unsigned int tab[TAILLE_TAB];
    srand(time(NULL));

    randomTabRemplissage(tab);

    visuTab(tab, 10, visuElem, "\b\b \n");

    return 0;
}
