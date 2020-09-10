/****************************************************\
 conversion ¡C vers ¡F avec fonction
\****************************************************/
#include <stdio.h> // inclusion de la librairie stdio

/* prototype des fonctions d<8e>clar<8e>e plus loin */
float convertir(int celc);



/* Fonction de conversion celcius->farenheit et retour de la valeur*/
float convertir(int celc)
{
   float farenheit = celc * (9.0 / 5) + 32 ;
   return farenheit;
}



int main ()
{
   	// Definition des variables
        int celcius;
   	float fahrenheit ;

        // Initialisation des variables
        printf("Entrer une temperature en Celsius (un nombre entier) : \n");
        scanf("%d", &celcius);

        // Calculs et op<8e>rations
        fahrenheit = convertir(celcius) ;

        // Affichage
        printf("La temperature vaut : %f Fahrenheit.\n", fahrenheit);
  	 printf("Au revoir.\n");
        return 0;
}

