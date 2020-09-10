#include <stdio.h>
#include <stdlib.h>
/*#include "SDL_Phelma"*/
#include "f_code.h"

int* affichebit(unsigned char n, unsigned char t[7])
{

    int i = 0;
    unsigned char mask = 0x80 ; // 1000 0000 en binaire char i;
    for ( i = 7 ; i >=0 ; i--)
    {
        t[i]  = (n & mask);
        t[i] = t[i] >> i;
        /*printf("%d", t[i]) ;*/
        mask >>= 1 ;
    }
    return t;
}



int cacheunoctet( unsigned char** img, unsigned char b, int k, char bitParPixel)
{

	unsigned char t1[7];
	unsigned char t2[7];
	int i = 0;
	unsigned char pas = 0;

	pas = 8/bitParPixel;
	affichebit((*img)[k], t1);
	affichebit(b, t2);

	for (i = k; i < k + pas; i++)
	{
		affichebit((*img)[k], t1);
		for (i = 0; i < bitParPixel - 1; i++)
		{
			t1[i] = t2[i];
		}
	}

	return k + pas ;
}



int imencode(char *fic, unsigned char**img, char bitParPixel, int nl, int nc)
{

	FILE *fp;
	FILE *fb;
	FILE *nouvelleImage=NULL;

  	int len = 0,indice_de_pixel=1;
	unsigned char *t = NULL;


	fb = fopen("copie_fic.pgm", "w+");
   	fp = fopen(fic, "r" );
    nouvelleImage=fopen("nouvelleImage.pgm", "w+");


   	if (fb == NULL) {
   	    printf("impossible d'ouvrir le fichier copie_fic.pgm, le fichier n'a pas ete cree\n ");
		return -1;
	}
	if (fp == NULL) {
	    printf("impossible d'ouvrir le fichier %s, rien n'a ete code\n ",fic);
		return -1;
	}

	fseek(fp, 2, 0);

   	len = ftell(fp);


	if (len > nl*nc*(8/bitParPixel) )
	{
	    printf("L'image est trop petite, impossible de continuer \n");
        fclose(fp);
        fclose(fb);
	    return -1;
	}
	else
	{
		t = calloc(1, sizeof(*t));

        fseek(fp, 0, 0);
        while(!feof(fp))
        {

            fread(t,sizeof(*t),1,fp);
            printf("%c",*t);
            //fwrite(t, sizeof(*t), 1, fb);
            indice_de_pixel=cacheunoctet(img, *t ,indice_de_pixel,bitParPixel);
        }


    }

    fprintf(nouvelleImage,"message.txt\n%d %d\n%d",nc,nl,255);
    for(indice_de_pixel=0;indice_de_pixel<nl*nc;indice_de_pixel++)
    {
        fprintf("%c",(*img)[indice_de_pixel]);
    }
    fclose(nouvelleImage);
    fclose(fp);
    fclose(fb);
    return 0;

}
