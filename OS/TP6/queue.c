
/**
 * @file queue.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Feb 10 11:03:20 2018
 * @brief Priority queue as ordered linked list.
 *
 * Priority queue as ordered linked list. 
 */

#include <stdlib.h>
#include <stdio.h>


#include "queue.h"

struct queue_t {
  struct queue_t *next;
  void            *ptr; 
};

int   emptyq( queue q ) { 
  return q == NULL;
}

queue new_queue( void ) {
  return NULL;
}

queue enqueue( queue q, void *p, comparator priority ) {
  queue c; 
  queue n = calloc( 1, sizeof( *n ) ); 
  
  n->ptr  = p;
  
  if ( emptyq( q ) || priority( n->ptr, q->ptr ) < 0 ) { 
    n->next = q; 
    return n; 
  }

  for ( c = q; !emptyq( c->next ) && priority( n->ptr, c->next->ptr ) > 0; c = c->next ); 

  n->next = c->next; 
  c->next = n;

  return q; 
}

void *queue_peek( queue q ) {
  if ( !emptyq( q ) ) return q->ptr;

  return NULL;
}

queue dequeue( queue q ) {
  
  if ( !emptyq( q ) ) { 
    queue n = q->next;

    free( q ); 

    return n; 
  }

  return new_queue();
}

void *queue_search( queue q, probe test, void *val ) {
  if ( test ) { 

    while ( !emptyq( q ) ) {
      if ( 0 == test( q->ptr, val ) ) return q->ptr; 

      q = q->next; 
    }
  }
  
  return new_queue();
}

void  visitq( queue q, callback c, size_t parm, probe test, void *val ) { 
  while ( !emptyq( q ) ) { 
    if ( NULL == test ) { 
      c( q->ptr, parm );
    }
    else if ( 0 == test( q->ptr, val ) ) c( q->ptr, parm );
    
    q = q->next;
  }
}

void  printq( queue q, callback print ) { 
  printf( "[ " );
  visitq( q, print, 0, NULL, 0 );
  printf( "]" );
}

queue delval( queue q, callback del, probe test, void *val ) { 
  if ( !emptyq( q ) ) { 
    if ( 0 == test( q->ptr, val ) ) { 
      queue n = q->next;
      del( q->ptr, 0 );
      free( q );
      return n; 
    }
    else q->next = delval( q->next, del, test, val ); 
  }

  return q; 
}

