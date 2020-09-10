#ifndef _GRAPH
#define _GRAPH

void affichegr(ELEMENT* e, SDL_Surface*  f1,int x, int y) ;
void effacegr(ELEMENT* e, SDL_Surface*  f1,int x, int y) ;
void graphe_pile(Pile p, SDL_Surface*  f1, int x, int y) ;
void effacegr_pile(Pile p, SDL_Surface*  f1, int x, int y);
void graphe_file(File L, SDL_Surface*  f1, int x, int y) ;
void effacegr_file(File L, SDL_Surface*  f1, int x, int y);
void graphe_pile_d(Pile p, SDL_Surface*  f1, int x, int y, int decalage);
void effacegr_pile_d(Pile p, SDL_Surface*  f1, int x, int y, int decalage) ;
void graphe_file_d(File L, SDL_Surface*  f1, int x, int y, int decalage);
void effacegr_file_d(File L, SDL_Surface*  f1, int x, int y, int decalage);
#endif

#include "bataille.h"
