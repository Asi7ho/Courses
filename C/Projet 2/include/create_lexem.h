#ifndef type_mot_h
#define type_mot_h

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <structure.h>
#include <notify.h>

/*Fonction qui permet de déterminer l'etat d'un lexeme*/
LEXEM create_lexem_from_token(char* line, int ligne);

#endif /* type_mot_h */
