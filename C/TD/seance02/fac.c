#include <stdio.h>

main() {
	int i, N; float S1 = 0, S2 = 0;
	printf("Entrer un entier N: "); scanf("%d", &N);
	for (i = 1; i < N+1; i++) {
		S1 = S1 + 1.0/(i*i); // Somme dans le sens croissant
	}
	printf("Dans le sens croissant S = %f \n", S1);
	
	for (i = N; i > 0; i--) {
		S2 = S2 + 1.0/(i*i); // Somme dans le sens décroissant
	}
	printf("Dans le sens décroissant S = %f", S2);
}
