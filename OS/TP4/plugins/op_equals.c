
/**
 * @file op_equals.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 16:24:08 2018
 * @brief equals.
 *
 * equals.
 */

#include <string.h>


#include "api.h"


object op_equals( shell s, object op ) { 
  object    args = op_args( op );
  object   first = caar( args ); 
  object  second = car( cadr( args ) );
  size_t       i; 
  size_t n, h, w;
  vec     v1, v2; 
  mat     m1, m2; 

  if ( first == second ) return shell_true( s );

  if ( T_SYMBOL == object_type( first  ) ) {
    if ( T_SYMBOL == object_type( second ) &&
	 !strcmp( object_string( first ), object_string( second ) ) ) return shell_true( s );
    first  = eval_expr( s, first  );
    if ( T_ERROR == object_type( first ) ) return first; 
  }
  if ( T_SYMBOL == object_type( second ) ) {
    second = eval_expr( s, second );
    if ( T_ERROR == object_type( second ) ) return second; 
  }
  
  if ( object_type( first ) != object_type( second ) ) return shell_false( s );

  switch( object_type( first ) ) {
  case T_NUMBER :
    if ( object_number( first ) != object_number( second ) ) return shell_false( s );
    return shell_true( s );
  case T_STRING :
    if ( strcmp( object_string( first ), object_string( second ) ) ) return shell_false( s );
    return shell_true( s );
  case T_VEC :
    v1 = object_vec( first );
    v2 = object_vec( second );
    n = vec_size( v1 ); 
    if ( n != vec_size( v2 ) ) return shell_false( s );
    for ( i = 0; i< n; i++ ) {
      if ( v1[ i ] != v2[ i ] ) return shell_false( s );
    }
    return shell_true( s ); 
  case T_MAT :
    m1 = object_mat( first );
    m2 = object_mat( second );
    h = mat_height( m1 ); 
    w = mat_width( m1 ); 
    if ( h != mat_height( m2 ) || w != mat_width( m2 ) ) return shell_false( s );
    for ( i = 0; i< h*w; i++ ) {
      if ( m1[ 0 ][ i ] != m2[ 0 ][ i ] ) {
	pprintf( shell_output( s ), "\nCoeffs [%lu,%lu] differ (%.16lf vs %.16lf).\n", i/w, i%w, m1[ 0 ][ i ], m2[ 0 ][ i ] );
	return shell_false( s );
      }
    }
    return shell_true( s );
  default : return shell_false( s );
  }
  
  return shell_false( s ); 
}
