#include "arbre.h"
#include "traceexp.h"

#define MAXFLOAT ((float)3.40282346638528860e+38) 

int main(int argc, char** argv) { char s[512];
  double xmin=-5, xmax=5;
  ARBRE r=NULL;
  ARBRE c=NULL;
  int dx=300,dy=200;
  SDL_Surface* f1;

  f1=newfenetregraphique(dx,dy);
  puts("Entrer une expression en notation prefixee"); gets(s);
  r=lire(s);
  puts("Expression :"); affiche(r); puts("");
  /* 
      Recopie de r dans c, et liberation de la memoire pour r 
      r n'est plus utilisable car n'existe plus
      On trace la copie de r
  */
  c = copie(r);
  r = libere(r);
  puts("L'expression r initiale (doit etre vide)");  affiche(r); puts("");
  puts("L'expression c copiee (doit etre la meme que r au debut)"); affiche(c); puts("");

  puts("Entrer les bornes du trace (xmin et xmax): attention au domaine de definition");
  scanf("%lf %lf",&xmin,&xmax);

  trace1courbe(f1,c,dx,xmin,xmax);

  getchar(); getchar();
  return 0;
}
