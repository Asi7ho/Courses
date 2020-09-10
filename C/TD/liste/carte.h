//
//  carte.h
//  liste
//
//  Created by Yann Debain on 02/02/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#include <stdio.h>

#ifndef _CARTE
#define _CARTE
enum { TREFLE,CARREAU,COEUR,PIQUE} COULEUR;

typedef struct {
    int rang;
    char visible;
    char couleur;
} CARTE;

void affiche (CARTE* e);
void affichevisible(CARTE* e);
void init_carte (CARTE* e);


#endif /* carte_h */
