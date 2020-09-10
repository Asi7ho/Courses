
#ifndef table_symbole_h
#define table_symbole_h

#include <stdio.h>
#include <string.h>
#include <file.h>
#include <structure.h>
#include <stdlib.h>
#include <annexe.h>
#include <dico.h>

/*Retourne le nombre d'étiquette contenu dans le code*/
int detecte_nombre_etiquette(FILE_LEXEME f);

/*Creer et charge la table des symboles en mémoire*/
SYMBOLE* charge_table_symbole(FILE_LEXEME f, INSTRUCTION_DEFINITION* dico, int nombre_etiquette, int nbr_instructions);

void visualiser_table_symbole(SYMBOLE* tab, int nombre_etiquette);

/*Verifie si l'operande est deja déclaré dans la table des symboles*/
int est_dans_la_table(SYMBOLE* tab, LEXEM lexeme, int nombre_etiquette, SYMBOLE* symbole);

#endif /* table_symbole_h */
