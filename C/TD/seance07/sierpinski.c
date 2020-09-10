#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <SDL_phelma.h>


void sierpinski(int x1, int y1, int x4, int y4, int n, SDL_Surface* f ) {

	int x2, y2;
	int x3, y3;

	if (n==0) {
		Draw_Line(f,x1, y1, x4, y4, 0); 
		SDL_Flip(f);
		
	} else {
		x2 = x1 + (x4 - x1) /4. + 1.75 * (y4 - y1) / 4.;
		y2 = y1 + (y4 - y1) /4. + 1.75 * (x4 - x1) / 4.;
		x3 = x4 - (x4 - x1) /4. + 1.75 * (y4 - y1) / 4.;
		y3 = y4 - (y4 - y1) /4. + 1.75 * (x4 - x1) / 4.;

		sierpinski(x2, y2, x1, y1, n-1,f);
		sierpinski(x2, y2, x3, y3, n-1, f);
		sierpinski(x4, y4, x3, y3, n-1, f);

		printf("x2 = %d, y2 = %d \n x3 = %d, y3 = %d\n", x2, y2, x3, y3);
	}
}



int main(int ar, char **av) {
	SDL_Surface* f;
  	int couleur;
        /* Initialisation de la bibliotheque video  */
        /* Creation d'une fenetre de dimension dimx x dimy, couleurs sur 32 bits */
  	
	f=newfenetregraphique(300,100);
  	
	if ( f== NULL) { 
		printf("Impossible d'ouvrir la fenetre d'affichage"); 
	exit(1);
	} else { 
		printf("Entrer un entier n: ");
		scanf("%d", &n);
  		couleur= 0; //0xff0000FF; 
		sierpinski(0,0,2,2,n);
		getchar();getchar();

}
