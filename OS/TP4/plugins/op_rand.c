
/**
 * @file op_rand.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 12:17:21 2018
 * @brief Uniform RNG.
 *
 * Uniform RNG. 
 */


#include <string.h>

#include "api.h"

typedef double(*generator)(void); 

static generator get_gen( object str ) {
  char *gen = object_string( str );

  if ( !strcmp( gen, "int10" ) ) return gen_rand_int10;
  if ( !strcmp( gen, "uniform" ) ) return gen_rand_uniform;
  if ( !strcmp( gen, "zeros" ) ) return gen_zeros; 

  return NULL;
}

object op_rand( shell s, object op ) { 
  object      args = op_args( op ); 
  size_t     nargs = list_length( args );
  generator    gen = NULL; 
  
  switch( nargs ) {
  case 1 :
    gen = get_gen( car( args ) ); 
    return make( T_NUMBER, gen() );
  case 2 :
    gen = get_gen( cadr( args ) ); 
    return make( T_VEC, new_vec( (size_t)object_number( car( args ) ), gen ) );
  case 3 :
    gen = get_gen( car( cddr( args ) ) ); 
    return make( T_MAT, new_mat( (size_t)object_number( car( args ) ),
				 (size_t)object_number( cadr( args ) ),
				 gen ) ); 
  default :
    return parse_error( "Wrong number of arguments for rand." );
  }

  UNUSED( s );
}
