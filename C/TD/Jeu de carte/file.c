//
//  file.c
//  td pile
//
//  Created by Yann Debain on 14/02/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#include "file.h"
#include <stdio.h>
#include <stdlib.h>


File creer_file(void){
    return creer_liste();
}

int file_vide(File f){
    return liste_vide(f);
}

void visualiser_file(File f){
    File stk;
    for(stk = f->suiv; stk != f; stk = stk->suiv){ //On s'arrete apres un tour
        affiche(&(stk->val));
        printf("\n");
    }
	affiche(&(f->val));
}

File enfiler(CARTE c, File f){
	File stk = calloc(1, sizeof(*stk));
    stk->val = c;
    if (!(file_vide(f))){
        stk->suiv = f->suiv;
        f->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}


//PRECONDITION : la fonction ne prend pas de liste vide
CARTE defiler(File *pf){
        
	File f = *pf;
    CARTE c;
    File stk;
        
    if (f->suiv == f) {   // Un seul element
        ELEMENT c = f->val;
        free(f);
        *pf = NULL;
         return c;
    }
        
    c = f->suiv->val;
    stk = f->suiv->suiv;
    free(f->suiv);
    f->suiv = stk;
    *pf = f;    
    return c;
        
}
