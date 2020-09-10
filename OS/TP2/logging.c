
/**
 * @file logging.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Fri Dec 29 09:48:51 2017
 * @brief Daemon logging.
 *
 * Daemon logging.
 */

#include "config.h"
#include "logging.h"

#include <stdio.h>  /* *printf */
#include <stdarg.h>
#include <unistd.h> /* getpid  */
#include <time.h>   /* time    */
#include <string.h> /* strlen  */


static int valog_msg( FILE *fp, char *file, int line, char *fmt, va_list va ) {
  char    buff[1+STRLEN];
  time_t   now = time( NULL ); 
  int      ret = snprintf( buff, sizeof(buff)-1,
			   "==%d== {%10s:%3d} [%s", getpid(), file, line, ctime( &now ) );

  buff[ strlen( buff )-1 ] = '\0'; 
  
  fprintf( fp, "%s]  ", buff );

  ret += vfprintf( fp, fmt, va );

  fflush( fp ); 

  return ret; 
}

int   log_msg( FILE *fp, char *file, int line, char *fmt, ... ) { 
  va_list  va;
  int     ret; 

  va_start( va, fmt );

  ret = valog_msg( fp, file, line, fmt, va ); 

  va_end( va );

  return ret; 
}

FILE *open_log_file( char *log ) {
  FILE *fp = fopen( log, "at" );

  if ( !fp ) {
    ABORT( log );
  }

  return fp; 
}

