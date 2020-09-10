#include <stdlib.h>
#include <stdio.h>
#include <SDL_phelma.h>
#include "son.h"

int main(int argc, char * argv[]){
  	unsigned char* son=NULL; 
  	unsigned char* son2=NULL;
	char wavfile[] = "johnny.wav"; 
  	int nb_samples, fd, amplitude=200, freq;
  	int i=0;
        SDL_Surface* f=NULL;
  	if (!Sound_Init()) fprintf(stderr,"init failed:%d.\n", Sound_GetError());

	puts("Ecouter le son initial: tapez une touche"); getchar();
	/* Jouer le son */
  	joue_son_wav(wavfile);

	/* Lire un fichier son */
  	son = get_son_wav_dyn(wavfile,&freq,&nb_samples); 

	/* afficher ses caracteristiques*/
	printf("le fichier %s a %d echantillons et une frequence d'echantillonage de %d\n",wavfile,freq,nb_samples);
  	puts("Ecouter le son recupere: tapez une touche"); getchar();

        f=newfenetregraphique(800,400);
	/* Ecouter au casque */
  	if (f) affiche_joue_son(son, nb_samples, freq,f, SDL_PH_ROUGE);
        else joue_son(son, nb_samples, freq);
	
	/* Ecouter un train de Dirac : meme nombre d’echantillons frequence d'echantillonnage à 44100Hz train à 441 Hz*/
	son2 = alloue_son(nb_samples);
	freq = 44100; fd = freq/100;
	for(i=0;i<nb_samples;i++) son2[i] = ((i%fd)==0)*amplitude;
  	printf("Signal en peigne de Dirac à %d Hz: tapez une touche\n",fd); getchar(); 
  	joue_son(son2, nb_samples, freq);
	libere_son(son2);

	/* Ecouter un signal continu : meme nombre d’echantillons*/
  	son2 = alloue_son(nb_samples);
  	for(i=0;i<nb_samples;i++) son2[i] = amplitude;
  	puts("Signal continu: tapez une touche"); getchar(); 
  	joue_son(son2, nb_samples, freq);

  	/* Liberer la memoire */
  	libere_son(son);
	libere_son(son2);
}
