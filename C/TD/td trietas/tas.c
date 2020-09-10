#include <stdio.h>


void augmentetas(double* tas, int i){ 
    
    double stk = 0;
        
        if (tas[(i-1)/2] < tas[i]){  //on échange le fils et le père si le fils est plus grand que le père
            stk = tas[i];
            tas[i] = tas[(i-1)/2];
            tas[(i-1)/2] = stk;
            augmentetas(tas, (i-1)/2); //on recommence avec père = fils 
        }
}



void constructiontas(double* tas, int n){
    
    int i;

    for (i = 1; i < n; i++){ //on construit le tas par augmentation du tas
        augmentetas(tas,i);
    }
}

void descendretas(double* tas, int i){	
    
	int k = 0;
	double stk = 0;

	// tant que tas[k] est plus petit qu'un des fils
	while ((2*k+1 < i) && ((tas[k] < tas[2*k+1] || tas[k] < tas[2*k+2]))) { // attention: il faut 2*k+1 < i  et 2*k+2 < i
        
		if(tas[2*k+1] > tas[2*k+2]){ //si le fils gauche est plus grand des fils on échange de place
            		stk = tas[2*k+1];
            		tas[2*k+1] = tas[k];
            		tas[k] = stk;
			k = 2*k+1;
        } else { //si le fils droit est plus grand des fils on échange de place
            	stk = tas[2*k+2];
           	tas[2*k+2] = tas[k];
            	tas[k] = stk;
		k = 2*k+2;
        }
    }
	// A ce stade, il se peut qu'on n'ait qu'un seul fils 
	// A faire: dans le cas où on n'a qu'un seul fils, échanger éventuellement
		
	if (tas[(i-1)/2] > tas[i]){
            stk = tas[i];
            tas[i] = tas[(i-1)/2];
            tas[(i-1)/2] = stk;
	}
}
void suppressiontas(double* tas, int i){
	
	double stk = 0;
	stk = tas[i-1];
	tas[i-1] = tas[0];
	tas[0] = stk;
	descendretas(tas,i-1);
}

/*
void tri(double* tas, int n){
    int i;
    constructiontas(tas,n); //on constuit un tas
    //puts("tas construit");
    for (i = n; i > 0; i--) {
	suppressiontas(tas, i); //on echange le premier element et le dermier et on supprime le dernier element
   }
}*/
