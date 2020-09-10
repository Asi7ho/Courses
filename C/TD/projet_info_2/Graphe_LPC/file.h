///
//  file.h
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#ifndef _FILE
#define _FILE
#include <stdio.h>
#include "structure.h"
#include "liste.h"

/*
 Créer une file vide.
 */
File creer_file(void);

/*
 Teste si la file est vide.
 */
int file_vide(File f);

/*
 Visualise les différentes informations contenues dans la file. 
 Ici permet d'afficher le parcours de la fourmi.
 */
void visualiser_file(File f);

/*
 Ajoute un pointeur d'arc à la file par la queue.
 On ajoute le pointeur de d'arc de la prochaine ville choisie pour completer la solution de la fourmi.
 */
File enfiler(POINTEUR_ARC e, File f);

#endif /* _FILE */
