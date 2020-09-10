//
//  graphe.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include "graphe.h"
#include <stdlib.h>

GRAPHE creer_graphe(unsigned int taille){
    
    GRAPHE g = (GRAPHE)calloc(taille, sizeof(*g));
    int i = 0;
    
    if (g!=NULL){
        for(i = 0; i<taille; i++){
            g[i].numero = i;
            g[i].voisin = creer_liste();
        }
    }
    return g;
}
 

GRAPHE ajout_arc(GRAPHE g, ARC a){
    
    POINTEUR_ARC pa = (POINTEUR_ARC)calloc(1, sizeof(*pa));
    POINTEUR_ARC pai = (POINTEUR_ARC)calloc(1, sizeof(*pa));
    
    pa->arrive = a.arrive;
    pa->depart = a.depart;
    pa->cout = a.cout;
    pa->pheromone = a.pheromone;
    
    pai->arrive = a.depart;
    pai->depart = a.arrive;
    pai->cout = a.cout;
    pai->pheromone = a.pheromone;
    
    g[pa->depart].voisin = ajout_tete(pa, g[pa->depart].voisin);
    
    g[pai->depart].voisin = ajout_tete(pai, g[pai->depart].voisin);
    
    return g;
}

void visualiser_graphe(GRAPHE g, unsigned int* taille){
    int i;
    for (i = 0; i< *taille; i++){
        if(!est_vide(g[i].voisin)){
            printf("Voisins du sommet %d\n", i);
            visualiser_liste(g[i].voisin);
            //printf("\n");
        }
    }
}
