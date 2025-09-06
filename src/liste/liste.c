#include "liste.h"
#include "../mem/alloc.h"
#include "removeElement.h"

/* {{{ Adding methods */

void addElementFirst(liste* l, void *data){
    element* new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;
    if(!listeEmpty(l)){
        l->first->prec = new;
    }
    else{
        l->end = new;
    }
    l->first = new;
    (l->nbreElement)++;
}

void addElementLast(liste *l, void *data){
    element* new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = NULL;
    new->prec = l->end;
    if(!listeEmpty(l)){
        l->end->suiv = new;
    }
    else{
        l->first = new;
    }
    l->end = new;
    (l->nbreElement)++;
}

void addElementTrie(liste *l, void *data, bool sensCroissant){
    if(!(l->comparElement == NULL)){
        if(listeEmpty(l)){
            addElementFirst(l, data);
        }
        else{
            if(sensCroissant){
                addElementTrieC(l, data);
            }
            else{
                addElementTrieD(l, data);
            }
        }
    }
    else{
        fprintf(stderr, "comparElement NULL\n");
    }
}

void addElementTrieC(liste *l, void *data){
    if((*(l->comparElement))(l->first->data, data) >= 0){
        addElementFirst(l, data);
    }
    else if((*(l->comparElement))(l->end->data, data) <= 0){
        addElementLast(l, data);
    }
    else{
        element *p = l->first;
        if((*(l->comparElement))(data, l->first->data) > 0){
            while((!elemEmpty(p->suiv)) && ((*(l->comparElement))(p->suiv->data, data) < 0)){
                p = p->suiv;
            }
        }
        addElementNext(p, data);
        (l->nbreElement)++;
    }
}

void addElementNext(element *e, void *data){
    /*printf("\ndans addElementNext : \n");
      printf("e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));*/
    element *new = my_calloc(sizeof(*new));
    new->data = data;
    new->suiv = e->suiv;
    new->prec = e;
    e->suiv->prec = new;
    e->suiv = new;
    //printf("\n");
}

void addElementTrieD(liste *l, void *data){
    if((*(l->comparElement))(l->first->data, data) <= 0){
        addElementFirst(l, data);
    }
    else if((*(l->comparElement))(l->end->data, data) >= 0){
        addElementLast(l, data);
    }
    else{
        element *p = l->first;
        if((*(l->comparElement))(data, l->first->data) < 0){
            while((!elemEmpty(p->suiv)) && ((*(l->comparElement))(p->suiv->data, data) > 0)){
                p = p->suiv;
            }
        }
        addElementNext(p, data);
        (l->nbreElement)++;
    }
}

/* }}} */
/* {{{ Removing methods */

void removeData(liste const *l, void *data){
    if(l->removeData != NULL){
        (*(l->removeData))(data);
    }
}

void removeFirstElement(liste *l){
    if(!listeEmpty(l)){
        element *p = l->first;
        l->first = l->first->suiv;
        (l->nbreElement)--;
        removeData(l, p->data);
        free(p);
        if(!listeEmpty(l)){
            l->first->prec = NULL;
        }
    }
}

void removeLastElement(liste* l){
    if(!listeEmpty(l)){
        element *p = l->end;
        l->end = l->end->prec;
        (l->nbreElement)--;
        removeData(l, p->data);
        free(p);
        if(!listeEmpty(l)){
            l->end->suiv = NULL;
        }
    }
}

void removeElementN(liste *l, unsigned int const n){
    printf("removeElementN NOT YET FONCTIONNAL\n");
    return;
    if(((int)n > 0) && (n <= l->nbreElement)){
        if(n == 1){
            removeFirstElement(l);
        }
        else if(n == l->nbreElement){
            removeLastElement(l);
        }
        else{
            printf("suppresion milieu : %d\n", n);
            element *p = NULL;
            unsigned int compt = 1;
            p = l->first;
            while(compt<n-1){
                p = p->suiv;
            }
            /*if(n<n/2){
              p = l->first;
              while(compt<n-1){
              p = p->suiv;
              }
              }
              else{
              n = l->nbreElement - n;
              p = l->end;
              while(compt<n-1){
              p = p->prec;
              }
              }*/
            removeNextElement(l, p);
        }
    }
    printf("\n");
}

void removeNextElement(liste *l, element *e){
    printf("removeNextElement NOT YET FONCTIONNAL\n");
    return;
    //printf("removeNextElement : e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));
    element *p = e->suiv;
    e->suiv = e->suiv->suiv;
    e->suiv->prec = e;
    //printf("removeNextElement : e : %d, e->suiv : %d\n", *((int*)e->data), *((int*)e->suiv->data));
    removeData(l, e->data);
    free(p);
    //printf("sorti removeNextElement\n");
}

/* }}} */
/* {{{ Trie methods */

/*void trieListeFusion(liste *l, bool sensCroissant){
}

void division(liste *l){

}

void fusion(liste *l, bool sensCroissant){

}*/

/* }}} */

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

