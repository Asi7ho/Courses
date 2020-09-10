#include "arbre.h"
#include "traceexp.h"

#define MAXFLOAT ((float)3.40282346638528860e+38) 

int main(int argc, char** argv) { char s[512];
  double xmin=-5, xmax=5;
  ARBRE r=NULL;
  int dx=400,dy=200;
  SDL_Surface* f1;

	/* 
	Creation d'une fenetre graphique
	*/
  f1=newfenetregraphique(dx,dy);
  Draw_Line(f1,0, 10, 200, 50,0xFFFFFFFF);
  	/*
	Lecture d'une expression prefixe au clavier 
 	 */
  puts("Entrer une expression en notation prefixee"); gets(s);
  r=lire(s);

 	 /*
	Affichage de cette expression en notation classique
 	 */
  puts("Expression :"); affiche(r); puts("");

	
  puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
  scanf("%lf %lf",&xmin,&xmax);

  /*
	Recherche du min et du max des ordonnees du graphe de la fonction 
  */
  trace1courbe(f1,r,dx,xmin,xmax);
  getchar(); getchar();

  /*
	Liberation de la memoire de l'arbre
  */
  r=libere(r);
  return 0;
}
