
/**
 * @file mat.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 14:17:53 2018
 * @brief Matrices.
 *
 * Matrices.
 */

#ifndef _MAT_H_
#define _MAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "port.h"

  typedef double **mat;

  size_t  mat_height( mat m ); 
  size_t  mat_width( mat m ); 
  size_t  pprint_mat( port p, mat m ); 
  mat     new_mat( size_t h, size_t w, double (*generator)(void) ); 
  void    del_mat( mat m ); 

  mat     mat_transpose( mat m );
  
#ifdef __cplusplus
}
#endif

#endif /* _MAT_H_ */
