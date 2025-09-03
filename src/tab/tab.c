#include "tab.h"

int searchInd(void const *tab, unsigned int const tailleTab, size_t const tailleType, void const *n, int (*compar)(void const*, void const*)){
    if(tab==NULL){
        fprintf(stderr, "tab NULL, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return -1;
    }
    else if(tailleTab<=1){
        fprintf(stderr, "tailleTab <= 1, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return -1;
    }
    else if(tailleType<1){
        fprintf(stderr, "tailleType < 1, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return -1;
    }
    else if(n==NULL){
        fprintf(stderr, "n NULL, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return -1;
    }
    else if(compar==NULL){
        fprintf(stderr, "compar NULL, fichier %s, ligne %d\n", __FILE__, __LINE__);
        return -1;
    }

    structDichoSearch s;
    s.tab = tab;
    //ne sert a rien pour la recherche dichotomique
    s.tailleTab = tailleTab;
    s.tailleType = tailleType;
    s.valSearched = n;
    s.compar = compar;
    s.deb = 0;
    s.pos = (tailleTab-1)/2;
    s.fin = tailleTab-1;

    return searchDicho(&s);
    return searchBasique(&s);
}

int searchBasique(structDichoSearch *s){
    unsigned int i;
    for(i=0; i<s->tailleTab; i++){
        if((*(s->compar))(s->tab + i*s->tailleType, s->valSearched) > 0){
            break;
        }
    }
    if((*(s->compar))(s->tab + i*s->tailleType, s->valSearched) == 0){
        return i;
    }
    return i-1;
}

int searchDicho(structDichoSearch *s){
    void const *addr = s->tab + (s->pos)*(s->tailleType);
    if((*(s->compar))(addr, s->valSearched) == 0){
        return s->pos;
    }
    else if((*(s->compar))(addr, s->valSearched)<0){
        s->pos = ((s->pos) * (s->fin))/2;
    }
    else{
        s->pos = ((s->deb) * (s->pos))/2;
    }

    return searchDicho(s);
}

void visuTab(void const *tab, unsigned int const tailleTab, size_t const tailleType, void (*visuElem)(void const *e), char const* s){
    unsigned int i;

    for(i=0; i<tailleTab; i++){
        (*visuElem)(tab + i*tailleType);
    }
    printf("%s", s);
}
