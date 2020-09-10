
/**
 * @file main.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 23 12:15:38 2018
 * @brief Testing protected alloc.
 *
 * Testing protected alloc.
 */

#include "palloc.h"


#include <stdio.h>
#include <stdlib.h>

#ifdef TEST_OVERFLOW
#define OVERFLOW 1
#else
#define OVERFLOW 0
#endif

#define NMEMB 10

int main ( ) {

  char *string = malloc( NMEMB*sizeof( *string ) ); 

  /* Detect writes one byte below pointer: */
  fprintf( stdout, "string @ %p\n", string );

#ifdef TEST_UNDERFLOW
  string[-1]               = 'G'; 
#endif
  
  /* Check we can write to both ends: */
  string[0]                = 'T'; 
  string[NMEMB-1]          = 'C'; 

  /* Detect access one byte above pointer: */
  string[NMEMB-1+OVERFLOW] = 'A'; 
  
  free( string ); 

  exit( EXIT_SUCCESS );
}
