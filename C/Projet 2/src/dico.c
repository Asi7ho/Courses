
#include <dico.h>

INSTRUCTION_DEFINITION* charge_dico(char* nom_fichier, int* nombre_instruction){
    
    int i;
    char s[8];
    char op1[8];
    char op2[8];
    char op3[8];
    char opcode[8];
    char rs[8];
    char rt[8];
    char rd[8];
    char sa[8];
    char imm[16];
    char func[16];
    char target[32];
    int nombre_P = 0;
    int nombre_R = 0;
    int nombre_I = 0;
    int nombre_J = 0;
    
    FILE* f;
    INSTRUCTION_DEFINITION* tab;
    
    f = fopen(nom_fichier,"r");
    
    if (f == NULL){
        printf("fichier non trouvé\n");
        return NULL;
    }
    
    if( fscanf(f,"%d", nombre_instruction) != 1){
        printf("nombre d'instruction non trouvé\n");
        return NULL;
    }
    
    tab=calloc(*nombre_instruction,sizeof(*tab));
    
    if(tab == NULL){
        printf("table du dictionnaire non creee\n");
        return NULL;
    }
    
    if( fscanf(f,"%d", &nombre_P) != 1){
        printf("nombre d'instruction non trouvé\n");
        return NULL;
    }
    
    for (i=0; i<nombre_P; i++) {
        if(fscanf(f, "%s %d %s %s %s %c", s, &(tab[i].nombre_operande), op1, op2, op3, &(tab[i].type)) != 6){
                free(tab);
            printf("Nombre de case du dico non respecté\n");
                return NULL;
            }
        tab[i].instruction = strdup(s);
        tab[i].type_operande1 = strdup(op1);
        tab[i].type_operande2 = strdup(op2);
        tab[i].type_operande3 = strdup(op3);
    }
    
    if( fscanf(f,"%d", &nombre_R) != 1){
        printf("nombre d'instruction non trouvé\n");
        return NULL;
    }
    
    for (i=nombre_P; i<nombre_R+nombre_P; i++) {
        if(fscanf(f, "%s %d %s %s %s %c %s %s %s %s %s %s", s, &(tab[i].nombre_operande), op1, op2, op3, &(tab[i].type), opcode, rs, rt, rd, sa,  func) != 12){
            free(tab);
            printf("Nombre de case du dico non respecté\n");
            return NULL;
        }
        tab[i].instruction = strdup(s);
        tab[i].type_operande1 = strdup(op1);
        tab[i].type_operande2 = strdup(op2);
        tab[i].type_operande3 = strdup(op3);
        tab[i].type_instruction.asR.opcode = strtol(strdup(opcode), NULL, 2);
        tab[i].type_instruction.asR.func = strtol(strdup(func), NULL, 2);
        if (verification_dico(rs) == 0){
            tab[i].type_instruction.asR.rs = strtol(strdup(rs), NULL, 2);
        }
        if (verification_dico(rt) == 0){
            tab[i].type_instruction.asR.rt = strtol(strdup(rs), NULL, 2);
        }
        if (verification_dico(rd) == 0){
            tab[i].type_instruction.asR.rd = strtol(strdup(rs), NULL, 2);
        }
        if (verification_dico(sa) == 0){
            tab[i].type_instruction.asR.rs = strtol(strdup(sa), NULL, 2);
        }
            
    }
    
    if( fscanf(f,"%d", &nombre_I) != 1){
        printf("nombre d'instruction non trouvé\n");
        return NULL;
    }
    
    for (i=nombre_R+nombre_P; i<nombre_R+nombre_P+nombre_I; i++) {
        if(fscanf(f, "%s %d %s %s %s %c %s %s %s %s", s, &(tab[i].nombre_operande), op1, op2, op3, &(tab[i].type), opcode, rs, rt, imm) != 10){
            free(tab);
            printf("Nombre de case du dico non respecté\n");
            return NULL;
        }
        tab[i].instruction = strdup(s);
        tab[i].type_operande1 = strdup(op1);
        tab[i].type_operande2 = strdup(op2);
        tab[i].type_operande3 = strdup(op3);
        tab[i].type_instruction.asI.opcode = strtol(strdup(opcode), NULL, 2);
        if (verification_dico(rs) == 0){
            tab[i].type_instruction.asI.rs = strtol(strdup(rs), NULL, 2);
        }
        if (verification_dico(rt) == 0){
            tab[i].type_instruction.asI.rt = strtol(strdup(rs), NULL, 2);
        }
        if (verification_dico(rd) == 0){
            tab[i].type_instruction.asI.immediate = strtol(strdup(imm), NULL, 2);
        }
    }
    
    if( fscanf(f,"%d", &nombre_J) != 1){
        printf("nombre d'instruction non trouvé\n");
        return NULL;
    }
    
    for (i=nombre_R+nombre_P+nombre_I; i<nombre_R+nombre_P+nombre_I+nombre_J; i++) {
        if(fscanf(f, "%s %d %s %s %s %c %s %s", s, &(tab[i].nombre_operande), op1, op2, op3, &(tab[i].type), opcode, target) != 8){
            free(tab);
            printf("Nombre de case du dico non respecté\n");
            return NULL;
        }
        tab[i].instruction = strdup(s);
        tab[i].type_operande1 = strdup(op1);
        tab[i].type_operande2 = strdup(op2);
        tab[i].type_operande3 = strdup(op3);
        tab[i].type_instruction.asJ.opcode = strtol(strdup(opcode), NULL, 2);
    }
    
    
    fclose(f);
    return tab;
}


INSTRUCTION_DEFINITION* recherche_dico(INSTRUCTION_DEFINITION* tab, LEXEM lexeme, int nombre_instruction){
    INSTRUCTION_DEFINITION* ptr_recherche = NULL;
    int i;
    for (i=0;i<nombre_instruction;i++){
        ptr_recherche=&tab[i];
        if(strcasecmp(lexeme.token, ptr_recherche->instruction) == 0){
            return ptr_recherche;
        }
    }
    return NULL;
}

int verification_dico(char* operande){
    int i = 0;
    for (i = 0; i < strlen(operande); i++){
        if (!isdigit(operande[i])){
            return 1;
        }
    }
    return 0;
}

int verification_operande(CORR_REG* table_registre, char* type_operande, LEXEM lexeme, int nbr_registre){
    
    /*printf("OPERANDE = %s\n", lexeme.token);*/
    
    if (strcmp(type_operande, "REG") == 0) {
        CORR_REG corr_reg = recherche_correspondance(table_registre, lexeme.token, nbr_registre);
        return corr_reg.correspondance;
    } else if (strcmp(type_operande, "IMM") == 0) {
        long immediat_number = 0;
        if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
            immediat_number = lexeme.valeur;
        } else if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){
            immediat_number = strtol(lexeme.token, NULL, 0);
        } else {
            WARNING_MSG("L'operande %s n'est pas bon -> ligne n°%d", lexeme. token, lexeme.numero_de_ligne);
        }
        if (immediat_number <= USHRT_MAX && immediat_number >= SHRT_MIN){
            return immediat_number;
        } else {
            WARNING_MSG("L'operande %s n'est pas bon -> ligne n°%d", lexeme. token, lexeme.numero_de_ligne);
        }
    } else if (strcmp(type_operande, "SA") == 0) {
        long decalage_num = strtol(lexeme.token, NULL, 0);
        if (decalage_num >= 0 && decalage_num <= 2*MB_LEN_MAX-1){
            return decalage_num;
        } else {
            WARNING_MSG("L'operande %s n'est pas bon -> ligne n°%d", lexeme. token, lexeme.numero_de_ligne);
        }
    } else if (strcmp(type_operande, "REL") == 0) {
        if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
            if (lexeme.valeur % 4 == 0){
                if (lexeme.valeur <= 2*USHRT_MAX-1 && lexeme.valeur >= -2*USHRT_MAX){/* codé sur 28 bits (pas de fonction utilisable dans la librairie limits)*/
                    return (lexeme.valeur >> 2);
                } else {
                    WARNING_MSG("Nombre absolue negatif ou trop grand -> ligne n°%d", lexeme.numero_de_ligne);
                }
            } else {
                WARNING_MSG("Nombre absolue non divisible par 4 -> ligne n°%d", lexeme.numero_de_ligne);
            }
        } else {
            long relative_number = strtol(lexeme.token, NULL, 0);
            if (relative_number % 4 == 0){
                if (relative_number <= 2*USHRT_MAX-1 && relative_number >= -2*USHRT_MAX){
                    return (relative_number >> 2);
                } else {
                    WARNING_MSG("Nombre relatif trop grand -> ligne n°%d", lexeme.numero_de_ligne);
                }
            } else {
                WARNING_MSG("Nombre relatif non divisible par 4 -> ligne n°%d", lexeme.numero_de_ligne);
            }
        }
    } else if (strcmp(type_operande, "ABS") == 0) {
        if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
            if (lexeme.valeur % 4 == 0){
                if (lexeme.valeur > -1 && lexeme.valeur <= 268435455){/* codé sur 28 bits (pas de fonction utilisable dans la librairie limits)*/
                    return (lexeme.valeur >> 2);
                } else {
                    WARNING_MSG("Nombre absolue negatif ou trop grand -> ligne n°%d", lexeme.numero_de_ligne);
                }
            } else {
                WARNING_MSG("Nombre absolue non divisible par 4 -> ligne n°%d", lexeme.numero_de_ligne);
            }
        } else {
            long absolute_number = strtol(lexeme.token, NULL, 0);
            if (absolute_number % 4 == 0){
                if (absolute_number >= 0 && absolute_number <= 268435455) {/* codé sur 28 bits (pas de fonction utilisable dans la librairie limits)*/
                    return (absolute_number >> 2);
                } else {
                        WARNING_MSG("Nombre absolue negatif ou trop grand -> ligne n°%d", lexeme.numero_de_ligne);
                    }
            } else {
                WARNING_MSG("Nombre absolue non divisible par 4 -> ligne n°%d", lexeme.numero_de_ligne);
            }
        }
    } else if (strcmp(type_operande, "BAS_OFF") == 0) {
        return lexeme.valeur;
    } else if (strcmp(type_operande, "NONE") == 0){
        return 0;
    } else {
        WARNING_MSG("Type operande inconnu %s -> ligne n°%d", type_operande, lexeme.numero_de_ligne);
    }
}

