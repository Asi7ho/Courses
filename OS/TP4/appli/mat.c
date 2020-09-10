
/**
 * @file mat.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 14:39:19 2018
 * @brief Matrices.
 *
 * Matrices.
 */

#include "mat.h"
#include "vec.h"

#define MAT_HEADER(v)  (((size_t*)v)-2)
#define MAT_HEIGHT(v)  (*MAT_HEADER(v))
#define MAT_WIDTH(v)   (*(MAT_HEADER(v)+1))

#define MAT_HMAX_PRINT 10
#define MAT_WMAX_PRINT 10


size_t  mat_height( mat m ) {
  return MAT_HEIGHT( m ); 
}

size_t  mat_width( mat m ) { 
  return MAT_WIDTH( m ); 
}

size_t  pprint_mat( port p, mat m ) { 
  size_t    h = mat_height( m ); 
  size_t    w = mat_width( m ); 
  size_t i, j; 
  size_t  ret = 0;

  pprintf( p, "[ " );
  if ( h > MAT_HMAX_PRINT ) {
    for ( i = 0; i< 3; i++ ) {
      
      ret += pprintf( p, "[ " );
      
      if ( w > MAT_WMAX_PRINT ) {
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ j ] );
	}
	
	ret += pprintf( p, "... " );
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ w - 3 + j ] );
	}
      }
      else {
	for ( j = 0; j< w; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ j ] ); 
	}
      }
      
      ret += pprintf( p, "]\n" ); 
    }

    ret += pprintf( p, "...\n" ); 
    
    for ( i = 0; i< 3; i++ ) {
      
      ret += pprintf( p, "[ " );
      
      if ( w > MAT_WMAX_PRINT ) {
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ h - 3 + i ][ j ] );
	}
	
	ret += pprintf( p, "... " );
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ h - 3 + i ][ w - 3 + j ] );
	}
      }
      else {
	for ( j = 0; j< w; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ j ] ); 
	}
      }
      
      ret += pprintf( p, "]\n" ); 
    }
  }
  else {
    for ( i = 0; i< h; i++ ) {
      
      ret += pprintf( p, "[ " );
      
      if ( w > MAT_WMAX_PRINT ) {
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ j ] );
	}
	
	ret += pprintf( p, "... " );
	
	for ( j = 0; j< 3; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ w - 3 + j ] );
	}
      }
      else {
	for ( j = 0; j< w; j++ ) {
	  ret += pprintf( p, "%lf ", m[ i ][ j ] ); 
	}
      }
      
      ret += pprintf( p, "]\n" ); 
    }
  }

  pprintf( p, "]" ); 
  
  return ret; 
}

mat     new_mat( size_t h, size_t w, double (*generator)(void) ) { 
  mat     m = (mat)(2+(size_t*)calloc( 1, 2*sizeof( size_t ) + h*sizeof( *m ) )); 
  size_t  r;

  m[0] = calloc( h*w, sizeof( **m ) );

  for ( r = 1; r < h; r++ ) {
    m[ r ] = m[ r - 1 ] + w; 
  }

  MAT_HEIGHT( m ) = h; 
  MAT_WIDTH( m )  = w;

  for ( r = 0; r < h*w; r++ ) {
    m[0][r] = generator(); 
  }

  return m; 
}

void    del_mat( mat m ) { 
  free( m[0] );
  free( MAT_HEADER( m ) );
}

mat     mat_transpose( mat m ) {
  size_t   h = mat_height( m );
  size_t   w = mat_width( m );
  mat      n = new_mat( w, h, gen_zeros );
  size_t   r;

  for ( r = 0; r < h; r++ ) {
    size_t c;

    for ( c = 0; c < w; c++ ) {
      n[ c ][ r ] = m[ r ][ c ]; 
    }
  }
  
  return n; 
}
