
#include <correspondance_registre.h>

CORR_REG* charge_dico_correspondance(char* nom_fichier, int* nombre_registre){
    
    FILE* f;
    int i;
    char rep_mnemotique[8];
    char rep_chiffre[8];

    CORR_REG* tab;
    
    f = fopen(nom_fichier,"r");
    
    if (f == NULL){
        printf("fichier non trouvé\n");
        return NULL;
    }
    
    if( fscanf(f,"%d",  nombre_registre) != 1){
        printf("nombre de registre non trouvé\n");
        return NULL;
    }
    
    tab=calloc(*nombre_registre,sizeof(*tab));
    
    if(tab == NULL){
        printf("table des registres non creee\n");
        return NULL;
    }
    for (i=0; i<*nombre_registre; i++) {
        if(fscanf(f, "%s %s %d", rep_mnemotique, rep_chiffre, &(tab[i].correspondance)) != 3){
            free(tab);
            return NULL;
        }
        tab[i].representation_mnemotique = strdup(rep_mnemotique);
        tab[i].representation_chiffre = strdup(rep_chiffre);

    }
    fclose(f);
    return tab;
}

CORR_REG recherche_correspondance(CORR_REG* tab, char* token, int nombre_registre){
    
    CORR_REG ptr_recherche;
    int i;
    
    for (i=0; i < nombre_registre; i++){
        
        ptr_recherche = tab[i];
        if(strcmp(token, ptr_recherche.representation_mnemotique) == 0 || strcmp(token, ptr_recherche.representation_chiffre) == 0){
            return ptr_recherche;
        }
    }
    WARNING_MSG("Le registre %s n'existe pas", token);
}
