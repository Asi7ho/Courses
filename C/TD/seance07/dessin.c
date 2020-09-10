#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <SDL.h>
#include <SDL_draw.h>
#include <SDL_image.h>
#include <SDL_phelma.h>




void sierpinski(float x1, float y1, int n, int l, SDL_Surface* f ) {

	float x2, y2;
	float x3, y3;
	float x4, y4;
	float x5, y5;
	float x6, y6;
	float x7, y7;
	float x8, y8;
	float x9, y9;
	int i = 1;
	

	if (n==1) {
		//Draw_Line(f,x1, y1, x4, y4, 0); 
		Draw_FillRect(f, x1, y1,  l, l, 0);
		SDL_Flip(f);
		
	} else {
		/*x2 = x1 + (x4 - x1) /4. + 1.75 * (y4 - y1) / 4.;
		y2 = y1 + (y4 - y1) /4. - 1.75 * (x4 - x1) / 4.;
		x3 = x4 - (x4 - x1) /4. + 1.75 * (y4 - y1) / 4.;
		y3 = y4 - (y4 - y1) /4. - 1.75 * (x4 - x1) / 4.;

		sierpinski(x2, y2, x1, y1, n-1, f);
		sierpinski(x2, y2, x3, y3, n-1, f);
		sierpinski(x4, y4, x3, y3, n-1, f);

		//printf("x2 = %d, y2 = %d \n x3 = %d, y3 = %d\n\n", x2, y2, x3, y3);*/
		
		x2 = x1; y2 = y1 + 100;
		x3 = x1 + 100; y3 = y1 + 100;
		x4 = x1 + 100; y4 = y1;
		x5 = x1 + 100; y5 = y1 - 100;
		x6 = x1; y6 = y1 - 100;
		x7 = x1 - 100; y7 = y1 - 100;
		x8 = x1 - 100; y8 = y1;
		x9 = x1 - 100; y9 = y1 + 100; 
		

		sierpinski(x1,y1,n-1,l,f);
		sierpinski(x2,y2,n-1,l/(i+1),f);
		sierpinski(x3,y3,n-1,l/(i+1),f);
		sierpinski(x4,y4,n-1,l/(i+1),f);
		sierpinski(x5,y5,n-1,l/(i+1),f);
		sierpinski(x6,y6,n-1,l/(i+1),f);
		sierpinski(x7,y7,n-1,l/(i+1),f);
		sierpinski(x8,y8,n-1,l/(i+1),f);
		sierpinski(x9,y9,n-1,l/(i+1),f);

		

		
	}
}



int main() {
	SDL_Surface* f;
	int n;
        
	/* Initialisation de la bibliotheque video  */
        /* Creation d'une fenetre de dimension dimx x dimy, couleurs sur 32 bits */
  	
	f=newfenetregraphique(800,600);
  	
	if ( f== NULL) { 
		printf("Impossible d'ouvrir la fenetre d'affichage"); 
	exit(1);
	} 
		printf("Entrer un entier n: ");
		scanf("%d", &n);
		sierpinski(300,300,n,50,f);
		getchar(); getchar();

	
}


