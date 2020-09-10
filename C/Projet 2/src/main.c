
/**
 * @file main.c
 * @author François Portet <francois.portet@imag.fr> from François Cayre
 * @brief Main entry point for MIPS assembler.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <global.h>
#include <notify.h>
#include <structure.h>
#include <file.h>
#include <file_de_data.h>
#include <file_de_bss.h>
#include <file_instruction.h>
#include <pseudo_instruction.h>
#include <table_symbole.h>
#include <relocation.h>
#include <correspondance_registre.h>
#include <dico.h>
#include <lex.h>
#include <gram.h>
#include <file_binaire.h>

/**
 * @param exec Name of executable.
 * @return Nothing.
 * @brief Print usage.
 *
 */
void print_usage( char *exec ) {
    fprintf(stderr, "Usage: %s file.s\n",
            exec);
}

/**
 * @param argc Number of arguments on the command line.
 * @param argv Value of arguments on the command line.
 * @return Whether this was a success or not.
 * @brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {

    unsigned int nlines = 0;
    int nbr_instructions = 0;
    int nbr_etiquette = 0;
    int nbr_registre = 0;
    char *file = NULL;
    char binary_file[STRLEN];
    char obj_file[STRLEN];
    
    FILE_LEXEME f = creer_file();
    FILE_DE_DATA fd = creer_file_data();
    FILE_DE_BSS fb = creer_file_bss();
    FILE_INSTRUCTION fi = creer_file_inst();
    FILE_RELOCATION frtxt = creer_file_reloc();
    FILE_RELOCATION frdata = creer_file_reloc();
    
    INSTRUCTION_DEFINITION* dico = NULL;
    CORR_REG* table_registre = NULL;
    

    /* macro INFO_MSG : uniquement si compilé avec -DVERBOSE. Cf. Makefile*/
    INFO_MSG("Un message INFO_MSG : Debut du programme %s", argv[0]);

    /* La macro suivante provoquerait l'affichage du message
       puis la sortie du programme avec un code erreur non nul (EXIT_FAILURE) */
    /* ERROR_MSG("Erreur. Arret du programme"); */


    if ( argc <2 ) {
        print_usage(argv[0]);
        exit( EXIT_FAILURE );
    }


    file = argv[argc-1];


    if ( NULL == file ) {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        exit( EXIT_FAILURE );
    }



    /* ---------------- do the lexical analysis -------------------*/
    lex_load_file( file, &nlines, &f); /* On charge le fichier */
    DEBUG_MSG("source code got %d lines",nlines);
    /*visualiser_file(f);*/
    
    /* ---------------- do the grammatical analysis -------------------*/
    table_registre = charge_dico_correspondance("correspondance_registre.txt", &nbr_registre);
    
    dico = charge_dico("Dico.txt", &nbr_instructions);
    
    nbr_etiquette = detecte_nombre_etiquette(f);
    /*printf("\nNOMBRE ETIQUETTE  = %d\n", nbr_etiquette);*/
    SYMBOLE* table_symbole = charge_table_symbole(f, dico, nbr_etiquette, nbr_instructions);
    
    automate_generale(f, dico, &fd, &fb, &fi, table_symbole, &frtxt, &frdata, nbr_etiquette, nbr_instructions, table_registre, nbr_registre);
    
    /*visualiser_file_inst(fi);*/
    
    pseudo_instruction(fi);
    code_binaire_instru(fi);
    visualiser_file_inst(fi);
    visualiser_file_data(fd);
    visualiser_file_bss(fb);
    
    visualiser_table_symbole(table_symbole, nbr_etiquette);
    visualiser_file_reloc(frtxt);
    visualiser_file_reloc(frdata);
    
    
    /* ---------------- do the binary genration file -------------------*/
    memset(binary_file, '\0', sizeof(binary_file));
    strncpy(binary_file, file, strlen(file) - 1);
    strcat(binary_file, "l");
    memset(obj_file, '\0', sizeof(obj_file));
    strncpy(obj_file, file, strlen(file) - 1);
    strcat(obj_file, "obj");
   
    printf("FILE = %s\nBINARY FILE = %s\nOBJ FILE = %s\n", file, binary_file, obj_file);
    
    create_file_l(file, binary_file, fi, fd, fb, table_symbole, nbr_etiquette, frtxt, frdata);
    create_obj_file(obj_file, fi, fd, fb);
    

    /* ---------------- Free memory and terminate -------------------*/
    free(dico);
    free(table_symbole);
    free(table_registre);

    exit( EXIT_SUCCESS );
}

