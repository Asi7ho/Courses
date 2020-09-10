#include <stdio.h>
#include <math.h>

main() {
	double x, p, xa, pa, eps = 1e-15;
	printf("Entrer un nombre positif: "); scanf("%lf", &x);
	do { 
		xa = x;
		x = sqrt(sqrt(x));
		pa = p;
		p = (x - 1)/(xa - 1);
	} while (fabs(x - xa) > eps * (fabs(x)+fabs(xa)) && fabs(p - pa) > eps * (fabs(p)+fabs(pa)));

	printf("la limite de x est %lf et la limite de p est %lf", x, p);
}
