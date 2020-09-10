#include <math.h>
#include "son.h"
#include <SDL_phelma.h>


int main(int argc, char * argv[]){
 	unsigned char *son=NULL;
	unsigned char *son1=NULL;
	unsigned char *son2=NULL;
	char wavfile2[] = "goelands.wav";
	char wavfile3[] = "johnny.wav";
	int freq,freq1,freq2,freq3,taille,taille1,taille2,taille3;
	int i=0;

	
	/* mixage de signaux avec frequence proportionnelle*/
	printf("faire un mixage de son avec frequence proportionnelle\n"); 
	son1 = get_son_wav_dyn(wavfile3,&freq1,&taille1);
	son2 = get_son_wav_dyn(wavfile2,&freq2,&taille2);
	printf("son 1\n"); 
	joue_son(son1, taille1, freq1);
	printf("son 2\n"); 
	joue_son(son2, taille2, freq2);
	printf("resultat\n");
	mixe(&son, &taille, &freq, son1, taille1,freq1, son2, taille2,freq2);
	joue_son(son, taille, freq);
	libere_son(son); libere_son(son1); libere_son(son2);
	return 0;
}
