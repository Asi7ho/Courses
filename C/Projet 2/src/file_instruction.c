
#include <file_instruction.h>

FILE_INSTRUCTION creer_file_inst(void){
    return NULL;
}

int file_inst_vide(FILE_INSTRUCTION f_i){
    return f_i == NULL;
}

void visualiser_file_inst(FILE_INSTRUCTION f_i){
    FILE_INSTRUCTION stk;
	int i = 0;
    printf("\n--------------------------- FILE D'INSTRUCTIONS -------------------------\n");
    if (f_i == NULL){
        printf("FILE VIDE\n");
        return;
    }
    for(stk = f_i->suiv; stk != f_i; stk = stk->suiv){ /* On s'arrete apres un tour */
        printf("[ nom = %s\n nombre d'operande = %d\n decalage = %08d\n numero de ligne = %d\n code binaire = %08x\n", stk->instruction.nom, stk->instruction.nombre_operande, stk->instruction.decalage, stk->instruction.ligne, stk->instruction.codeBinaire);
        for (i = 0; i < stk->instruction.nombre_operande; i++){
            printf(" -> operande n°%d = [ %s = %d] : %s  LIGNE n°%d \n", i+1, stk->instruction.operande[i].token,stk->instruction.operande[i].valeur, type_string(stk->instruction.operande[i].type_lexeme), stk->instruction.operande[i].numero_de_ligne);
        }
        printf(" ]\n\n");
    }
    printf("[ nom = %s\n nombre d'operande = %d\n decalage = %08d\n numero de ligne = %d\n code binaire = %08x\n", stk->instruction.nom, stk->instruction.nombre_operande, stk->instruction.decalage, stk->instruction.ligne, stk->instruction.codeBinaire);
    for (i = 0; i < stk->instruction.nombre_operande; i++){
        printf(" -> operande n°%d = [ %s = %d] : %s  LIGNE n°%d \n", i+1, stk->instruction.operande[i].token,stk->instruction.operande[i].valeur, type_string(stk->instruction.operande[i].type_lexeme), stk->instruction.operande[i].numero_de_ligne);
    }
    printf(" ]\n\n");
}

FILE_INSTRUCTION enfiler_inst(INSTRUCTION i, FILE_INSTRUCTION f_i){ /* On enfile par la queue pour pouvoir visualiser les mots dans l'odre */
    FILE_INSTRUCTION stk = calloc(1, sizeof(*stk));
    if (stk == NULL){
        return NULL;
    }
    stk->instruction = i;
    if (!(file_inst_vide(f_i))){
        stk->suiv = f_i->suiv;
        f_i->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}
