#include "f_decode.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "f_code.h"
#include "f_image.h"

unsigned char ** ouvreImage( char nomImage[],int *nc,int *nl)
{
    FILE* image=NULL;
    int i=0;
    char c;
    unsigned char **p;
    *nl=0;
    *nc=0;
    image=fopen(nomImage,"r");
    puts("4\n");
    if(image==NULL)
    {
        printf("problème ouverture image\n");
    return(-1);
    }

    fscanf(image,"P5\n%d %d",nl,nc); //La taille de l'image est ecrite dans le fichier pgm
    puts("5\n");
    printf("nc=%d  nl=%d\n",*nc,*nl);


    printf("%X",fgetc(image));//un octet sert à stocker 255
    printf("%X",fgetc(image));
    printf("%X",fgetc(image));
    printf("%X",fgetc(image));
    printf("%X",fgetc(image));

    puts("6\n");
    p=(unsigned char**)alloue_image(*nl,*nc);
    if(p==NULL)
    {
        return -1;
    }
    puts("L'allocation de l'image fonctionne\n");
    c=fgetc(image);
    i=0;
    while (c!=EOF&&i<((*nc)*(*nl)))
    {
        (*p)[i]=c;
        c=fgetc(image);
        i++;
    }
    puts("8\n");

    fclose(image);
    return p ;

}

int malectureimage(unsigned char* fic, unsigned char im[], int* pnl, int* pnc) {
	FILE* f;
	char pixel[800*600];
	int i;int j = 0;
	f = fopen(fic, "r");
	if (f == NULL) {
		return -1;
	}
	if (strcmp(fgets(pixel, 255, f), "P5\n") != 0) {
		return -1;
	}
	do {
		fgets(pixel, 255, f);
	} while (pixel[0] == '#');
	sscanf(pixel, "%d %d", pnl, pnc);

	do {
		fgets(pixel, 255, f);
	} while (pixel[0] == '#');

	sscanf(pixel, "%d", &i);
	if (i > 255) {
		return -1;
	}
	printf("%d, %d", *pnl, *pnc);

	fread(im, (*pnl)*(*pnc), 1, f);
	for (j  = 0; j < (*pnl)*(*pnc) + 1; j++)
	{
		printf("%d ", im[j]);
	}
	printf("\n");

	fclose(f);
	return 0;
}







/*unsigned char extraitunoctet (unsigned char **img, int *p_k, char bitParPixel)
{

    unsigned char t[7];
    unsigned char stockage[7];
    int j = 0;
    int i ; //pixel a partir duquel on commence à dechiffrer
    char octet = 0;
    char pas = 0;
    pas = 8/bitParPixel;

    for ( i = *p_k; i < *p_k + pas + 1; i++)
    {
        affichebit((*img)[*p_k], t);
        for (j = 0; j < 7; j+=2)
        {
            stockage[j] = t[0];
            stockage[j+1] = t[1];
        }
    }

    for (j = 0; j < 7; j++) {
        octet += (char)(stockage[j]*pow(2, j));
    }
    *p_k=*p_k+pas;
    return octet;
}
*/
/*
unsigned char extraitunoctet (unsigned char **img, int *p_k, char bitParPixel)
{
    unsigned char t[8];
    unsigned char octet=0,bit=1; //1=00000001 en binnaire
    char nbrDePixelsAPrendre;
    int i,j;
    nbrDePixelsAPrendre = 8/bitParPixel;


    for (i=0;i<nbrDePixelsAPrendre;  i++)
    {
        bit=1;
        for (j=(i+1)*bitParPixel-1;j>i*bitParPixel;j--)//On met le bit de poid fort devant le bit de poid faible, avec deux bits par pixel, j parcourt le tableau de la facon suivante: 1,0,3,2,5,4,7,6 et bit prend les valeurs 1,2,1,2,1,2,1,2
        {
            t[j]=(*img)[(*p_k)+i] & bit;

            bit=bit<<1;
        }
    }
    puts("2\n");

    for (j=0;j<8;j++)
    {
        octet=octet + t[j];
    }



    *p_k=*p_k+nbrDePixelsAPrendre;


    printf("%c\n",octet);
    return octet;
}
*/
unsigned char extraitunoctet (unsigned char **img, int *p_k, char bitParPixel)
{
    unsigned char t[4];
    unsigned char octet=0,bit=0x03; //=00000011 en binnaire
    char nbrDePixelsAPrendre;
    int i;
    nbrDePixelsAPrendre = 4;


    for (i=0;i<nbrDePixelsAPrendre;  i++)
    {

        t[i]=((*img)[(*p_k)+i] & bit)<<(6-2*i);



        octet=octet+t[i];
    }


    *p_k=*p_k+4;
    return octet;

}

int imdecode (unsigned char** img, char bitParPixel, int nl, int nc)
{

FILE *nouveauFichierAvecLeMessage=NULL;
int tailleDuFichier=0, i, indiceDePixel=0;
//char* messageCache=NULL ;
char nomDuFichier[12], lettreEnCours;
unsigned char octetEnCours=0;


puts("9\n");
for(i=0;i<12;i++)//on extrait le nom du fichier qui fait 12 caracteres
{
nomDuFichier[i]=extraitunoctet(img,&indiceDePixel,bitParPixel) ;
}
puts("9\n");

printf("le nom du fichier extrait est %s\n",nomDuFichier);
//on extrait sa taille qui est un entier int donc 4 octets
for (i=0;i<4;i++)
{
    octetEnCours=extraitunoctet(img,&indiceDePixel,bitParPixel);
    tailleDuFichier+=pow(2,8*i)*octetEnCours;
    printf(" %d\n",octetEnCours);
}

printf("la taille du fichier est %d\n",tailleDuFichier);

if(tailleDuFichier<=0)
{
printf("probleme lecture taille fichier\n");
return(-1);
}

puts("11\n");

nouveauFichierAvecLeMessage=fopen(nomDuFichier,"w+"); // On ouvre un nouveau fichier qui contient le texte

if(nouveauFichierAvecLeMessage==NULL)
{
printf("probleme ouverture fichier\n");
return(-1);
}

puts("12\n");/*Pour ce qui suit, on peut lire et ecrire en un seul coup, il faudra modifier ca a la fin*/

/*
messageCache=malloc(tailleDuFichier*sizeof(char)); // On verifie qu'on peut stocker le message en memoire, Bien sur, on pourrait recopier lettre à lettre dans le fichier texte
if(messageCache==NULL)
{
printf("probleme allocation memoire\n");
fclose(nouveauFichierAvecLeMessage);
return(-1);
}



for(i=0;i<tailleDuFichier;i++)
{
messageCache[i]=extraitunoctet(img,&indiceDePixel,bitParPixel);
}

puts("13\n");
//recopie dans le fichier
for (i=0;i<tailleDuFichier;i++)
{
fprintf(nouveauFichierAvecLeMessage,"%c",messageCache[i]);
}
puts("14\n");
*/

//L'alternative pour ne pas avoir à tout stocker en memoire :

for(i=0;i<tailleDuFichier&&lettreEnCours!='\0';i++)
{
    lettreEnCours=extraitunoctet(img,&indiceDePixel,bitParPixel);
    fprintf(nouveauFichierAvecLeMessage,"%c",lettreEnCours);
}

puts("15\n");


printf("le fichier %s a ete extrait correctement\n",nomDuFichier);

fclose(nouveauFichierAvecLeMessage);

return 0;
}
