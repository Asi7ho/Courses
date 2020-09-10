
/**
 * @file eval.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Jan 27 15:45:54 2018
 * @brief Evaluator.
 *
 * Evaluator.
 */

#include <string.h>
#include <dlfcn.h>

#include "config.h"
#include "eval.h"
#include "object.h"

typedef object(*op_code)( shell, object );

object eval_operator( shell s, object op ) {
  char     plugfile[ 1+STRLEN ];
  char      plugfun[ 1+STRLEN ];
  void    *dlhandle = NULL;
  op_code   execute;
  object     result = NULL;
  char      *op_str = op_name( op );

  /* TODO: Construct plugin file name. Use sprintf.
     Plugins are located in the PLUGINS_DIR directory.
     The plugin function name is op_str.
  */

  sprintf(plugfile, "../plugins/op_%s.so",op_str);

  //printf("LIB DYN : %s\n", plugfile);

  /* TODO: Open plugin file. On error, return an error object
     with make( T_ERROR, char *err_str ).
  */
  dlhandle = dlopen(plugfile, RTLD_LAZY);
  if (dlhandle == NULL){
    return make(T_ERROR, dlerror());
  }

  /* TODO: Construct plugin function name. */
  sprintf(plugfun, "op_%s", op_str);
  printf("FCT : %s\n", plugfun);

  /* TODO: Get pointer to plugin function in 'execute'. */
  execute = dlsym(dlhandle, plugfun);
  if (execute == NULL) {
    return make(T_ERROR, dlerror());
  }
  /* TODO: Execute plugin function pointer. Store result
     in 'result' (rather odd, isn't it?). The parameters
     are 's' and 'op'.
  */
  result = execute(s, op);

  /* TODO: Close plugin handler. */
  dlclose(dlhandle);
  return result;
}

object eval_expr( shell s, object expr ) {

  object found = NULL;

  switch( object_type( expr ) ) {

  case T_OPERATOR :
    return eval_operator( s, expr );

  case T_SYMBOL :
    found = find_symbol( object_string( expr ), shell_variables( s ) );

    if ( !found ) {
      char   error[ 1+STRLEN ];
      sprintf( error, "Unbound variable '%s'.", object_string( expr ) );
      return parse_error( error );
    }

    return cdr( found );

  default :
    break;
  }

  return expr;
}
