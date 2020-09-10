#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL_phelma.h>


/* 
	Cette fonction permet de gerer le clavier et la souris 
*/
int gestionclavier(int* px, int* py) { SDL_Event event;
	/* On attend un evenement, qui peut etre tapez sur le clavier, bouger la souris, etc... 
           Si on ne fait rien, le programme reste bloque ici.
	*/
   SDL_WaitEvent(&event); 
	/* On recupere cet evenement et on le traite en fonction de sa nature.
	*/
   switch(event.type) {
	/* Si on decide de quitter la fenetre  en cliquant sur fermeture
	notre fonction retourne 0 
	*/
      case SDL_QUIT: return 0;
	/* Ca, ce sont les evenements de type : on appui sur une touche */
	/* On retourne 1 a 5 selon la touche tapee (fleche ou ESC), ou -1 si c'est une autre touche */
      case SDL_KEYDOWN :
        switch(event.key.keysym.sym) {
          case SDLK_UP : return 1;
          case SDLK_DOWN : return 2;
          case SDLK_LEFT: return 3;
          case SDLK_RIGHT : return 4;
          case SDLK_ESCAPE : return 5;
        };
        return -1;

	/* La, on bouge la souris  : on met les nouvelles coordonnees dans *px et *py
	et on retourne 6
	*/
      case SDL_MOUSEMOTION: *px=event.motion.x; *py=event.motion.y; 
         return 6;

	/* La, on clic sur la souris */
	/* on met les nouvelles coordonnees dans *px et *py
	et on retourne 7 a 9 selon le bouton clique */
      case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
           case SDL_BUTTON_LEFT : *px=event.button.x; *py=event.button.y; return 7;  
           case SDL_BUTTON_MIDDLE : *px=event.button.x; *py=event.button.y; return 8;
           case SDL_BUTTON_RIGHT : *px=event.button.x; *py=event.button.y; return 9;
        }
   }
	// Sinon c'est un autre evenement, on retourne -2
   return -2;
}


int main(int argc, char *argv[])
{   SDL_Surface* fenetre;
    int rep,x,y,dimx=300, dimy=200;


        /* Creation d'une fenetre de dimension dimx x dimy, couleurs sur 32 bits */
    fenetre = newfenetregraphique(dimx, dimy);

	/* La boule de traitement : 
 		on affiche le type de l'evement et des infos sur cet evenement
	*/
    do {
	rep=gestionclavier(&x,&y);	
		/* Il y a eu un evenement graphique */
	switch(rep) {
		case 0 : printf("On doit quitter\n"); break;
		case 1: printf("On a appuye sur la touche Fleche Haut\n"); break;
		case 2: printf("On a appuye sur la touche Fleche Bas\n"); break;
		case 3: printf("On a appuye sur la touche Fleche Gauche\n"); break;
		case 4: printf("On a appuye sur la touche Fleche Droite\n"); break;
		case 5: printf("On a appuye sur la touche Escape et on quitte aussi\n"); break;
		case 6: printf("On a bouge la souris en coordonnees x=%d y=%d\n",x,y); break;
		case 7: printf("On a le bouton gauche, la souris est en coordonnees x=%d y=%d\n",x,y); break;
		case 8: printf("On a le bouton milieu, la souris est en coordonnees x=%d y=%d\n",x,y); break;
		case 9: printf("On a le bouton droit, la souris est en coordonnees x=%d y=%d\n",x,y); break;
	}
    } while(rep!=0 && rep!=5);
    SDL_Quit();

    return EXIT_SUCCESS;
}
