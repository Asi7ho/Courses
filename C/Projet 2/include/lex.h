/**
 * @file lex.h
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexem-related stuff.
 *
 * Contains lexem types definitions, some low-level syntax error codes,
 * the lexem structure definition and the associated prototypes.
 */

#ifndef _LEX_H_
#define _LEX_H_


#include <stdio.h>
#include <structure.h>
#include <global.h>
#include <string.h>
#include <create_lexem.h>
#include <file.h>
#include <notify.h>

void lex_read_line( char *, int, FILE_LEXEME*);
void lex_load_file( char *, unsigned int *, FILE_LEXEME*);
void lex_standardise( char*, char*  );

#endif /* _LEX_H_ */

