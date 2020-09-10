//
//  fourmis.h
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef fourmis_h
#define fourmis_h

#include <stdio.h>
#include "structure.h"
#include "file.h"

#define NOMBRE_DE_FOURMIS 2 //optimisation en puissance de 2
#define COEFFICIENT_EVAPORATION_PHEROMONE 0.5
#define IMPORTANCE_PHEROMONE 1
#define IMPORTANCE_VISIBILITEE 2
#define VALEUR_INITIALE_PHEROMONE 1E-5
#define QUANTITE_PHEROMONE_A_DEPOSEE 1
#define MAX_CYCLE 1

/*
 Creer le tableau de fourmis qui parcoura le graphe.
 */
FOURMIS* creer_fourmis(GRAPHE g, unsigned int taille_graphe);


/*
 Retourne le pointeur sur arc de la prochaine ville pas encore visité par la fourmi.
 Le calcul de cette prochaine ville se fait par probabilité.
 */
POINTEUR_ARC choix_prochaine_ville(GRAPHE g, FOURMIS* pf,  unsigned int taille);


/*
 Met à jour la quantite de pheromones sur un arc.
 Cette fonction va permettre aux fourmis de se déplacer selon le nombre de phéromones et ainsi donner une préférence sur le chemin.
 */
double mise_a_jour_des_pheromones(GRAPHE g, FOURMIS f);


/*
 Retourne la file qui contieint le parcours de la fourmi f.
 */
File parcours(GRAPHE g, FOURMIS* pf, unsigned int taille);


#endif /* fourmis_h */
