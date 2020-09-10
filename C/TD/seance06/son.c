#include <stdio.h>
#include "son.h"
#include <stdlib.h>

unsigned char* alloue_son(int nb_samples) {

	unsigned char* t = NULL;
	t = calloc(nb_samples, sizeof(*t));

	return t;
}

void libere_son(void* son) {
	free(son);
}

unsigned char max (unsigned char* son, int taille) {
	int i = 0;
	unsigned char Max = son[0];

	for(i = 1; i < taille; i++) {
		if (son[i] > Max) {
			Max = son[i];
		}
	}
	return Max;
}

unsigned char* inverse (unsigned char* son, int taille) {
	unsigned char* tinverse = NULL;
	int i = 0;

	tinverse = calloc(taille, sizeof(*tinverse));

	for (i = 0; i < taille; i++) {
		tinverse[i] = son[taille-1-i];
	}

	return tinverse;
}

unsigned char* sous_echantillonnage_brutal(unsigned char* son, int taille, int T) {
	unsigned char* techantillon = NULL;
	int i = 0;

	techantillon = calloc(taille/T, sizeof(*techantillon));

	for (i = 0; i < taille/T; i++) {
		techantillon[i] = son[i*T];
	}

	return techantillon;
}

void mixe_meme_frequence(unsigned char** pson, int *taille, unsigned char* son1, int taille1, unsigned char* son2, int taille2) {

	int i = 0;
	unsigned char M;
	float m, m1, m2;
	
m1 = (float)max(son1, taille1);
m2 = (float)max(son2, taille2);

	
	if (taille1 < taille2) {
		*taille = taille1;
	} else {
		*taille = taille2;
	}
	
	*pson = alloue_son(*taille);

	if (max(son1, taille1) > max(son2, taille2)) {
		m = m1;
	} else {
		m = m2;
	} M = (unsigned char) m;
//m = m/(m1+m2);
m = (m1+m2);
	for (i = 0; i < *taille; i++) {
		(*pson)[i] = (unsigned char)( 
                          ( (float)son1[i] + (float)son2[i] )
                                * (float)M/m);
                    // *(float) M/( (float)max(son1,taille1)+(float) (max(son2, taille2)) )   ) ;
	}
}
