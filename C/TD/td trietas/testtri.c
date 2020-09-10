#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "tas.h"

void tri(double* tas, int n){
    int i;
    constructiontas(tas,n); //on constuit un tas
    puts("tas construit");
    for (i = n; i > 0; i--) {
	suppressiontas(tas, i); //on echange le premier element et le dermier et on supprime le dernier element
   }
}



/* fonction de comparaison utilsee pour le quick sort */
int compar( const void* ax, const void * ay) {
  return( *((double *)ax)- *((double *)ay));
}

int  main(){ 
  int LO; 			/* Nombre de reels a trier : 10E LO */
  clock_t avant, apres;
  double temps1, temps2;
  double *t1, *t2;
  int i,j,k,l;
 
	/* Lecture du nombre d’octaves au clavier */
  printf("Nombre d'octave a trier "); fflush(stdout); scanf("%d", &LO);
	/* Creation des tableaux a trier */
  if ( (t1=calloc(pow(10,LO+1),sizeof(*t1))) == NULL) {printf("Allocation impossible\n"); exit(1); } 
  if ( (t2=calloc(pow(10,LO+1),sizeof(*t1))) == NULL) {printf("Allocation impossible\n"); exit(1); } 

	/* Initialisation du generateur aleatoire */
  srand(getpid());

	/* Premiere boucle sur le nombre d’octave */
  for (k=10,i=0; i<LO; k*=10,i++) 
    for (j=1; j<2; j++) {
	/* j*k est le nombre d’elements a trier */
      printf("Nombre d'elements %d ", j*k); fflush(stdout);
		/* Tirage aleatoire des nombres a trier */
      for (l=0; l<j*k; l++) t1[l]=rand()%100; 
		/* Copie dans le deuxieme tableau */
      memcpy(t2,t1,j*k*sizeof(*t1));
      avant = clock();
	      /* Tri par quick sort */
      qsort(t1,j*k,sizeof(*t1),compar); 
      apres = clock(); 

/*
	puts("tableau a trier");
	for (l = 0; l < j*k; l++) printf("%lf ", t2[l]);
	puts("");
*/


      temps1=(double)(apres - avant)/CLOCKS_PER_SEC;
 	      /* Tri par ma fonction */
      avant = clock(); tri(t2,j*k); apres = clock();
      temps2=(double)(apres - avant)/CLOCKS_PER_SEC;
	      /* Affichage des temps des 2 tris */
      printf("Quick sort :%lf Mon tri par tas: %lf\n", temps1, temps2);
      if (memcmp(t1,t2,j*k*sizeof(*t1))!=0) printf("Erreur de tri ..................................\n");


	puts("mon tas trie");
	for (l = 0; l < j*k; l++) printf("%lf ", t2[l]);
	puts("");



    } 
	/* Liberation memoire */
  free(t1); free(t2);
  return EXIT_SUCCESS;
}
