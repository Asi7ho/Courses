
/**
 * @file list.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 12:42:00 2017
 * @brief Lists.
 *
 * Lists.
 */

#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
  
  typedef void (*callback)(void*);
  typedef int  (*comparator)(void*,void*);

  struct list_t;

  typedef struct list_t *list; 

#define  nil NULL

  int    empty( list l );
  
  void  *car( list l );
  list   cdr( list l );

  void   set_car( void *t, list l );
  void   set_cdr( list n, list l );

  list   cons( void *elt, list l );

  void  *__pop( list *l );
#define  pop(l) __pop( &(l) )

  list   consort( void *elt, list l, comparator compare );

  int    length( list l ); 
  void   visit( list l, callback apply );

  void   delete( list l, callback del ); 

#ifdef __cplusplus
}
#endif

#endif /* _LIST_H_ */
