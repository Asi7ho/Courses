#include <SDL.h>
#include <SDL_phelma.h>
#include "bataille.h"

#define  DIMX 400
#define  DIMY 550

int  main(int argc, char** argv) {  int i;
    // le talon est une file de cartes
    File talon=creer_file();
    //paquet des joueurs
    File paquet1=creer_file();
    File paquet2=creer_file();
	// pile de bataille
	Pile bataille1 = creer_pile();
	Pile bataille2 = creer_pile();
    // Une carte
    CARTE c1;
	CARTE c2;
    // La fenetre graphique
    SDL_Surface* f1=NULL;
    
    /* On cree une fenetre graphique */
    f1=newfenetregraphique(DIMX,DIMY);
    
    /* On distribue les cartes aux joueurs */
    distribution(&paquet1, &paquet2, 1, 13); 
    graphe_file(paquet1,f1,200,100);
    graphe_file(paquet2,f1,500,100);
	//getchar();
/*return;*/
while(paquet1 != NULL || paquet2 != NULL) {
 
    /*switch (event.type){
            
            // Si on ferme la fenêtre on retourne 0
            case SDL_QUIT: return 0;
            
            case SDL_MOUSEMOTION:
                *px=event.motion.x; *py=event.motion.y;
            
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT :
                        *px=event.button.x;
                        *py=event.button.y;
                        
						/* On affiche une carte isolee prise sur le talon en 50,300 */
						
						if (file_vide(paquet1)) return -1;
                        c1=defiler(&paquet1);
                        c1.visible=1 ;
                        affichegr(&c1,f1,100,200);
						bataille1 = empiler(c1,bataille1);
                        
						if (file_vide(paquet2)) return -2;
                        c2=defiler(&paquet2);
                        c2.visible=1 ;
                        affichegr(&c2,f1,100,350);
						bataille2 = empiler(c2,bataille2);
						
                    
                         while (c1.rang = c2.rang) {
						
							if (file_vide(paquet1)) return -3;
                            c1 = defiler(&paquet1);
                            c1.visible = 0;
                            affichegr(&c1,f1,100,200);
							bataille1 = empiler(c1,bataille1);
                           	
							if (file_vide(paquet1)) return -4;
                            c2 = defiler(&paquet2);
                            c2.visible = 0;
                            affichegr(&c2,f1,100,350);
							bataille2 = empiler(c2,bataille2);
                            
                            c1 = defiler(&paquet1);
                            c1.visible = 1;
                            affichegr(&c1,f1,100,200);
							bataille1 = empiler(c1,bataille1);
                            
                            c2 = defiler(&paquet2);
                            c2.visible = 1;
                            affichegr(&c2,f1,100,350);
							bataille2 = empiler(c2,bataille2);
							
							graphe_pile(bataille1, f1, 100, 200);
							graphe_pile(bataille2, f1, 100, 350);
                        }
                        
                        if(c1.rang > c2.rang){
                           	c1 = depiler(&bataille1);
							c1.visible = 0;
							c2 = depiler(&bataille1);
							c2.visible = 0;

							paquet1 = enfiler(c1,paquet1);
							paquet2 = enfiler(c2,paquet1);

                        }
                        
                        if(c1.rang < c2.rang){
                            c1 = depiler(&bataille1);
							c1.visible = 0;
							c2 = depiler(&bataille1);
							c2.visible = 0;

							paquet1 = enfiler(c1,paquet2);
							paquet2 = enfiler(c2,paquet2);

                        }
                }
	printf("Perdu");
}
