#include "arbre.h"
#include <signal.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_draw.h>


SDL_Surface* newfenetregraphique(int,int);
void trace1courbe(SDL_Surface*  f1, ARBRE r, int nb, double xmin, double xmax);
void trace2courbe(SDL_Surface*  f1, ARBRE r, ARBRE d, int nb, double xmin, double xmax);
void chercheminmax(ARBRE r, int n, double xmin, double xmax, double* min, double* max) ;
void plot(SDL_Surface* f1, ARBRE r, int NBP, double xmin, double xmax, double ymin, double ymax, int coul) ;


