
#include <file_binaire.h>

void code_binaire_instru(FILE_INSTRUCTION f_i){
    
    FILE_INSTRUCTION stk;
    R_INSTRUCTION R;
    I_INSTRUCTION I;
    J_INSTRUCTION J;
    
    int codeBinaire = 0;
    
    if (f_i == NULL) {
        /* Si la file d'instruction est vide alors on a rien a faire*/
        return;
    } else {
        /* On vérifie chaque instruction*/
        for(stk = f_i->suiv; stk != f_i; stk = stk->suiv){
            
            if (strcasecmp(stk->instruction.nom,"ADD") == 0){
                R.func = 0x20;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"SUB") == 0){
                R.func = 0x22;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"SLT") == 0){
                R.func = 0x2A;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"AND") == 0){
                R.func = 0x24;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"0R") == 0){
                R.func = 0x25;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"X0R") == 0){
                R.func = 0x26;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = stk->instruction.operande[1].valeur;
                R.rt = stk->instruction.operande[2].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"ROTR") == 0){
                R.func = 0x02;
                R.sa = stk->instruction.operande[2].valeur;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = 0x01;
                R.rt = stk->instruction.operande[1].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"SLL") == 0){
                R.func = 0x00;
                R.sa = stk->instruction.operande[2].valeur;
                R.rd = 0x00;
                R.rs = stk->instruction.operande[0].valeur;
                R.rt = stk->instruction.operande[1].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"SRL") == 0){
                R.func = 0x02;
                R.sa = stk->instruction.operande[2].valeur;
                R.rd = 0x00;
                R.rs = stk->instruction.operande[0].valeur;
                R.rt = stk->instruction.operande[1].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"MULT") == 0){
                R.func = 0x18;
                R.sa = 0x00;
                R.rd = 0x00;
                R.rs = stk->instruction.operande[0].valeur;
                R.rt = stk->instruction.operande[1].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"DIV") == 0){
                R.func = 0x1A;
                R.sa = 0x00;
                R.rd = 0x00;
                R.rs = stk->instruction.operande[0].valeur;
                R.rt = stk->instruction.operande[1].valeur;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"MFHI") == 0){
                R.func = 0x10;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = 0x00;
                R.rt = 0x00;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"MFLO") == 0){
                R.func = 0x12;
                R.sa = 0x00;
                R.rd = stk->instruction.operande[0].valeur;
                R.rs = 0x00;
                R.rt = 0x00;
                R.opcode = 0x00;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (R.opcode << 26);
                codeBinaire = codeBinaire | (R.rs << 21);
                codeBinaire = codeBinaire | (R.rt << 16);
                codeBinaire = codeBinaire | (R.rd << 11);
                codeBinaire = codeBinaire | (R.sa << 6);
                codeBinaire = codeBinaire | (R.func << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"ADDI") == 0){
                I.immediate=stk->instruction.operande[2].valeur;
                I.rt=stk->instruction.operande[0].valeur;
                I.rs=stk->instruction.operande[1].valeur;
                I.opcode=0x08;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"BEQ") == 0){
                I.immediate=stk->instruction.operande[2].valeur;
                I.rt=stk->instruction.operande[1].valeur;
                I.rs=stk->instruction.operande[0].valeur;
                I.opcode=0x04;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"BNE") == 0){
                I.immediate=stk->instruction.operande[2].valeur;
                I.rt=stk->instruction.operande[1].valeur;
                I.rs=stk->instruction.operande[0].valeur;
                I.opcode=0x05;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"LW") == 0){
                I.immediate=stk->instruction.operande[1].valeur;
                I.rt=stk->instruction.operande[0].valeur;
                I.rs=stk->instruction.operande[2].valeur;
                I.opcode=0x23;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"SW") == 0){
                I.immediate=stk->instruction.operande[1].valeur;
                I.rt=stk->instruction.operande[0].valeur;
                I.rs=stk->instruction.operande[2].valeur;
                I.opcode=0x2B;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"LUI") == 0){
                I.immediate=stk->instruction.operande[1].valeur;
                I.rt=stk->instruction.operande[0].valeur;
                I.rs=0x00;
                I.opcode=0x0F;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"BGTZ") == 0){
                I.immediate=stk->instruction.operande[1].valeur;
                I.rt= 0x00;
                I.rs=stk->instruction.operande[0].valeur;
                I.opcode=0x07;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate << 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"BLEZ") == 0){
                I.immediate=stk->instruction.operande[1].valeur;
                I.rt= 0x00;
                I.rs=stk->instruction.operande[0].valeur;
                I.opcode=0x06;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (I.opcode << 26);
                codeBinaire = codeBinaire | (I.rs << 21);
                codeBinaire = codeBinaire | (I.rt << 16);
                codeBinaire = codeBinaire | (I.immediate<< 0);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"J") == 0){
                J.target=stk->instruction.operande[0].valeur;
                J.opcode=0x02;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (J.opcode << 26);
                codeBinaire = codeBinaire | (J.target << 21);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"B") == 0){
                J.target=stk->instruction.operande[0].valeur;
                J.opcode=0x02;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (J.opcode << 26);
                codeBinaire = codeBinaire | (J.target << 21);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else if (strcasecmp(stk->instruction.nom,"JAL") == 0){
                J.target=stk->instruction.operande[0].valeur;
                J.opcode=0x03;
                
                codeBinaire = 0;
                /*On code l'instruction*/
                codeBinaire = codeBinaire | (J.opcode << 26);
                codeBinaire = codeBinaire | (J.target << 21);
                
                stk->instruction.codeBinaire = codeBinaire;
                
            } else {
                WARNING_MSG("Instruction non connue à la ligne n°%d", stk->instruction.ligne);
            }
        }
        
        if (strcasecmp(stk->instruction.nom,"ADD") == 0){
            R.func = 0x20;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"SUB") == 0){
            R.func = 0x22;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"SLT") == 0){
            R.func = 0x2A;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"AND") == 0){
            R.func = 0x24;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"0R") == 0){
            R.func = 0x25;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"X0R") == 0){
            R.func = 0x26;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = stk->instruction.operande[1].valeur;
            R.rt = stk->instruction.operande[2].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"ROTR") == 0){
            R.func = 0x02;
            R.sa = stk->instruction.operande[2].valeur;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = 0x01;
            R.rt = stk->instruction.operande[1].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"SLL") == 0){
            R.func = 0x00;
            R.sa = stk->instruction.operande[2].valeur;
            R.rd = 0x00;
            R.rs = stk->instruction.operande[0].valeur;
            R.rt = stk->instruction.operande[1].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"SRL") == 0){
            R.func = 0x02;
            R.sa = stk->instruction.operande[2].valeur;
            R.rd = 0x00;
            R.rs = stk->instruction.operande[0].valeur;
            R.rt = stk->instruction.operande[1].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"MULT") == 0){
            R.func = 0x18;
            R.sa = 0x00;
            R.rd = 0x00;
            R.rs = stk->instruction.operande[0].valeur;
            R.rt = stk->instruction.operande[1].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"DIV") == 0){
            R.func = 0x1A;
            R.sa = 0x00;
            R.rd = 0x00;
            R.rs = stk->instruction.operande[0].valeur;
            R.rt = stk->instruction.operande[1].valeur;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"MFHI") == 0){
            R.func = 0x10;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = 0x00;
            R.rt = 0x00;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"MFLO") == 0){
            R.func = 0x12;
            R.sa = 0x00;
            R.rd = stk->instruction.operande[0].valeur;
            R.rs = 0x00;
            R.rt = 0x00;
            R.opcode = 0x00;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (R.opcode << 26);
            codeBinaire = codeBinaire | (R.rs << 21);
            codeBinaire = codeBinaire | (R.rt << 16);
            codeBinaire = codeBinaire | (R.rd << 11);
            codeBinaire = codeBinaire | (R.sa << 6);
            codeBinaire = codeBinaire | (R.func << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"ADDI") == 0){
            I.immediate=stk->instruction.operande[2].valeur;
            I.rt=stk->instruction.operande[0].valeur;
            I.rs=stk->instruction.operande[1].valeur;
            I.opcode=0x08;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"BEQ") == 0){
            I.immediate=stk->instruction.operande[2].valeur;
            I.rt=stk->instruction.operande[1].valeur;
            I.rs=stk->instruction.operande[0].valeur;
            I.opcode=0x04;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"BNE") == 0){
            I.immediate=stk->instruction.operande[2].valeur;
            I.rt=stk->instruction.operande[1].valeur;
            I.rs=stk->instruction.operande[0].valeur;
            I.opcode=0x05;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"LW") == 0){
            I.immediate=stk->instruction.operande[1].valeur;
            I.rt=stk->instruction.operande[0].valeur;
            I.rs=stk->instruction.operande[2].valeur;
            I.opcode=0x23;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"SW") == 0){
            I.immediate=stk->instruction.operande[1].valeur;
            I.rt=stk->instruction.operande[0].valeur;
            I.rs=stk->instruction.operande[2].valeur;
            I.opcode=0x2B;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"LUI") == 0){
            I.immediate=stk->instruction.operande[1].valeur;
            I.rt=stk->instruction.operande[0].valeur;
            I.rs=0x00;
            I.opcode=0x0F;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"BGTZ") == 0){
            I.immediate=stk->instruction.operande[1].valeur;
            I.rt= 0x00;
            I.rs=stk->instruction.operande[0].valeur;
            I.opcode=0x07;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate << 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"BLEZ") == 0){
            I.immediate=stk->instruction.operande[1].valeur;
            I.rt= 0x00;
            I.rs=stk->instruction.operande[0].valeur;
            I.opcode=0x06;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (I.opcode << 26);
            codeBinaire = codeBinaire | (I.rs << 21);
            codeBinaire = codeBinaire | (I.rt << 16);
            codeBinaire = codeBinaire | (I.immediate<< 0);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"J") == 0){
            J.target=stk->instruction.operande[0].valeur;
            J.opcode=0x02;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (J.opcode << 26);
            codeBinaire = codeBinaire | (J.target << 21);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"B") == 0){
            J.target=stk->instruction.operande[0].valeur;
            J.opcode=0x02;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (J.opcode << 26);
            codeBinaire = codeBinaire | (J.target << 21);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else if (strcasecmp(stk->instruction.nom,"JAL") == 0){
            J.target=stk->instruction.operande[0].valeur;
            J.opcode=0x03;
            
            codeBinaire = 0;
            /*On code l'instruction*/
            codeBinaire = codeBinaire | (J.opcode << 26);
            codeBinaire = codeBinaire | (J.target << 21);
            
            stk->instruction.codeBinaire = codeBinaire;
            
        } else {
            WARNING_MSG("Instruction non connue à la ligne n°%d", stk->instruction.ligne);
        }
    }
}

void create_file_l(char *file_or, char *file, FILE_INSTRUCTION fi, FILE_DE_DATA fd, FILE_DE_BSS fb, SYMBOLE* tab, int nombre_etiquette, FILE_RELOCATION f_rtxt, FILE_RELOCATION f_rdata){
    
    char line[STRLEN];
    int numero_ligne = 0;
    int codeBinaireString = 0;
	int i = 0;
    
    ETAT_GRAMMATICALE etat = INIT_GENERALE;
    
    FILE_RELOCATION stk_reloc;
    FILE_INSTRUCTION stk_inst;
    FILE_DE_DATA stk_data;
    FILE_DE_BSS stk_bss;
    
    /*On verifie que les files ne sont pas vide avant de faire les opérations*/
    if (fi != NULL){
        stk_inst = fi->suiv;
    }
    
    if (fd != NULL){
        stk_data = fd->suiv;
    }
    
    if (fb != NULL){
        stk_bss = fb->suiv;
    }
    
    FILE *fo = NULL;
    FILE *fp   = NULL;
    
    /*On ouvre les fichiers*/
    fo = fopen(file_or, "r");
    fp = fopen(file, "w");
    
    if (fo == NULL){
        WARNING_MSG("Fichier %s non connu", file);
    }
    
    while(!feof(fo)) {
    
        switch(etat){
    
           case INIT_GENERALE:
    

				if (fgets( line, STRLEN-1, fo ) == NULL){
                    etat = INIT_GENERALE;
                } else {
                    if (strcmp(".text\n", line) == 0) {
                        /*Si on rencontre un .text on lit la file d'instruction*/
                    	numero_ligne++;
                    	fprintf(fp, "%3d                   %s", numero_ligne, line);
                    	etat = INIT_TEXT;
                	} else if (strcmp(".data\n", line) == 0){
                        /*Si on rencontre un .data on lit la file de data*/
                    	numero_ligne++;
                    	fprintf(fp, "%3d                   %s", numero_ligne, line);
                    	etat = INIT_DATA;
                	} else if (strcmp(".bss\n", line) == 0) {
                        /*Si on rencontre un .bss on lit la file de bss*/
                    	fprintf(fp, "%3d                   %s", numero_ligne, line);
                    	etat = INIT_BSS;
                    } else if (strlen(line) == 1){
                        /*Si on rencontre une ligne vide on lit la ligne suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d\n", numero_ligne);
                        etat = INIT_GENERALE;
                    }else {
                    	numero_ligne++;
                    	fprintf(fp, "%3d                   %s", numero_ligne, line);
                    	etat = INIT_GENERALE;
                	}
				}
                break;
            
            case INIT_TEXT:
                if (strcmp(".data\n", line) == 0){
                    /*Si on rencontre un .data on lit la file de data*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_DATA;
                } else if (strcmp(".bss\n", line) == 0){
                    /*Si on rencontre un .bss on lit la file de bss*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_BSS;
                } else if (NULL != fgets( line, STRLEN-1, fo )){
                    if (line[0] == '#'){
                        /*Si une ligne est un commentaire, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_TEXT;
                    } else if (line[strlen(line) - 2] == ':'){
                        /*Si on a une etiquette, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_TEXT;
                    } else if (strlen(line) == 1){
                        /*Si on rencontre une ligne vide on lit la ligne suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d\n", numero_ligne);
                        etat = INIT_TEXT;
                    } else if (stk_inst->instruction.ligne == stk_inst->suiv->instruction.ligne){
                        /*Si on a une pseudo instruction qui est composée de 2 instructions, on affiche les 2 instructions*/
                        fprintf(fp, "%3d %08X %08X %s", stk_inst->instruction.ligne, stk_inst->instruction.decalage, stk_inst->instruction.codeBinaire, line);
                        stk_inst = stk_inst->suiv;
                        fprintf(fp, "%3d %08X %08X\n", stk_inst->instruction.ligne, stk_inst->instruction.decalage, stk_inst->instruction.codeBinaire);
                        numero_ligne++;
                        stk_inst = stk_inst->suiv;
                    } else {
                        fprintf(fp, "%3d %08X %08X %s", stk_inst->instruction.ligne, stk_inst->instruction.decalage, stk_inst->instruction.codeBinaire, line);
                        numero_ligne++;
                        
                        /*On verifie si on a parcourut la file d'instruction ou non*/
                        if (stk_inst != fi){
                            stk_inst = stk_inst->suiv;
                        } else {
                            etat = INIT_GENERALE;
                        }
                        
                    }
                } else {
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_TEXT;
                }
                break;
            
            case INIT_DATA:
                if (strcmp(".text\n", line) == 0){
                    /*Si on rencontre un .text on lit la file d'instruction*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_TEXT;
                } else if (strcmp(".bss\n", line) == 0){
                    /*Si on rencontre un .bss on lit la file de bss*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_BSS;
                } else if (NULL != fgets( line, STRLEN-1, fo )){
                    if (line[strlen(line) - 2] == ':'){
                        /*Si on a une etiquette, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_DATA;
                    } else if (strlen(line) == 1){
                        /*Si on rencontre une ligne vide on lit la ligne suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d\n", numero_ligne);
                        etat = INIT_DATA;
                    } else if (line[0] == '#'){
                        /*Si une ligne est un commentaire, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_TEXT;
                    } else if (stk_data->data.ligne == stk_data->suiv->data.ligne){
                        /*Si on declare plusieurs données sur la même ligne*/
                        int codeBinaire = 0;
                        int nbrVirgule = 0;
                        int emplacement = 24;
                        int decalage = stk_data->data.decalage;
                        while (stk_data->data.ligne == stk_data->suiv->data.ligne){
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << emplacement;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << emplacement;
                            }
                            emplacement -= 8;
                            nbrVirgule++;
                            stk_data = stk_data->suiv;
                        }
                        /*Si on déclare plus de 4 données en même temps*/
                        fprintf(fp, "%3d %08X %08X %s", stk_data->data.ligne, decalage, codeBinaire, line);
                        if ((nbrVirgule + 1) % 4 == 1){
                            codeBinaire = 0;
                            decalage = stk_data->data.decalage;
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 0;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 0;
                            }
                            fprintf(fp, "%3d %08X %08X\n", stk_data->data.ligne, decalage, codeBinaire);
                            stk_data = stk_data->suiv;
                        } else if ((nbrVirgule + 1) % 4 == 2){
                            codeBinaire = 0;
                            decalage = stk_data->data.decalage;
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 8;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 8;
                            }
                            stk_data = stk_data->suiv;
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 0;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 0;
                            }
                            fprintf(fp, "%3d %08X %08X\n", stk_data->data.ligne, decalage, codeBinaire);
                            stk_data = stk_data->suiv;
                        } else if ((nbrVirgule + 1) % 4 == 3){
                            codeBinaire = 0;
                            decalage = stk_data->data.decalage;
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 16;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 16;
                            }
                            stk_data = stk_data->suiv;
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 8;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 8;
                            }
                            stk_data = stk_data->suiv;
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                codeBinaire = codeBinaire | stk_data->data.valeur.AsNumber << 0;
                            } else {
                                codeBinaire = codeBinaire | ((~stk_data->data.valeur.AsNumber) + 1) << 0;
                            }
                            fprintf(fp, "%3d %08X %08X\n", stk_data->data.ligne, decalage, codeBinaire);
                            stk_data = stk_data->suiv;
                        }
                    } else {
                        numero_ligne++;
                        fprintf(fp, "%3d %08X ", stk_data->data.ligne, stk_data->data.decalage);
                        /*On vérifie si on a une donnée de type STRING ou un nombre*/
                        if (stk_data->data.etat == STRING){
                            codeBinaireString = 0;
                            codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[1] << 24);
                            codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[2] << 16);
                            codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[3] << 8);
                            codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[4] << 0);
                            fprintf(fp, "%08X %s", codeBinaireString, line);
                            for (i = 5; i < strlen(stk_data->data.valeur.AsString) - 1; i = i + 4){
                                codeBinaireString = 0;
                                if (i < strlen(stk_data->data.valeur.AsString) - 1){
                                    codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[i] << 24);
                                }
                                if (i+1 < strlen(stk_data->data.valeur.AsString) - 1){
                                    codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[i+1] << 16);
                                }
                                if (i+2 < strlen(stk_data->data.valeur.AsString) - 1){
                                    codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[i+2] << 8);
                                }
                                if (i+3 < strlen(stk_data->data.valeur.AsString) - 1){
                                    codeBinaireString = codeBinaireString | (stk_data->data.valeur.AsString[i+3] << 0);
                                }
                                fprintf(fp, "%3d %08X %08X\n", stk_data->data.ligne, stk_data->data.decalage+i-1, codeBinaireString);
                            }
                        } else {
                            /*On traite le cas ou la donnée est une nombre positif ou negatif*/
                            if (stk_data->data.valeur.AsNumber > 0){
                                fprintf(fp, "%08lX %s", stk_data->data.valeur.AsNumber, line);
                            } else {
                                fprintf(fp, "%08lX %s", ~stk_data->data.valeur.AsNumber + 1, line);
                            }
                        }

                        /*On verifie si on a parcourut la file de data ou non*/
                        if (stk_data != fd){
                            stk_data = stk_data->suiv;
                        } else {
                            etat = INIT_GENERALE;
                        }
                    }
                } else {
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_DATA;
                }
                break;
            
            case INIT_BSS:
                if (strcmp(".text\n", line) == 0){
                    /*Si on rencontre un .text on lit la file d'instruction*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_TEXT;
                } else if (strcmp(".data\n", line) == 0){
                    /*Si on rencontre un .bss on lit la file de bss*/
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_DATA;
                } else if (NULL != fgets( line, STRLEN-1, fo )){
                    if (line[strlen(line) - 2] == ':'){
                        /*Si on a une etiquette, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_BSS;
                    } else if (strlen(line) == 1){
                        /*Si on a une ligne, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d\n", numero_ligne);
                        etat = INIT_BSS;
                    } else if (line[0] == '#'){
                        /*Si une ligne est un commentaire, on affiche la ligne et on va à la suivante*/
                        numero_ligne++;
                        fprintf(fp, "%3d                   %s", numero_ligne, line);
                        etat = INIT_BSS;
                    } else {
                        numero_ligne++;
                        fprintf(fp, "%3d %08X %08lX %s", stk_bss->bss.ligne, stk_bss->bss.decalage, stk_bss->bss.valeur.AsNumber, line);
                        
                        /*On verifie si on a parcourut la file de data ou non*/
                        if (stk_bss != fb){
                            stk_bss = stk_bss->suiv;
                        } else {
                            etat = INIT_GENERALE;
                        }
                    }
                } else {
                    numero_ligne++;
                    fprintf(fp, "%3d                   %s", numero_ligne, line);
                    etat = INIT_BSS;
                }
                break;
        }
    
    }
    
    /*On affiche la table des symboles*/
    if (tab != NULL) {
        fprintf(fp, "\n.symtab\n");
        for (i = 0; i < nombre_etiquette; i++){
            fprintf(fp, "%3d    %s:%08X %s\n", tab[i].ligne, section_string(tab[i].section), tab[i].decalage, tab[i].nom);
        }
    }
    
    /*On affiche la table de relocation de la section .text*/
    fprintf(fp, "\nrel.text\n");
    if (f_rtxt != NULL){
        for(stk_reloc = f_rtxt->suiv; stk_reloc != f_rtxt; stk_reloc = stk_reloc->suiv){
            fprintf(fp, "%08X        %s        %s:%08X  %s\n", stk_reloc->reloc.decalage_depart, stk_reloc->reloc.relocation, section_string(stk_reloc->reloc.section_arrive), stk_reloc->reloc.decalage_arrivee, stk_reloc->reloc.nom);
        }
        fprintf(fp, "%08X        %s        %s:%08X  %s\n\n", stk_reloc->reloc.decalage_depart, stk_reloc->reloc.relocation, section_string(stk_reloc->reloc.section_arrive), stk_reloc->reloc.decalage_arrivee, stk_reloc->reloc.nom);
    }
    
    /*On affiche la table de relocation de la section .data*/
    fprintf(fp, "\nrel.data\n");
    if (f_rdata != NULL){
        for(stk_reloc= f_rdata->suiv; stk_reloc!= f_rdata; stk_reloc= stk_reloc->suiv){
            fprintf(fp, "%08X        %s        %s:%08X  %s\n", stk_reloc->reloc.decalage_depart, stk_reloc->reloc.relocation, section_string(stk_reloc->reloc.section_arrive), stk_reloc->reloc.decalage_arrivee, stk_reloc->reloc.nom);
        }
        fprintf(fp, "%08X        %s        %s:%08X  %s\n\n", stk_reloc->reloc.decalage_depart, stk_reloc->reloc.relocation, section_string(stk_reloc->reloc.section_arrive), stk_reloc->reloc.decalage_arrivee, stk_reloc->reloc.nom);
    }
    
    fclose(fp);
    
}

void create_obj_file(char *file, FILE_INSTRUCTION fi, FILE_DE_DATA fd, FILE_DE_BSS fb){
    
    FILE_INSTRUCTION stk_inst;
    FILE_DE_DATA stk_data;
    FILE_DE_BSS stk_bss;
    
    int nbrOctet = 0;
    int code = 0;
    
    FILE *fp   = NULL;
    
    /*On crée un fichier objet binaire*/
    fp = fopen(file, "wb");
    
    /*On vérifie que la file d'instruction est non vide*/
    if (fi != NULL){
        /*On récupère le nombre d'octet nécessaire pour coder la section*/
        nbrOctet = fi->instruction.decalage;
        fwrite(&nbrOctet, sizeof(int), 1, fp);
        /*On écrit le code binaire*/
        for(stk_inst = fi->suiv; stk_inst != fi; stk_inst = stk_inst->suiv){
            code  = 0;
            code = stk_inst->instruction.codeBinaire & 0xFF000000;
            code = code >> 24;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_inst->instruction.codeBinaire & 0x00FF0000;
            code = code >> 16;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_inst->instruction.codeBinaire & 0x0000FF00;
            code = code >> 8;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_inst->instruction.codeBinaire & 0x000000FF;
            code = code >> 0;
            fwrite(&code, sizeof(int), 1, fp);
        }
        code  = 0;
        code = stk_inst->instruction.codeBinaire & 0xFF000000;
        code = code >> 24;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_inst->instruction.codeBinaire & 0x00FF0000;
        code = code >> 16;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_inst->instruction.codeBinaire & 0x0000FF00;
        code = code >> 8;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_inst->instruction.codeBinaire & 0x000000FF;
        code = code >> 0;
        fwrite(&code, sizeof(int), 1, fp);
    } else {
        fwrite(&nbrOctet, sizeof(int), 1, fp);
    }
    
    nbrOctet = 0;
    /*On vérifie que la file de data est non vide*/
    if (fd != NULL){
        /*On récupère le nombre d'octet nécessaire pour coder la section*/
        nbrOctet = fd->data.decalage;
        fwrite(&nbrOctet, sizeof(int), 1, fp);
        /*On écrit le code binaire*/
        for(stk_data = fd->suiv; stk_data != fd; stk_data = stk_data->suiv){
            code  = 0;
            code = stk_data->data.valeur.AsNumber & 0xFF000000;
            code = code >> 24;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_data->data.valeur.AsNumber & 0x00FF0000;
            code = code >> 16;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_data->data.valeur.AsNumber & 0x0000FF00;
            code = code >> 8;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_data->data.valeur.AsNumber & 0x000000FF;
            code = code >> 0;
            fwrite(&code, sizeof(int), 1, fp);
        }
        code  = 0;
        code = stk_data->data.valeur.AsNumber & 0xFF000000;
        code = code >> 24;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_data->data.valeur.AsNumber & 0x00FF0000;
        code = code >> 16;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_data->data.valeur.AsNumber & 0x0000FF00;
        code = code >> 8;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_data->data.valeur.AsNumber & 0x000000FF;
        code = code >> 0;
        fwrite(&code, sizeof(int), 1, fp);
    } else {
        fwrite(&nbrOctet, sizeof(int), 1, fp);
    }
    
    /*On vérifie que la file de bss est non vide*/
    if (fb != NULL){
        /*On écrit le code binaire*/
        for(stk_bss = fb->suiv; stk_bss != fb; stk_bss = stk_bss->suiv){
            code  = 0;
            code = stk_bss->bss.valeur.AsNumber & 0xFF000000;
            code = code >> 24;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_bss->bss.valeur.AsNumber & 0x00FF0000;
            code = code >> 16;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_bss->bss.valeur.AsNumber & 0x0000FF00;
            code = code >> 8;
            fwrite(&code, sizeof(int), 1, fp);
            code  = 0;
            code = stk_bss->bss.valeur.AsNumber & 0x000000FF;
            code = code >> 0;
            fwrite(&code, sizeof(int), 1, fp);
        }
        code  = 0;
        code = stk_bss->bss.valeur.AsNumber & 0xFF000000;
        code = code >> 24;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_bss->bss.valeur.AsNumber & 0x00FF0000;
        code = code >> 16;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_bss->bss.valeur.AsNumber & 0x0000FF00;
        code = code >> 8;
        fwrite(&code, sizeof(int), 1, fp);
        code  = 0;
        code = stk_bss->bss.valeur.AsNumber & 0x000000FF;
        code = code >> 0;
        fwrite(&code, sizeof(int), 1, fp);
    }
    
    return;
}
