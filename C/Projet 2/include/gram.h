
#ifndef collection_h
#define collection_h

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <structure.h>
#include <annexe.h>
#include <file_de_data.h>
#include <file_de_bss.h>
#include <file_instruction.h>
#include <dico.h>
#include <notify.h>
#include <table_symbole.h>
#include <relocation.h>


/*Automate qui permet de creer les remplir les files d'instructions, de data et de bss. Elle permet aussi de completer la table des symbole*/
void automate_generale(FILE_LEXEME f, INSTRUCTION_DEFINITION* dico, FILE_DE_DATA* pf_d, FILE_DE_BSS* pf_b, FILE_INSTRUCTION* pf_i, SYMBOLE* tab_symbole, FILE_RELOCATION* pf_rtxt, FILE_RELOCATION* pf_rdata, int nbr_etiquette, int nbr_instructions, CORR_REG* table_registre, int nbr_registre);

#endif /* collection_h */
