//
//  f_code.h
//  Projet programmation
//
//  Created by Yann Debain on 01/01/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#ifndef f_code_h
#define f_code_h

int* affichebit(unsigned char n, unsigned char t[7]);


/*
 * Cache l’octet « b » dans l’image img, en demarrant au pixel « k ».
 * Cette fonction cache l’octet « b » dans les pixels k,k+1,k+2,k+3), etc. selon le nombre de bits de poids faible bitParPixel caches dans un pixel.
 * PARAMETRES :
 * img : l’image dans laquelle cacher l’octet « b ».
 * b : l’octet a cacher, a raison de bitParPixel bits par pixel.
 * k : indice du pixel ou on commence a cacher l’octet.
 * RETOUR : l’indice du prochain pixel pour cacher le prochain octet secret.
    Avec 2 bits par pixel, c’est k+4.
 */
 int cacheunoctet( unsigned char** img, unsigned char b, int k, char bitParPixel) ;

/*
 * Cache le contenu du fichier de nom fileName dans l’image img, a raison de bitParPixel bits de poids faible par pixels.
 * Cette fonction commence par cacher le nom et la taille du fichier a cacher, puis lit le contenu du fichier octet par octet et cache ce contenu PARAMETRES :
 * fic: nom du fichier contenant le message a cacher
 * img : l’image dans laquelle cache le fichier.
 * b : l’octet a cacher, a raison de bitParPixel bits par pixel. bitParPixel : nombre de bits de poids faible utilise
 * nl : nombre de lignes de l’image
 * nc : nombre de colonnes de l’image
 * RETOUR : 0 en l’absence d’erreur, un nombre non nul en cas d’erreur.
 */
 int imencode(char *fic, unsigned char**img, char bitParPixel, int nl, int nc) ;

#endif /* f_code_h */
