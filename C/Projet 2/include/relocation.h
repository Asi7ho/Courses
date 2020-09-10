
#ifndef relocation_h
#define relocation_h

#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <annexe.h>

/*Créer une file qui va servir de table de relocation*/
FILE_RELOCATION creer_file_reloc(void);

/*Verifie si la file est vide ou non*/
int file_reloc_vide(FILE_RELOCATION f_r);

void visualiser_file_reloc(FILE_RELOCATION f_r);

/*Enfile une relocation faite dans le code assembleur dans la table de relocation*/
FILE_RELOCATION enfiler_reloc(RELOCATION r, FILE_RELOCATION f_r);


#endif /* relocation_h */
