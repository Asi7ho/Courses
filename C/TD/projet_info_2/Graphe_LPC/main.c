//
//  main.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include "fichiers.h"
#include "graphe.h"
#include "structure.h"
#include "fourmis.h"
#include <math.h>

#include <stdlib.h>
#include <stdio.h>

#define NOM_DU_FICHIER "graphe11.txt"


/*
 Test n°1 : On teste le chargement du graphe et les différentes fonctions qui permettent de construire le graphe. On vérifie que le graphe a bien été créé
 */

/*int main(int argc, const char * argv[]) {
    
    GRAPHE G=NULL;
    unsigned int tailleGraphe=0;
    G = chargeFichierGraphe(NOM_DU_FICHIER, &tailleGraphe);
    printf("chargement ok\n");
    visualiser_graphe(G, &tailleGraphe);
 
    return 0;
    
}*/

/*
 Test n°2 : On teste la gestion des fourmis, on vérfie que les phéromones changent bien et que les fourmis se déplacent comme le veut le sujet
 */

/*int main(int argc, const char * argv[]) {
    
    GRAPHE G=NULL;
    FOURMIS* tab = NULL;
    double poids;
    unsigned int tailleGraphe=0;
    int i = 0;
    
    G = chargeFichierGraphe(NOM_DU_FICHIER, &tailleGraphe);
    tab = creer_fourmis(G, tailleGraphe);
    
    for (i = 0; i < NOMBRE_DE_FOURMIS; i++){
        printf("\nFile de la fourmis %d\n", i+1);
        parcours(G,tab+i, tailleGraphe);
        visualiser_file(tab[i].solution);
        printf("\n");
        poids = poids_solution(tab[i].solution);
        printf("poids de la solution de la fourmis n°%d  = %lf\n", i+1, poids);
    }
    return 0;
}*/


/* 
 Main général du code : On affiche la file avec la meilleure solution sur le nombre de cycle et le poids du cycle et le meilleur chemin . La gestion du meileur chemin est fait par le main.
 */

int main(int argc, const char * argv[]) {
    
    GRAPHE G=NULL;
    FOURMIS* tab = NULL;
    Liste l = NULL;
    Liste stk = NULL;
    File caca;
    
    unsigned int tailleGraphe=0;
    double meilleur_poids = 0;
    int i = 0;
    int j = 0;
    double max_phero = 0;
    
    G = chargeFichierGraphe(NOM_DU_FICHIER, &tailleGraphe);
    tab = creer_fourmis(G, tailleGraphe);
    
    for (i = 0; i < NOMBRE_DE_FOURMIS; i++){
        
        for (j = 0; j < MAX_CYCLE; j++){
        
            parcours(G,tab+i, tailleGraphe);
        
        }
    }
    
    printf("\nchemin le plus cours\n");
    for ( i = 0; i < tailleGraphe; i++){
        
        for (l = G[i].voisin; l != NULL; l = l->suiv){
            
            if (l->val->pheromone > max_phero){
                
                max_phero = l->val->pheromone;
                stk = l;
            }
        }

        if (max_phero != VALEUR_INITIALE_PHEROMONE){ //pour des petits cycles on évite les passages par des villes inutiles
            
            printf ("depart  = %d  arrivée = %d  cout = %lf pheromone = %lf\n",stk->val->depart, stk->val->arrive, stk->val->cout, stk->val->pheromone);
            meilleur_poids += stk->val->cout;
        }
        max_phero = 0;
    }
    
    printf("\ncout du meilleur chemin = %lf", meilleur_poids);
    printf("\n\n");
    
    return 0;
}
