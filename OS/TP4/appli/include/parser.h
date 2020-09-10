
/**
 * @file parser.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Jan 25 19:04:41 2018
 * @brief Parser.
 *
 * Parser.
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "lexem.h"
#include "shell.h"

  object get_symbol_value( shell s, object sym ); 
  object parse_expr( shell s, lexem *exp );

#ifdef __cplusplus
}
#endif

#endif /* _PARSER_H_ */
