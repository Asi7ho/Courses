//
//  fichiers.h
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef fichiers_h
#define fichiers_h

#include <stdio.h>
#include "graphe.h"
#include "structure.h"
#include "fourmis.h"
#include  <stdlib.h>

/*
 Charge le fichier graphe dans le répertoire source pour pouvoir extraire les différentes données (comme le nombre de sommet, le cout d'un arc et les voisins) et ainsi creer le graphe.
 */
GRAPHE chargeFichierGraphe(char* nomDuFichier,unsigned int *pTailleGraphe);


#endif /* fichiers_h */
