#ifndef _SON
#define _SON
/* 
Ce fichier definit les prototypes des fonctions de manipulation des sons
*/
void libere_son(void*);
unsigned char * alloue_son(int nb_samples);
unsigned char* cree_sin(unsigned char amplitude, int taille, int freqsin, int freqson);
unsigned char* inverse(unsigned char* son, int taille);
unsigned char* sous_echantillonnage_brutal(unsigned char* son, int taille, int T);
unsigned char* sur_echantillonnage_brutal(unsigned char* son, int taille, int T);
void mixe_meme_frequence(unsigned char **son, int *taille, unsigned char* son1, int taille1, unsigned char* son2, int taille2);
void mixe(unsigned char **son, int *taille, int *frequence, unsigned char* son1, int taille1, int frequence1, unsigned char* son2, int taille2, int frequence2);
unsigned char mixe_echantillon(unsigned char ech1,unsigned char ech2, unsigned char maxe1, unsigned char maxe2);
#endif


