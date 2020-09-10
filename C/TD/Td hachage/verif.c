#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "liste.h"

#define A 31

typedef Liste* TABLE;


unsigned int hachage(unsigned char* mot, int dim_tab_hach) {
    
    int S = 0;
    int i = 0;
    unsigned int puiss = 1;
    int len = strlen(mot);

    for ( i = 0; i < len; i++, puiss = A*puiss) {
        S = (S + tolower(mot[i])*puiss)%dim_tab_hach;  // On ne veut pas repasser dans les négatifs
    }

    return S;
}

TABLE creer_tab_hachage(int dim_tab_hachage) {
    
    TABLE tab;
    int i = 0;

    tab = (TABLE)calloc(dim_tab_hachage, sizeof(*tab));

    if (tab!=NULL){
        for(i = 0; i<dim_tab_hachage; i++){
           tab[i] = creer_liste();
        }
    }
    return tab;
}

TABLE ajouter_tab(unsigned char* mot, TABLE tab, int dim_tab_hach) {
    int h = hachage(mot, dim_tab_hach);
	//printf("tab[%d] = %s %p\n",h,mot,tab[h]); 
    tab[h] = ajout_tete(mot, tab[h]);
    return tab;
}

TABLE tab_hachage(char* fichier, int dim_tab_hach){
    FILE* f;
    char s[255];

    f = fopen(fichier, "r");
    if (f == NULL){
        printf("fichier inexistant");
        return NULL;
    }

    TABLE t_hachage = creer_tab_hachage(dim_tab_hach);
    if (t_hachage == NULL) {
        return NULL;
    }

    while(fgets(s, 255, f)!=NULL){
		s[strlen(s)-1] = 0;
        t_hachage = ajouter_tab(s, t_hachage, dim_tab_hach);		
    }

    fclose(f);
    return t_hachage;
}

void affiche_tab_hachage(TABLE tab, int dim_tab_hach){
    int i = 0;
    for (i = 0; i<dim_tab_hach; i++) {
        if(!est_vide(tab[i])){
            visualiser_liste(tab[i]);
        }
    }
}

int main() {

	FILE* f;
	char s[512];
	int i_h =0;

	f = fopen("longtemps.utf8.txt", "r");

	if (f == NULL) {
		printf("fichier non existant");
		return -1;
	}	
	
	int dim_tab_hach = 0;

    printf("Entrer une grandeur de table de hachage: ");
    scanf("%d", &dim_tab_hach);

   TABLE h = tab_hachage("dico6.utf8.txt", dim_tab_hach);
	//visualiser_liste(h[20]);
    //affiche_tab_hachage(h, dim_tab_hach);
	
	i_h = hachage("jour", dim_tab_hach);//calcul de la clé hachage
	//printf("hash(jour)=%d\n", i_h);
	//visualiser_liste(h[i_h]);
	//printf ("jour = %p \n",rech("jour", h[i_h]));

	while(lecture(f, s) != 0) {
		i_h = hachage(s, dim_tab_hach);//calcul de la clé hachage
		if (rech(s, h[i_h]) == NULL){
			printf("le mot %s est absent du dictionnaire\n", s);// on va a tab[h]et on compare pour voir s'il existe
		}
	}
}
