
/**
 * @file shell.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 17:35:34 2018
 * @brief Shell. 
 *
 * Shell. 
 */


#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include "config.h"
#include "object.h"
#include "lexem.h"
#include "parser.h"
#include "eval.h"

struct shell_t {
  struct termios    settings;
  port                 input; 
  port                output;
  char               *prompt; 
  char                  *ans; 
  object           variables;

  object                true; 
  object               false; 
  
  object               accum; 
  object               stack; 
};

object shell_true( shell s ) {
  return s->true; 
}

object shell_false( shell s ) {
  return s->false; 
}
port   shell_input( shell s ) {
  return s->input;
}

port   shell_output( shell s ) {
  return s->output;
}

char  *shell_prompt( shell s ) {
  return s->prompt;
}

char  *shell_ans( shell s ) {
  return s->ans;
}

object parse_error( char *error ) {
  return make( T_ERROR, error ); 
}

object shell_accum( shell s ) {
  return s->accum; 
}

object shell_variables( shell s ) {
  return s->variables; 
}

void   shell_set_accum( shell s, void *val ) {
  s->accum = val; 
}

shell  new_shell( FILE *input, FILE *output, char *prompt, char *ans ) {
  shell                     s = calloc( 1, sizeof( *s ) ); 

  s->input     = new_port( input ); 
  s->output    = new_port( output ); 
  s->prompt    = strdup( prompt ); 
  s->ans       = strdup( ans ); 
  s->variables = NULL;

  s->true      = make( T_TRUE , NULL ); 
  s->false     = make( T_FALSE, NULL ); 

  s->accum     = make( T_OK, NULL );
  s->stack     = NULL;

  if ( 0 ) { 
    struct termios new_settings;
  
    tcgetattr( 0, &(s->settings) );
    
    new_settings = s->settings; 

    new_settings.c_lflag &= (~ICANON);
    
    new_settings.c_cc[VTIME] = 0; 
    new_settings.c_cc[VMIN]  = 1; 
    
    tcsetattr(0, TCSANOW, &new_settings);
  }
  
  return s; 
}

object shell_def_var( shell s, char *name, object val ) { 

  s->variables = add_symbol( name, val, s->variables ); 

  return val; 
}

object shell_ref_var( shell s, char *name ) { 

  object found = find_symbol( name, s->variables ); 
  
  if ( !found ) { 
    char   error[ 1+STRLEN ];
    sprintf( error, "Unbound variable '%s'.", name ); 
    return parse_error( error );
  }

  s->accum = found; 

  return s->accum; 
}

object shell_update_var( shell s, char *name, void *new_val ) {

  if ( T_ERROR != object_type( s->accum ) ) {
    object found = find_symbol( name, s->variables ); 
  
    if ( !found ) { 
      char   error[ 1+STRLEN ];
      sprintf( error, "Unbound variable '%s'.", name );
      return parse_error( error ); 
    }
    
    update_symbol_value( found, new_val );
  }
  
  return 0; 
}

void   shell_print_vars( shell s ) {
  object vars = s->variables;
  while ( vars ) {
    pprint_object( s->output, caar( vars ) );
    pputc( '\n', s->output ); 
    vars = cdr( vars ); 
  }
}

static void get_real_time( struct timespec *ts ) {

#ifdef __MACH__ 
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif

}

int    run_shell( shell s ) { 

  /* shell_def_var( s, "totor" , make( T_SYMBOL, "a-symbol" ) ); */
  /* shell_def_var( s, "albert", make( T_NUMBER, 3.14 ) ); */

  pprintf( shell_output( s ), "\n * Exit with Ctrl-C | Ctrl-D\n\n" );
  
  while ( 1 ) { 
    struct timespec      now; 
    struct timespec     then; 
    size_t          duration = 0; 
    object              expr = NULL;
    lexem              input = NULL; 

    pprintf( shell_output( s ), "%s", shell_prompt( s ) ); 
    pflush( shell_output( s ) ); 

    input = read_input_line( shell_input( s ) );

    get_real_time( &then );

    expr = parse_expr( s, &input ); 

    expr = eval_expr( s, expr );

    get_real_time( &now );

    duration = (now.tv_sec - then.tv_sec)*1e9 + now.tv_nsec - then.tv_nsec;

    pprintf( shell_output( s ), "\n{%.2fms} ", duration/(double)1e6 ); 
    
    pprintf( shell_output( s ), shell_ans( s ) ); 
    
    pprint_object( shell_output( s ), expr ); 
    
    pputc( '\n', shell_output( s ) );
    pputc( '\n', shell_output( s ) );
  } 

  pputc( '\n', shell_output( s ) );
  
  return EXIT_SUCCESS; 
} 

void del_shell( void ) { 
  extern shell interp;

  if ( 0 ) {
    tcsetattr( 0, TCSANOW, &(interp->settings) );
  }
  
  del_port( interp->input ); 
  del_port( interp->output ); 
  
  free( interp->prompt );  
  free( interp->ans ); 
  free( interp ); 
}

