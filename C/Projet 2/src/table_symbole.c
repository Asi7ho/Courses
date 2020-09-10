
#include <table_symbole.h>

int detecte_nombre_etiquette(FILE_LEXEME f){
    
    int nombre_etiquette = 0;
    FILE_LEXEME stk;
    LEXEM lexeme;
    
    for (stk = f->suiv; stk != f; stk = stk->suiv){
        lexeme = stk->val;
        if (lexeme.type_lexeme == ETIQUETTE){
            nombre_etiquette += 1;
        }
    }
    
    lexeme = stk->val;
    if (lexeme.type_lexeme == ETIQUETTE){
        nombre_etiquette += 1;
    }
    return nombre_etiquette;
}

SYMBOLE* charge_table_symbole(FILE_LEXEME f, INSTRUCTION_DEFINITION* dico, int nombre_etiquette, int nbr_instructions){
    
    FILE_LEXEME stk;
    LEXEM lexeme;
    int etat = INIT_GENERALE;
    SECTION section;
    SYMBOLE symbole;
    INSTRUCTION_DEFINITION* instruction_def;
    
    char* stk_lexeme = NULL;
    int decalage_inst = 0;
    int decalage_data = 0;
    int decalage_bss = 0;
    int compteur_operande = 0;
    int i = 0;
    
    SYMBOLE* tab = calloc(nombre_etiquette, sizeof(*tab));
    
    for (stk = f->suiv; stk != f; stk = stk->suiv){
        
        lexeme = stk->val;
        switch(etat){
                
            case INIT_GENERALE:
                if (strcasecmp(".text", lexeme.token) == 0) {
                    etat = INIT_TEXT;
                    section = INST;
                } else if (strcasecmp(".data", lexeme.token) == 0){
                    etat = INIT_DATA;
                    section = DATA;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                }
                else {
                    etat = INIT_GENERALE;
                }
                break;
                
            case INIT_TEXT:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie que l'instruction existe dans notre dictionnaire*/
                    instruction_def = recherche_dico(dico, lexeme, nbr_instructions);
                    etat = TEXT;
                    if (instruction_def->nombre_operande == 0){ /*Si le nombre d'operande est nul alors il ajoute tout de suite cette instruction dans la file*/
                        decalage_inst += 4;
                        compteur_operande = 0;
                        etat = INIT_TEXT;
                    }
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS; /*On change de section*/
                    section = BSS;
                } else if (strcasecmp(".data", lexeme.token) == 0){
                    etat = INIT_DATA; /*On change de section*/
                    section = DATA;
                } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_inst;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_TEXT;
                } else {
                    etat = INIT_TEXT;
                }
                break;
                
            case TEXT:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                    if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0 || stk->suiv->val.type_lexeme == ETIQUETTE){ /*On vérifie que l'operande est dans la table des symboles*/
                        if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                            decalage_inst += 8;
                        } else {
                            decalage_inst += 4;
                        }
                        compteur_operande = 0;
                        etat = INIT_TEXT;
                        
                    } else {
                        etat = TEXT;
                    }
                    
                } else if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || stk->suiv->val.type_lexeme == ETIQUETTE){
                    if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    etat = INIT_TEXT;
                }else if (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0){
                    if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    etat = INIT_TEXT;
                } else if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = TEXT;
                    compteur_operande += 1;
                } else if (lexeme.type_lexeme == PARENTHESE_OUVERTE) { /*TO DO: Traiter les offset(base)*/
                    etat = TEXT;
                    compteur_operande += 1;
                } else if (lexeme.type_lexeme == PARENTHESE_FERMEE){
                    if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    etat = INIT_TEXT;
                } else {
                    etat = TEXT;
                }
                
                break;
                
            case INIT_BSS:
                if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".data", lexeme.token) == 0){
                    etat = INIT_DATA; /*On change de section*/
                    section = DATA;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT; /*On change de section*/
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_bss;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_BSS;
                } else {
                    etat = INIT_BSS;
                }
                break;
                
            case INIT_DATA:
                if (strcasecmp(".word", lexeme.token) == 0){
                    etat = WORD;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS; /*On change de section*/
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT; /*On change de section*/
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_data;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_DATA;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case ASCIIZ:
                if (lexeme.type_lexeme == STRING){ /*On teste le type de l'operande*/
                    decalage_data += (strlen(lexeme.token) + 1); /*On prend en compte le caractere '\0'*/
                    etat = SUITE_DATA_ASCIIZ;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case SPACE:
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                    if (section == DATA){ /*On ajoute l'operande dans la bonne section*/
                        decalage_data += 4;
                        etat = INIT_DATA;
                    } else if (section == BSS) {
                        decalage_bss += 4;
                        etat = INIT_BSS;
                    } else {
                        etat = INIT_DATA;
                    }
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case BYTE:
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                    decalage_data += 1;
                    etat = SUITE_DATA_BYTE;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case WORD:
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                    decalage_data += 4;
                    etat = SUITE_DATA_WORD;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case SUITE_DATA_ASCIIZ:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = ASCIIZ; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_data;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_DATA;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case SUITE_DATA_BYTE:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = BYTE; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_data;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_DATA;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
            case SUITE_DATA_WORD:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = WORD; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    stk_lexeme = lexeme.token;
                    stk_lexeme[strlen(stk_lexeme)-1] = 0;
                    symbole.nom = stk_lexeme;
                    symbole.section = section;
                    symbole.ligne = lexeme.numero_de_ligne;
                    symbole.decalage = decalage_data;
                    tab[i] = symbole;
                    i += 1;
                    etat = INIT_DATA;
                } else {
                    etat = INIT_DATA;
                }
                break;
                
        }
    }
    
    lexeme = stk->val;
    switch(etat){
            
        case INIT_GENERALE:
            if (strcasecmp(".text", lexeme.token) == 0) {
                etat = INIT_TEXT;
                section = INST;
            } else if (strcasecmp(".data", lexeme.token) == 0){
                etat = INIT_DATA;
                section = DATA;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            }
            else {
                etat = INIT_GENERALE;
            }
            break;
            
        case INIT_TEXT:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie que l'instruction existe dans notre dictionnaire*/
                instruction_def = recherche_dico(dico, lexeme, nbr_instructions);
                etat = TEXT;
                if (instruction_def->nombre_operande == 0){ /*Si le nombre d'operande est nul alors il ajoute tout de suite cette instruction dans la file*/
                    decalage_inst += 4;
                    compteur_operande = 0;
                    etat = INIT_TEXT;
                }
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS; /*On change de section*/
                section = BSS;
            } else if (strcasecmp(".data", lexeme.token) == 0){
                etat = INIT_DATA; /*On change de section*/
                section = DATA;
            } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_inst;
                tab[i] = symbole;
                i += 1;
                etat = INIT_TEXT;
            } else {
                etat = INIT_TEXT;
            }
            break;
            
        case TEXT:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0 || stk->suiv->val.type_lexeme == ETIQUETTE){ /*On vérifie que l'operande est dans la table des symboles*/
                    if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    etat = INIT_TEXT;
                    
                } else {
                    etat = TEXT;
                }
                
            } else if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || stk->suiv->val.type_lexeme == ETIQUETTE){
                if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                    decalage_inst += 8;
                } else {
                    decalage_inst += 4;
                }
                compteur_operande = 0;
                etat = INIT_TEXT;
            }else if (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0){
                if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                    decalage_inst += 8;
                } else {
                    decalage_inst += 4;
                }
                compteur_operande = 0;
                etat = INIT_TEXT;
            } else if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = TEXT;
                compteur_operande += 1;
            } else if (lexeme.type_lexeme == PARENTHESE_OUVERTE) { /*TO DO: Traiter les offset(base)*/
                etat = TEXT;
                compteur_operande += 1;
            } else if (lexeme.type_lexeme == PARENTHESE_FERMEE){
                if (((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P')) || ((strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')) || strcasecmp(instruction_def->instruction, "BLT") == 0){
                    decalage_inst += 8;
                } else {
                    decalage_inst += 4;
                }
                compteur_operande = 0;
                etat = INIT_TEXT;
            } else {
                etat = TEXT;
            }
            
            break;
            
        case INIT_BSS:
            if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".data", lexeme.token) == 0){
                etat = INIT_DATA; /*On change de section*/
                section = DATA;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT; /*On change de section*/
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_bss;
                tab[i] = symbole;
                i += 1;
                etat = INIT_BSS;
            } else {
                etat = INIT_BSS;
            }
            break;
            
        case INIT_DATA:
            if (strcasecmp(".word", lexeme.token) == 0){
                etat = WORD;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS; /*On change de section*/
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT; /*On change de section*/
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){ /*Si on a une etiquette on complette la table des symbole*/
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_data;
                tab[i] = symbole;
                i += 1;
                etat = INIT_DATA;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case ASCIIZ:
            if (lexeme.type_lexeme == STRING){ /*On teste le type de l'operande*/
                decalage_data += (strlen(lexeme.token) + 1); /*On prend en compte le caractere '\0'*/
                etat = SUITE_DATA_ASCIIZ;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case SPACE:
            if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                if (section == DATA){ /*On ajoute l'operande dans la bonne section*/
                    decalage_data += 4;
                    etat = INIT_DATA;
                } else if (section == BSS) {
                    decalage_bss += 4;
                    etat = INIT_BSS;
                } else {
                    etat = INIT_DATA;
                }
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case BYTE:
            if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                decalage_data += 1;
                etat = SUITE_DATA_BYTE;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case WORD:
            if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                decalage_data += 4;
                etat = SUITE_DATA_WORD;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case SUITE_DATA_ASCIIZ:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = ASCIIZ; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_data;
                tab[i] = symbole;
                i += 1;
                etat = INIT_DATA;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case SUITE_DATA_BYTE:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = BYTE; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_data;
                tab[i] = symbole;
                i += 1;
                etat = INIT_DATA;
            } else {
                etat = INIT_DATA;
            }
            break;
            
        case SUITE_DATA_WORD:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = WORD; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                stk_lexeme = lexeme.token;
                stk_lexeme[strlen(stk_lexeme)-1] = 0;
                symbole.nom = stk_lexeme;
                symbole.section = section;
                symbole.ligne = lexeme.numero_de_ligne;
                symbole.decalage = decalage_data;
                tab[i] = symbole;
                i += 1;
                etat = INIT_DATA;
            } else {
                etat = INIT_DATA;
            }
            break;
            
    }
    
    return tab;
}

void visualiser_table_symbole(SYMBOLE* tab, int nombre_etiquette){
    int i = 0;
    if (tab == NULL) {
        return;
    }
    
    printf("\n.symtab\n");
    for (i = 0; i < nombre_etiquette; i++){
        printf("% d    %s:%08d %s\n", tab[i].ligne, section_string(tab[i].section), tab[i].decalage, tab[i].nom);
    }
}

int est_dans_la_table(SYMBOLE* tab, LEXEM lexeme, int nombre_etiquette, SYMBOLE* symbole){
    int i = 0;
    for (i = 0; i < nombre_etiquette; i++){
        if (strcmp(lexeme.token, tab[i].nom) == 0){
            (*symbole) = tab[i];
            return 0;
        }
    }
    return 1;
}
