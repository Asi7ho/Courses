#include <stdio.h>
#include <math.h>

main() { 
	double a,b,c,v, eps = 1e-15, va;
	a = 1; // a = u1
	b = 2; // b = u2
	do {
		c = a + b;
		a = b;
		b = c; // b = Ui
		va = v;
		v = b/a; // Vi = Ui/Ui-1
	} while (fabs(v - va) > eps * (fabs(v) + fabs(va))); 
	printf(" le nombre d'or est: %lf", v);
	

}
