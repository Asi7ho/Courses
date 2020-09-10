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

void copie(double t1[], int n1, double t2[], int* pn2) {
	double* i1 = NULL;
	
	*pn2 = 1;

	t2[0] = t1[0];

	for (i1 = t1+1; i1 < t1+n1; i1++) {
		if (ardval(t2, *pn2, *i1) == NULL) {
			t2[*pn2] = *i1;
			printf("in %lf\n", t2[*pn2]);
			*pn2++;
		}
	}
}			

main () {
	double t1[5]; 
	double t2[5];
	int pn = 0;
	lecture(t1, 5);
	copie(t1, 5, t2, &pn);
	printf("%d\n", pn);
	affiche(t2, pn);
}

				
				
