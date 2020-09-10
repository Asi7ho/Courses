
/**
 * @file eval.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Jan 27 15:46:06 2018
 * @brief Evaluator.
 *
 * Evaluator.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "shell.h"
#include "object.h"

  object eval_expr( shell s, object expr );
  


#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
