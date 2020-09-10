
#ifndef pseudo_instruction_h
#define pseudo_instruction_h

#include <stdio.h>
#include <stdlib.h>
#include <file_instruction.h>

/*Remplace la file contenant les pseudo instructions par les instructions lues par l'assembleur*/
FILE_INSTRUCTION pseudo_instruction(FILE_INSTRUCTION f_i);

#endif /* pseudo_instruction_h */
