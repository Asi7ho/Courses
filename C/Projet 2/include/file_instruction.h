
#ifndef file_instruction_h
#define file_instruction_h

#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <annexe.h>

/*Créer une file vide*/
FILE_INSTRUCTION creer_file_inst(void);

/*Teste si la file est vide*/
int file_inst_vide(FILE_INSTRUCTION f_i);

void visualiser_file_inst(FILE_INSTRUCTION f_i);

/*Ajoute une INSTRUCTION à la file par la queue*/
FILE_INSTRUCTION enfiler_inst(INSTRUCTION i, FILE_INSTRUCTION f_i);

#endif /* file_instruction_h */
