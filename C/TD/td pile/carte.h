#ifndef _CARTE
#define _CARTE
enum { TREFLE,CARREAU,COEUR,PIQUE} COULEUR;

typedef struct {
    int rang;
    char visible;
    char couleur;
} CARTE;

void affiche (CARTE* e);
void affichevisible(CARTE* e);
void init_carte (CARTE* e);

#endif
