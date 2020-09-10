#ifndef _CARTE
#define _CARTE

//#define MODEGRAPHIQUE 

enum { TREFLE,CARREAU,COEUR,PIQUE} COULEUR;

typedef struct { 
  unsigned int** data; 
  int lig, col; 
} ICONE;

typedef struct { 
  int rang; 
  char visible; 
  char couleur; 
#ifdef MODEGRAPHIQUE
  ICONE im,dos;
#endif
} ELEMENT;

typedef ELEMENT CARTE;
#endif
