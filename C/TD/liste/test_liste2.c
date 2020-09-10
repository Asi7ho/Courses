#include <stdio.h>
#include "liste.h"

main() { CARTE c; Liste l1,l2,l3,l4,l5; 
  char rep; int n;
  l1=creer_liste(); l2=creer_liste();
  l3=creer_liste(); l4=creer_liste();
  l5=creer_liste();

  printf("Test des concaténations\n");

  c.rang=11; c.couleur= COEUR; l1=ajout_tete(c,l1);
  c.rang=8; c.couleur= COEUR; l1=ajout_tete(c,l1);
  c.rang=7; c.couleur= CARREAU; l1=ajout_tete(c,l1);
  c.rang=12; c.couleur= PIQUE; l1=ajout_tete(c,l1);
  c.rang=9; c.couleur= PIQUE; l1=ajout_tete(c,l1);
  printf("Liste l1: "); visualiser_liste(l1); 

  c.rang=10; c.couleur= CARREAU; l2=ajout_tete(c,l2);
  c.rang=1; c.couleur= TREFLE; l2=ajout_tete(c,l2);
  c.rang=3; c.couleur= CARREAU; l2=ajout_tete(c,l2);
  c.rang=5; c.couleur= PIQUE; l2=ajout_tete(c,l2);
  printf("Liste l2: "); visualiser_liste(l2);

  l4=copie(l1); 
  printf("Copie de l1 dans l4\n");
  printf("Liste l1: "); visualiser_liste(l1); 
  printf("Liste l4: "); visualiser_liste(l4);

  l5=l1; 
  printf("Affectation de la liste l1 dnas l5\n");
  printf("Liste l5: "); visualiser_liste(l5);
  printf("Liste l1: "); visualiser_liste(l1);

  printf("Concatenation de l2 a la suite de l1 dans l3 \n");
  l3=concat(l1,l2);
  printf("Liste l1: "); visualiser_liste(l1);
  printf("Liste l2: "); visualiser_liste(l2);
  printf("Liste l3: "); visualiser_liste(l3);
  printf("Liste l4: "); visualiser_liste(l4);
  printf("Liste l5: "); visualiser_liste(l5);
}
