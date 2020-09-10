#include <stdio.h>
#include <string.h>

int nbligne(char *fic) { FILE* fp; int c=0; char flag=0;
  if ( (fp=fopen(fic,"rt"))==NULL) return 0;
  else
    while (!feof(fp))
      if (fgetc(fp)=='\n') {if (flag) c++; flag=0;}
      else flag=1;
  fclose(fp);
  return c;
}

int lecture(FILE* fp, char* mot) {
  mot[0]='\0' ;	// vide le mot precedemment saisi
  do
  { if (fscanf(fp,"%[^\n\' ,-.!?;:\"()]s",mot)==EOF) return 0; 
    fscanf(fp,"%*c"); // saute le separateur (caractere suivant)
  } while( !strlen(mot) ) ; // attente d'un veritable mot
  return 1;
}

int lectureLigneDico(char* mot, int n, FILE *fp) {
  if (fgets(mot,n,fp)==NULL) return 0;
  mot[strlen(mot)-1] =0;
  return 1;
}
