
/**
 * @file logging.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 13:08:52 2017
 * @brief Logging.
 *
 * Logging.
 */

#ifndef _LOGGING_H_
#define _LOGGING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
  
#include <stdlib.h> /* abort         */ 
#include <stdio.h>  /* FILE*         */ 
#include <errno.h>  /* errno         */ 
#include <string.h> /* strerror      */ 


  FILE *open_log_file( char *log ); 
  int   log_msg( FILE *fp, char *file, int line, char *fmt, ... );

#define LOG(fp, ...) log_msg( fp, __FILE__, __LINE__, __VA_ARGS__ )

#define ABORT(msg)							\
  fprintf( stderr, "\t[%s:%d] %s %s\n",					\
	   __FILE__, __LINE__, msg, strerror( errno ) );		\
  LOG( logfile, " * FATAL (%s: %s)\n",					\
       msg, strerror( errno ) );					\
  delete_lock_file();							\
  abort() 

#ifdef __cplusplus
}
#endif

#endif /* _LOGGING_H_ */
