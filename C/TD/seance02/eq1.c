#include <stdio.h>

main(){ 
	float a1, a2, b1, b2, c1, c2, det;
	printf("Entrer un systeme a1*x + b1*y = c1 et a2*x + b2*y = c2 \n");
	printf("a1 = "); 
	scanf("%f", &a1);
	printf("b1 = "); 
	scanf("%f", &b1);
	printf("c1 = "); 
	scanf("%f", &c1);
	printf("a2 = "); 
	scanf("%f", &a2);
	printf("b2 = "); 
	scanf("%f", &b2);
	printf("c2 = "); 
	scanf("%f", &c2);
	
	det = a1*b2 - a2*b1;

	if (det == 0) {
		printf("le systeme admet une infinité de solution ou aucune solution");
	} else {
		printf("le systeme admet comme solution x = %f et y = %f ", (c1*b2 - c2*b1)/det, (a1*c2 - a2*c1)/det);
	}	
}

