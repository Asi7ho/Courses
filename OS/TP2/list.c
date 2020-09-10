
/**
 * @file list.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 12:46:26 2017
 * @brief Lists.
 *
 * Lists.
 */

#include "list.h"
#include "event.h"

#include <stdlib.h> /* NULL */

struct list_t {
  
  void           *car; 
  struct list_t  *cdr; 
  
}; 

int    empty( list l ) {
  return nil == l; 
}

void  *car( list l ) {
  return l->car; 
}

list   cdr( list l ) {
  return l->cdr; 
}

void   set_car( void *t, list l ) {
  l->car = t; 
}

void   set_cdr( list n, list l ) {
  n->cdr = l; 
}

void  *__pop( list *l ) {
  
  if ( empty( *l ) ) {
    return NULL; 
  }
  else {
    list     n = cdr( *l ); 
    void *data = car( *l ); 
    free( *l ); 
    *l = n; 
    return data; 
  }
}

list  cons( void *elt, list l ) {
  list n = calloc( 1, sizeof( *n ) );

  set_car( elt, n );

  set_cdr( n, l );

  return n; 
}

list  consort( void *elt, list l, comparator compare ) {
  list  tmp = l;
  list prev = nil;
  list  ptr = cons( elt, nil ); 

  if ( empty( tmp ) ) {
    return ptr; 
  }

  if ( compare( elt, car( l ) ) < 0 ) {
    set_cdr( ptr, l ); 
    return ptr; 
  }

  while( !empty( tmp ) ) {
    if ( compare( elt, car( tmp ) ) > 0 ) {
      prev = tmp;
      tmp = cdr( tmp );
      continue; 
    }
    else {
      set_cdr( prev, ptr );
      set_cdr( ptr, tmp );
      return l; 
    }
  }
  
  set_cdr( prev, ptr );

  return l; 
}

void visit( list l, callback apply ) {  
  for( ; !empty( l ); apply( car( l ) ), l = cdr( l ) ); 
}

int  length( list l ) {
  int n = 0;

  for ( ; !empty( l ); l = cdr( l ), n++ ); 

  return n; 
}

void delete( list l, callback del ) {
  
  while( !empty( l ) ) {    
    list next = cdr( l );
    
    del( car( l ) );

    free( l );
    
    l = next; 
  }
}

