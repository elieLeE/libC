#include "../../src/liste/liste.h"
#include "test.h"

int main(){
    liste l = creer_liste();

    test_add_and_remove_element(&l);

    /*
     * simplie l'initilisation de la liste
     * et permet de tester avec et sans liberation explicaites des datas
     * */
    l = creer_liste();

    test_add_element_trie_c(&l);
    test_add_element_trie_d(&l);

    /*l = creerListe((void(*)(void *))visuElement,
     * (int(*)(void*, void*))comparElement, (void(*)(void *))removeElement);
      testRemoveElement(&l);*/

    return 0;
}
