
#include <file_de_bss.h>

FILE_DE_BSS creer_file_bss(void){
    return NULL;
}

int file_bss_vide(FILE_DE_BSS f_b){
    return f_b == NULL;
}

void visualiser_file_bss(FILE_DE_BSS f_b){
    FILE_DE_BSS stk;
    printf("\n--------------------------- FILE DE BSS -------------------------\n");
    if (f_b == NULL){
        printf("FILE VIDE\n");
        return;
    }
    for(stk = f_b->suiv; stk != f_b; stk = stk->suiv){ /* On s'arrete apres un tour */
        printf("[ nom = %s\n etat = %s\n decalage = %08d\n numero de ligne = %d\n ", stk->bss.nom, type_string(stk->bss.etat), stk->bss.decalage, stk->bss.ligne);
        switch(stk->bss.etat) {
            case DECIMAL:
                printf("valeur = %ld ]", stk->bss.valeur.AsNumber);
                break;
                
            case OCTAL:
                printf("valeur = %lo ]", stk->bss.valeur.AsNumber);
                break;
                
            case HEXADECIMAL:
                printf("valeur = %lx ]", stk->bss.valeur.AsNumber);
                break;
                
            case DECIMAL_ZERO:
                printf("valeur = %ld ]", stk->bss.valeur.AsNumber);
                break;
                
            case STRING:
                printf("valeur = %s ]", stk->bss.valeur.AsString);
                break;
        }
        printf("\n\n");
    }
    printf("[ nom = %s\n etat = %s\n decalage = %08d\n numero de ligne = %d\n ", stk->bss.nom, type_string(stk->bss.etat), stk->bss.decalage, stk->bss.ligne);
    switch(stk->bss.etat) {
        case DECIMAL:
            printf("valeur = %ld ]", stk->bss.valeur.AsNumber);
            break;
            
        case OCTAL:
            printf("valeur = %lo ]", stk->bss.valeur.AsNumber);
            break;
            
        case HEXADECIMAL:
            printf("valeur = %lx ]", stk->bss.valeur.AsNumber);
            break;
            
        case DECIMAL_ZERO:
            printf("valeur = %ld ]", stk->bss.valeur.AsNumber);
            break;
            
        case STRING:
            printf("valeur = %s ]", stk->bss.valeur.AsString);
            break;
    }
    printf("\n\n");
}

FILE_DE_BSS enfiler_bss(DATA_BSS b, FILE_DE_BSS f_b){ /* On enfile par la queue pour pouvoir visualiser les mots dans l'odre */
    FILE_DE_BSS stk = calloc(1, sizeof(*stk));
    if (stk == NULL){
        return NULL;
    }
    stk->bss = b;
    if (!(file_bss_vide(f_b))){
        stk->suiv = f_b->suiv;
        f_b->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}
