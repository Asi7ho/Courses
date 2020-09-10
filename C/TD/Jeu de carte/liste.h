//
//  liste.h
//  liste
//
//  Created by Yann Debain on 02/02/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#ifndef liste_h
#define liste_h

#include <stdio.h>
#include "carte.h"

typedef CARTE ELEMENT;
struct cellule
{
    ELEMENT val;
    struct cellule *suiv;
};

typedef struct cellule Maillon;
typedef struct cellule * Liste;

Liste creer_liste(void);
int liste_vide(Liste l);
void visualiser_liste(Liste l);
Liste ajout_tete(CARTE e , Liste l);
Liste supprimer_tete(Liste ) ;
Liste ajout_queue(CARTE e , Liste l);
Liste supprimen(int n, Liste l);

#endif /* liste_h */
