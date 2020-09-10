
/**
 * @file lexem.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 18:22:57 2018
 * @brief Lexems.
 *
 * Lexems.
 */

#ifndef _LEXEM_H_
#define _LEXEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "port.h"

  
  typedef enum {
    EOS,
    NEWLINE,
    EQUAL,
    DOUBLE_EQUAL,
    EXCLAMATION,
    EXCLAM_EQUAL,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    LPAREN,
    RPAREN,
    COMMA,
    SEMICOLON, 
    SYMBOL,
    NUMBER,
    STRING
  } lex_t; 

  struct lexem_t;

  typedef struct lexem_t *lexem;

  lexem read_input_line( port input );
  void  print_lexems( lexem q );
  lexem lexem_next( lexem lex ); 
  int   eat_blanks( port input ); 
  lexem new_lexem( lex_t type, char *val ); 
  void  print_lexem( void *lex );
  void  del_lexem( void *lex );
  lexem next_lexem( port input ); 
  lex_t lexem_type( lexem l );
  char *lexem_val( lexem l );

#ifdef __cplusplus
}
#endif

#endif /* _LEXEM_H_ */
