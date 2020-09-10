
/**
 * @file op_exit.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Mon Jan 29 21:52:54 2018
 * @brief exit.
 *
 * exit.
 */

#include <stdlib.h>

#include "api.h"

object op_exit( shell s, object op ) { 
  object args = op_args( op ); 
  double  val = object_number( car( args ) );

  /* pprintf( shell_output( s ), "ARGS: " );  */
  /* pprint_object( shell_output( s ), args );  */
  /* pprintf( shell_output( s ), "\n" );  */
  
  pprintf( shell_output( s ), "^D\n" );
  
  exit( (int)val );

  return NULL;
}
