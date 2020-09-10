#include <stdio.h>

void swap( double* a, double* b) {
	double c;
	c = *a; 
	*a = *b;
	*b = c;
}

main () {
	double a = 10, b = 2;
	swap(&a, &b);
	printf(" a = %lf et b = %lf", a, b);
}

