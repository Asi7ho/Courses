#include "liste.h"
#include <stdio.h>
#include <stdlib.h>

Liste creer_liste(void) {	return NULL;	}

int est_vide(Liste L) {	return !L;	}

Liste ajout_tete(ELEMENT e, Liste L)
{ Liste p=(Liste) calloc(1,sizeof(*p));
  if (p==NULL) return NULL; 
  p->val=strdup(e);
  p->suiv=L;
  return p;
}

Liste rech(ELEMENT e, Liste L)
{ Liste p=L;
  while(!est_vide(p)&&(strcasecmp(p->val,e)!=0)) p=p->suiv;
  return p;
}


void visualiser_liste(Liste l) {
    Liste p;
	puts("");
    for (p = l; p != NULL; p = p->suiv){
        affiche(&(p->val));
	printf("\n");
    }
	
}

void affiche (ELEMENT * pelt)
{
    printf ("%s ", *pelt);
}

