#include <stdio.h>
#include <math.h>

main() {
	int i, N; double Arctan1 = 0, x = 1/5.0, Arctan2 = 0, y = 1.0/239;
	printf("Entrer un entier N: "); scanf("%d", &N);
	for (i = 0; i < N+1; i++) {
		Arctan1 = Arctan1 + pow(-1,i) * pow(x, 2*i+1)/(2*i+1);
		Arctan2 = Arctan2 + pow(-1,i) * pow(y, 2*i+1)/(2*i+1);
	}
	printf("Pi = %lf", 16*Arctan1 - 4*Arctan2);
}
