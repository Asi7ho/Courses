
#include <gram.h>

void automate_generale(FILE_LEXEME f, INSTRUCTION_DEFINITION* dico, FILE_DE_DATA* pf_d, FILE_DE_BSS* pf_b, FILE_INSTRUCTION* pf_i, SYMBOLE* tab_symbole, FILE_RELOCATION* pf_rtxt, FILE_RELOCATION* pf_rdata, int nbr_etiquette, int nbr_instructions, CORR_REG* table_registre, int nbr_registre){
    
    FILE_LEXEME stk;
    LEXEM lexeme;
    DATA_BSS data;
    DATA_BSS bss;
    INSTRUCTION instruction;
    INSTRUCTION_DEFINITION* instruction_def = NULL;
    ETAT_GRAMMATICALE etat = INIT_GENERALE;
    SECTION section;
    SYMBOLE symbole;
    RELOCATION reloc;
    
    char* stk_lexeme = NULL;
    long stk_valeur = 0;
    int decalage_inst = 0;
    int decalage_data = 0;
    int decalage_bss = 0;
    int compteur_operande = 0;
    
    
    for (stk = f->suiv; stk != f; stk = stk->suiv){
        
        lexeme = stk->val;
        switch(etat){
                
            case INIT_GENERALE:
                if (lexeme.type_lexeme == COMMENTAIRE){
                    etat = INIT_GENERALE; /*Si c'est un commentaire on va a l'instruction suivante*/
                } else if (strcmp(".set", lexeme.token) == 0){
                    etat = DEBUT_SET;
                } else if (strcasecmp(".text", lexeme.token) == 0) {
                    etat = INIT_TEXT;
                    section = INST;
                } else if (strcmp(".data", lexeme.token) == 0){
                    etat = INIT_DATA;
                    section = DATA;
                } else if (strcmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> Pas de directive", lexeme.numero_de_ligne);
                    etat = INIT_GENERALE;
                }
                break;
                    
            case DEBUT_SET:
                    if (strcmp("noreorder", lexeme.token) == 0){
                        DEBUG_MSG("\n\n.set noreorder -> PAS D'OPTIMISATION"); /* .set doit toujours etre suivit de noreorder */
                        etat = INIT_GENERALE;
                    } else {
                        WARNING_MSG("Erreur a la ligne n° %d -> noreorder manquant", lexeme.numero_de_ligne);
                        etat = INIT_GENERALE;
                    }
                    break;
                
            case INIT_TEXT:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                    instruction_def = recherche_dico(dico, lexeme, nbr_instructions); /*On verifie que l'instruction existe dans notre dictionnaire*/
                    if (instruction_def == NULL){
                        WARNING_MSG("Une instruction est inconnue -> verifier instruction %s à la ligne %d", lexeme.token, lexeme.numero_de_ligne);
                    } else {
                        if (strcasecmp(instruction_def->instruction, "LW") == 0 && stk->suiv->suiv->suiv->val.type_lexeme != MOT_OU_INSTRUCTION){
                            instruction_def = recherche_dico(instruction_def + 1, lexeme, nbr_instructions - 5);
                        } else if (strcasecmp(instruction_def->instruction, "LW") == 0 && stk->suiv->suiv->suiv->val.type_lexeme != MOT_OU_INSTRUCTION){
                            instruction_def = recherche_dico(instruction_def + 1, lexeme, nbr_instructions - 6);
                        } else {
                            etat = TEXT;
                            stk_lexeme = lexeme.token;
                            if (instruction_def->nombre_operande == 0){ /*Si le nombre d'operande est nul alors on ajoute tout de suite cette instruction dans la file*/
                                instruction.section = section;
                                instruction.nom = stk_lexeme;
                                instruction.nombre_operande = instruction_def->nombre_operande;
                                instruction.ligne = lexeme.numero_de_ligne;
                                instruction.decalage = decalage_inst;
                                decalage_inst += 4;
                                compteur_operande = 0;
                                *pf_i = enfiler_inst(instruction, *pf_i);
                                etat = INIT_TEXT;
                            }
                        }
                    }
                } else if (strcmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS; /*On change de section*/
                    section = BSS;
                } else if (strcmp(".data", lexeme.token) == 0){
                    etat = INIT_DATA; /*On change de section*/
                    section = DATA;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_TEXT;
                } else if (lexeme.type_lexeme == COMMENTAIRE){
                    etat = INIT_TEXT; /*Si c'est un commentaire on va a l'instruction suivante*/
                } else if (lexeme.type_lexeme == PARENTHESE_FERMEE) { /*TO DO: Traiter les offset(base)*/
                    etat = INIT_TEXT;
                } else {
                    WARNING_MSG("Erreur sur %s a la ligne n°%d", lexeme.token, lexeme.numero_de_ligne);
                    etat = INIT_TEXT;
                }
                break;
                
            case TEXT:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                    
                    if ((est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0 && recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL) || (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0 && (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0 || stk->suiv->val.type_lexeme == ETIQUETTE))){
                        
                        /*On effectue une demande de relocation*/
                        
                        lexeme.valeur = symbole.decalage;
                        
                        reloc.nom = lexeme.token;
                        reloc.section_arrive = symbole.section;
                        reloc.decalage_arrivee = symbole.decalage;
                        reloc.decalage_depart = decalage_inst;
                        reloc.section_de_depart = section;
                        
                        if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')){
                            reloc.relocation = "R_MIPS_HI16";
                            *pf_rtxt = enfiler_reloc(reloc, *pf_rtxt);
                            reloc.decalage_depart = decalage_inst + 4;
                            reloc.relocation = "R_MIPS_LO16";
                        } else if (instruction_def->type == 'J'){
                            reloc.relocation = "R_MIPS_26";
                        } else if (instruction_def->type == 'I'){
                            reloc.relocation = "R_MIPS_16";
                        } else if (instruction_def->type == 'R'){
                            reloc.relocation = "R_MIPS_32";
                        } else {
                            WARNING_MSG("ERREUR DE RELOCALISATION -> ligne n°%d", lexeme.numero_de_ligne);
                        }
                        
                        *pf_rtxt = enfiler_reloc(reloc, *pf_rtxt);
                        
                        instruction.section = section;
                        instruction.nom = stk_lexeme;
                        instruction.nombre_operande = instruction_def->nombre_operande;
                        instruction.ligne = lexeme.numero_de_ligne;
                        instruction.decalage = decalage_inst;
                        if (compteur_operande + 1 == 1){
                            lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                        } else if (compteur_operande + 1 == 2){
                            lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                        } else if (compteur_operande + 1 == 3) {
                            lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                        } else {
                            WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                        }
                        
                        instruction.operande[compteur_operande] = lexeme;
                        if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                            decalage_inst += 8;
                        } else {
                            decalage_inst += 4;
                        }
                        compteur_operande = 0;
                        *pf_i = enfiler_inst(instruction, *pf_i);
                        etat = INIT_TEXT;
                        
                    } else {
                        WARNING_MSG("SYMBOLE ou INSTRUCTION %s NON TROUVE a la ligne %d", lexeme.token, lexeme.numero_de_ligne);
                        etat = TEXT;
                    }
                    
                } else if (instruction_def->nombre_operande == 1) {
                
                    instruction.section = section;
                    instruction.nom = stk_lexeme;
                    instruction.nombre_operande = instruction_def->nombre_operande;
                    instruction.ligne = lexeme.numero_de_ligne;
                    instruction.decalage = decalage_inst;
                    
                    if (compteur_operande + 1 == 1){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                    } else {
                        WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                    }
                    
                    instruction.operande[compteur_operande] = lexeme;
                    decalage_inst += 4;
                    compteur_operande = 0;
                    *pf_i = enfiler_inst(instruction, *pf_i);
                    etat = INIT_TEXT;
                    
                }else if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || stk->suiv->val.type_lexeme == ETIQUETTE || stk->suiv->val.type_lexeme == PARENTHESE_FERMEE){
                    
                    if (compteur_operande + 1 == 1){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 2){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 3) {
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                    } else {
                        WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                    }
                    
                    instruction.operande[compteur_operande] = lexeme;
                    if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    *pf_i = enfiler_inst(instruction, *pf_i);
                    etat = INIT_TEXT;
                    
                } else if (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0){
                    
                    instruction.section = section;
                    instruction.nom = stk_lexeme;
                    instruction.nombre_operande = instruction_def->nombre_operande;
                    instruction.ligne = lexeme.numero_de_ligne;
                    instruction.decalage = decalage_inst;
                    if (compteur_operande + 1 == 1){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 2){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 3) {
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                    } else {
                        WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                    }
                    
                    instruction.operande[compteur_operande] = lexeme;
                    if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    *pf_i = enfiler_inst(instruction, *pf_i);
                    etat = INIT_TEXT;
                    
                } else if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    
                    etat = TEXT;
                    compteur_operande += 1;
                    
                } else if (lexeme.type_lexeme == PARENTHESE_OUVERTE) { /*TO DO: Traiter les offset(base)*/
                    
                    etat = TEXT;
                    compteur_operande += 1;
                    
                } else {
                    
                    instruction.section = section;
                    instruction.nom = stk_lexeme;
                    instruction.nombre_operande = instruction_def->nombre_operande;
                    instruction.ligne = lexeme.numero_de_ligne;
                    instruction.decalage = decalage_inst;
                    
                    if (compteur_operande + 1 == 1){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 2){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 3) {
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                    } else {
                        WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                    }
                    
                    instruction.operande[compteur_operande] = lexeme;
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
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_BSS;
                } else if (lexeme.type_lexeme == COMMENTAIRE){ /*Si c'est un commentaire on va a l'instruction suivante*/
                    etat = INIT_BSS;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_BSS;
                }
                break;
                
            case INIT_DATA:
                if (strcasecmp(".word", lexeme.token) == 0){
                    etat = WORD;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS; /*On change de section*/
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT; /*On change de section*/
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_DATA;
                } else if (lexeme.type_lexeme == COMMENTAIRE){ /*Si c'est un commentaire on va a l'instruction suivante*/
                    etat = INIT_DATA;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
            case ASCIIZ:
                if (lexeme.type_lexeme == STRING){ /*On teste le type de l'operande*/
                    data.section = section;
                    data.nom = stk_lexeme;
                    data.etat = lexeme.type_lexeme;
                    data.decalage = decalage_data;
                    data.ligne = lexeme.numero_de_ligne;
                    data.valeur.AsString = lexeme.token;
                    
                    decalage_data += (strlen(lexeme.token) + 1); /*On prend en compte le caractere '\0'*/
                    
                    *pf_d = enfiler_data(data, *pf_d);
                    etat = SUITE_DATA_ASCIIZ;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> .asciiz doit une chaine de caractère comme operande", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
            case SPACE:
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                    if (section == DATA){ /*On ajoute l'operande dans la bonne section*/
                        data.section = section;
                        data.nom = stk_lexeme;
                        data.etat = lexeme.type_lexeme;
                        data.decalage = decalage_data;
                        data.ligne = lexeme.numero_de_ligne;
                        data.valeur.AsNumber = strtol(lexeme.token, NULL, 0);
                        decalage_data += data.valeur.AsNumber;
                        *pf_d = enfiler_data(data, *pf_d);
                        etat = INIT_DATA;
                    } else if (section == BSS) {
                        bss.section = section;
                        bss.nom = stk_lexeme;
                        bss.etat = lexeme.type_lexeme;
                        bss.decalage = decalage_bss;
                        bss.ligne = lexeme.numero_de_ligne;
                        bss.valeur.AsNumber = strtol(lexeme.token, NULL, 0);
                        decalage_bss += bss.valeur.AsNumber;
                        *pf_b = enfiler_bss(bss, *pf_b);
                        etat = INIT_BSS;
                    } else {
                        WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                        etat = INIT_DATA;
                    }
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> .space doit un nombre comme operande", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
            case BYTE:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie si on a une relocation ou pas*/
                    if (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0){
                        stk_valeur = symbole.decalage;
                        
                        reloc.nom = lexeme.token;
                        reloc.section_arrive = symbole.section;
                        reloc.decalage_arrivee = symbole.decalage;
                        reloc.decalage_depart = decalage_data;
                        reloc.section_de_depart = section;
                        reloc.relocation = "R_MIPS_32";
                        *pf_rdata = enfiler_reloc(reloc, *pf_rdata);
                    }
                } else {
                    stk_valeur = strtol(lexeme.token, NULL, 0);
                }
                
                if (stk_valeur < CHAR_MIN || stk_valeur > UCHAR_MAX){
                    WARNING_MSG("DEPASSEMENT DE VALEUR -> ligne n°%d", lexeme.numero_de_ligne);
                } else {
                    if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO || lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On teste le type de l'operande*/
                        data.section = section;
                        data.nom = stk_lexeme;
                        data.etat = lexeme.type_lexeme;
                        data.decalage = decalage_data;
                        data.ligne = lexeme.numero_de_ligne;
                        data.valeur.AsNumber = stk_valeur;
                        
                        decalage_data += 1;
                        
                        *pf_d = enfiler_data(data, *pf_d);
                        etat = SUITE_DATA_BYTE;
                    } else {
                        WARNING_MSG("Erreur a la ligne n° %d -> .byte doit un nombre comme operande", lexeme.numero_de_ligne);
                        etat = INIT_DATA;
                    }
                }
                break;
                
            case WORD:
                if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie si on a une relocation ou pas*/
                    if (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0){
                        stk_valeur = symbole.decalage;
                        
                        reloc.nom = lexeme.token;
                        reloc.section_arrive = symbole.section;
                        reloc.decalage_arrivee = symbole.decalage;
                        reloc.decalage_depart = decalage_data;
                        reloc.section_de_depart = section;
                        reloc.relocation = "R_MIPS_32";
                        *pf_rdata = enfiler_reloc(reloc, *pf_rdata);
                    }
                } else {
                    stk_valeur = strtol(lexeme.token, NULL, 0);
                }
                
                if (stk_valeur < INT_MIN || stk_valeur > UINT_MAX){
                    WARNING_MSG("DEPASSEMENT DE VALEUR -> ligne n°%d", lexeme.numero_de_ligne);
                } else {
                    if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO || lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On teste le type de l'operande*/
                        data.section = section;
                        data.nom = stk_lexeme;
                        data.etat = lexeme.type_lexeme;
                        data.decalage = decalage_data;
                        data.ligne = lexeme.numero_de_ligne;
                        data.valeur.AsNumber = stk_valeur;
                        
                        decalage_data += 4;
                        
                        *pf_d = enfiler_data(data, *pf_d);
                        etat = SUITE_DATA_WORD;
                    } else {
                        WARNING_MSG("Erreur a la ligne n° %d -> .word doit un nombre comme operande", lexeme.numero_de_ligne);
                        etat = INIT_DATA;
                    }
                }
                break;
                
            case SUITE_DATA_ASCIIZ:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = ASCIIZ; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_DATA;
                } else if (lexeme.type_lexeme == COMMENTAIRE){
                    etat = INIT_DATA;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
                
            case SUITE_DATA_BYTE:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = BYTE; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_DATA;
                } else if (lexeme.type_lexeme == COMMENTAIRE){
                    etat = INIT_DATA;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
            case SUITE_DATA_WORD:
                if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                    etat = WORD; /*On cree une autre cellule avec le decalage qu'il faut*/
                } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                    etat = WORD;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".byte", lexeme.token) == 0){
                    etat = BYTE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".space", lexeme.token) == 0){
                    etat = SPACE;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                    etat = ASCIIZ;
                    stk_lexeme = lexeme.token;
                } else if (strcasecmp(".bss", lexeme.token) == 0) {
                    etat = INIT_BSS;
                    section = BSS;
                } else if (strcasecmp(".text", lexeme.token) == 0){
                    etat = INIT_TEXT;
                    section = INST;
                } else if (lexeme.type_lexeme == ETIQUETTE){
                    etat = INIT_DATA;
                } else if (lexeme.type_lexeme == COMMENTAIRE){
                    etat = INIT_DATA;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
                break;
                
        }
        
    }
    
    lexeme = stk->val;
    switch(etat){
            
        case INIT_GENERALE:
            if (lexeme.type_lexeme == COMMENTAIRE){
                etat = INIT_GENERALE; /*Si c'est un commentaire on va a l'instruction suivante*/
            } else if (strcmp(".set", lexeme.token) == 0){
                etat = DEBUT_SET;
            } else if (strcasecmp(".text", lexeme.token) == 0) {
                etat = INIT_TEXT;
                section = INST;
            } else if (strcmp(".data", lexeme.token) == 0){
                etat = INIT_DATA;
                section = DATA;
            } else if (strcmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> Pas de directive", lexeme.numero_de_ligne);
                etat = INIT_GENERALE;
            }
            break;
            
        case DEBUT_SET:
            if (strcmp("noreorder", lexeme.token) == 0){
                DEBUG_MSG("\n\n.set noreorder -> PAS D'OPTIMISATION"); /* .set doit toujours etre suivit de noreorder */
                etat = INIT_GENERALE;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> noreorder manquant", lexeme.numero_de_ligne);
                etat = INIT_GENERALE;
            }
            break;
            
        case INIT_TEXT:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                instruction_def = recherche_dico(dico, lexeme, nbr_instructions); /*On verifie que l'instruction existe dans notre dictionnaire*/
                if (instruction_def == NULL){
                    WARNING_MSG("Une instruction est inconnue -> verifier instruction %s à la ligne %d", lexeme.token, lexeme.numero_de_ligne);
                } else {
                    if (strcasecmp(instruction_def->instruction, "LW") == 0 && stk->suiv->suiv->suiv->val.type_lexeme != MOT_OU_INSTRUCTION){
                        instruction_def = recherche_dico(instruction_def + 1, lexeme, nbr_instructions - 5);
                    } else if (strcasecmp(instruction_def->instruction, "LW") == 0 && stk->suiv->suiv->suiv->val.type_lexeme != MOT_OU_INSTRUCTION){
                        instruction_def = recherche_dico(instruction_def + 1, lexeme, nbr_instructions - 6);
                    } else {
                        etat = TEXT;
                        stk_lexeme = lexeme.token;
                        if (instruction_def->nombre_operande == 0){ /*Si le nombre d'operande est nul alors on ajoute tout de suite cette instruction dans la file*/
                            instruction.section = section;
                            instruction.nom = stk_lexeme;
                            instruction.nombre_operande = instruction_def->nombre_operande;
                            instruction.ligne = lexeme.numero_de_ligne;
                            instruction.decalage = decalage_inst;
                            decalage_inst += 4;
                            compteur_operande = 0;
                            *pf_i = enfiler_inst(instruction, *pf_i);
                            etat = INIT_TEXT;
                        }
                    }
                }
            } else if (strcmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS; /*On change de section*/
                section = BSS;
            } else if (strcmp(".data", lexeme.token) == 0){
                etat = INIT_DATA; /*On change de section*/
                section = DATA;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_TEXT;
            } else if (lexeme.type_lexeme == COMMENTAIRE){
                etat = INIT_TEXT; /*Si c'est un commentaire on va a l'instruction suivante*/
            } else if (lexeme.type_lexeme == PARENTHESE_FERMEE) { /*TO DO: Traiter les offset(base)*/
                etat = INIT_TEXT;
            } else {
                WARNING_MSG("Erreur sur %s a la ligne n°%d", lexeme.token, lexeme.numero_de_ligne);
                etat = INIT_TEXT;
            }
            break;
            
        case TEXT:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){
                
                if ((est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0 && recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL) || (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0 && (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0 || stk->suiv->val.type_lexeme == ETIQUETTE))){
                    
                    /*On effectue une demande de relocation*/
                    
                    lexeme.valeur = symbole.decalage;
                    
                    reloc.nom = lexeme.token;
                    reloc.section_arrive = symbole.section;
                    reloc.decalage_arrivee = symbole.decalage;
                    reloc.decalage_depart = decalage_inst;
                    reloc.section_de_depart = section;
                    
                    if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')){
                        reloc.relocation = "R_MIPS_HI16";
                        *pf_rtxt = enfiler_reloc(reloc, *pf_rtxt);
                        reloc.decalage_depart = decalage_inst + 4;
                        reloc.relocation = "R_MIPS_LO16";
                    } else if (instruction_def->type == 'J'){
                        reloc.relocation = "R_MIPS_26";
                    } else if (instruction_def->type == 'I'){
                        reloc.relocation = "R_MIPS_16";
                    } else if (instruction_def->type == 'R'){
                        reloc.relocation = "R_MIPS_32";
                    } else {
                        WARNING_MSG("ERREUR DE RELOCALISATION -> ligne n°%d", lexeme.numero_de_ligne);
                    }
                    
                    *pf_rtxt = enfiler_reloc(reloc, *pf_rtxt);
                    
                    instruction.section = section;
                    instruction.nom = stk_lexeme;
                    instruction.nombre_operande = instruction_def->nombre_operande;
                    instruction.ligne = lexeme.numero_de_ligne;
                    instruction.decalage = decalage_inst;
                    if (compteur_operande + 1 == 1){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 2){
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                    } else if (compteur_operande + 1 == 3) {
                        lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                    } else {
                        WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                    }
                    
                    instruction.operande[compteur_operande] = lexeme;
                    if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                        decalage_inst += 8;
                    } else {
                        decalage_inst += 4;
                    }
                    compteur_operande = 0;
                    *pf_i = enfiler_inst(instruction, *pf_i);
                    etat = INIT_TEXT;
                    
                } else {
                    WARNING_MSG("SYMBOLE ou INSTRUCTION %s NON TROUVE a la ligne %d", lexeme.token, lexeme.numero_de_ligne);
                    etat = TEXT;
                }
                
            } else if (instruction_def->nombre_operande == 1) {
                
                instruction.section = section;
                instruction.nom = stk_lexeme;
                instruction.nombre_operande = instruction_def->nombre_operande;
                instruction.ligne = lexeme.numero_de_ligne;
                instruction.decalage = decalage_inst;
                
                if (compteur_operande + 1 == 1){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                } else {
                    WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                }
                
                instruction.operande[compteur_operande] = lexeme;
                decalage_inst += 4;
                compteur_operande = 0;
                *pf_i = enfiler_inst(instruction, *pf_i);
                etat = INIT_TEXT;
                
            }else if (recherche_dico(dico, stk->suiv->val, nbr_instructions) != NULL || stk->suiv->val.type_lexeme == ETIQUETTE || stk->suiv->val.type_lexeme == PARENTHESE_FERMEE){
                
                if (compteur_operande + 1 == 1){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 2){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 3) {
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                } else {
                    WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                }
                
                instruction.operande[compteur_operande] = lexeme;
                if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                    decalage_inst += 8;
                } else {
                    decalage_inst += 4;
                }
                compteur_operande = 0;
                *pf_i = enfiler_inst(instruction, *pf_i);
                etat = INIT_TEXT;
                
            } else if (strcmp(".data", stk->suiv->val.token) == 0 || strcmp(".bss", stk->suiv->val.token) == 0){
                
                instruction.section = section;
                instruction.nom = stk_lexeme;
                instruction.nombre_operande = instruction_def->nombre_operande;
                instruction.ligne = lexeme.numero_de_ligne;
                instruction.decalage = decalage_inst;
                if (compteur_operande + 1 == 1){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 2){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 3) {
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                } else {
                    WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                }
                
                instruction.operande[compteur_operande] = lexeme;
                if ((strcasecmp(instruction_def->instruction, "LW") == 0 && instruction_def->type == 'P') || (strcasecmp(instruction_def->instruction, "SW") == 0 && instruction_def->type == 'P')|| strcasecmp(instruction_def->instruction, "BLT") == 0){
                    decalage_inst += 8;
                } else {
                    decalage_inst += 4;
                }
                compteur_operande = 0;
                *pf_i = enfiler_inst(instruction, *pf_i);
                etat = INIT_TEXT;
                
            } else if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                
                etat = TEXT;
                compteur_operande += 1;
                
            } else if (lexeme.type_lexeme == PARENTHESE_OUVERTE) { /*TO DO: Traiter les offset(base)*/
                
                etat = TEXT;
                compteur_operande += 1;
                
            } else {
                
                instruction.section = section;
                instruction.nom = stk_lexeme;
                instruction.nombre_operande = instruction_def->nombre_operande;
                instruction.ligne = lexeme.numero_de_ligne;
                instruction.decalage = decalage_inst;
                
                if (compteur_operande + 1 == 1){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande1, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 2){
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande2, lexeme, nbr_registre);
                } else if (compteur_operande + 1 == 3) {
                    lexeme.valeur = verification_operande(table_registre, instruction_def->type_operande3, lexeme, nbr_registre);
                } else {
                    WARNING_MSG ("%d : NOMBRE D'OPERANDE NON CONFORME -> ligne n°%d", compteur_operande + 1, lexeme.numero_de_ligne);
                }
                
                instruction.operande[compteur_operande] = lexeme;
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
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_BSS;
            } else if (lexeme.type_lexeme == COMMENTAIRE){ /*Si c'est un commentaire on va a l'instruction suivante*/
                etat = INIT_BSS;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                etat = INIT_BSS;
            }
            break;
            
        case INIT_DATA:
            if (strcasecmp(".word", lexeme.token) == 0){
                etat = WORD;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS; /*On change de section*/
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT; /*On change de section*/
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_DATA;
            } else if (lexeme.type_lexeme == COMMENTAIRE){ /*Si c'est un commentaire on va a l'instruction suivante*/
                etat = INIT_DATA;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
        case ASCIIZ:
            if (lexeme.type_lexeme == STRING){ /*On teste le type de l'operande*/
                data.section = section;
                data.nom = stk_lexeme;
                data.etat = lexeme.type_lexeme;
                data.decalage = decalage_data;
                data.ligne = lexeme.numero_de_ligne;
                data.valeur.AsString = lexeme.token;
                
                decalage_data += (strlen(lexeme.token) + 1); /*On prend en compte le caractere '\0'*/
                
                *pf_d = enfiler_data(data, *pf_d);
                etat = SUITE_DATA_ASCIIZ;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> .asciiz doit une chaine de caractère comme operande", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
        case SPACE:
            if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO){ /*On teste le type de l'operande*/
                if (section == DATA){ /*On ajoute l'operande dans la bonne section*/
                    data.section = section;
                    data.nom = stk_lexeme;
                    data.etat = lexeme.type_lexeme;
                    data.decalage = decalage_data;
                    data.ligne = lexeme.numero_de_ligne;
                    data.valeur.AsNumber = strtol(lexeme.token, NULL, 0);
                    decalage_data += data.valeur.AsNumber;
                    *pf_d = enfiler_data(data, *pf_d);
                    etat = INIT_DATA;
                } else if (section == BSS) {
                    bss.section = section;
                    bss.nom = stk_lexeme;
                    bss.etat = lexeme.type_lexeme;
                    bss.decalage = decalage_bss;
                    bss.ligne = lexeme.numero_de_ligne;
                    bss.valeur.AsNumber = strtol(lexeme.token, NULL, 0);
                    decalage_bss += bss.valeur.AsNumber;
                    *pf_b = enfiler_bss(bss, *pf_b);
                    etat = INIT_BSS;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> .space doit un nombre comme operande", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
        case BYTE:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie si on a une relocation ou pas*/
                if (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0){
                    stk_valeur = symbole.decalage;
                    
                    reloc.nom = lexeme.token;
                    reloc.section_arrive = symbole.section;
                    reloc.decalage_arrivee = symbole.decalage;
                    reloc.decalage_depart = decalage_data;
                    reloc.section_de_depart = section;
                    reloc.relocation = "R_MIPS_32";
                    *pf_rdata = enfiler_reloc(reloc, *pf_rdata);
                }
            } else {
                stk_valeur = strtol(lexeme.token, NULL, 0);
            }
            
            if (stk_valeur < CHAR_MIN || stk_valeur > UCHAR_MAX){
                WARNING_MSG("DEPASSEMENT DE VALEUR -> ligne n°%d", lexeme.numero_de_ligne);
            } else {
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO || lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On teste le type de l'operande*/
                    data.section = section;
                    data.nom = stk_lexeme;
                    data.etat = lexeme.type_lexeme;
                    data.decalage = decalage_data;
                    data.ligne = lexeme.numero_de_ligne;
                    data.valeur.AsNumber = stk_valeur;
                    
                    decalage_data += 1;
                    
                    *pf_d = enfiler_data(data, *pf_d);
                    etat = SUITE_DATA_BYTE;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> .byte doit un nombre comme operande", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
            }
            break;
            
        case WORD:
            if (lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On verifie si on a une relocation ou pas*/
                if (est_dans_la_table(tab_symbole, lexeme, nbr_etiquette, &symbole) == 0){
                    stk_valeur = symbole.decalage;
                    
                    reloc.nom = lexeme.token;
                    reloc.section_arrive = symbole.section;
                    reloc.decalage_arrivee = symbole.decalage;
                    reloc.decalage_depart = decalage_data;
                    reloc.section_de_depart = section;
                    reloc.relocation = "R_MIPS_32";
                    *pf_rdata = enfiler_reloc(reloc, *pf_rdata);
                }
            } else {
                stk_valeur = strtol(lexeme.token, NULL, 0);
            }
            
            if (stk_valeur < INT_MIN || stk_valeur > UINT_MAX){
                WARNING_MSG("DEPASSEMENT DE VALEUR -> ligne n°%d", lexeme.numero_de_ligne);
            } else {
                if (lexeme.type_lexeme == DECIMAL || lexeme.type_lexeme == OCTAL || lexeme.type_lexeme == HEXADECIMAL || lexeme.type_lexeme == DECIMAL_ZERO || lexeme.type_lexeme == MOT_OU_INSTRUCTION){ /*On teste le type de l'operande*/
                    data.section = section;
                    data.nom = stk_lexeme;
                    data.etat = lexeme.type_lexeme;
                    data.decalage = decalage_data;
                    data.ligne = lexeme.numero_de_ligne;
                    data.valeur.AsNumber = stk_valeur;
                    
                    decalage_data += 4;
                    
                    *pf_d = enfiler_data(data, *pf_d);
                    etat = SUITE_DATA_WORD;
                } else {
                    WARNING_MSG("Erreur a la ligne n° %d -> .word doit un nombre comme operande", lexeme.numero_de_ligne);
                    etat = INIT_DATA;
                }
            }
            break;
            
        case SUITE_DATA_ASCIIZ:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = ASCIIZ; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_DATA;
            } else if (lexeme.type_lexeme == COMMENTAIRE){
                etat = INIT_DATA;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
            
        case SUITE_DATA_BYTE:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = BYTE; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_DATA;
            } else if (lexeme.type_lexeme == COMMENTAIRE){
                etat = INIT_DATA;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
        case SUITE_DATA_WORD:
            if (lexeme.type_lexeme == VIRGULE){ /* S'il y a une virgule alors c'est qu'on a un autre operande*/
                etat = WORD; /*On cree une autre cellule avec le decalage qu'il faut*/
            } else if (strcasecmp(".word", lexeme.token) == 0){ /*On fait les tests sur l'instruction suivant si pas de virgule*/
                etat = WORD;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".byte", lexeme.token) == 0){
                etat = BYTE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".space", lexeme.token) == 0){
                etat = SPACE;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".asciiz", lexeme.token) == 0){
                etat = ASCIIZ;
                stk_lexeme = lexeme.token;
            } else if (strcasecmp(".bss", lexeme.token) == 0) {
                etat = INIT_BSS;
                section = BSS;
            } else if (strcasecmp(".text", lexeme.token) == 0){
                etat = INIT_TEXT;
                section = INST;
            } else if (lexeme.type_lexeme == ETIQUETTE){
                etat = INIT_DATA;
            } else if (lexeme.type_lexeme == COMMENTAIRE){
                etat = INIT_DATA;
            } else {
                WARNING_MSG("Erreur a la ligne n° %d -> cette directive n'a pas sa place ici", lexeme.numero_de_ligne);
                etat = INIT_DATA;
            }
            break;
            
    }
}
    

