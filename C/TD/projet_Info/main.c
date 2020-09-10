#include <stdio.h>
#include <stdlib.h>
#include "f_image.h"
#include "f_decode.h"
#include "f_code.h"

int main()
{



    int nc,nl,i=0,choix;
    char nomDuFichierImageAvecMessageCache[200],nomDuFichierTexteAvecMessageACacher[200],c;
    unsigned char **image=NULL;

    puts("Que voulez vous faire ? \n1) Extraire un message d'une image PGM5\n2)Cacher un fichier dans une image\n");
    scanf("%d",&choix);

    if(choix==2){
    puts("Entrer le nom du fichier image suivi du nom du fichier texte\n");

    scanf("%s %s",nomDuFichierImageAvecMessageCache,nomDuFichierTexteAvecMessageACacher); //Lena.pgm fichier_a_cacher.txt
    puts("1\n");
    image=ouvreImage( nomDuFichierImageAvecMessageCache,&nc,&nl);
         if(image!=NULL)
    //if(!malectureimage(image , nomDuFichierImageAvecMessageCache,&nc,&nl))
    {
        printf("nl=%d  nc=%d\n",nl,nc);
        imencode(nomDuFichierTexteAvecMessageACacher, image,2,nl,nc);
    }
    }

    if (choix==1)
    {
         puts("Entrer le nom du fichier image\n");
         scanf("%s",nomDuFichierImageAvecMessageCache); //LenaAvecBonjour.pgm
         image=ouvreImage( nomDuFichierImageAvecMessageCache,&nc,&nl);
         if(image!=NULL)
         //if(!malectureimage(image , nomDuFichierImageAvecMessageCache,&nc,&nl))
         {
             imdecode (image,2,nl,nc);
         }
    }
    if (choix==3)
    {
         puts("Entrer le nom du fichier image\n");
         scanf("%s",nomDuFichierImageAvecMessageCache); //LenaAvecBonjour.pgm
         image=ouvreImage( nomDuFichierImageAvecMessageCache,&nc,&nl);
         if(image!=NULL)
         //if(!malectureimage(image , nomDuFichierImageAvecMessageCache,&nc,&nl))
         {
             while(c!=-1)
             {
             /*c=extraitunoctet (&image,&i,2);*/
             c=(*image)[i];
             printf("%c",c);
             i++;
             }
         }
    }


    /*puts("2\n");
        imdecode(&image,2,nl,nc);
        puts("3\n");*/
       /* while (i<nc*nl)
        {
            printf("%c",extraitunoctet(&image,&i,2));

        }*/
    printf("\nGoodbye world\n");
    return 0;
}
