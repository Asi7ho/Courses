
#include <pseudo_instruction.h>

FILE_INSTRUCTION pseudo_instruction(FILE_INSTRUCTION f_i){
    FILE_INSTRUCTION stk;
    INSTRUCTION remplace_inst;
    INSTRUCTION stk_inst;
    
    unsigned faible = 0x00FF;
    unsigned fort = 0xFF00;
    
    if (f_i == NULL){
        return f_i;
    } else {
        for (stk = f_i->suiv; stk != f_i; stk = stk->suiv){
            
            if (strcasecmp(stk->instruction.nom, "LW") == 0 && stk->instruction.nombre_operande == 2) {
                
                stk_inst = stk->instruction;
                
                /* On remplace le LW dans la file_instruction par LUI et LW */
                
                
                stk->instruction.nom = "LUI";
                stk->instruction.nombre_operande = 2;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0].token = "$at";
                stk->instruction.operande[0].type_lexeme = REGISTRE;
                stk->instruction.operande[0].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[0].valeur = 1;
                stk->instruction.operande[1].token = "16 bit poids fort";
                stk->instruction.operande[1].type_lexeme = DECIMAL;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = stk_inst.operande[1].valeur & fort;
                
                
                remplace_inst.nom = "LW";
                remplace_inst.nombre_operande = 3;
                remplace_inst.ligne = stk_inst.ligne;
                remplace_inst.decalage = stk_inst.decalage + 4;
                remplace_inst.operande[0].token = stk_inst.operande[0].token;
                remplace_inst.operande[0].type_lexeme = stk_inst.operande[0].type_lexeme;
                remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[0].valeur = stk_inst.operande[0].valeur;
                remplace_inst.operande[1].token = "16 bit poids faible";
                remplace_inst.operande[1].type_lexeme = DECIMAL;
                remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[1].valeur = stk_inst.operande[1].valeur & faible;
                remplace_inst.operande[2].token = "$at";
                remplace_inst.operande[2].type_lexeme = REGISTRE;
                remplace_inst.operande[2].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[2].valeur = 1;
                stk = enfiler_inst(remplace_inst, stk);
                
                /* LW reg, target -> LUI $at, 16bitpoidsfaible et LW reg, 16bitpoidsfort($at) */
            }
            
            if (strcasecmp(stk->instruction.nom, "SW") == 0) {
                
                stk_inst = stk->instruction;
                
                remplace_inst.nom = "LUI";
                remplace_inst.nombre_operande = 2;
                remplace_inst.ligne = stk->instruction.ligne;
                remplace_inst.decalage = stk->instruction.decalage + 4;
                remplace_inst.operande[0].token = "$at";
                remplace_inst.operande[0].type_lexeme = REGISTRE;
                remplace_inst.operande[0].numero_de_ligne = stk->instruction.ligne;
                remplace_inst.operande[0].valeur = 1;
                remplace_inst.operande[1].token = "1";
                remplace_inst.operande[1].type_lexeme = DECIMAL;
                remplace_inst.operande[1].numero_de_ligne = stk->instruction.ligne;
                remplace_inst.operande[1].valeur = 1;
                
                
                /* On remplace le SW dans la file_instruction par LUI et SW */
                
                stk->instruction.nom = "LUI";
                stk->instruction.nombre_operande = 2;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0].token = "$at";
                stk->instruction.operande[0].type_lexeme = REGISTRE;
                stk->instruction.operande[0].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[0].valeur = 1;
                stk->instruction.operande[1].token = "16 bit poids fort";
                stk->instruction.operande[1].type_lexeme = DECIMAL;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = stk_inst.operande[1].valeur & fort;
                
                remplace_inst.nom = "SW";
                remplace_inst.nombre_operande = 3;
                remplace_inst.ligne = stk_inst.ligne;
                remplace_inst.decalage = stk_inst.decalage + 4;
                remplace_inst.operande[0].token = stk_inst.operande[0].token;
                remplace_inst.operande[0].type_lexeme = stk_inst.operande[0].type_lexeme;
                remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[0].valeur = stk_inst.operande[0].valeur;
                remplace_inst.operande[1].token = "16 bit poids faible";
                remplace_inst.operande[1].type_lexeme = DECIMAL;
                remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[1].valeur = stk_inst.operande[1].valeur & faible;
                remplace_inst.operande[2].token = "$at";
                remplace_inst.operande[2].type_lexeme = REGISTRE;
                remplace_inst.operande[2].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[2].valeur = 1;
                stk = enfiler_inst(remplace_inst, stk);
            }
            
            if (strcasecmp(stk->instruction.nom, "NOP") == 0) {
                
                stk_inst = stk->instruction;
                
                stk->instruction.nom = "SLL";
                stk->instruction.nombre_operande = 3;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0].token = "$0";
                stk->instruction.operande[0].type_lexeme = REGISTRE;
                stk->instruction.operande[0].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[0].valeur = 0;
                stk->instruction.operande[1].token = "$0";
                stk->instruction.operande[1].type_lexeme = REGISTRE;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = 0;
                stk->instruction.operande[2].token = "0";
                stk->instruction.operande[2].type_lexeme = DECIMAL_ZERO;
                stk->instruction.operande[2].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[2].valeur = 0;
                
            }
            
            if (strcasecmp(stk->instruction.nom, "MOVE") == 0) {
                
                stk_inst = stk->instruction;
                
                /* On remplace le MOVE dans la file_instruction par ADD */
                
                stk->instruction.nom = "ADD";
                stk->instruction.nombre_operande = 3;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0] = stk_inst.operande[0];
                stk->instruction.operande[1] = stk_inst.operande[1];
                stk->instruction.operande[2].token = "$zero";
                stk->instruction.operande[2].type_lexeme = REGISTRE;
                stk->instruction.operande[2].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[2].valeur = 0;
            }
            
            if (strcasecmp(stk->instruction.nom, "NEG") == 0) {
                
                stk_inst = stk->instruction;
                
                /* On remplace le NEG dans la file_instruction par SUB */
                
                stk->instruction.nom = "SUB";
                stk->instruction.nombre_operande = 3;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0] = stk_inst.operande[0];
                stk->instruction.operande[2] = stk_inst.operande[1];
                stk->instruction.operande[1].token = "$zero";
                stk->instruction.operande[1].type_lexeme = REGISTRE;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = 0;
            }
            
            if (strcasecmp(stk->instruction.nom, "LI") == 0) {
                
                stk_inst = stk->instruction;
                
                /* On remplace le LI dans la file_instruction par ADDI */
                
                stk->instruction.nom = "ADDI";
                stk->instruction.nombre_operande = 3;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[0] = stk_inst.operande[0];
                stk->instruction.operande[2] = stk_inst.operande[1];
                stk->instruction.operande[1].token = "$zero";
                stk->instruction.operande[1].type_lexeme = REGISTRE;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = 0;
            }
            
            if (strcasecmp(stk->instruction.nom, "BLT") == 0) {
                
                stk_inst = stk->instruction;
                
                /* On remplace le BLT dans la file_instruction par SLT et BNE */
                
                stk->instruction.nom = "SLT";
                stk->instruction.nombre_operande = 3;
                stk->instruction.ligne = stk_inst.ligne;
                stk->instruction.decalage = stk_inst.decalage;
                stk->instruction.operande[1] = stk_inst.operande[0];
                stk->instruction.operande[2] = stk_inst.operande[1];
                stk->instruction.operande[0].token = "$1";
                stk->instruction.operande[1].type_lexeme = REGISTRE;
                stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
                stk->instruction.operande[1].valeur = 1;
                
                
                remplace_inst.nom = "BNE";
                remplace_inst.nombre_operande = 3;
                remplace_inst.ligne = stk_inst.ligne;
                remplace_inst.decalage = stk_inst.decalage + 4;
                remplace_inst.operande[0].token = "$1";
                remplace_inst.operande[0].type_lexeme = REGISTRE;
                remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[0].valeur = 0;
                remplace_inst.operande[1].token = "$zero";
                remplace_inst.operande[1].type_lexeme = REGISTRE;
                remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
                remplace_inst.operande[1].valeur = 0;
                remplace_inst.operande[2] = stk_inst.operande[2];
                stk = enfiler_inst(remplace_inst, stk);
            }
        }
        
        if (strcasecmp(stk->instruction.nom, "LW") == 0 && stk->instruction.nombre_operande == 2) {
            
            stk_inst = stk->instruction;
            
            /* On remplace le LW dans la file_instruction par LUI et LW */
            
            
            stk->instruction.nom = "LUI";
            stk->instruction.nombre_operande = 2;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[0].token = "$at";
            stk->instruction.operande[0].type_lexeme = REGISTRE;
            stk->instruction.operande[0].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[0].valeur = 1;
            stk->instruction.operande[1].token = "16 bit poids fort";
            stk->instruction.operande[1].type_lexeme = DECIMAL;
            stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[1].valeur = stk_inst.operande[1].valeur & fort;
            
            
            remplace_inst.nom = "LW";
            remplace_inst.nombre_operande = 3;
            remplace_inst.ligne = stk_inst.ligne;
            remplace_inst.decalage = stk_inst.decalage + 4;
            remplace_inst.operande[0].token = stk_inst.operande[0].token;
            remplace_inst.operande[0].type_lexeme = stk_inst.operande[0].type_lexeme;
            remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[0].valeur = stk_inst.operande[0].valeur;
            remplace_inst.operande[1].token = "16 bit poids faible";
            remplace_inst.operande[1].type_lexeme = DECIMAL;
            remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[1].valeur = stk_inst.operande[1].valeur & faible;
            remplace_inst.operande[2].token = "$at";
            remplace_inst.operande[2].type_lexeme = REGISTRE;
            remplace_inst.operande[2].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[2].valeur = 1;
            stk = enfiler_inst(remplace_inst, stk);
            
            /* LW reg, target -> LUI $at, 16bitpoidsfaible et LW reg, 16bitpoidsfort($at) */
        }
        
        if (strcasecmp(stk->instruction.nom, "SW") == 0) {
            
            stk_inst = stk->instruction;
            
            remplace_inst.nom = "LUI";
            remplace_inst.nombre_operande = 2;
            remplace_inst.ligne = stk->instruction.ligne;
            remplace_inst.decalage = stk->instruction.decalage + 4;
            remplace_inst.operande[0].token = "$at";
            remplace_inst.operande[0].type_lexeme = REGISTRE;
            remplace_inst.operande[0].numero_de_ligne = stk->instruction.ligne;
            remplace_inst.operande[0].valeur = 1;
            remplace_inst.operande[1].token = "1";
            remplace_inst.operande[1].type_lexeme = DECIMAL;
            remplace_inst.operande[1].numero_de_ligne = stk->instruction.ligne;
            remplace_inst.operande[1].valeur = 1;
            
            
            /* On remplace le SW dans la file_instruction par LUI et SW */
            
            stk->instruction.nom = "LUI";
            stk->instruction.nombre_operande = 2;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[0].token = "$at";
            stk->instruction.operande[0].type_lexeme = REGISTRE;
            stk->instruction.operande[0].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[0].valeur = 1;
            stk->instruction.operande[1].token = "16 bit poids fort";
            stk->instruction.operande[1].type_lexeme = DECIMAL;
            stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[1].valeur = stk_inst.operande[1].valeur & fort;
            
            remplace_inst.nom = "SW";
            remplace_inst.nombre_operande = 3;
            remplace_inst.ligne = stk_inst.ligne;
            remplace_inst.decalage = stk_inst.decalage + 4;
            remplace_inst.operande[0].token = stk_inst.operande[0].token;
            remplace_inst.operande[0].type_lexeme = stk_inst.operande[0].type_lexeme;
            remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[0].valeur = stk_inst.operande[0].valeur;
            remplace_inst.operande[1].token = "16 bit poids faible";
            remplace_inst.operande[1].type_lexeme = DECIMAL;
            remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[1].valeur = stk_inst.operande[1].valeur & faible;
            remplace_inst.operande[2].token = "$at";
            remplace_inst.operande[2].type_lexeme = REGISTRE;
            remplace_inst.operande[2].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[2].valeur = 1;
            stk = enfiler_inst(remplace_inst, stk);
        }
        
        if (strcasecmp(stk->instruction.nom, "MOVE") == 0) {
            
            stk_inst = stk->instruction;
            
            /* On remplace le MOVE dans la file_instruction par ADD */
            
            stk->instruction.nom = "ADD";
            stk->instruction.nombre_operande = 3;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[0] = stk_inst.operande[0];
            stk->instruction.operande[1] = stk_inst.operande[1];
            stk->instruction.operande[2].token = "$zero";
            stk->instruction.operande[2].type_lexeme = REGISTRE;
            stk->instruction.operande[2].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[2].valeur = 0;
        }
        
        if (strcasecmp(stk->instruction.nom, "NEG") == 0) {
            
            stk_inst = stk->instruction;
            
            /* On remplace le NEG dans la file_instruction par SUB */
            
            stk->instruction.nom = "SUB";
            stk->instruction.nombre_operande = 3;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[0] = stk_inst.operande[0];
            stk->instruction.operande[2] = stk_inst.operande[1];
            stk->instruction.operande[1].token = "$zero";
            stk->instruction.operande[1].type_lexeme = REGISTRE;
            stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[1].valeur = 0;
        }
        
        if (strcasecmp(stk->instruction.nom, "LI") == 0) {
            
            stk_inst = stk->instruction;
            
            /* On remplace le LI dans la file_instruction par ADDI */
            
            stk->instruction.nom = "ADDI";
            stk->instruction.nombre_operande = 3;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[0] = stk_inst.operande[0];
            stk->instruction.operande[2] = stk_inst.operande[1];
            stk->instruction.operande[1].token = "$zero";
            stk->instruction.operande[1].type_lexeme = REGISTRE;
            stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[1].valeur = 0;
        }
        
        if (strcasecmp(stk->instruction.nom, "BLT") == 0) {
            
            stk_inst = stk->instruction;
            
            /* On remplace le BLT dans la file_instruction par SLT et BNE */
            
            stk->instruction.nom = "SLT";
            stk->instruction.nombre_operande = 3;
            stk->instruction.ligne = stk_inst.ligne;
            stk->instruction.decalage = stk_inst.decalage;
            stk->instruction.operande[1] = stk_inst.operande[0];
            stk->instruction.operande[2] = stk_inst.operande[1];
            stk->instruction.operande[0].token = "$1";
            stk->instruction.operande[1].type_lexeme = REGISTRE;
            stk->instruction.operande[1].numero_de_ligne = stk_inst.ligne;
            stk->instruction.operande[1].valeur = 1;
            
            
            remplace_inst.nom = "BNE";
            remplace_inst.nombre_operande = 3;
            remplace_inst.ligne = stk_inst.ligne;
            remplace_inst.decalage = stk_inst.decalage + 4;
            remplace_inst.operande[0].token = "$1";
            remplace_inst.operande[0].type_lexeme = REGISTRE;
            remplace_inst.operande[0].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[0].valeur = 0;
            remplace_inst.operande[1].token = "$zero";
            remplace_inst.operande[1].type_lexeme = REGISTRE;
            remplace_inst.operande[1].numero_de_ligne = stk_inst.ligne;
            remplace_inst.operande[1].valeur = 0;
            remplace_inst.operande[2] = stk_inst.operande[2];
            stk = enfiler_inst(remplace_inst, stk);
        }
    }
}

