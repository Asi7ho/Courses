
/**
 * @file vec.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Jan 24 18:38:18 2018
 * @brief Vectors.
 *
 * Vectors.
 */

#ifndef _VEC_H_
#define _VEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "port.h" 


  typedef double *vec; 

  double  gen_zeros( void ); 
  double  gen_rand_uniform( void ); 
  double  gen_rand_int10( void );
  
  size_t  vec_size( vec v ); 
  size_t  pprint_vec( port p, vec v ); 
  vec     new_vec( size_t sz, double (*generator)(void) ); 
  void    del_vec( vec v );
  double  vec_inner_product( vec v1, vec v2 );

#ifdef __cplusplus
}
#endif

#endif /* _VEC_H_ */
