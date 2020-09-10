//
//  pile.c
//  td pile
//
//  Created by Yann Debain on 14/02/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#include "pile.h"

int pile_vide(Pile p) {
    return liste_vide(p);
}

Pile creer_pile(void) {
   return creer_liste();
}

Pile empiler(CARTE c, Pile p){
   return ajout_tete(c, p);
}


//PRECONDITION : Ne prend pas de pile vide
CARTE sommet(Pile p){
    return p->val;
}

CARTE depiler(Pile* pp) {

    CARTE c = (*pp)->val;
    *pp = supprimer_tete(*pp);
    return c;
}

void visualiser_pile(Pile p){
    visualiser_liste(p);
}

