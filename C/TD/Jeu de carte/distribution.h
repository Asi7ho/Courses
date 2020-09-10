#include "bataille.h"

#ifndef _DISTRIBUTION
#define _DISTRIBUTION

#define IMAGE_PATH "/users/prog1a/C/librairie/cartes/"

ELEMENT prend_une_carte(char* tab, int nbcarterestant ) ;
int distribution(File* aj1, File *aj2, int alea, int nbcarte);
int melanger(File* aj, int alea, int nbcarte);
#endif

