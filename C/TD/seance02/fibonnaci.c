#include <stdio.h>

main() { 
	int n,i; float a,b,c;
	a = 0; // a = u0
	b = 1; // b = u1
	printf("Entrer un entier n: "); 
	scanf("%d", &n);
	printf("u0 = 0\nu1 = 1\n");
	for (i = 2; i < n + 1; i++) {
		c = a + b;
		a = b;
		b = c; // b = Ui
		printf("u%d = %f\n", i, c);
	}
}
