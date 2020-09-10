
/**
 * @file lex.c
 * @author François Portet <francois.portet@imag.fr>
 * @brief Lexical analysis routines for MIPS assembly syntax.
 *
 * These routines perform the analysis of the lexeme of an assembly source code file.
 */

#define _POSIX_C_SOURCE 200112L
#include <lex.h>

/**
 * @param line String of the line of source code to be analysed.
 * @param nline the line number in the source code.
 * @return should return the collection of lexemes that represent the input line of source code.
 * @brief This function performs lexical analysis of one standardized line.
 *
 */


void lex_read_line( char *line, int nline, FILE_LEXEME* pf) {
    char *seps = " \t";
    char *token = NULL;
    char save[STRLEN];
    char* commentaire = NULL;
    char* string = NULL;
    
    LEXEM l;
    
    /* copy the input line so that we can do anything with it without impacting outside world*/
    memcpy(save, line, STRLEN) ;
   
    commentaire = strchr(line, '#');
    string = strchr(line, '"');
    
    if (commentaire != NULL) {
        memset(save, '\0', sizeof(save));
        strncpy(save, line, strlen(line) - strlen(commentaire));
        for( token = strtok(save, seps ); NULL != token; token = strtok( NULL, seps )) {
            l = create_lexem_from_token(token, nline);
            *pf = enfiler(l, *pf); /* On stocke le mot dans la file de ligne*/
        }
    } else if (string != NULL){
        l = create_lexem_from_token(".asciiz", nline);
        *pf = enfiler(l, *pf); /* On stocke le mot dans la file de ligne*/
        l = create_lexem_from_token(string, nline);
        *pf = enfiler(l, *pf); /* On stocke le mot dans la file de ligne*/
    }else {
        for( token = strtok(line, seps ); NULL != token; token = strtok( NULL, seps )) {
            l = create_lexem_from_token(token, nline);
            *pf = enfiler(l, *pf); /* On stocke le mot dans la file de ligne*/
        }
    }
}

/**
 * @param file Assembly source code file name.
 * @param nlines Pointer to the number of lines in the file.
 * @return should return the collection of lexemes
 * @brief This function loads an assembly code from a file into memory.
 *
 */
void lex_load_file( char *file, unsigned int *nlines, FILE_LEXEME* pf) {

    FILE        *fp   = NULL;
    char         line[STRLEN]; /* original source line */
    char         res[2*STRLEN]; /* standardised source line, can be longeur due to some possible added spaces*/
    
    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) ) {
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;

            if ( 0 != strlen(line) ) {
                lex_standardise( line, res );
                lex_read_line(res,*nlines, pf);
            }
        }
    }

    fclose(fp);
    return;
}



/**
 * @param in Input line of source code (possibly very badly written).
 * @param out Line of source code in a suitable form for further analysis.
 * @return nothing
 * @brief This function will prepare a line of source code for further analysis.
 */

/* note that MIPS assembly supports distinctions between lower and upper case*/

void lex_standardise( char* in, char* out );
