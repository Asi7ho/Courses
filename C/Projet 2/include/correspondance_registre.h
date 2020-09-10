
#ifndef correspondance_registre_h
#define correspondance_registre_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <structure.h>
#include <notify.h>

/*Charge un dictionnaire de registre pour faire la correspondance entre les formes mnémoniques et les formes numeriques*/
CORR_REG* charge_dico_correspondance(char* nom_fichier, int* nombre_registre);

/*Vérifie si le registre existe*/
CORR_REG recherche_correspondance(CORR_REG* tab, char* token, int nombre_registre);

#endif /* correspondance_registre_h */
