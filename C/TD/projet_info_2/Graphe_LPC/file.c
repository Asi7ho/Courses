///
//  file.c
//  Graphe LPC
//
//  Copyright © 2017 Debain-Milcent. All rights reserved.
//

#include "file.h"
#include <stdio.h>
#include <stdlib.h>


File creer_file(void){
    return creer_liste();
}

int file_vide(File f){
    return est_vide(f);
}

void visualiser_file(File f){
 File stk;
    
    if(f == NULL) {
        printf("FILE VIDE\n");
        return;
    }
    
    for(stk = f->suiv; stk != f; stk = stk->suiv){ //On s'arrete apres un tour
     printf("depart = %d arrivée = %d cout = %lf pheromone = %lf\n", stk->val->depart, stk->val->arrive, stk->val->cout, stk->val->pheromone);
 }
}


File enfiler(POINTEUR_ARC  e, File f){
    File stk = calloc(1, sizeof(*stk));
    if(stk == NULL) {
        printf ("aie\n");
        exit(1);
    }
    stk->val = e;
    if (! file_vide(f) ){
        stk->suiv = f->suiv;
        f->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}

