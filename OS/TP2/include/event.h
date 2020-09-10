
/**
 * @file event.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 12:54:52 2017
 * @brief Events.
 *
 * Events.
 */

#ifndef _EVENT_H_
#define _EVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "list.h"

#include <time.h>


  struct event_t;

  typedef struct event_t *event; 

  void   del_event( void *e );
  void   print_event( void *e ); 

  time_t event_date( event ev );
  char  *event_command( event ev ); 
  
  list   read_events( char *file );

#ifdef __cplusplus
}
#endif

#endif /* _EVENT_H_ */
