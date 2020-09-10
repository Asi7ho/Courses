
/**
 * @file object.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 22:11:29 2018
 * @brief Objects.
 *
 * Objects.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "port.h"
#include "vec.h"
#include "mat.h"
  
    
  typedef enum {
    T_OK,
    T_ERROR,
    T_PORT, 
    T_INPUT_PORT, 
    T_OUTPUT_PORT, 
    T_TRUE,
    T_FALSE,
    T_NIL,
    T_OPERATOR,
    T_TREE,
    T_LIST,
    T_VEC,
    T_MAT,
    T_SYMBOL,
    T_NUMBER,
    T_STRING
  } type_t; 
  
  struct object_t;
  
  typedef struct object_t *object; 

  object car( object list ); 
  object cdr( object list );
#define  caar(l) car(car(l))
#define  cadr(l) car(cdr(l))
#define  cdar(l) cdr(car(l))
#define  cddr(l) cdr(cdr(l))
  
  object cons( object ca, object cd ); 

  char  *op_name( object op );
  object op_args( object op );
  type_t object_type( object obj );
  double object_number( object obj );
  object object_cdr( object obj );
  char  *object_string( object obj );
  vec    object_vec( object obj );
  mat    object_mat( object obj );
  object make__( type_t type, ... );
#define  make( ... ) make__( __VA_ARGS__, NULL )
  
  int    pprint_object( port p, object obj );

  object find_symbol( char *name, object list ); 
  object add_symbol( char *sym, object val, object syms ); 
  void   update_symbol_value( object sym, object new_val );

  size_t list_length( object list );
  object list_reverse( object list );
  
#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
