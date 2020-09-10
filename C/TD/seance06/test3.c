#include <math.h>
#include "son.h"
#include <SDL_phelma.h>


int main(int argc, char * argv[]){
 	unsigned char *son=NULL;
	unsigned char *son1=NULL;
	unsigned char *son2=NULL;
	char wavfile2[512]; 
	char wavfile1[512];
	int freq,freq1,freq2,freq3,taille,taille1,taille2,taille3;
	int i=0;

  	SDL_Surface* p= newfenetregraphique(600,400);
	
	/* mixage de signaux a meme frequence */
	printf("faire un mixage de son à meme frequence\n"); 
        printf("Entrer le nom du premier fichier son "); fflush(stdout); scanf("%s",wavfile1); 
  	if ( (son1 = get_son_wav_dyn(wavfile1,&freq1,&taille1)) ==NULL) { 
           printf("Pas de fichier son %s\n",wavfile1); exit(1);
        }
	printf("son 1\n"); 
	if (p) affiche_joue_son(son1, taille1, freq1,p,SDL_PH_ROUGE);
        else joue_son(son1, taille1, freq1);
        printf("Entrer le nom du deuxieme fichier son "); fflush(stdout); scanf("%s",wavfile2); 
  	if ( (son2 = get_son_wav_dyn(wavfile2,&freq2,&taille2)) ==NULL) { 
           printf("Pas de fichier son %s\n",wavfile2); exit(1);
        }
	printf("son 2\n"); 
	if (p) affiche_joue_son(son2, taille2, freq2,p,SDL_PH_VERT);
 	else joue_son(son2, taille2, freq2);
        if (freq2==freq1) {
	  mixe_meme_frequence(&son, &taille, son1, taille1, son2, taille2);
	  printf("resultat\n"); 
	  if (p) affiche_joue_son(son, taille, freq1,p,SDL_PH_BLEU);
 	  else joue_son(son, taille, freq1);
	  libere_son(son); 
        }
        else printf("Pas le meme frequence !!"); 
        libere_son(son1); libere_son(son2);

	return 0;
}
