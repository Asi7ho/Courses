
#include <annexe.h>

char* type_string (int etat){
    switch(etat){
        case DECIMAL_ZERO:
            return "ZERO";
        case DECIMAL:
            return "DECIMAL";
        case OCTAL:
            return "OCTAL";
        case HEXADECIMAL:
            return "HEXADECIMAL";
        case VIRGULE:
            return "VIRGULE";
        case PARENTHESE_OUVERTE:
            return "PARENTHESE OUVERTE";
        case PARENTHESE_FERMEE:
            return "PARENTHESE FERMEE";
        case STRING:
            return "STRING";
        case MOT_OU_INSTRUCTION:
            return "MOT OU INSTRUCTION";
        case ETIQUETTE:
            return "ETIQUETTE";
        case REGISTRE:
            return "REGISTRE";
        case COMMENTAIRE:
            return "COMMENTAIRE";
        case DIRECTIVE:
            return "DIRECTIVE";
        case ERREUR:
            return "ERREUR";
    }
    return "PAS DE TYPE CONNU";
}

char* section_string (int section){
    switch(section){
        case INST:
            return ".text";
        case DATA:
            return ".data";
        case BSS:
            return ".bss";
    }
    return "PAS DE TYPE CONNU";
}












