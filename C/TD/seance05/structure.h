#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_

#define NLEN 44 
#define SLEN  4 
#define RLEN 24 

struct fiche_t {
  char nom[NLEN];
  char symbole[SLEN];
  int numeroatomique; 
  double masseatomique; 
} ;

typedef struct fiche_t FICHE;

#endif

