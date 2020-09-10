//
//  graphe.h
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef graphe_h
#define graphe_h

#include <stdio.h>
#include "structure.h"
#include "liste.h"


/*
 Retourne un graphe vide de taille "taille".
 */
GRAPHE creer_graphe(unsigned int taille);


/*
 Retroune un graphe complété.
 On ajoute un arc au graphe pour le complété et avoir le graphe complet demandé par la gestion de fichier
 */
GRAPHE ajout_arc(GRAPHE g, ARC a);


/*
 Permet de visualiser le graphe.
 On peut suivre l'évolution du graphe au fil du temps.
 */
void visualiser_graphe(GRAPHE g, unsigned int* taille);


#endif /* graphe_h */
