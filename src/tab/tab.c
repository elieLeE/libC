#include "tab.h"

int searchInd(void const *tab, int const tailleTab, size_t const tailleType, void const *n, int (*compar)(void const*, void const*)){
    int i;
    for(i=0; i<tailleTab; i++){
	if((*compar)(tab + i*tailleType, n) > 0){
	    break;
	}
    }
    if((*compar)(tab + i*tailleType, n) == 0){
	return i;
    }
    return i-1;
}

void visuTab(void const *tab, unsigned int const tailleTab, size_t const tailleType, void (*visuElem)(void const *e), char const* s){
    unsigned int i;
    
    for(i=0; i<tailleTab; i++){
	(*visuElem)(tab + i*tailleType);
    }
    printf("%s", s);
}
