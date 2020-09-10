
/**
 * @file vec.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 13:46:43 2018
 * @brief Vectors.
 *
 * Vectors.
 */

#include <stdlib.h> /* RAND_MAX */

#include "vec.h"

#define VEC_HEADER(v)  (((size_t*)v)-1)
#define VEC_SIZE(v)    (*VEC_HEADER(v))

#define VEC_MAX_PRINT  10

double  gen_zeros( void ) {
  return 0.;
}

double  gen_rand_int10( void ) { 
  return (rand()%10); 
}

double  gen_rand_uniform( void ) {
  return rand()/(double)RAND_MAX; 
}

size_t  vec_size( vec v ) {
  return VEC_SIZE( v ); 
}

size_t  pprint_vec( port p, vec v ) { 
  size_t ret = 0; 
  size_t   i; 
  size_t   n = vec_size( v ); 

  ret += pprintf( p, "[ " );
  
  if ( n > VEC_MAX_PRINT ) {
    for ( i = 0; i< 3; i++ ) {
      ret += pprintf( p, "%lf ", v[ i ] ); 
    }

    ret += pprintf( p, "... " );
    
    for ( i = 0; i< 3; i++ ) {
      ret += pprintf( p, "%lf ", v[ n - 3 + i ] ); 
    }
  }
  else {
    for ( i = 0; i< n; i++ ) {
      ret += pprintf( p, "%lf ", v[ i ] ); 
    }
  }

  ret += pprintf( p, "]" );

  return ret; 
}

vec     new_vec( size_t sz, double (*generator)(void) ) {
  size_t  i = 0; 
  vec     v = (vec)(1+(size_t*)calloc( 1, sizeof( size_t ) + sz*sizeof( *v ) ));

  VEC_SIZE( v ) = sz; 

  for ( i = 0; i< sz; i++ ) {
    v[ i ] = generator(); 
  }
  
  return v; 
}

void    del_vec( vec v ) { 
  free( VEC_HEADER( v ) ); 
}

double  vec_inner_product( vec v1, vec v2 ) {
  size_t n = vec_size( v1 );
  size_t i;

  double r = 0.;

  for ( i = 0; i< n; i++ ) { 
    r += v1[ i ] * v2[ i ]; 
  }

  return r; 
}
