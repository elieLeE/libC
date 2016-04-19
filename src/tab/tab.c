#include "tab.h"

int searchInd(void *tab, int tailleTab, size_t tailleType, void * n, int (*compar)(void*, void*)){
    int i;
    for(i=0; i<tailleTab; i++){
	if((*compar)(tab + i*tailleType, n) != -1){
	    break;
	}
    }
    if((*compar)(tab + i*tailleType, n) == 1){
	i--;
    }
    return i;
}

void visuTab(unsigned int* tab, unsigned int tailleTab, void (*visuElem)(void *e), char* s){
    unsigned int i;
    
    for(i=0; i<tailleTab; i++){
	(*visuElem)(tab + i);
    }
    printf("%s", s);
}
