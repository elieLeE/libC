#include "liste.h"

liste* creerListe(){
    return NULL;
}

bool listeEmpty(liste *l){
    return (l==NULL);
}

void visuListe(liste *l, void (*visuElement)(void *data), char *s){
    liste *p = l;
    while(!listeEmpty(p)){
	(*visuElement)(p->data);
	p = p->suiv;
    }
    printf("%s", s);
}

void addElementFirst(liste** l, void* data){
    liste* new = my_calloc(sizeof(struct liste));
    new->data = data;
    new->suiv = *l;
    *l = new;
}

void addElementLast(liste *l, void* data){
    liste* p = l;
    liste* new = my_calloc(sizeof(struct liste));
    new->data = data;
    new->suiv = NULL;
    while(!listeEmpty(p->suiv)){
	p = p->suiv;
    }
    p->suiv = new;
}

void removeFirstElement(liste **l){
    liste *p = *l;
    *l = (*l)->suiv;
    free(p->data);
    free(p);
}

void removeLastElement(liste* l){
    liste* p = l;
    while(!listeEmpty(p->suiv)){
	//printf("%d\n", *((int *)p->data));
	p = p->suiv;
    }
    printf("%d\n", *(int *)p->data);
    //free(p->data);
    printf("ici\n");
    free(p);
    printf("ici 2\n");
}

void freeListe(liste** l){
    while(!listeEmpty(*l)){
	removeFirstElement(l);
    }
}

