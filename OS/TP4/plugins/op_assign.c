
/**
 * @file op_assign.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Tue Jan 30 13:24:25 2018
 * @brief assign.
 *
 * assign.
 */

#include "api.h"

object op_assign( shell s, object op ) {
  object    args = op_args( op );
  object symbols = car( args );
  object  values = cadr( args ); 

  while ( symbols ) {
    object not_found = shell_ref_var( s, object_string( car( symbols ) ) );
    
    if ( T_ERROR == object_type( not_found ) ) return not_found; 

    if ( T_SYMBOL == object_type( car( values ) ) ) {
      object sym = shell_ref_var( s, object_string( car( values ) ) );
      if ( T_ERROR == object_type( sym ) ) return sym;
      
      shell_update_var( s, object_string( car( symbols ) ), cdr( sym ) );
    }
    else {
      shell_update_var( s, object_string( car( symbols ) ), car( values ) );
    }

    symbols = cdr( symbols ); 
    values  = cdr( values  ); 
  }
  
  return make( T_OK, NULL ); 
  
}
