#include <math.h>
#include <SDL_phelma.h>
#include "son.h"

main(int argc, char * argv[]){
 	unsigned char *son;
	unsigned char *son2;
	char wavfile[] = "johnny.wav";
	int nb_samples = 10000; 
	int amplitude = 64;
	int freq = 44100;
	int T = 5;
	int freqsin = 3000;

	SDL_Surface* f=newfenetregraphique(800,400);

	if (!Sound_Init()) fprintf(stderr,"init failed:%d.\n", Sound_GetError());
	
	/* sinusoide */
    /*
	printf("Ecouter le son d'une sinusoide de frequence %d: tapez une touche\n",freqsin); getchar();
	son = cree_sin(amplitude, nb_samples,freqsin, freq);
	if (f) affiche_joue_son(son, nb_samples, freq,f,SDL_PH_ROUGE);
	else joue_son(son, nb_samples, freq);
	libere_son(son);
     */
    
	/* chargement du son */
	printf("Ecouter le son suivant\n"); getchar();
  	son = get_son_wav_dyn(wavfile,&freq,&nb_samples); 
	if (f) affiche_joue_son(son, nb_samples, freq,f,SDL_PH_BLEU);
    else joue_son(son, nb_samples, freq);

	/* sous echantillonnage */
	printf("Ecouter le son sous-echantillonne à un taux de %d\n",T); getchar();
	son2 = sous_echantillonnage_brutal(son, nb_samples, T);
	if (f) affiche_joue_son(son2, nb_samples/T, freq/T,f,SDL_PH_VERT); 
        else joue_son(son2, nb_samples/T, freq/T);	
	libere_son(son2);
       
	getchar(); SDL_phelma_clear(f);

        /* En accelere */
    printf("Ecouter le son accelere sous-echantillonne à un taux de 2\n");
    if (f) affiche_joue_son(son, nb_samples, 2*freq,f,SDL_PH_JAUNE);
    else joue_son(son, nb_samples, 2*freq);
    getchar(); SDL_phelma_clear(f);

	/* inverse */
	printf("Ecouter l'inverse du son\n"); 
	son2 = inverse(son, nb_samples);
	if (f) affiche_joue_son(son2, nb_samples, freq, f, SDL_PH_MAGENTA);
	else joue_son(son2, nb_samples, freq);
    getchar();
	libere_son(son2);
    libere_son(son);
}
