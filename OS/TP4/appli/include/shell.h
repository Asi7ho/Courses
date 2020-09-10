
/**
 * @file shell.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 17:33:52 2018
 * @brief Shell.
 *
 * Shell.
 */

#ifndef _SHELL_H_
#define _SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "object.h"

  struct shell_t;

  typedef struct shell_t *shell;

  shell  new_shell( FILE *input, FILE *output, char *prompt, char *ans ); 
  object shell_def_var( shell s, char *name, object val ); 
  object shell_ref_var( shell s, char *name );
  object shell_update_var( shell s, char *name, void *new_val );
  void   shell_print_vars( shell s ); 
  int    run_shell( shell s );
  void   del_shell( void ); 
  void   shell_print_vars( shell s ); 
  port   shell_input( shell s );
  port   shell_output( shell s );
  char  *shell_prompt( shell s ); 
  char  *shell_ans( shell s ); 
  object parse_error( char *error );
  void   shell_print_parse_error( shell s );
  object shell_accum( shell s ); 
  object shell_variables( shell s ); 
  void   shell_set_accum( shell s, void *val ); 
  void   shell_print_accum( shell s );
  object shell_true( shell s );
  object shell_false( shell s );
  
#ifdef __cplusplus
}
#endif

#endif /* _SHELL_H_ */
