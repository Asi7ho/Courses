#include <create_lexem.h>


LEXEM create_lexem_from_token(char* line, int ligne){
    
    int etat = INIT;
    char c;
    int i = 0;
    LEXEM l;
    
    for(i = 0; i < strlen(line)+1; i++){ /* On examine chaque caractère de la chaine */
        c = line[i];
        
        /* printf("%c\n", c); */
       
        switch(etat){ /* On détermine l'etat du lexeme par un automate */
            case INIT:
                if (isdigit(c)){ /* On regarde si c est un chiffre ou non */
                    if (c == '0'){
                        etat = DECIMAL_ZERO;
                    } else if (isblank(c)) {
                        etat = INIT;
                    } else {
                        etat = DECIMAL;
                    }
                } else {
                    if (c == '#'){ /* On teste pour savoir si c'est un caractère special */
                        etat = COMMENTAIRE;
                    } else if (c == '$') {
                        etat = DEBUT_REGISTRE;
                    } else if (c == '.') {
                        etat = DEBUT_DIRECTIVE;
                    } else if (c == '-') {
                        etat = SIGNE_MOINS;
                    } else if (c == ','){
                        etat = VIRGULE;
                    } else if (c == '('){
                        etat = PARENTHESE_OUVERTE;
                    } else if (c == ')'){
                        etat = PARENTHESE_FERMEE;
                    } else if (isblank(c)){
                        etat = INIT;
                    } else if (c == '"'){
                        etat = STRING;
                    } else { /* Sinon c'est un mot */
                        etat = MOT_OU_INSTRUCTION;
                    }
                }
                break;
                
            case DECIMAL_ZERO:
                if (c == 'x' || c == 'X'){
                    etat = DEBUT_HEXADECIMAL;
                } else if (isdigit(c) && c < '8'){
                    etat = OCTAL;
                } else if (isspace(c) || c == '\0') { /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est un ZERO\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = DECIMAL_ZERO;
                    l.numero_de_ligne = ligne;
                } else {
                    etat = ERREUR;
                }
                break;
                
            case DEBUT_HEXADECIMAL:
                if (isxdigit(c)){
                    etat = HEXADECIMAL;
                } else {
                    etat = ERREUR;
                }
                break;
                
            case HEXADECIMAL:
                if (isxdigit(c)){
                    etat = HEXADECIMAL;
                } else if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est un HEXADECIMAL\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = HEXADECIMAL;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else {
                    etat = ERREUR;
                }
                break;
                
            case DECIMAL:
                if(isdigit(c)){
                    etat = DECIMAL;
                } else if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est un DECIMAL\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = DECIMAL;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else {
                    etat = ERREUR;
                }
                break;
                
            case OCTAL:
                if(isdigit(c) && c < '8'){
                    etat = OCTAL;
                } else if (isspace(c) || c == '\0') { /*Si on change de mot ou si on est en fin de phrase */
                   /* printf(" || c'est un OCTAL\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = OCTAL;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else {
                    etat = ERREUR;
                }
                break;
                
            case SIGNE_MOINS: /* Pour les nombres négatifs on verifie si c'est un nombre et on refait la meme chose que pour les nombres non signes */
                if(isdigit(c)){
                    if (c == '0'){
                        etat = DECIMAL_ZERO;
                    } else {
                        etat = DECIMAL;
                    }
                
                } else {
                    etat = ERREUR;
                }
                break;
            
            case COMMENTAIRE:
                etat = COMMENTAIRE;
                if (c == '\0'){
                    l.token = strdup(line);
                    l.type_lexeme = COMMENTAIRE;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                }
                break;
                
            case DEBUT_REGISTRE:
                if (isspace(c) || c == '\0'){
                    l.token = strdup(line);
                    l.type_lexeme = ERREUR;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else {
                    etat = REGISTRE;
                }
                break;
                
            case REGISTRE:
                etat = REGISTRE;
                if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est un REGISTRE\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = REGISTRE;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                }
                break;
                
            case DEBUT_DIRECTIVE:
                if (isspace(c) || c == '\0'){
                    l.token = strdup(line);
                    l.type_lexeme = ERREUR;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else {
                    etat = DIRECTIVE;
                }
                break;

            case DIRECTIVE:
                if (!isdigit(c)){ /* On verifie que la directive a du sens (que ce n'est pas un nombre */
                    etat = DIRECTIVE;
                    if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                        /*printf(" || c'est une DIRECTIVE\n");*/
                        l.token = strdup(line);
                        l.type_lexeme = DIRECTIVE;
                        l.numero_de_ligne = ligne;
                        etat = INIT;
                    }
                } else {
                    etat = ERREUR;
                }
                break;
                
            case STRING:
                etat = STRING;
                if (c == '"'){
                    l.token = strdup(line);
                    l.type_lexeme = STRING;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                }
                break;
                
            case MOT_OU_INSTRUCTION:
                etat = MOT_OU_INSTRUCTION;
                if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est un MOT ou une INSTRUCTION\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = MOT_OU_INSTRUCTION;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                } else if (c == ':'){
                    etat = ETIQUETTE; /*Si il y a ':' a la fin du mot alors c'est une etiquette */
                }
                break;
                
            case ETIQUETTE:
                etat = ETIQUETTE;
                /*printf(" || c'est une ETIQUETTE\n");*/
                l.token = strdup(line);
                l.type_lexeme = ETIQUETTE;
                l.numero_de_ligne = ligne;
                etat = INIT;
                break;
                
                
            case VIRGULE:
                etat = VIRGULE;
                if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est une VIRGULE\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = VIRGULE;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                }
                break;
                
            case PARENTHESE_OUVERTE:
                etat = PARENTHESE_OUVERTE;
                if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                    /*printf(" || c'est une PARENTHESE\n");*/
                    l.token = strdup(line);
                    l.type_lexeme = PARENTHESE_OUVERTE;
                    l.numero_de_ligne = ligne;
                    etat = INIT;
                }
                break;
            
            case PARENTHESE_FERMEE:
            etat = PARENTHESE_FERMEE;
            if (isspace(c) || c == '\0'){ /*Si on change de mot ou si on est en fin de phrase */
                /*printf(" || c'est une PARENTHESE\n");*/
                l.token = strdup(line);
                l.type_lexeme = PARENTHESE_FERMEE;
                l.numero_de_ligne = ligne;
                etat = INIT;
            }
            break;
                
                
            case ERREUR:
                /*printf(" || Il y a une ERREUR\n");*/
                WARNING_MSG("LEXEME %s NON VALIDE A LA LIGNE n°%d", line, ligne);
                l.token = strdup(line);
                l.type_lexeme = ERREUR;
                l.numero_de_ligne = ligne;
                etat = INIT;
                break;
        }
    }
    return l;
}
