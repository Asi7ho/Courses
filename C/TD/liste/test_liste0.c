#include <stdio.h>
#include "liste.h"

main() { CARTE c; Liste l1,l2,l3,l4,l5;
    l1=creer_liste(); l2=creer_liste();
    printf("test de l'ajout en tete d'une carte et de l'affichage\n");
    printf("Ajout en tete de l'as de coeur\n");
    c.rang=1;    c.couleur=COEUR;
    l1=ajout_tete(c,l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Ajout en tete du roi de coeur\n");
    c.rang=13;    c.couleur=COEUR;
    l1=ajout_tete(c,l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Ajout en tete de l'as de pique\n");
    c.rang=1;    c.couleur=PIQUE;
    l1=ajout_tete(c,l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Ajout en tete du roi de pique\n");
    c.rang=13;    c.couleur=PIQUE;
    l1=ajout_tete(c,l1);
    printf("Liste : ");    visualiser_liste(l1);
    
    l2=creer_liste();
    printf("test de l'ajout en queue d'une carte et de l'affichage\n");
    printf("Ajout en queue de l'as de coeur\n");
    c.rang=1;    c.couleur=COEUR;
    l2=ajout_queue(c,l2);
    printf("Liste : ");    visualiser_liste(l2);
    printf("Ajout en queue du roi de coeur\n");
    c.rang=13;    c.couleur=COEUR;
    l2=ajout_queue(c,l2);
    printf("Liste : ");    visualiser_liste(l2);
    printf("Ajout en queue de l'as de pique\n");
    c.rang=1;    c.couleur=PIQUE;
    l2=ajout_queue(c,l2);
    printf("Liste : ");    visualiser_liste(l2);
    printf("Ajout en queue du roi de pique\n");
    c.rang=13;    c.couleur=PIQUE;
    l2=ajout_queue(c,l2);
    printf("Liste : ");    visualiser_liste(l2);
    
 
    printf("test de la suppression en tete d'une carte et de l'affichage\n");
    printf("Liste : ");    visualiser_liste(l1);
    printf("Suppression en tete\n");
    l1=supprimer_tete(l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Suppression en tete\n");
    l1=supprimer_tete(l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Suppression en tete\n");
    l1=supprimer_tete(l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Suppression en tete\n");
    l1=supprimer_tete(l1);
    printf("Liste : ");    visualiser_liste(l1);
    printf("Suppression en tete\n");
    l1=supprimer_tete(l1);
    printf("Liste : ");    visualiser_liste(l1);

    printf("test de la suppression en tete d'une carte et de l'affichage\n");
    printf("Liste : ");    visualiser_liste(l2);
    printf("Suppression de la deuxieme carte\n");
    l2=supprimen(2,l2);
    visualiser_liste(l2);
    printf("Suppression de la quatrieme carte\n");
    l2=supprimen(4,l2);
    visualiser_liste(l2);
    printf("Suppression de la deuxieme carte\n");
    l2=supprimen(2,l2);
    visualiser_liste(l2);
    printf("Suppression de la deuxieme carte\n");
    l2=supprimen(2,l2);
    visualiser_liste(l2);
    printf("Suppression de la premiere carte\n");
    l2=supprimen(1,l2);
    visualiser_liste(l2);
 
}
