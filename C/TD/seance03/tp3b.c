#include <stdio.h>
#include <SDL_phelma.h>
#include <math.h>
#define N 300

void init(double tab[], int n, double a, double b) {
	double x = 0;
	double dx;
	int i = 0;
	dx = 1.0/(n-1);
	while (x <= a) {
		tab[i] = (b/a)*x;
		i += 1;
		x += dx;
	}
	while (x > a && x <= 1) {
		tab[i] = (b/(a-1))*(x-1);
		i += 1;
		x += dx;
	}
}

int calcul(double cordet[], double cordet1[], double cordet2[], int n, double dt) {
	double x = 0;
	double dx;
	double r;
	double v;
	int i = 1;
	int j;
	cordet[0] = 0;
	cordet[n-1]= 0;
	dx = 1.0/(n-1);
	r = 300*dt/dx;
	if (dx >= v*dt) {
		while (i < n-1) {
		cordet[i] = r*r*(cordet1[i+1]+cordet1[i-1]) + 2*(1-r*r)*cordet1[i]-cordet2[i];
		i += 1;
		}
		for (i = 0; i < n-1; i++) {
			cordet2[i] = cordet1[i];
			cordet1[i] = cordet[i];
		}
	return 1;
	}
}


/* Fonction qui affiche dans la fenetre une courbe dont les valeurs sont contenues dans tab. L'echelle des ordonnées est comprise entre min et max. La couleur sera 0 pour le noir.
*/
void CourbeminmaxCouleur_double(SDL_Surface* f1,double* tab, int nbpoints, double min, double max, int coul) {
  int i, i1,val1,i2,val2;
  if (nbpoints<=0) return;
 
  i1 = 0;
  val1 = f1->h -(tab[0]-min)*f1->h/(max-min);
  for (i=1; i<nbpoints; i++) {
    val2 = f1->h-(tab[i]-min)*f1->h/(max-min);
    i2 = floor(((i)*(double)f1->w)/ (double)(nbpoints-1));
    Draw_Line(f1,i1,val1,i2,val2,coul);
    i1=i2; val1=val2;
  }
  SDL_Flip(f1);
}
 
/* Fonction qui efface la courbe dans la fenetre */
void Clear(SDL_Surface* f) {
  SDL_FillRect(f, NULL, SDL_MapRGB(f->format,255,255,255));
  SDL_Flip(f);
}


void affiche(double tab[], int n) {
	SDL_Surface* f1;
	Clear(f1);
 	if (f1==NULL) { 
		puts("impossible d'ouvrir une fenetre graphique"); 
	} else {
    		CourbeminmaxCouleur_double(f1, tab, N, -0.1, 0.1,0x00000000);
	}
}

 

int main() {

	double cordet[300];
	double cordet1[300];
	double cordet2[300];
	int n, i, dimx=N*2, dimy=200 ;
	int t = 0;
	double dt = 10e-6;
  	SDL_Surface*  f1;
  	f1=newfenetregraphique(dimx,dimy);

	init(cordet1, 300, 0.15, 0.01);
	init(cordet2, 300, 0.15, 0.01);

	while (calcul(cordet, cordet1, cordet2, 300, 10e-6) == 1 && t <= 1) {
		affiche(cordet, 300);
		t += dt;
	}	
	
}


	
