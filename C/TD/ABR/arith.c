#include <stdio.h>
#include <string.h>
#include "arbre.h"

void affiche(ARBRE r) {
    if (r != NULL){
        printf("(");
        affiche(r->fg);
        printf("%c", r->val);
        affiche(r->fd);
        printf(")");
    }
}



double eval(ARBRE r, double x){
    
    if (isdigit(r->val) != 0){
        return atof(r->val);
    }

    if (strlen(r->val) == 1){
        return x;
    }

    if (strcasecmp(r->val, '+') == 0){
        return eval(atof(r->fg), x) + eval(atof(r->fd), x);
    }

     if (strcasecmp(r->val, '-') == 0){
        return eval(atof(r->fg), x) - eval(atof(r->fd), x);
    }

    if (strcasecmp(r->val, '*') == 0){
        return eval(atof(r->fg), x) * eval(atof(r->fd), x);
    }

    if (strcasecmp(r->val, '/') == 0){
        return eval(atof(r->fg), x) / eval(atof(r->fd), x);
    }

    if (strcasecmp(r->val, '^') == 0){
        return pow(eval(atof(r->fg), x), eval(atof(r->fd), x));
    }
}


ARBRE copie(ARBRE r){
    
    ARBRE a;
    fg = r->fg;
    fd = r->fd;
    
    a = calloc(1, sizeof(*a));

    if (a == NULL){
        printf("Erreur");
    }

    a->val = r->val;
    a->type = r->type;
    a->fg = fg;
    a->fd = fd;
}


ARBRE deriv(ARBRE r){
    
}

