
#ifndef charge_dico_h
#define charge_dico_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <structure.h>
#include <notify.h>
#include <correspondance_registre.h>
#include <limits.h>

/*Permet de charger le dictionnaire des instruction en memoire*/
INSTRUCTION_DEFINITION* charge_dico(char* nom_fichier, int* nombre_instruction);

/*Verifie si l'instruction existe*/
INSTRUCTION_DEFINITION* recherche_dico(INSTRUCTION_DEFINITION* tab, LEXEM lexeme, int nombre_instruction);

/*Verifie si les opérandes sont conformes à ce que dit le dictionnaire*/
int verification_operande(CORR_REG* table_registre, char* type_operande, LEXEM lexeme, int nbr_registre);

/*Verifie si une operande est prédeterminé ou non*/
int verification_dico(char* operande);

#endif /* charge_dico_h */
