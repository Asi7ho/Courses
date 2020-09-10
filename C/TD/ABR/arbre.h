#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL_phelma.h>

#ifndef _ARBRE
#define _ARBRE

double atof();

typedef enum { OPERATEUR_BINAIRE, OPERATEUR_UNAIRE, VALEUR, VARIABLE } TYPE;
typedef
  struct noeud {
    char * val;
    TYPE type;
    struct noeud* fg, *fd; } * ARBRE;

ARBRE creernoeud(char *c);
ARBRE construitarbre(char *c) ;
ARBRE lire(char *s) ;
void affiche(ARBRE r) ;
double eval(ARBRE r, double x) ;
ARBRE copie(ARBRE b);
ARBRE deriv(ARBRE r) ;
ARBRE libere(ARBRE r) ;
ARBRE simplifie(ARBRE r) ;
#endif
