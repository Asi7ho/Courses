//
//  simplifie.c
//  
//
//  Created by Michel Desvignes on 09/03/2017.
//
//
#include "arbre.h"
#include "simplifie.h"
/*
	recherche un noeud de type VALEUR dont tout le chemin comporte des neoud de meme type ie + ou *
 Permet alors de simplifier les expressions en regroupant les constantes
 */
ARBRE perefeuillecte(ARBRE r, char t) { ARBRE c1, c2;
    if (r==NULL) return r;
    else if (r->type == OPERATEUR_BINAIRE && r->val[0]==t)
        if (r->fd->type == VALEUR|| r->fg->type == VALEUR) return r;
        else { ARBRE c1=perefeuillecte(r->fg,t), c2=perefeuillecte(r->fd,t);
            if (c1) return c1;
            if (c2) return c2;
            return NULL;
        }
    return NULL;
}

#define SWAP(x,y,type) { type inter; inter=(x); (x)=(y); (y)=inter; }

ARBRE simplifie1(ARBRE r);


int arrangecste(ARBRE r, char oper) {
    if (r!=NULL && r->type==OPERATEUR_BINAIRE &&  r->val[0]==oper ) {
        ARBRE c3=perefeuillecte(r->fg,oper);
        ARBRE c4=perefeuillecte(r->fd,oper);
        if (c3 && c4) {
            if (c3->fd->type==VALEUR)
                if (c4->fd->type==VALEUR) SWAP(c3->fd,c4->fg,ARBRE)
                    else SWAP(c3->fd,c4->fd,ARBRE)
                        else if (c4->fd->type==VALEUR) SWAP(c3->fg,c4->fg,ARBRE)
                            else SWAP(c3->fg,c4->fd,ARBRE)
                                return 1;
        }
    }
    return 0;
}


ARBRE simplifie1(ARBRE r) {
    if (r!=NULL) {
        switch(r->type) {
            case VALEUR: case VARIABLE : return r;
            case OPERATEUR_UNAIRE: r->fg=simplifie1(r->fg); return r;
            case OPERATEUR_BINAIRE : r->fd=simplifie1(r->fd); r->fg=simplifie1(r->fg);
                if (r->fd && r->fd->type==VALEUR && r->fg && r->fg->type==VALEUR) { char s[512]; int i;
                    double x=eval(r,0); sprintf(s,"%lf",x); for(i=strlen(s)-1; i>0 && s[i]=='0'; i--) s[i]=0; libere(r); return creernoeud(s);
                }
                if(r->val[0]=='+' ) { ARBRE c1=r->fd, c2=r->fg, c=NULL;
                    if((c1->type==VALEUR && atof(c1->val)==0 && (c=c1)!=NULL) || (c2->type==VALEUR && atof(c2->val)==0&& (c=c2)!=NULL)){
                        free(r->val); free(r); free(c->val); free(c); return simplifie1(c==c2? c1 : c2);
                    }
                    if(c1->type==VALEUR && (c=perefeuillecte(r->fg,'+'))) {
                        if (c->fd->type==VALEUR) SWAP(r->fd,c->fg,ARBRE) else SWAP(r->fd,c->fd,ARBRE)
                            return simplifie1(r);
                    }
                    if(c2->type==VALEUR && (c=perefeuillecte(r->fd,'+'))) {
                        if (c->fd->type==VALEUR) SWAP(r->fg,c->fg,ARBRE) else SWAP(r->fg,c->fd,ARBRE)
                            return simplifie1(r);
                    }
                    else { return (r); }
                }
                else if(r->val[0]=='-') { ARBRE c1=r->fd, c2=r->fg;
                    if(c1->type==VALEUR && atof(c1->val)==0) { free(r->val); free(r); free(c1->val); free(c1); return simplifie1(c2); }
                    else { return (r); }
                }
                else if (r->val[0]=='*') { ARBRE c1=r->fd, c2=r->fg, c3=NULL, c=NULL;
                    if(c1->type==VALEUR && atof(c1->val)==0) { libere(r); return creernoeud("0"); }
                    if(c2->type==VALEUR && atof(c2->val)==0) { libere(r); return creernoeud("0"); }
                    if(c1->type==VALEUR && atof(c1->val)==1) { free(r->val); free(r); free(c1->val); free(c1); return simplifie1(c2); }
                    if(c2->type==VALEUR && atof(c2->val)==1) { free(r->val); free(r); free(c2->val); free(c2); return simplifie1(c1); }
                    if(c1->type==VALEUR && (c=perefeuillecte(r->fg,'*'))) {
                        if (c->fd->type==VALEUR) SWAP(r->fd,c->fg,ARBRE) else SWAP(r->fd,c->fd,ARBRE)
                            return simplifie1(r);
                    }
                    if(c2->type==VALEUR && (c=perefeuillecte(r->fd,'*'))) {
                        if (c->fd->type==VALEUR) SWAP(r->fg,c->fg,ARBRE) else SWAP(r->fg,c->fd,ARBRE)
                            return simplifie1(r);
                    }
                    return (r);
                }
                else if (r->val[0]=='/') { ARBRE c1=r->fd, c2=r->fg;;
                    if(c1->type==VALEUR && atof(c1->val)==1) { free(r->val); free(r);; free(c1->val); free(c1); return simplifie1(c2); }
                    else { return (r); }
                }
                else if (r->val[0]=='^') { ARBRE c1=r->fd, c2=r->fg;;
                    if(c1->type==VALEUR && atof(c1->val)==1) { free(r->val); free(r);; free(c1->val); free(c1); return simplifie1(c2); }
                    if(c1->type==VALEUR && atof(c1->val)==0) { libere(r); return(creernoeud("1")); }
                    else { return (r); }
                }
                else { return r; }
        }
    }
    return NULL;
}


ARBRE  simplifie(ARBRE r) {
    do { 
        r=simplifie1(r);
    } while (arrangecste(r,'*')==1 || arrangecste(r,'+')==1);
    return (r);
}

