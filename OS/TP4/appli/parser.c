
/**
 * @file parser.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 19:07:01 2018
 * @brief Parser.
 *
 * Parser.
 */

#include <stdlib.h>
#include <string.h>


#include "config.h"
#include "parser.h"
#include "lexem.h"
#include "eval.h"


object get_symbol_value( shell s, object sym ) {

  if ( T_SYMBOL == object_type( sym ) ) {
    object found = find_symbol( object_string( sym ), shell_variables( s ) );
    
    if ( !found ) {
      char error[ 1+STRLEN ];
      sprintf( error, "Unbound variable '%s'.", object_string( sym ) ); 
      return parse_error(  error ); 
    }
    
    return cdr( found ); 
  }

  return sym; 
}

object parse_declare( shell s, lexem *varval ) {
  object symbols = NULL; 
  object  values = NULL;
  object     val = NULL; 

  /* object     sym = symbols; */

  /* while ( sym ) { */
  /*   /\* Cheating on Scheme's let* *\/ */
  /*   pprintf( shell_output( s ), "DEFINING %s\n", object_string( sym ) ); */
  /*   shell_def_var( s, object_string( sym ), make( T_NIL, NULL ) ); */
    
  /*   sym = cdr( sym ); */
  /* } */
  
  while ( 1 ) { 

    if ( SEMICOLON == lexem_type( *varval ) ) { 
      return car( values ); 
    }
    
    if ( SYMBOL != lexem_type( *varval ) ) {
      return parse_error( "Expected symbol for variable declaration." );
    }

    symbols   = cons( make( T_SYMBOL, lexem_val( *varval ) ), symbols );
    
    *varval   = lexem_next( *varval );
    
    switch( lexem_type( *varval ) ) {
    case SEMICOLON : 
      values  = cons( make( T_NIL, NULL ), values ); 
      break; 
    case COMMA :
      *varval = lexem_next( *varval ); 
      values  = cons( make( T_NIL, NULL ), values ); 

      shell_def_var( s, object_string( car( symbols ) ), car( values ) );
    
      break; 
    case EQUAL :
      *varval = lexem_next( *varval );
      val = parse_expr( s, varval ); 
      if ( T_SYMBOL != object_type( val ) ) { 
	values  = cons( eval_expr( s, val ), values );
      }
      else {
	values  = cons( val, values );
      }

      if ( T_ERROR == object_type( car( values ) ) ) return car( values );

      shell_def_var( s, object_string( car( symbols ) ), car( values ) );
    
      if ( COMMA != lexem_type( *varval ) && SEMICOLON != lexem_type( *varval ) ) {
	return parse_error( "Unexpected input after variable value." );
      }

      if ( COMMA == lexem_type( *varval ) ) {
	*varval = lexem_next( *varval );
      }
      break; 
    default : return parse_error( "Unexpected input." );
    }
    
  }
}

object parse_expr( shell s, lexem *lexems ) { 
  lexem    symbol = NULL;
  object     args = NULL;
  double      num = 0.;
  
  switch( lexem_type( *lexems ) ) {
  case EXCLAMATION :
    *lexems = lexem_next( *lexems ); 
    return make( T_OPERATOR, "not", cons( eval_expr( s, parse_expr( s, lexems ) ), NULL ) );
  case SYMBOL :
    symbol = *lexems;

    if ( !strcmp( "var", lexem_val( symbol ) ) ) {
      *lexems = lexem_next( *lexems );
      return parse_declare( s, lexems ); 
    }
    
    if ( !strcmp( "true", lexem_val( symbol ) ) ) {
      *lexems = lexem_next( *lexems );

      if ( DOUBLE_EQUAL == lexem_type( *lexems ) ) {
	*lexems = lexem_next( lexem_next( *lexems ) ); 
	args    = eval_expr( s, parse_expr( s, lexems ) );
	return make( T_OPERATOR, "equals", cons( cons( shell_true(s), NULL ), cons( cons( args, NULL ), NULL ) ) ); 
      }
      else if ( EXCLAM_EQUAL == lexem_type( *lexems ) ) {
	*lexems = lexem_next( lexem_next( *lexems ) ); 
	args    = eval_expr( s, parse_expr( s, lexems ) );
	return make( T_OPERATOR, "not", make( T_OPERATOR, "equals", cons( cons( shell_true(s), NULL ), cons( cons( args, NULL ), NULL ) ) ) ); 
      }
      else return shell_true(s); 
    }
    
    if ( !strcmp( "false", lexem_val( symbol ) ) ) {
      *lexems = lexem_next( *lexems );

      if ( DOUBLE_EQUAL == lexem_type( *lexems ) ) {
	*lexems = lexem_next( lexem_next( *lexems ) ); 
	args    = eval_expr( s, parse_expr( s, lexems ) );
	return make( T_OPERATOR, "equals", cons( cons( shell_false(s), NULL ), cons( cons( args, NULL ), NULL ) ) ); 
      }
      else if ( EXCLAM_EQUAL == lexem_type( *lexems ) ) {
	*lexems = lexem_next( lexem_next( *lexems ) ); 
	args    = eval_expr( s, parse_expr( s, lexems ) );
	return make( T_OPERATOR, "not", make( T_OPERATOR, "equals", cons( cons( shell_false(s), NULL ), cons( cons( args, NULL ), NULL ) ) ) ); 
      }
      else return shell_false(s); 
    }
    
    switch( lexem_type( lexem_next( *lexems ) ) ) {

    case DOUBLE_EQUAL :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      args    = parse_expr( s, lexems );
      return make( T_OPERATOR, "equals", cons( cons( make( T_SYMBOL, lexem_val( symbol ) ), NULL ), cons( cons( args, NULL ), NULL ) ) ); 
      break;    

    case EQUAL :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      args    = eval_expr( s, parse_expr( s, lexems ) );
      return make( T_OPERATOR, "assign", cons( cons( make( T_SYMBOL, lexem_val( symbol ) ), NULL ), cons( cons( args, NULL ), NULL ) ) );

    case LPAREN :
      *lexems = lexem_next( *lexems );
      *lexems = lexem_next( *lexems );
      while ( RPAREN != lexem_type( *lexems ) ) {
	object arg = eval_expr( s, parse_expr( s, lexems ) ); 
	args = cons( arg, args );

	if ( COMMA != lexem_type( *lexems ) && RPAREN != lexem_type( *lexems ) ) {
	  char error[ 1+STRLEN ];
	  sprintf( error, "Missing comma before %s.", lexem_val( *lexems ) );
	  return parse_error( error );
	}
	if ( RPAREN == lexem_type( *lexems ) ) break; 
	*lexems= lexem_next( *lexems );
      }

      if ( NULL == args ) {
	args = cons( NULL, NULL );
      }      
      else
	if ( args && T_LIST != object_type( args ) ) args = cons( args, NULL );

      *lexems = lexem_next( *lexems );
      return make( T_OPERATOR, lexem_val( symbol ), list_reverse( args ) );

    case PLUS :
      *lexems = lexem_next( lexem_next( *lexems ) );       
      return make( T_OPERATOR, "plus", make( T_SYMBOL, lexem_val( symbol ) ), parse_expr( s, lexems ) );
      
    case MINUS :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      return make( T_OPERATOR, "minus", make( T_SYMBOL, lexem_val( symbol ) ), parse_expr( s, lexems ) );
      
    case TIMES :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      return make( T_OPERATOR, "mult", cons( make( T_SYMBOL, lexem_val( symbol ) ), cons( parse_expr( s, lexems ), NULL ) ) );
      
    case DIV :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      return make( T_OPERATOR, "div", make( T_SYMBOL, lexem_val( symbol ) ), parse_expr( s, lexems ) );
      
    default : 
      *lexems = lexem_next( *lexems ); 
      return make( T_SYMBOL, lexem_val( symbol ) );
    }
    
    break;
  case NUMBER :
    num = atof( lexem_val( *lexems ) ); 
    switch( lexem_type( lexem_next( *lexems ) ) ) {
      
    case DOUBLE_EQUAL :
      *lexems = lexem_next( lexem_next( *lexems ) ); 
      args    = eval_expr( s, parse_expr( s, lexems ) );
      return make( T_OPERATOR, "equals", cons( cons( make( T_NUMBER, num ), NULL ), cons( cons( args, NULL ), NULL ) ) ); 

    case PLUS :
      *lexems = lexem_next( *lexems ); 
      *lexems = lexem_next( *lexems ); 
      return make( T_OPERATOR, "plus", make( T_NUMBER, num ), parse_expr( s, lexems ) );
      
    case MINUS :
      *lexems = lexem_next( *lexems ); 
      *lexems = lexem_next( *lexems ); 
      return make( T_OPERATOR, "minus", make( T_NUMBER, num ), parse_expr( s, lexems ) );
      
    case TIMES :
      *lexems = lexem_next( *lexems ); 
      *lexems = lexem_next( *lexems ); 
      return make( T_OPERATOR, "mult", cons( make( T_NUMBER, num ), cons( parse_expr( s, lexems ), NULL ) ) );
      
    case DIV :
      *lexems = lexem_next( *lexems ); 
      *lexems = lexem_next( *lexems ); 
      return make( T_OPERATOR, "div", make( T_NUMBER, num ), parse_expr( s, lexems ) );

    default :
      *lexems = lexem_next( *lexems ); 
      return make( T_NUMBER, num ); 
    }

  case STRING :
    symbol = *lexems;
    *lexems = lexem_next( *lexems );
    return make( T_STRING, lexem_val( symbol ) );
  case NEWLINE :
    *lexems = lexem_next( *lexems );
    return make( T_OK, NULL );
  case EOS :
    return make( T_OPERATOR, "exit", cons( make( T_NUMBER, 0.0 ), NULL ) );
  default :
    pprintf( shell_output( s ), "GOT: %s\n", lexem_val( *lexems ) );

    return parse_error( "Unexpected input." );
  }
  
  /* fprintf( stderr, "LEXEMS START:\n" ); */
  /* print_lexems( lexems ); */
  /* fflush( stdout ); */
  /* fprintf( stderr, "LEXEMS STOP\n" ); */

  return parse_error( "Parse error." ); 
}

/* object parse_expr_save( shell s ) {  */

/*   port   input = shell_input( s );  */
/*   lexem    lex = NULL;  */

/*  restart : */
  
/*   lex = next_lexem( input );  */
  
/*   switch( lexem_type( lex ) ) { */

/*   case SEMICOLON : */
/*     return shell_accum( s );  */
    
/*   case SYMBOL  : */
/*     if ( !strcmp( "var", lexem_val( lex ) ) ) {  */
/*       object n = NULL; */

/*       lex = next_lexem( input ); */

/*       if ( SEMICOLON == lexem_type( lex ) ) return 0;  */
  
/*       if ( SYMBOL != lexem_type( lex ) ) {  */
/* 	return parse_error( "Expected symbol after var keyword." ); */
/*       } */

/*       n   = make( T_SYMBOL, lexem_val( lex ) );  */

/*       lex = next_lexem( input ); */
      
/*       if ( EQUAL != lexem_type( lex ) && */
/* 	   SEMICOLON != lexem_type( lex ) ) {  */
/* 	return parse_error( "Expected = or ; after symbol." );  */
/*       } */

/*       if ( SEMICOLON == lexem_type( lex ) ) {  */
/* 	object nil = make( T_NIL, NULL ); */
	
/* 	shell_def_var( s, object_string( n ), nil ); */
/* 	shell_set_accum( s, nil ); */

/* 	return 0; */
/*       } */

/*       shell_def_var( s, object_string( n ), NULL );  */

/*       shell_set_accum( s, n );  */
      
/*       parse_assignment( s ); */

/*       return shell_accum( s );  */
/*     } */
/*     else { */
/*       shell_set_accum( s, make( T_SYMBOL, lexem_val( lex ) ) );  */
/*     } */
/*     break; */
    
/*   case EQUAL : */
/*     return parse_assignment( s );  */

/*     break; */
/*   case NUMBER : */
/*     shell_set_accum( s, make( T_NUMBER, lexem_val( lex ) ) ); */
/*     break;  */
/*   case STRING : */
/*     shell_set_accum( s, make( T_STRING, lexem_val( lex ) ) );  */
/*     break; */
/*   default : */
/*     break;  */
/*   } */
  
/*   goto restart;  */
/* } */
