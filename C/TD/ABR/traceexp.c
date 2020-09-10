#include "arbre.h"
#include "traceexp.h"

#define MIN(x,y) ( (x)>(y)?(y):(x))
#define MAX(x,y) ( (x)<(y)?(y):(x))
#define MAXFLOAT ((float)3.40282346638528860e+38) 

SDL_Surface* newfenetregraphique(int dimx, int dimy) {
  SDL_Surface* f=NULL;
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
  signal(SIGINT, SIG_DFL);
  f = SDL_SetVideoMode(dimx, dimy, 32, SDL_HWSURFACE);
  if (f!=NULL) SDL_FillRect(f, NULL, SDL_MapRGB(f->format,255,255,255));
  return f;
}

void CourbeminmaxCouleur(SDL_Surface* f1,double* tab, int nbpoints, double min, double max, int coul) { 
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

void chercheminmax(ARBRE r, int n, double xmin, double xmax, double* min, double* max) {
  int i ;               /* Variable de boucle */
  double x;

  for (*max = *min = eval(r,xmin),i=1 ; i<n ; i++) { x=eval(r,xmin+i*(xmax-xmin)/n);
    if (x< *min) *min=x; 
    else if (x> *max) *max=x;
  }
}

void plot(SDL_Surface* f1, ARBRE r, int NBP, double xmin, double xmax, double ymin, double ymax, int coul) {
  double *t;
  int i;
  if ( (t=calloc(NBP,sizeof(*t)))==NULL) return;
  for (i=0; i<NBP; i++) t[i]=eval(r,xmin+i*(xmax-xmin)/NBP);
  CourbeminmaxCouleur(f1,t,NBP,ymin,ymax,coul);
  free(t);
}

/*
	Trace une courbe en rouge sur nb points entre les valeurs xmin et xmax
*/
void trace1courbe(SDL_Surface* f1, ARBRE r, int nb, double xmin, double xmax)  {
  double min, max;
  int dx=f1->w;
  int dy=f1-> h ;

  chercheminmax(r,nb,xmin,xmax,&min,&max);
  if (max>=MAXFLOAT || min <=-MAXFLOAT) { puts("Hors du domaine de definition"); exit(1); }
  if (min<0 && max>0) Draw_Line(f1,0,dy-(int)(dy*min/(min-max)),dx-1,dy-(int)(dy*min/(min-max)),0x0);
  if (xmin<0 && xmax>0) Draw_Line(f1,(int)(dx*xmin/(xmin-xmax)),0,(int)(dx*xmin/(xmin-xmax)),dy-1,0x0);
  plot(f1,r,dx,xmin,xmax,min,max,0x0000FF00); 
}

/*
        Trace 2  courbeq en rouge et vert  sur nb points entre les valeurs xmin et xmax
*/

void trace2courbe(SDL_Surface* f1, ARBRE r, ARBRE d, int nb, double xmin, double xmax)  {
  double fmin, fmax, dmin, dmax, min, max;
  int dx=f1->w;
  int dy=f1-> h ;

  /*
	Recherche du min et du max des ordonnees du graphe de la fonction et de sa derive
  */
  chercheminmax(r,nb,xmin,xmax,&fmin,&fmax);
  chercheminmax(d,nb,xmin,xmax,&dmin,&dmax);
  min=MIN(dmin,fmin); max=MAX(dmax,fmax);
  if (max>=MAXFLOAT || min <=-MAXFLOAT) { puts("Hors du domaine de definition"); exit(1); }
 
  /*
	Trace des axes des abcisses et des ordonnees
  */
  if (min<0 && max>0) Draw_Line(f1,0,dy-(int)(dy*min/(min-max)),dx-1,dy-(int)(dy*min/(min-max)),0x0);
  if (xmin<0 && xmax>0) Draw_Line(f1,(int)(dx*xmin/(xmin-xmax)),0,(int)(dx*xmin/(xmin-xmax)),dy-1,0x0);
/*
  if (min<0 && max>0) Ligne(f1,0,dy-(int)(dy*min/(min-max)),dx-1,dy-(int)(dy*min/(min-max)));
  if (xmin<0 && xmax>0) Ligne(f1,(int)(dx*xmin/(xmin-xmax)),0,(int)(dx*xmin/(xmin-xmax)),dy-1);
*/

  /* 
	Trace de la fonction et de sa derive. 
  */
  plot(f1,r,dx,xmin,xmax,min,max,0x00FF0000); 
  plot(f1,d,dx,xmin,xmax,min,max,0x0000FF00); 
}
