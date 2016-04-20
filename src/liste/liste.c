#include "liste.h"

liste creerListe(void (*visuElement)(void *data), void (*removeData)(void *data)){
    liste l;
    l.first = NULL;
    l.end = NULL;
    l.nbreElement = 0;
    l.visuElement = visuElement;
    l.removeData = removeData;

    return l;
}

bool listeEmpty(element *e){
    return (e==NULL);
}

void visuListe(liste l, char *s){
    element *p = l.first;
    while(!listeEmpty(p)){
	(*(l.visuElement))(p->data);
	p = p->suiv;
    }
    printf("%s", s);
}

void addElementFirst(liste* l, void* data){
    element* new = my_calloc(sizeof(struct element));
    new->data = data;
    new->suiv = l->first;
    new->prec = NULL;
    if(!listeEmpty(l->first)){
	l->first->prec = new;
    }
    else{
	l->end = new;
    }
    l->first = new;
    (l->nbreElement)++;
}

void addElementLast(liste *l, void* data){
    element* new = my_calloc(sizeof(struct element));
    new->data = data;
    new->suiv = NULL;
    new->prec = l->end;
    if(!listeEmpty(l->first)){
	l->end->suiv = new;
    }
    else{
	l->first = new;
    }
    l->end = new;
    (l->nbreElement)++;
}

void removeFirstElement(liste *l){
    if(!listeEmpty(l->first)){
	element *p = l->first;
	l->first = l->first->suiv;
	(l->nbreElement)--;
	(*(l->removeData))(p->data);
	free(p);
	if(!listeEmpty(l->first)){
	    l->first->prec = NULL;
	}
    }
}

void removeLastElement(liste* l){
    if(!listeEmpty(l->first)){
	element *p = l->end;
	l->end = l->end->prec;
	(l->nbreElement)--;
	(*(l->removeData))(p->data);
	free(p);
	if(!listeEmpty(l->first)){
	    l->end->suiv = NULL;
	}
    }
}

void freeListe(liste* l){
    while(!listeEmpty(l->first)){
	removeFirstElement(l);
    }
}

