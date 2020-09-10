
/**
 * @file palloc.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 23 12:11:14 2018
 * @brief Protected alloc.
 *
 * Protected alloc. 
 */

#ifndef _PALLOC_H_
#define _PALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>   /* size_t */
  
  void *pmalloc( size_t size );
  void *pcalloc( size_t nmemb, size_t size ); 
  void  pfree( void *ptr ); 

#ifdef USE_PALLOC
#define malloc(n)     pmalloc(n)
#define calloc(n,s)   pcalloc((n),(s))
#define free(ptr)     pfree(ptr)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _PALLOC_H_ */
