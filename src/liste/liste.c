#include "liste.h"

liste creerListe(void (*visuElement)(void const *data), int (*comparElement)(void const *, void const *), void (*removeData)(void *data)){
    liste l;
    l.first = NULL;
    l.end = NULL;
    l.nbreElement = 0;
    l.visuElement = visuElement;
    l.comparElement = comparElement;
    l.removeData = removeData;

    return l;
}

bool listeEmpty(liste *l){
    return (l->first==NULL);
}

bool elemEmpty(element *e){
    return (e == NULL);
}

void visuListe(liste l, char const *s){
    if(l.visuElement != NULL){
        element *p = l.first;
        while(!elemEmpty(p)){
            (*(l.visuElement))(p->data);
            p = p->suiv;
        }
        printf("%s", s);
    }
    else{
        fprintf(stderr, "impossible de visuliser la liste, visuElement NULL\n");
    }
}

void freeListe(liste* l){
    while(!listeEmpty(l)){
        removeFirstElement(l);
    }
    l->first = NULL;
    l->end = NULL;
}

