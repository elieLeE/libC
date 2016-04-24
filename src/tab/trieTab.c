#include "trieTab.h"

void trieFusion(void *tab, unsigned int const tailleTab, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2)){
    if(tab == NULL){
	fprintf(stderr, "tab NULL fichier %s\n", __FILE__);
	return;
    }
    if(tailleTab<=0){
	fprintf(stderr, "tailleTab <= 0, fichier %s\n", __FILE__);
	return;
    }
    if(comparElement == NULL){
	fprintf(stderr, "comparElement NULL, fichier %s\n", __FILE__);
	return;
    }

    //division(tab, 0, tailleTab/2-1, tailleTab-1, tailleType, comparElement);
    division(tab, 0, tailleTab-1, tailleType, comparElement);
}

void division(void *tab, unsigned int const deb, unsigned int const fin, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2)){
    if(deb<fin){
	unsigned int mil = (deb+fin)/2;
	division(tab, deb, mil, tailleType, comparElement);
	division(tab, mil+1, fin, tailleType, comparElement);
	fusion(tab, deb, mil, fin, tailleType, comparElement);
    }
}

/*
 * on travaille sur les addresses car les tests ont montre que c'etait legerement plus rapide qu'avec des variabes normales => moins de calcul
 * */
void fusion(void *tab, unsigned int const deb, unsigned int const mil, unsigned int const fin, size_t const tailleType, int (*comparElement)(void const *d1, void const *d2)){
    void *tab1, *tab2;
    unsigned int tailleTab1 = mil-deb+1;
    unsigned int tailleTab2 = fin-mil;
    void *addr1, *addr2, *addr3;
    void *maxAddr1, *maxAddr2;

    tab1 = my_calloc(tailleTab1*tailleType);
    tab2 = my_calloc(tailleTab2*tailleType);

    memcpy(tab1, tab+deb*tailleType, tailleTab1*tailleType);
    memcpy(tab2, tab+(mil+1)*tailleType, tailleTab2*tailleType);

    //adresses des derniers elements de tab1 et tab2
    maxAddr1 = tab1+tailleTab1*tailleType;
    maxAddr2 = tab2+tailleTab2*tailleType;

    //tant que tab1 et tab2 ne sont pas arrive a leur dernier element, on continue
    for(addr1=tab1, addr2=tab2, addr3=tab+deb*tailleType; addr1<maxAddr1 && addr2<maxAddr2; addr3 = addr3+tailleType){
	if(comparElement(addr1, addr2)<0){
	    memcpy(addr3, addr1, tailleType);
	    addr1 = addr1 + tailleType;
	}
	else{
	    memcpy(addr3, addr2, tailleType);
	    addr2 = addr2 + tailleType;

	}
    }

    //on finit de remplir le tableau avec les elements du sous-tableau non vide
    if(addr1!=maxAddr1){
	while(addr1<maxAddr1){
	    memcpy(addr3, addr1, tailleType);
	    addr1 = addr1 + tailleType;
	    addr3 = addr3 + tailleType;
	}
    }
    else{
	while(addr2<maxAddr2){
	    memcpy(addr3, addr2, tailleType);
	    addr2 = addr2 + tailleType;
	    addr3 = addr3 + tailleType;
	}
    }
}
