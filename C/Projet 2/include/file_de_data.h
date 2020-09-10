

#ifndef file_de_data_h
#define file_de_data_h

#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <annexe.h>

/*Créer une file vide*/
FILE_DE_DATA creer_file_data(void);

/*Teste si la file est vide*/
int file_data_vide(FILE_DE_DATA f_d);

void visualiser_file_data(FILE_DE_DATA f_d);

/*Ajoute un DATA à la file par la queue*/
FILE_DE_DATA enfiler_data(DATA_BSS data, FILE_DE_DATA f_d);

#endif /* file_de_data_h */
