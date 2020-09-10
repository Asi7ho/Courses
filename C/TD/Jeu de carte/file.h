#ifndef _FILE
#define _FILE
#include <stdio.h>
#include "carte.h"
#include "liste.h"

#include <stdio.h>


typedef Liste File;

File creer_file(void); //Créer une file vide
int file_vide(File f); //Teste si la file est vide
void visualiser_file(File f);
File enfiler(CARTE c, File f); //Ajoute une carte à la file par la queue
CARTE defiler(File* pf); //Renvoie la carte en tete de file eT la supprime de la file

#endif /* _FILE */
