
/**
 * @file event.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Dec 28 12:54:42 2017
 * @brief Events.
 *
 * Events.
 */

#include "config.h"

#include <time.h>   /* time_t */
#include <ctype.h>
#include <string.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* free   */

#include "logging.h"
#include "event.h"

/* Forward declaration */
char *strptime(const char *s, const char *format, struct tm *tm);

struct event_t {
  time_t      when;
  char    *command; 
};

time_t event_date( event ev ) {
  return ev->when; 
}

char  *event_command( event ev ) {
  return ev->command; 
}

static int   event_comp( void *e1, void *e2 ) {
  event ev1 = e1; 
  event ev2 = e2;

  if ( !ev1 || !ev2 ) return 1;
  
  return (int)(ev1->when - ev2->when); 
}
  
static event new_event( time_t when, char *command ) {
  event    e = calloc( 1, sizeof( *e ) );

  e->when    = when;
  e->command = strdup( command );

  return e; 
}

void  del_event( void *e ) {
  event ev = e; 
  free( ev->command ); 
  free( ev ); 
}

void  print_event( void *e ) {
  event             ev = e; 
  char       time_buff[1+STRLEN];
  struct tm       *tmp = NULL;

  if ( !ev ) return; 
    
  tmp = localtime( &(ev->when) );

  if ( !tmp ) {
    ABORT( "localtime" ); 
  }

  if ( 0 == strftime( time_buff, sizeof( time_buff ), "%a, %d %b %Y %H:%M:%S", tmp ) ) {
    ABORT( "strftime" ); 
  }

  printf( "%s -> %s\n", time_buff, ev->command );
}
  
static list  skip_past( list events ) {
  time_t now = time( NULL );

  while( !empty( events ) && event_date( car( events ) ) < now ) {
    list n = cdr( events ); 
    LOG( logfile, "* Skipping event %ds in the past: %s.\n",
	 now-event_date( car( events ) ), 
	 event_command( car( events ) ) );
    del_event( car( events ) );
    free( events );
    events = n; 
  }

  return events; 
}

list  read_events( char *file ) {
  FILE     *fp = fopen( file, "rt" );
  char    buff[1+STRLEN];
  list  events = nil;

  if ( !fp ) {
    ABORT( file ); 
  }

  while( fgets( buff, STRLEN, fp ) ) {
    struct tm       tm;
    time_t       today = time( NULL ); 
    char      *command = NULL;

    buff[ strlen( buff ) - 1 ] = '\0';

    memset( &tm, 0, sizeof( struct tm ) );

    localtime_r( &today, &tm );
    
    command = strptime( buff, "%H:%M:%S", &tm );

    if ( !command ) {
      ABORT( "strptime" );
    }

    while( isblank( *(command++) ) ); command--; 

    events = consort( new_event( mktime( &tm ), command ), events, event_comp ); 
  }

  fclose( fp );

  return skip_past( events ); 
}

