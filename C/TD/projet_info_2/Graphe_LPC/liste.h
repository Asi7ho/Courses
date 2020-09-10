///
//  liste.h
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef _LISTE
#define _LISTE

#include <stdio.h>
#include <string.h>
#include "structure.h"


/*
 Créer une liste vide.
 */
Liste creer_liste(void);


/*
 Teste si la liste est vide.
 */
int est_vide(Liste L);


/*
 Ajoute un pointeur d'arc à la liste.
 On ajoute le pointeur de d'arc d'un voisin du sommet du graphe choisi pour le completer et avoir la liste complète des voisins de chaque sommets 
 */
Liste ajout_tete(POINTEUR_ARC e, Liste L);


/*
 Visualise les différentes informations contenues dans la liste.
 Ici permet d'afficher les voisins de chaque sommets du graphe.
 */
void visualiser_liste(Liste L);

#endif
