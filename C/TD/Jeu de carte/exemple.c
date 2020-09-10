#include <SDL.h>
#include <SDL_phelma.h>
#define DECALAGE (20)
#include "file.h"
#include "pile.h"
#include "carte.h"

#define  DIMX 400
#define  DIMY 550
int  main(int argc, char** argv) {  int i;
        // le talon est une file de cartes 
  File talon=creer_file();
        // p est une pile de cartes
  Pile p=creer_pile();
        // Une carte 
  CARTE c;
        // La fenetre graphique
  SDL_Surface* f1=NULL;

        /* On crée et mélange toutes les cartes (ici 5*4), avec leur representation sous forme d'image dans la file jeu*/
  i=melanger(&talon,1,5);
  if (i==0) { puts("Il manque les fichiers graphiques"); exit(1); }

        /* On cree une fenetre graphique */
  f1=newfenetregraphique(DIMX,DIMY);

        /* On affiche le talon complet en x=125, y=10 : Les cartes sont affichees avec un decalage nul, donc sans decalage vertical */
  graphe_file_d(talon,f1,50,10,0);

        /* On cree une pile avec les premieres cartes du talon */
  for (i=0; i<10; i++) {
        c=defiler(&talon);
        c.visible=1 ;
        p=empiler(c,p);
   }
        /* On affiche la pile en x=450, y=100; Les cartes sont affichees avec un decalage verticale de -20 pixels */

	printf( "TAILLLE PILE: %d\n", taille_pile( p ) );

  graphe_pile_d(p,f1,200,100,-20);


        /* On affiche une carte isolee prise sur le talon en 50,300 */
   c=defiler(&talon);
   c.visible=1 ;
   affichegr(&c,f1,50,300);

  getchar();
}

