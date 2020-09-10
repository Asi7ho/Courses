#include "carte.h"
#include <stdio.h>

void affiche (CARTE* e) {
    static char* mesc[]= {"trefle","carreau","coeur","pique"};
    static char* mesval[] = {"as","2","3","4","5","6","7","8","9","dix","valet","dame","roi"};
    printf("%s de %s ;",mesval[e->rang-1],mesc[e->couleur]);
}

void affichevisible(CARTE* e) {
    static char* mesc[]= {"trefle","carreau","coeur","pique"};
    static char* mesval[] = {"as","2","3","4","5","6","7","8","9","dix","valet","dame","roi"};
    if (e->visible) printf("%s de %s ;",mesval[e->rang-1],mesc[e->couleur]);
}

void init_carte (CARTE* e) {
    e->rang=0;
    e->visible=0;
    e->couleur=0;
}
