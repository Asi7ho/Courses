#include <stdio.h>
#include "structure.h"

main() {
	FILE* f1;
	FILE* f2;
	FICHE element;
	f1 = fopen("tabletriee.txt", "rt");
	f2 = fopen("tabletriee.bin", "w+b");
	if (f1 == NULL) {
		printf("Imposible à ouvrir");
	}
	if (f2 == NULL) {
		printf("Impossible à ouvir");
	}
	while(!feof(f1)) {
		fscanf(f1, "%s %s %d %lf", element.nom, element.symbole, &element.numeroatomique, &element.masseatomique);
		fseek(f2, sizeof(element), element.numeroatomique-1);
		fwrite(&element, sizeof(element), 1, f2);
	}
	fclose(f1);
	fclose(f2);
}
