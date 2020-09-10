
/**
 * @file op_mult.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 17:27:18 2018
 * @brief mult.
 *
 * mult.
 */

#include <string.h>


#include "api.h"


static mat mat_mult( mat m1, mat m2 ) {
  size_t    dim = mat_width( m1 );
  size_t    i, j, k;
  size_t    h = mat_height( m1 );
  size_t    w = mat_width( m2 );
  mat       m = new_mat( h, w, gen_zeros );

  /* TODO: Compute matrix multiplication.
     Matrix coefficients are accessed through
     the regular m[][] syntax.
   */
   for (i = 0; i < dim; i++) {
     for (j = 0; j < h; j++) {
       for(k = 0; k < w; k ++) {
         m[i][j] += m1[i][k]*m2[k][j];
       }
     }
   }
  return m;
}

object op_mult( shell s, object op ) {
  object    args = op_args( op );
  object   first = car( args );
  object  second = cadr( args );
  vec     v1, v2;
  mat     m1, m2;
  if ( T_SYMBOL == object_type( first  ) ) {
    first  = eval_expr( s, first  );
    if ( T_ERROR == object_type( first ) ) return first;
  }
  if ( T_SYMBOL == object_type( second ) ) {
    second = eval_expr( s, second );
    if ( T_ERROR == object_type( second ) ) return second;
  }

  if ( object_type( first ) == object_type( second ) ) {
    switch( object_type( first ) ) {
    case T_STRING :
      return parse_error( "Don't know how to multiply strings." );
    case T_NUMBER :
      return make( T_NUMBER, object_number( first )*object_number( second ) );
    case T_VEC :
      v1 = object_vec( first );
      v2 = object_vec( second );
      if ( vec_size( v1 ) != vec_size( v2 ) )
	return parse_error( "Vector sizes mismatch." );
      return make( T_NUMBER, vec_inner_product( v1, v2 ) );
    case T_MAT :
      m1 = object_mat( first );
      m2 = object_mat( second );
      if ( mat_width( m1 ) != mat_height( m2 ) ) return parse_error( "Matrix dimensions mismatch." );
      return make( T_MAT, mat_mult( m1, m2 ) );
    default : break;
    }
  }

  return parse_error( "Multiplication failed." );
}
