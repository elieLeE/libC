#include "fonctionTest.h"

void randomTabRemplissage(unsigned int* tab, unsigned int taille){
    unsigned int min = 0;
    unsigned int max = taille*10;
    unsigned int i;

    for(i=0; i<taille; i++){
	tab[i] = rand()%(max - min)+min;
    }
}

void constantTabRemplissage(unsigned int* tab, unsigned int taille){
    unsigned int i;

    for(i=0; i<taille; i++){
	tab[i] = i;
    }
    /*tab[0] = 51;
    tab[1] = 60;
    tab[2] = 19;
    tab[3] = 24;
    tab[4] = 4;
    tab[5] = 51;
    tab[6] = 65;
    tab[7] = 93;
    tab[8] = 76;
    tab[9] = 84;*/
}

bool verifTriageTab(unsigned int* tab, unsigned int tailleTab){
    unsigned int i;
    bool croissant = (tab[0] < tab[1]);

    for(i=1; i<tailleTab-1; i++){
	if(croissant){
	    if(tab[i] > tab[i+1]){
		return false;
	    }
	}
	else{
	    if(tab[i] < tab[i+1]){
		return false;
	    }
	}
    }
    return true;
}

/*void affResultatTest(char *s, bool result){
    printf("%s %s\n", s, result?"ok":"not ok");
}*/

