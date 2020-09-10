//
//  fourmis.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fourmis.h"
#include "graphe.h"

FOURMIS* creer_fourmis(GRAPHE g, unsigned int taille_graphe){
    
    int i;
    
    FOURMIS* tab_fourmis = calloc(NOMBRE_DE_FOURMIS, sizeof(*tab_fourmis));
    
    for (i = 0; i < NOMBRE_DE_FOURMIS; i++){
        
        tab_fourmis[i].ville_depart = rand()%taille_graphe;
        tab_fourmis[i].ville_courante = tab_fourmis[i].ville_depart;
        tab_fourmis[i].solution = creer_file();
        tab_fourmis[i].solution = enfiler(g[tab_fourmis[i].ville_depart].voisin->val, tab_fourmis[i].solution);
    }
    
    return tab_fourmis;
}

POINTEUR_ARC choix_prochaine_ville(GRAPHE g, FOURMIS* f, unsigned int taille){
    
    double tirage = 0;
    double cumul_proba = 0;
    double Fx = 0;
    File stk = f->solution;
    SOMMET p = g[stk->val->depart];
    
    tirage = (double)rand()/RAND_MAX;

    while(p.voisin != NULL) {
        while(stk == NULL){
            if (p.voisin->val->arrive != stk->val->arrive){
                cumul_proba += pow(stk->val->pheromone, IMPORTANCE_PHEROMONE)*pow(stk->val->cout, IMPORTANCE_VISIBILITEE);
                
            }
            stk = stk->suiv;
        }
        p.voisin = p.voisin->suiv;
    }

    stk = f->solution;
    p = g[stk->val->depart];
    
    while(p.voisin != NULL) {
            if (p.voisin->val->arrive != stk->val->arrive){
                Fx += (pow(stk->val->pheromone, IMPORTANCE_PHEROMONE)*pow(stk->val->cout, IMPORTANCE_VISIBILITEE))/cumul_proba;
                
                if (tirage <= Fx){
                    return p.voisin->val;
                }
            }
        p.voisin = p.voisin->suiv;
    }
    
    return NULL;
    
}



double mise_a_jour_des_pheromones(GRAPHE g, FOURMIS f){
    
    double delta_taux_i_j = 0;
    double poids_solution = 0;
    double phero = 0;
    
    phero = g[f.ville_courante].voisin->val->pheromone;
    File stk;
    
    for(stk = f.solution->suiv; stk != f.solution; stk = stk->suiv){
        delta_taux_i_j = QUANTITE_PHEROMONE_A_DEPOSEE/stk->val->cout;
    }
    phero = COEFFICIENT_EVAPORATION_PHEROMONE*phero + delta_taux_i_j;
    
    return phero;
}

File parcours(GRAPHE g, FOURMIS* f, unsigned int taille){
    
    int i;
    File stk = f->solution;
    
    for(i = 0; i < taille; i++) {
        stk = enfiler(choix_prochaine_ville(g, f, taille), stk);
        f->ville_courante = stk->val->arrive;
        stk->val->pheromone = mise_a_jour_des_pheromones(g, *f);
    }
    return stk;
}





