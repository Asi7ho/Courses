//
//  f_image.h
//  Projet programmation
//
//  Created by Yann Debain on 01/01/2017.
//  Copyright © 2017 Yann Debain. All rights reserved.
//

#ifndef f_image_h
#define f_image_h


/*
 *Libere la memoire prealablement allouee pour une image de nbl*nbc pixels.
 *PARAMETRES :
 *	p_image : l'image à liberer
 */
void libere_image(unsigned char ** p_image);

/*
 *Alloue une image de nbl*nbc pixels
 *La zone de donnee de l'image, dans le champs img, est contigue en mémoire.
 *La zone de donnee est initialise à 0
 *PARAMETRES :
 *	nbl : nombre de lignes de l'image a allouer
 *	nbc : nombre de colonne de l'image a allouer
 *RETOUR : pointeur vers l'image allouee, ou NULL en cas d'erreur
 */
unsigned char**  alloue_image(int nbl, int nbc);

#endif /* f_image_h */
