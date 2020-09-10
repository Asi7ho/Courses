#include <stdio.h>

void swap( double* a, double* b) {
	double c;
	c = *a; 
	*a = *b;
	*b = c;
}

void swap3(double* a, double* b, double* c) {
	swap(a, b);
	swap(a, c);
}

main() {
	double a = 10, b = 2, c = 3;
	swap3(&a, &b,&c);
	printf("a = %lf, b = %lf et c = %lf", a, b, c);
}

