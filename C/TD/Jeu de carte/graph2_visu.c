#include "bataille.h"


	/* Affichage graphique d'une carte */
void affichegr(ELEMENT* e, SDL_Surface*  f1,int x, int y) {
  if (e->visible!=0) {
    if ((e->im).data!=NULL) afficheim32SDL(f1,(e->im).data,(e->im).lig,(e->im).col,x,y);
    else  if ((e->dos).data!=NULL) afficheim32SDL(f1,(e->dos).data,(e->im).lig,(e->im).col,x,y);
  }
  else {
    if ((e->dos).data!=NULL) afficheim32SDL(f1,(e->dos).data,(e->im).lig,(e->im).col,x,y);
  }
}

	/* Efface une carte */
void effacegr(ELEMENT* e, SDL_Surface*  f1,int x, int y) { SDL_Rect r;
  r.x = x; r.y = y; r.h = e->im.lig; r.w = e->im.col;
  SDL_FillRect(f1, &r, SDL_MapRGB(f1->format,255,255,255));
  SDL_Flip(f1);
}

	/* Affiche une pile */
void graphe_pile(Pile p, SDL_Surface*  f1, int x, int y) {
   int j=taille_pile(p)-1;
   if (!pile_vide(p)) { graphe_pile(p->suiv,f1,x,y); affichegr(&p->val,f1,x,y-DECALAGE*j); }
}	

	/* Efface une pile */
void effacegr_pile(Pile p, SDL_Surface*  f1, int x, int y) { Pile c; int i;
  for (i=0,c=p; !pile_vide(c); i++,c=c->suiv) effacegr(&c->val,f1,x,y-DECALAGE*i);
}

	/* Affiche une file implemente par une liste circulaire */
void graphe_file(File L, SDL_Surface*  f1, int x, int y) { File p;
	/*affichegr(&L->val,f1,x,y-DECALAGE*0);
	return;*/
  if (!file_vide(L)) { int i=0;
    p=L; 
	do { 
	p=p->suiv; 
	affiche(&p->val);
	printf("x : %d y : %d\n",x,y-DECALAGE*i);
	affichegr(&p->val,f1,x,y-DECALAGE*i);
	i++;
	} 
	while (p!=L);
  }
}
	
	/* Efface toute une file */
void effacegr_file(File L, SDL_Surface*  f1, int x, int y) { File p;
  if (!file_vide(L)) { int i=0;
    p=L; do { p=p->suiv; effacegr(&p->val,f1,x,y-DECALAGE*i++);} while (p!=L);
  }
}

void graphe_pile_d(Pile p, SDL_Surface*  f1, int x, int y, int decalage) {
   int j=taille_pile(p)-1;
   if (!pile_vide(p)) { graphe_pile_d(p->suiv,f1,x,y, decalage); affichegr(&p->val,f1,x,y-decalage*j); }
}

void effacegr_pile_d(Pile p, SDL_Surface*  f1, int x, int y, int decalage) { Pile c; int i;
  for (i=0,c=p; !pile_vide(c); i++,c=c->suiv) effacegr(&c->val,f1,x,y-decalage*i);
}

void graphe_file_d(File L, SDL_Surface*  f1, int x, int y, int decalage) { File p;
  if (!file_vide(L)) { int i=0;
    p=L; do { p=p->suiv; affichegr(&p->val,f1,x,y-decalage*i++);} while (p!=L);
  }
}

void effacegr_file_d(File L, SDL_Surface*  f1, int x, int y, int decalage) { File p;
  if (!file_vide(L)) { int i=0;
    p=L; do { p=p->suiv; effacegr(&p->val,f1,x,y-decalage*i++);} while (p!=L);
  }
}
