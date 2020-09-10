
#include <relocation.h>


#include <file_instruction.h>

FILE_RELOCATION creer_file_reloc(void){
    return NULL;
}

int file_reloc_vide(FILE_RELOCATION f_r){
    return f_r == NULL;
}

void visualiser_file_reloc(FILE_RELOCATION f_r){
    
    FILE_RELOCATION stk;
        
    if (f_r == NULL){
        return;
    }
    
    
     printf("\nrel%s\n", section_string(f_r->reloc.section_de_depart));
    
    for(stk = f_r->suiv; stk != f_r; stk = stk->suiv){
        printf("%08x        %s        %s:%08x  %s\n", stk->reloc.decalage_depart, stk->reloc.relocation, section_string(stk->reloc.section_arrive), stk->reloc.decalage_arrivee, stk->reloc.nom);
     }
     printf("%08x        %s        %s:%08x  %s\n\n", stk->reloc.decalage_depart, stk->reloc.relocation, section_string(stk->reloc.section_arrive), stk->reloc.decalage_arrivee, stk->reloc.nom);
}

FILE_RELOCATION enfiler_reloc(RELOCATION r, FILE_RELOCATION f_r){ /* On enfile par la queue pour pouvoir visualiser les mots dans l'odre */
    FILE_RELOCATION stk = calloc(1, sizeof(*stk));
    if (stk == NULL){
        return NULL;
    }
    stk->reloc = r;
    if (!(file_reloc_vide(f_r))){
        stk->suiv = f_r->suiv;
        f_r->suiv = stk;
        return stk;
    } else {
        stk->suiv = stk;
        return stk;
    }
}
