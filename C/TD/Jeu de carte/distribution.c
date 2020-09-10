
#include "bataille.h"


#define IMAGE_PATH "/users/prog1a/C/librairie/cartes/"

/* Version efficace de distribution : a chaque tour i, on echange dans le tableau de marquage tab, la carte tiree et au hazard et la carte en position i
*/

ELEMENT prend_une_carte(char* tab, int nbcarterestant ) { ELEMENT c;
  char* cou[]={"trefle","carreau","coeur","pique"};
  char nom[256];
  unsigned char* r=NULL,* g=NULL,* b=NULL;
  FILE* f=NULL;
  int i,j, k;
  memset(&c,0,sizeof(c));
  j=nbcarterestant!=0 ? random()%nbcarterestant : 0;
  k=tab[j] ;
  i=tab[j]; tab[j]=tab[nbcarterestant-1]; tab[nbcarterestant-1]=i;
  c.rang=1+k/4; c.couleur=k%4; c.visible=0;
  sprintf(nom,"%s%s_%d.ppm",IMAGE_PATH,cou[c.couleur],c.rang-1);
  if ( (c.im.data=lectureimage32(nom,&(c.im.lig),&(c.im.col)))==NULL) {
    printf("Lecture impossible du fichier graphique : %s\n",nom);
    c.im.data=NULL; return c;
  }
  else {
    c.dos.data=lectureimage32(IMAGE_PATH"dos.ppm",&(c.dos.lig),&(c.dos.col));
  }
  return c;
}

int distribution(File* aj1, File *aj2, int alea, int nbcarte) { int i;
  ELEMENT c;
  char* tab=calloc(nbcarte*4,1); /* Indique les cartes deja utilisees par un 1 */
  for(i=0; i<nbcarte*4; i++) tab[i]=i;
  srandom(alea);        /* initialisation du generateur aleatoire */
  for (i=0; i<nbcarte*4/2; i++) {
    c=prend_une_carte(tab,nbcarte*4-2*i); *aj1=enfiler(c,*aj1); 
    c=prend_une_carte(tab,nbcarte*4-2*i-1); *aj2=enfiler(c,*aj2); 
  }
  free(tab); tab=NULL;
  return 1;
}


int melanger(File* aj, int alea, int nbcarte) { int i;
  ELEMENT c;
  char* tab=calloc(nbcarte*4,1); /* Indique les cartes deja utilisees par un 1 */
  for(i=0; i<nbcarte*4; i++) tab[i]=i;
  srandom(alea);        /* initialisation du generateur aleatoire */
  for (i=0; i<nbcarte*4; i++) {
     *aj=enfiler(prend_une_carte(tab,nbcarte*4-i),*aj);
  }
  free(tab); tab=NULL;
  return 1;
}


