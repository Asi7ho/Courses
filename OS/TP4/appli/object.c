
/**
 * @file object.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 22:13:25 2018
 * @brief Objects.
 *
 * Objects.
 */

#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "mat.h"
#include "port.h"
#include "object.h"

typedef struct function_t {
  void  *apply; 
} *function; 

struct object_t { 
  type_t                   t;
  
  union {
    char                *str;
    double            number;
    struct { 
      struct object_t   *car; 
      struct object_t   *cdr;
    }                   list;
    struct {
      struct object_t  *sons; 
      struct object_t  *bros;
    }                   tree;
    struct {
      char             *name;
      struct object_t  *args;
    }                     op;
    port                port;
    vec                  vec;
    mat                  mat; 
  }                        _;
  
};

char  *op_name( object op ) {
  return op->_.op.name;
}

object op_args( object op ) {
  return car( op->_.op.args );
}

object car( object list ) {
  return list->_.list.car; 
}

object cdr( object list ) {
  return list->_.list.cdr; 
}

object cons( object ca, object cd ) { 
  
  object n = make( T_LIST, NULL ); 

  n->_.list.car = ca; 
  n->_.list.cdr = cd; 
  
  return n; 
}

type_t object_type( object obj ) {
  return obj->t; 
}

double object_number( object obj ) {
  return obj->_.number; 
}

char  *object_string( object obj ) {
  return obj->_.str; 
}

vec    object_vec( object obj ) {
  return obj->_.vec; 
}

mat    object_mat( object obj ) {
  return obj->_.mat; 
}

object make__( type_t type, ... ) {
  va_list   va; 
  object   arg = NULL; 
  object   obj = calloc( 1, sizeof( *obj ) );
  
  va_start( va, type );

  obj->t     = type;

  switch( obj->t ) {
  case T_TRUE   :
  case T_FALSE  :
  case T_OK     :
    break;
  case T_NIL    :
    obj->_.str = NULL; 
    break; 
  case T_SYMBOL :
  case T_STRING :
  case T_ERROR  :
    obj->_.str = strdup( va_arg( va, char* ) ); 
    break; 
  case T_INPUT_PORT :
    obj->_.port = new_port( fopen( va_arg( va, char* ), "rt" ) ); 
    break;
  case T_PORT :
  case T_OUTPUT_PORT :
    obj->_.port = new_port( fopen( va_arg( va, char* ), "wt" ) ); 
    break;
  case T_NUMBER :
    obj->_.number = va_arg( va, double ); 
    break;
  case T_VEC :
    obj->_.vec = va_arg( va, vec ); 
    break; 
  case T_MAT :
    obj->_.mat = va_arg( va, mat ); 
    break; 
  case T_LIST :
    obj->_.list.car = NULL; 
    obj->_.list.cdr = NULL; 
    break; 
  case T_TREE :
    obj->_.tree.sons = NULL; 
    obj->_.tree.bros = NULL; 
    break; 
  case T_OPERATOR :
    obj->_.op.name = strdup( va_arg( va, char* ) );
    arg = va_arg( va, object ); 
    while ( arg ) {
      obj->_.op.args = cons( arg, obj->_.op.args );
      
      arg = va_arg( va, object ); 
    }
    break; 
  }
  
  va_end( va );

  if ( T_INPUT_PORT == obj->t || T_OUTPUT_PORT == obj->t ) {
    obj->t = T_PORT; 
  }


  return obj; 
}

static int    pprint_pair( port p, object obj ) {
  if ( !obj ) {
    return 0;
  }
  else { 
    int ret = pprint_object( p, car( obj ) );
    ret += pprintf( p, " " );
    if ( cdr( obj ) && T_LIST == object_type( cdr( obj ) ) ) {
      return ret + pprint_pair( p, cdr( obj ) );
    }
    else {
      if ( cdr( obj ) ) {
	ret += pprintf( p, ". " );
	return ret + pprint_object( p, cdr( obj ) );
      }
      return pprint_pair( p, cdr( obj ) );
    }
  }
}

int    pprint_object( port p, object obj ) { 

  int ret = 0; 
  
  if ( !obj ) return ret; 
  
  switch( object_type( obj ) ) {
  case T_TRUE :
    return pprintf( p, "true" ); 
  case T_FALSE :
    return pprintf( p, "false" ); 
  case T_OK :
    return pprintf( p, "ok" ); 
  case T_ERROR :
    return pprintf( p, "ERROR: %s", object_string( obj ) ); 
  case T_NIL :
    return pprintf( p, "nil" );
  case T_OPERATOR :
    ret += pprintf( p, "#<%s>", obj->_.op.name );
    ret += pprint_pair( p, obj->_.op.args ); 
    return ret;
  case T_VEC :
    return pprint_vec( p, obj->_.vec ); 
  case T_MAT :
    return pprint_mat( p, obj->_.mat ); 
  case T_LIST :
    ret += pprintf( p, "( " ); 
    ret += pprint_pair( p, obj ); 
    ret += pprintf( p, ")" ); 
    return ret; 
  case T_SYMBOL :
    return pprintf( p, "%s", obj->_.str ); 
  case T_NUMBER :
    return pprintf( p, "%lf", obj->_.number ); 
  case T_STRING :
    return pprintf( p, "\"%s\"", obj->_.str );
  default :
    return pprintf( p,"#<unknown>" );
  }
}


object find_symbol( char *name, object list ) {
  while ( list ) {
    if ( !strcmp( name, object_string( caar( list ) ) ) ) return car( list ); 
    list = cdr( list );
  }

  return NULL; 
}

object add_symbol( char *sym, object val, object syms ) {
  return cons( cons( make( T_SYMBOL, sym ), val ), syms ); 
}

void   update_symbol_value( object sym, object new_val ) {
  sym->_.list.cdr = new_val; 
  return; 
}

size_t list_length( object list ) {
  size_t n = 0;
  
  while( list ) {
    n++;
    list = cdr( list ); 
  }

  return n; 
}


object list_reverse( object list ) {
  object rev = NULL;

  while ( list ) {
    rev = cons( car( list ), rev );
    list = cdr( list );
  }

  return rev; 
}

