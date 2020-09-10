#ifndef _FILE
#define _FILE

#include <stdio.h>
#include <stdlib.h>
#include <annexe.h>
#include <structure.h>

/*Créer une file vide*/
FILE_LEXEME creer_file(void);

/*Teste si la file est vide*/
int file_vide(FILE_LEXEME f);

void visualiser_file(FILE_LEXEME f);

/*Ajoute un LEXEME à la file par la queue*/
FILE_LEXEME enfiler(LEXEM l, FILE_LEXEME f);

#endif /* _FILE */
