#include <stdio.h>

double* ardval(double t[], int n, double val) {
	double *i;
	for (i = t; i < t+n; i++) {
		if (*i == val) {
			return i;
		}
	}
	return NULL;
}

void lecture(double* t, int n) {
	int i;
	for (i = 0; i < n; i++) {
		scanf("%lf", t+i);
	}
}

void affiche(double* t, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%lf\n", t[i]);
	}
}

main () {
	double t[5];
	double* a = NULL;
	lecture (t, 5);
	affiche (t, 5);
	a = ardval(t, 5, 2);
	*a = 0;
	affiche(t, 5);
}
