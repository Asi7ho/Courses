
/**
 * @file port.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Jan 27 10:27:15 2018
 * @brief Ports.
 *
 * Ports.
 */

#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "port.h"

struct port_t {
  FILE    *fp;
  char   buff[ 1+STRLEN ];
  size_t  ptr; 
  size_t   sz; 
};

port new_port( FILE *fp ) { 
  port p = calloc( 1, sizeof( *p ) ); 

  p->fp  = fp; 
  p->ptr = 0;
  p->sz  = 0;

  memset( p->buff, 0, 1+STRLEN );
  
  return p; 
}

void del_port( port p ) {
  if ( stdin != p->fp && stdout != p->fp && stderr != p->fp ) {
    fclose( p->fp );
  }
  
  free( p );

  return; 
}

int  pgetc( port p ) {
  return fgetc( p->fp );
  
  if ( p->sz == p->ptr && !feof( p->fp ) ) { 
    char *r = fgets( p->buff, STRLEN, p->fp );
    if ( !r ) return EOF;
    p->sz   = strlen( p->buff );
    if ( 1 < p->sz ) {
      p->sz--;
      p->buff[ p->sz ] = '\0';
    }
    p->ptr  = 0;
  }

  return p->buff[ p->ptr++ ]; 
}

void pungetc( int c, port p ) {
  ungetc( c, p->fp );
  return;
  p->buff[ --p->ptr ] = c; 
}

void pputc( int c, port p ) {
  fputc( c, p->fp );
  if ( '\n' == c ) fflush( p->fp );
  return;
  p->buff[ p->ptr++ ] = c;
  if ( '\n' == c || STRLEN == p->ptr ) { 
    pflush( p );
  }
}

void pflush( port p ) {
  fflush( p->fp );
  return;
  fprintf( p->fp, "%s", p->buff );
  fflush( p->fp );
  memset( p->buff, 0, 1+STRLEN );
  p->ptr = 0; 
}

int  pprintf_va( port p, char *fmt, va_list va ) {

  char  buff[ 1+STRLEN ]; 
  int    ret = vsprintf( buff, fmt, va ); 

  fprintf( p->fp, "%s", buff );

  return ret; 
  
  if ( STRLEN - p->ptr < (unsigned)ret ) { 
    pflush( p ); 
  }

  memcpy( p->buff + p->ptr, buff, ret ); 
  p->ptr += ret; 

  pflush( p ); 

  return ret; 
}

int  pprintf( port p, char *fmt, ... ) { 
  va_list    va; 
  int       ret; 

  va_start( va, fmt ); 

  ret = pprintf_va( p, fmt, va ); 
  
  va_end( va ); 
  
  return ret; 
}
