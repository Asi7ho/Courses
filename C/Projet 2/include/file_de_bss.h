
#ifndef file_de_bss_h
#define file_de_bss_h

#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <annexe.h>

/*Creer une file de donnée qui se trouve dans la section .bss*/
FILE_DE_BSS creer_file_bss(void);

/*Vérifie si la file est vide*/
int file_bss_vide(FILE_DE_BSS f_b);

/*Affiche la file*/
void visualiser_file_bss(FILE_DE_BSS f_b);

/*Permet d'ajouter une donnée de la section .bss à la file*/
FILE_DE_BSS enfiler_bss(DATA_BSS b, FILE_DE_BSS f_b);

#endif /* file_de_bss_h */
