
#include <file_de_data.h>

FILE_DE_DATA creer_file_data(void){
    return NULL;
}

int file_data_vide(FILE_DE_DATA f_d){
    return f_d == NULL;
}

void visualiser_file_data(FILE_DE_DATA f_d){
    FILE_DE_DATA stk;
     printf("\n--------------------------- FILE DE DATA -------------------------\n");
    if (f_d == NULL){
        printf("FILE VIDE\n");
        return;
    }
    for(stk = f_d->suiv; stk != f_d; stk = stk->suiv){ /* On s'arrete apres un tour */
        printf("[ nom = %s\n etat = %s\n decalage = %08d\n numero de ligne = %d\n ", stk->data.nom, type_string(stk->data.etat), stk->data.decalage, stk->data.ligne);
        switch(stk->data.etat) {
            case DECIMAL:
                printf("valeur = %ld ]", stk->data.valeur.AsNumber);
                break;
                
            case OCTAL:
                printf("valeur = %lo ]", stk->data.valeur.AsNumber);
                break;
                
            case HEXADECIMAL:
                printf("valeur = %lx ]", stk->data.valeur.AsNumber);
                break;
                
            case DECIMAL_ZERO:
                printf("valeur = %ld ]", stk->data.valeur.AsNumber);
                break;
                
            case MOT_OU_INSTRUCTION:
                printf("valeur = %ld ]", stk->data.valeur.AsNumber);
                break;
                
            case STRING:
                printf("valeur = %s ]", stk->data.valeur.AsString);
                break;
        }
        printf("\n\n");
    }
    printf("[ nom = %s\n etat = %s\n decalage = %08d\n numero de ligne = %d\n ", stk->data.nom, type_string(stk->data.etat), stk->data.decalage, stk->data.ligne);
    /* AFFICHER LES VALEURS DE L'UNION*/
    switch(stk->data.etat) {
        case DECIMAL:
            printf("valeur = %ld ]", stk->data.valeur.AsNumber);
            break;
            
        case OCTAL:
            printf("valeur = %lo ]", stk->data.valeur.AsNumber);
            break;
            
        case HEXADECIMAL:
            printf("valeur = %lx ]", stk->data.valeur.AsNumber);
            break;
            
        case DECIMAL_ZERO:
            printf("valeur = %ld ]", stk->data.valeur.AsNumber);
            break;
            
        case MOT_OU_INSTRUCTION:
            printf("valeur = %ld ]", stk->data.valeur.AsNumber);
            break;
            
        case STRING:
            printf("valeur = %s ]", stk->data.valeur.AsString);
            break;
    }
    printf("\n\n");
}

FILE_DE_DATA enfiler_data(DATA_BSS d, FILE_DE_DATA f_d){ /* On enfile par la queue pour pouvoir visualiser les mots dans l'odre */
    FILE_DE_DATA stk = calloc(1, sizeof(*stk));
    if (stk == NULL){
        return NULL;
    }
    stk->data = d;
    if (!(file_data_vide(f_d))){
        stk->suiv = f_d->suiv;
        f_d->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}
