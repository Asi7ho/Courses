#include <canonique.h>


/**
 * @param in Input line of source code (possibly very badly written).
 * @param out Line of source code in a suitable form for further analysis.
 * @return nothing
 * @brief This function will prepare a line of source code for further analysis.
 */
/* note that MIPS assembly supports distinctions between lower and upper case*/


void lex_standardise( char* in, char* out ) {
    unsigned int i, j;
    
    while (isblank(in[0])){           /* on enleve les espaces ou tabulations en debut de phrase */
        for(i = 0; i < strlen(in); i++){
            in[i] = in[i+1];
        }
    }
    /*printf("%s\n",in);*/
    
    
    
    
    for ( i= 0, j= 0; i< strlen(in); i++ ) {
        
        
        if (in[i]==':' && isblank(in[i-1])) { /* transforme a : en a: */
            out[j-1]=in[i];
        }
        else if (isblank(in[i]) && in[i-1]=='-'){ /* transforme - 5 en -5 */
            out[j]=in[i+1];
        }
        else if ((in[i]=='(' || in[i] == ')') && (!isblank(in[i-1]) && isblank(in[i+1]))){ /* transforme (a ) en ( a ) */
            out[j++] = ' ';
            out[j++] = in[i];
        }
        else if ((in[i]=='(' || in[i] == ')') && (isblank(in[i-1]) && !isblank(in[i+1]))){ /* transforme ( a) en ( a ) */
            out[j] = ' ';
            out[j++] = in[i];
            out[j++] = ' ';
        }
        else if ((in[i]=='(' || in[i] == ')') && (!isblank(in[i-1]) && !isblank(in[i+1]))){ /* transforme (a) en ( a ) */
            out[j++] = ' ';
            out[j++] = in[i];
            out[j++] = ' ';
        }
        else if (in[i]==',' && (!isblank(in[i-1]) && isblank(in[i+1]))){ /* transforme a, b en a , b */
            out[j++] = ' ';
            out[j++] = in[i];
        }
        else if (in[i]==',' && (isblank(in[i-1]) && !isblank(in[i+1]))){ /* transforme a ,b en a , b */
            out[j] = ' ';
            out[j++] = in[i];
            out[j++] = ' ';
        }
        else if (in[i] ==',' && (!isblank(in[i-1]) && !isblank(in[i+1]))){ /* transforme a,b en a , b */
            out[j++] = ' ';
            out[j++] = in[i];
            out[j++] = ' ';
        }
        else if (isblank(in[i]) && isblank(in[i+1])){ /*transforme tous les blancs consécutifs en un espace simple */
            out[j] = in[i+1];
        } else if (in[i] == '"'){ /*On ne modifie pas les strings de l'utilisateur */
            out[j++]=in[i];
            i++;
            while (in[i] != '"'){
                out[j++]=in[i];
                i++;
            }
            out[j++]=in[i];
        } else {
            out[j++]=in[i]; /* on recopie si tout va bien */
        }
    }
    out[j]='\0'; /* fin de phrase */
    
    /*printf("%s\n%s\n", in, out);*/
}
