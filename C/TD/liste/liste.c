//
//  liste.c
//  liste
//
//  Created by Yann Debain on 02/02/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#include "liste.h"
#include "carte.h"
#include <stdio.h>
#include <stdlib.h>



Liste creer_liste(void) {
    return NULL;
}

int liste_vide(Liste l) {
    return l != NULL;
}

void visualiser_liste(Liste l) {
    Liste p;
    for (p = l; p != NULL; p = p->suiv){
        affiche(&(p->val));
	printf("\n");
    }
}

Liste ajout_tete(CARTE e, Liste l){
    Liste p = calloc(1, sizeof(*p));
    if (p == NULL){
        return NULL;
    }
    p->val = e;
    p->suiv = l;
    return l;
}

Liste supprimer_tete(Liste l){
    if(l == NULL){
        return NULL;
    }
    Liste stk = l->suiv;
    free(l);
    return stk;
}

Liste ajout_queue(CARTE e, Liste l){
    Liste p = calloc(l, sizeof(*p));
    Liste c;
    if (p == NULL){
        return NULL;
    }

	if (l == NULL) return ajout_tete(e, l);

    for (c = l; c->suiv != NULL; c = c->suiv){
        
    }
    p->val = e;
    p->suiv = NULL;
    c->suiv = p;
    return l;
}

Liste supprimen(int n, Liste l){
    Liste p = l;
    int i;

if(l == NULL){
        return NULL;
    }
    
    for(i = 0; i<n-1; i++){
        p = p->suiv; // a chaque tour de boucle, p pointe le maillon num i dans la liste (le i+1eme maillon, donc)
    }
    Liste supp = p->suiv;
    p->suiv = p->suiv->suiv; 
    free(supp);
    return l;
}

Liste concat(Liste l1, Liste l2){
    Liste p;
    if (l1 == NULL){
        return l2;
    }
    for(p = l1; p->suiv != NULL; p = p->suiv){
        
    }
    p->suiv = l2;
    return l1;
}

Liste copie(Liste l){
    Liste l_copie;
    Liste p;
    for(p = l; p != NULL; p = p->suiv){
	l_copie = ajout_queue(p->val, l_copie);
    }
    return l_copie;
}



