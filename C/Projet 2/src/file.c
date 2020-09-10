
#include <file.h>


FILE_LEXEME creer_file(void){
    return NULL;
}

int file_vide(FILE_LEXEME f){
    return f == NULL;
}

void visualiser_file(FILE_LEXEME f){
    FILE_LEXEME stk;
    
    if (f == NULL){
        printf("FILE VIDE\n");
        return;
    }
    for(stk = f->suiv; stk != f; stk = stk->suiv){ /* On s'arrete apres un tour */
        printf("[ %s ] : %s -> LIGNE n°%d", stk->val.token, type_string(stk->val.type_lexeme), stk->val.numero_de_ligne);
        printf("\n");
    }
    printf("[ %s ] : %s -> LIGNE n°%d", stk->val.token, type_string(stk->val.type_lexeme), stk->val.numero_de_ligne);
    printf("\n");
}

 FILE_LEXEME enfiler(LEXEM l, FILE_LEXEME f){ /* On enfile par la queue pour pouvoir visualiser les mots dans l'odre */
    FILE_LEXEME stk = calloc(1, sizeof(*stk));
    if (stk == NULL){
        return NULL;
    }
    stk->val = l;
    if (!(file_vide(f))){
        stk->suiv = f->suiv;
        f->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}
