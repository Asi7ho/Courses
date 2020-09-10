#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL_phelma.h>


int malectureimage(char* fic, unsigned char * im, int* pnl,int *pnc);

main(int ac, char**av) { int nl,nc,i,res;
	/* Variables contenant les images */
  unsigned char im[800*600], im2[800*600];
	/* Variable representant la fenetre d'affichage*/
  SDL_Surface* fenetre=NULL;

	/* 
	  On verifie que le programme est lance  avec un nom de fichier 
	*/
  if (ac !=2  || av[1]==NULL ){
      printf("Usage : %s fichier_image_entree \n",av[0]); exit(1);
  }

	/* Lecture de l'image, des nombres de lignes et colonnes */
  res=malectureimage(av[1],im,&nl,&nc);
printf("%d %d \n", nl, nc);
  if (res) { printf("Lecture image %s Impossible\n",av[1]); exit(1); }

	/* Creation d'une fenetre de nc colonnes, 2*nl lignes */
  fenetre=newfenetregraphique(nc,nl);
  if (fenetre==NULL) { printf("Creation fenetre Impossible\n"); exit(1); }
	/* Affichage dans la fenetre de l'image im, de dimensions rÃ©elles nl et nc
	 Le coin superieur gauche de l'image est affiche la position 
  	 x=0,y=0 de la fenetre
	*/
  affiche_image_ligne(fenetre, im,nl,nc,0,0);
  puts("Tapez CR pour continuer"); getchar();

	/* Calcul de l'inverse video : le noir devient blanc, le blanc devient noir */
  for (i=0; i<nl*nc; i++)
      im2[i]=255-im[i];


	/* Affichage dans la fenetre de l'image im2, de dimensions rÃ©elles nl et nc
	 Le coin superieur gauche de l'image est affichee la position 
  	 x=0,y=nl de la fenetre
	*/
  affiche_image_ligne(fenetre, im2,nl,nc,0,nl);
  puts("Tapez CR pour continuer"); getchar();
}
