
/**
 * @file lexem.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 18:25:10 2018
 * @brief Lexems.
 *
 * Lexems.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "config.h"
#include "lexem.h"

struct lexem_t {
  lex_t            type; 
  char             *val; 
  struct lexem_t  *next; 
};

lex_t lexem_type( lexem l ) {
  return l->type; 
}

char *lexem_val( lexem l ) {
  return l->val; 
}

lexem lexem_next( lexem lex ) { 
  return lex->next; 
}

lexem new_lexem( lex_t type, char *val ) {
  lexem l = calloc( 1, sizeof( *l ) );

  l->type = type;
  l->val  = strdup( val );

  l->next = l; 
  
  return l; 
}

int eat_blanks( port input ) { 
  int c = -1;

  c = pgetc( input ); 

  while ( isblank( c ) ) c = pgetc( input ); 

  return c; 
}

void  print_lexem( void *lex ) { 
  lexem l = lex;

  printf( "%s\n", l->val );
  fflush( stdout );
}

void  del_lexem( void *lex ) { 
  free( ((lexem)lex)->val ); 
  free( lex ); 
}

static int is_end( char c ) {
  switch ( c ) {
  case EOF  : 
  case '\n' : 
  case ';'  : 
  case '('  : 
  case ')'  : 
  case ','  : 
  case '='  :
  case '+'  :
  case '-'  :
  case '*'  :
  case '/'  :
  case '!'  :
    return 1; 
  default :
    return 0; 
  }
}

lexem next_lexem( port input ) { 
  int        c = EOF;
  char    buff[ 1+STRLEN ];
  size_t     n = 0;
  int     peek = EOF;

  memset( buff, 0, 1+STRLEN );

 restart :
  
  c = pgetc( input ); 

  switch( c ) {
  case ' '  :
  case '\t' : goto restart; 
  case EOF  : return new_lexem( EOS, "end-of-stream" ); 
  case ';'  : return new_lexem( SEMICOLON, ";" ); 
  case '\n' : return new_lexem( NEWLINE, "\n" );
  case '='  :
    peek = pgetc( input );
    if ( '=' == peek ) return new_lexem( DOUBLE_EQUAL, "==" );
    else {
      pungetc( peek, input );
      return new_lexem( EQUAL, "=" );
    }
  case '!'  :
    peek = pgetc( input );
    if ( '=' == peek ) return new_lexem( EXCLAM_EQUAL, "==" );
    else {
      pungetc( peek, input );
      return new_lexem( EXCLAMATION, "!" );
    }
    return new_lexem( EXCLAMATION, "!" ); 
  case '('  : return new_lexem( LPAREN, "(" ); 
  case ')'  : return new_lexem( RPAREN, ")" ); 
  case ','  : return new_lexem( COMMA, "," ); 
  case '+'  : return new_lexem( PLUS, "+" ); 
  case '-'  : return new_lexem( MINUS, "-" ); 
  case '*'  : return new_lexem( TIMES, "*" ); 
  case '/'  : return new_lexem( DIV, "/" ); 
  default   : break; 
  }

  if ( '"' == c ) {
    c = pgetc( input );
    
    while( EOF != c && '"' != c ) { 
      buff[ n++ ] = c;
      c = pgetc( input );
    }

    return new_lexem( STRING, buff ); 
  }

  if ( '+' == c || '-' == c || '.' == c || isdigit( c ) ) {
    do {
      buff[ n++ ] = c;
      c = pgetc( input );
      
    } while ( !isblank( c ) && !is_end( c ) );

    if ( is_end( c ) ) pungetc( c, input );

    return new_lexem( NUMBER, buff ); 
  }

  while ( !isblank( c ) &&
	  !is_end( c ) ) {     
    buff[ n++ ] = c; 
    c = pgetc( input ); 
  }

  if ( is_end( c ) ) pungetc( c, input );

  return new_lexem( SYMBOL, buff ); 
}

lexem read_input_line( port input ) {
  lexem q = next_lexem( input );
  lexem n = NULL;

  while ( 1 ) { 
    switch ( lexem_type( q ) ) {
    case EOS : 
    case NEWLINE :

      n = q->next; 

      q->next = NULL; 
  
      return n; 

    default :
      n = next_lexem( input );

      n->next = q->next;
      q->next = n;
      q = n; 
    }
  }
}
  
void print_lexems( lexem q ) {

  while( q ) {
    print_lexem( q );

    q = q->next; 
  }

  return; 
}
