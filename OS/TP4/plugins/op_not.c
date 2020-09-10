
/**
 * @file op_not.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 16:10:44 2018
 * @brief not.
 *
 * not.
 */

#include "api.h"


object op_not( shell s, object op ) { 
  object    args = op_args( op );

  if ( shell_true( s ) != car( args ) ) return shell_true( s );
  
  return shell_false( s ); 
}
