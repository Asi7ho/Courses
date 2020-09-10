
/**
 * @file op_sobel.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Feb  3 14:07:15 2018
 * @brief Sobel image filter.
 *
 * Sobel image filter. 
 */


#include <math.h>

#include "api.h"

static double sob_kernel_x[ 3 ][ 3 ] = {
  { 1, 0, -1 }, 
  { 2, 0, -2 }, 
  { 1, 0, -1 } 
};

static double sob_kernel_y[ 3 ][ 3 ] = {
  {  1,  2,  1 }, 
  {  0,  0,  0 }, 
  { -1, -2, -1 } 
};

static mat sobel( mat img ) { 
  int    h = mat_height( img ); 
  int    w = mat_width( img ); 
  mat   gx = new_mat( h, w, gen_zeros ); 
  mat   gy = new_mat( h, w, gen_zeros ); 
  int r, c;

  for ( r = 1; r < h - 1; r++ ) { 
    for ( c = 1; c < w - 1; c++ ) { 
      int i, j;

      for ( i = -1; i< 2; i++ ) { 
	for ( j = -1; j< 2; j++ ) {
	  gx[ r ][ c ] += sob_kernel_x[ i + 1 ][ j + 1 ] * img[ r + i ][ c + j ];
	  gy[ r ][ c ] += sob_kernel_y[ i + 1 ][ j + 1 ] * img[ r + i ][ c + j ];
	}
      }
    }
  }

  for ( r = 1; r < h - 1; r++ ) { 
    for ( c = 1; c < w - 1; c++ ) { 
      gx[ r ][ c ] = sqrt( gx[ r ][ c ]*gx[ r ][ c ] + gy[ r ][ c ]*gy[ r ][ c ] ); 
    }
  }

  return gx; 
}

object op_sobel( shell s, object op ) { 
  object    args = op_args( op ); 
  mat        img = NULL; 

  if ( 1 != list_length( args ) ) return make( T_ERROR, "Wrong number of args for Sobel filter." );

  if ( NULL == car( args ) || T_MAT != object_type( car( args ) ) )
    return make( T_ERROR, "Argument must be a matrix." );
  
  img = object_mat( car( args ) ); 

  return make( T_MAT, sobel( img ) );

  UNUSED( s );
}
