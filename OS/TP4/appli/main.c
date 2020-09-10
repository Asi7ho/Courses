
/**
 * @file main.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Jan 24 18:13:15 2018
 * @brief Main entry point. 
 *
 * Main entry point. 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "config.h"
#include "shell.h"

/* atexit(3) needs the shell to be
   global (extern qualifier in shell.c) 
*/ 
shell interp; 

int main ( int argc, char *argv[] ) {
  
  interp = new_shell( stdin, stdout, "$ ", "=> " ); 

  atexit( del_shell );

  exit( run_shell( interp ) ); 

  UNUSED( argc );
  UNUSED( argv );
}
