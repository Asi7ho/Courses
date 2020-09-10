//
//  structure.h
//  Graphe LPC
//
//  Created by Yann Debain on 28/04/2017
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef structure_h
#define structure_h

typedef struct arc* POINTEUR_ARC;

struct cellule {
    POINTEUR_ARC val;
    struct cellule * suiv;} ;

typedef struct cellule * Liste;



/*
 Definition de la structure d'un arc : 
 Le numéro de la ville de départ
 Le numéro de la ville d'arrivée
 Le cout du trajet
 Le nombre de phéromones présent sur le trajet
 */
struct arc {
    unsigned int depart;
    unsigned int arrive;
    double cout;
    double pheromone;
};



/*
 Definition de la structure d'un sommet : 
 Le nom du sommet
 Le numéro du sommet dans le graphe
 La position su sommet en x et y
 La liste des voisins du sommet
 */
struct sommet {
    char nom[64];
    unsigned int numero;
    double x;
    double y;
    Liste voisin;
};


typedef Liste File;

/*
 Definition de la structure de fourmi : 
 La ville de départ de la fourmis qui est tirée au hasard
 La ville où la ville se trouve à chaque itération
 La file de la solution de la fourmi, ie la file de l'ensemble des villes parcourues par la fourmi
 */
struct fourmis {
    unsigned int ville_depart;
    unsigned int ville_courante;
    File solution;
};


typedef struct arc ARC;
typedef struct arc* POINTEUR_ARC;
typedef struct sommet SOMMET;
typedef struct sommet* GRAPHE;
typedef struct fourmis FOURMIS;

#endif /* structure_h */
