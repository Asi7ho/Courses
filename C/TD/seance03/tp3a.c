#include <stdio.h>

int max(int x, int y) {
	if (x > y) {
		return x;
	} else {
		return y;
	}
}

int max3(int x, int y, int z) {
	int a;
	a  = max (x, y);
	if (a > z) {
		return a;
	} else {
		return z;
	}
}

double puiss(double x, int n) {
	int i;
	double a;
	a = 1;
	for (i = 0; i < n+1; i++) {
		a = x;
		x = a*x;
	}
	return x;
}

int pgcd(int a, int b) {
	int r;
	while (b != 0) {
		r = a%b;
		a = b;
		b = r;
	}
	return a;
}

double produit(double tab[], int n) {
	int i;
	double prod = 1;
	for (i = 0; i < n; i++) {
		prod = prod * tab[i];
	}
	return prod;
}

main() {
	int a,b,c,n,i;
	double tabprod[10], x;
	a = 15;
	b = 27;
	c = 18;
	x = 3;
	n = 3;
	tabprod[0] = 17.43;
	for ( i = 1; i < 10; i++) {
		tabprod[i] = 3.76*i;
	}
	printf("le max entre %d et %d est %d\n", a,b, max(a,b));
	printf("le max entre %d, %d et %d est %d\n", a,b,c,max3(a,b,c));
	printf("%lf à la puissance %d est %lf\n", x,n,puiss(x,n));
	printf("le pgcd entre %d et %d est %d\n", b,c,pgcd(b,c));
	printf("le produit des elements du tableau donne %lf\n", produit(tabprod, 10));

}
