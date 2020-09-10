
/**
 * @file port.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Jan 27 10:33:04 2018
 * @brief Ports.
 *
 * Ports.
 */

#ifndef _PORT_H_
#define _PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

  struct port_t;

  typedef struct port_t *port; 

  port new_port( FILE *fp );

  void del_port( port p );

  int  pgetc( port p );
  void pungetc( int c, port p );
  void pputc( int c, port p );
  void pflush( port p ); 
  int  pprintf( port p, char *fmt, ... );
  int  pprintf_va( port p, char *fmt, va_list va ); 
  
#ifdef __cplusplus
}
#endif

#endif /* _PORT_H_ */
