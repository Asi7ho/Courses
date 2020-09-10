
#ifndef file_binaire_h
#define file_binaire_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <notify.h>
#include <global.h>
#include <structure.h>
#include <annexe.h>
#include <dico.h>

void code_binaire_instru(FILE_INSTRUCTION f_i);

void create_file_l(char *file_or, char *file, FILE_INSTRUCTION fi, FILE_DE_DATA fd, FILE_DE_BSS fb, SYMBOLE* tab, int nombre_etiquette, FILE_RELOCATION f_rtxt, FILE_RELOCATION f_rdata);

void create_obj_file(char *file, FILE_INSTRUCTION fi, FILE_DE_DATA fd, FILE_DE_BSS fb);

#endif /* file_binaire_h */
