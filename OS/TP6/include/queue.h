
/**
 * @file queue.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Feb 10 11:03:33 2018
 * @brief Queue.
 *
 * Queue.
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

  typedef int  (*probe)(void*, void*);
  typedef probe comparator; 
  typedef void (*callback)(void*,ssize_t); 

  struct queue_t; 

  typedef struct queue_t *queue; 

  queue new_queue( void );
  int   emptyq( queue q ); 
  queue enqueue( queue q, void *p, comparator priority ); 
  void *queue_peek( queue q ); 
  queue dequeue( queue q ); 
  void  visitq( queue q, callback c, size_t parm, probe test, void *val ); 
  void  printq( queue q, callback print ); 
  queue delval( queue q, callback del, probe test, void *val ); 
  void *queue_search( queue q, probe test, void *val );
  
#ifdef __cplusplus
}
#endif

#endif /* _QUEUE_H_ */
