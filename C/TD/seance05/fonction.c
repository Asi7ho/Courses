#include <stdio.h>


int malectureimage(char* fic, unsigned char im[], int* pnl, int* pnc) {
	FILE* f; 
	char pixel[800*600];
	int i;int j = 0;
	f = fopen(fic, "r");
	if (f == NULL) {
		return -1;
	}
	if (strcmp(fgets(pixel, 255, f), "P5\n") != 0) {
		return -1;
	}
	do {
		fgets(pixel, 255, f);
	} while (pixel[0] == '#');
	sscanf(pixel, "%d %d", pnl, pnc);

	do {
		fgets(pixel, 255, f);
	} while (pixel[0] == '#');
	
	sscanf(pixel, "%d", &i);
	if (i > 255) {
		return -1;
	}
	printf("%d, %d", *pnl, *pnc);
	
	fread(im, (*pnl)*(*pnc), 1, f);
	for (j  = 0; j < (*pnl)*(*pnc) + 1; j++)
	{
		printf("%d ", im[j]);
	}
	printf("\n");
	fclose(f);
}

	
