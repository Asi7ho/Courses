//
//  fichiers.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include "fichiers.h"



GRAPHE chargeFichierGraphe(char* nomDuFichier,unsigned int *pTailleGraphe){
    
    FILE* f = NULL;
    
    SOMMET S;
    ARC A;
    GRAPHE graphe;
    
    char ligne[512];
    
    unsigned int nbrDeSommets;
    unsigned int nbrDArretes;
    unsigned int numeroDuSommet;
    unsigned int numeroDuSommetLu;
    unsigned int numeroDeLArrete;
    double x;
    double y;
    
    f = fopen(nomDuFichier,"r");
    
    if (f==NULL) {
        
        printf("Impossible d'ouvrir le fichier, verifiez que le fichier existe");
        return NULL;
        
    }
    
    fscanf(f,"%d %d ^\n", &nbrDeSommets, &nbrDArretes);
    fgets(ligne,512,f);

    graphe=creer_graphe(nbrDeSommets);
    *pTailleGraphe = nbrDeSommets;

    for(numeroDuSommet=0 ; numeroDuSommet < nbrDeSommets; numeroDuSommet++ ){

        fscanf(f,"%d %lf %lf %s ^\n",&(S.numero),&(S.x),&(S.y),S.nom);
        S.voisin= creer_liste();
        
        if (numeroDuSommet!= S.numero) {
            
            printf("Erreur lors du chargement du fichier, le chargement du fichier s'est arrété à la ligne correspondant au sommet %d, le fichier %s semble corrompu\n", numeroDuSommet, nomDuFichier);
            fclose(f);
            return NULL;
            
        }
        
        graphe[numeroDuSommet]=S;
        
    }
    
    //printf("%d sommets du graph chargés avec succes\n", nbrDeSommets);
    
    fgets(ligne, 512, f);
    
    for (numeroDeLArrete = 0; numeroDeLArrete < nbrDArretes; numeroDeLArrete++){
        fscanf(f,"%d %d %lf",&(A.depart),&(A.arrive),&(A.cout));
        A.pheromone=VALEUR_INITIALE_PHEROMONE;
        
        graphe=ajout_arc(graphe,A);
        
    }
    
    //printf("%d arcs chargés avec succes\n", nbrDArretes);
    fclose(f);
    
    return graphe;
}
