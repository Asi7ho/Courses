///
//  liste.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include "liste.h"
#include <stdio.h>
#include <stdlib.h>

Liste creer_liste(void) {
    return NULL;
}

int est_vide(Liste L) {
    return !L;
}

Liste ajout_tete(POINTEUR_ARC e, Liste L){
    Liste p=(Liste) calloc(1,sizeof(*p));
    if (p==NULL) return NULL;
    p->val=e;
    p->suiv=L;
    return p;
}

void visualiser_liste(Liste l) {
    while (l!=NULL) {
        printf ("depart  = %d  arrivée = %d  cout = %lf pheromone = %lf\n",l->val->depart, l->val->arrive, l->val->cout, l->val->pheromone);
        l = l->suiv;
    }
}


