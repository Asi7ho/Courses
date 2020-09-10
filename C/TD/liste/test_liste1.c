#include <stdio.h>
#include "liste.h"

main() { CARTE c; Liste l1,l2,l3,l4,l5; 
  char rep; int n;
  l1=creer_liste(); l2=creer_liste();
  l3=creer_liste(); l4=creer_liste();
  l5=creer_liste();
  do {
    printf("quitter(0); Ajouter en tete(1); Ajouter en queu(2); Supprimer en tete(3); visualiser_liste(4); Supprimer(5)"); fflush(stdout);
    rep=getchar();
    switch(rep) {
      case '1': printf("Valeur(1..13) et couleur (0..3) \n"); scanf("%d %hhd",&(c.rang),&(c.couleur)); 
                if (c.rang>=1 && c.rang<=13 && c.couleur>=0 && c.couleur<=3) l1=ajout_tete(c,l1);
                break;
      case '2': printf("Valeur(1..13) et couleur (0..3) \n"); scanf("%d %hhd",&(c.rang),&(c.couleur));
                if (c.rang>=1 && c.rang<=13 && c.couleur>=0 && c.couleur<=3) l1=ajout_queue(c,l1);
                break;
      case '3': l1=supprimer_tete(l1); break;
      case '4': visualiser_liste(l1); break;
      case '5': printf("Quelle est la position a supprimer ? \n"); 
                scanf("%d",&n); 
                if (n>=0) 
                    l1=supprimen(n,l1);
                break;
    }
    getchar();
  } while (rep !='0');

}
