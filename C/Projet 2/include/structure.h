#ifndef structure_h
#define structure_h

/*Collection des différents types de lexemes*/
typedef enum {
    INIT, MOT_OU_INSTRUCTION, ETIQUETTE, REGISTRE, COMMENTAIRE, DIRECTIVE, SIGNE_MOINS, DECIMAL_ZERO, DEBUT_HEXADECIMAL, DECIMAL, OCTAL, HEXADECIMAL, ERREUR, VIRGULE, PARENTHESE_OUVERTE, PARENTHESE_FERMEE, STRING, DEBUT_REGISTRE, DEBUT_DIRECTIVE
} ETAT;

typedef enum {
    INIT_GENERALE, INIT_TEXT, INIT_DATA, INIT_BSS, DEBUT_SET, BYTE, WORD, ASCIIZ, SPACE, TEXT, SUITE_DATA_BYTE, SUITE_DATA_WORD, SUITE_DATA_ASCIIZ, SUITE_DATA_SPACE, SUITE_BSS_SPACE
} ETAT_GRAMMATICALE;

typedef enum {
    REG, IMM, SA, REL, ABS, BAS_OFF
} TYPE_OPERANDE;

typedef enum {
    INST, DATA, BSS
} SECTION;

/*Structure des different type d'instruction*/

typedef struct R {
    unsigned int func:6,
    sa:5,
    rd:5,
    rt:5,
    rs:5,
    opcode:6;
}R_INSTRUCTION;

typedef struct I {
    unsigned int immediate:16,
    rt:5,
    rs:5,
    opcode:6;
}I_INSTRUCTION;

typedef struct J {
    unsigned int target:26,
    opcode:6;
}J_INSTRUCTION;

/*Union des differents type d'instructions*/

typedef union {
    R_INSTRUCTION asR;
    I_INSTRUCTION asI;
    J_INSTRUCTION asJ;
} TYPE_INSTRUCTION;

/*Union des differents types de valeur prises par les donnees*/
typedef union {
    long AsNumber;
    char* AsString;
} VALEUR_UNION;

/*Structure d'un lexeme*/
typedef struct {
    char* token;
    ETAT type_lexeme;
    int numero_de_ligne;
    int valeur;
} LEXEM;

/*Structure etablir la correspondance des registre*/
typedef struct {
    char* representation_mnemotique;
    char* representation_chiffre;
    int correspondance;
} CORR_REG;

/*Structure pour identifier pour chaque mot du dictionnaire d'instructions*/
typedef struct {
    char* instruction;
    int nombre_operande;
    char* type_operande1;
    char* type_operande2;
    char* type_operande3;
    char type;
    TYPE_INSTRUCTION type_instruction;
} INSTRUCTION_DEFINITION;

/*Structure pour creer la collection d'instruction*/
typedef struct{
    SECTION section;
    char* nom;
    int nombre_operande;
    int ligne;
    int decalage;
    LEXEM operande[3];
    int codeBinaire;
    char* line;
} INSTRUCTION;

/*Structure pour creer la table des symboles*/
typedef struct{
    SECTION section;
    char* nom;
    int ligne;
    int decalage;
} SYMBOLE;

/*Structure pour creer la collection de data et bss*/
typedef struct{
    SECTION section;
    char* nom;
    ETAT etat;
    int decalage;
    int ligne;
    VALEUR_UNION valeur;
} DATA_BSS;

/*Structure pour creer la table de relocation */
typedef struct {
    char* nom;
    SECTION section_de_depart;
    SECTION section_arrive;
    int decalage_depart;
    int decalage_arrivee;
    char* relocation;
} RELOCATION;

/*Structure d'une liste chainee pour stocker les lexemes d'une phrase*/
typedef struct cell {
    LEXEM val;
    struct cell* suiv;
}* FILE_LEXEME;

/*Structure de file de collection des instructions*/
typedef struct cellule1 {
    INSTRUCTION instruction;
    struct cellule1* suiv;
}* FILE_INSTRUCTION;

/*Structure de file de collection de data*/
typedef struct cellule2 {
    DATA_BSS data;
    struct cellule2* suiv;
}* FILE_DE_DATA;

/*Structure de file de collection de bss*/
typedef struct cellule3 {
    DATA_BSS bss;
    struct cellule3* suiv;
}* FILE_DE_BSS;

/* Structure de file de la table de relocation */
typedef struct cellule4 {
    RELOCATION reloc;
    struct cellule4* suiv;
}* FILE_RELOCATION;


#endif /* structure_h */
