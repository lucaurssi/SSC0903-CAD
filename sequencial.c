#include<stdio.h>
#include<stdlib.h>


/*
	Gaussian Smoothing:
	-smoothing filter with the following weights:
	1 2 1
	2 4 2
	1 2 1
*/
int smoothing(int **mat, int IMG_SIZE, int i, int j){
	int sum = 0;
	int divisor = 4; 
	
	// first smoothing column
	// 1 x x
	// 2 x x
	// 1 x x
	if(i!=0){ // not in column 0
		divisor += 2;
		if(j!=0){
			sum+= mat[i-1][j-1]; // not in line 0
			divisor += 1;
		}
		sum+= mat[i-1][j] * 2; 
		if(j!=IMG_SIZE-1) {
			sum+= mat[i-1][j+1]; // not at the last line
			divisor += 1;
		}
	}
	
	// middle smoothing column
	// x 2 x
	// x 4 x
	// x 2 x
	if(j!=0) {
		sum+= mat[i][j-1] * 2; 
		divisor += 2;
	}
	sum += mat[i][j] * 4; // current position
	if(j!=IMG_SIZE-1) {
		sum+= mat[i][j+1] * 2;
		divisor += 2;
	}
	
	// last smoothing column
	// x x 1
	// x x 2
	// x x 1
	if(i!=IMG_SIZE-1){
		divisor += 2;
		if(j!=0) {
			sum+= mat[i+1][j-1];
			divisor += 1;
		}
		sum+= mat[i+1][j] * 2; 
		if(j!=IMG_SIZE-1) {
			sum+= mat[i+1][j+1];
			divisor += 1;
		}
	}
	
	return sum/divisor;
}

void print_mat(int **mat, int IMG_SIZE){
	for(int i=0; i<IMG_SIZE; i++){
		for(int j=0; j<IMG_SIZE; j++)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
	fflush(stdout);
	return;
}

int main () {
	
	int IMG_SIZE = 5;
	srand(1);
	
	// create image matrix - initialized with zeros
	int **mat = (int**)calloc (IMG_SIZE, sizeof (int*));
	for (int i=0; i < IMG_SIZE; i++)
		mat[i] = (int*)calloc (IMG_SIZE, sizeof (int));
	
	// create result image matrix
	int **new_mat = (int**)calloc (IMG_SIZE, sizeof (int*));
	for (int i=0; i < IMG_SIZE; i++)
		new_mat[i] = (int*)calloc (IMG_SIZE, sizeof (int));

	
	// populating 'mat' with random values between 0-255
	for(int i=0; i<IMG_SIZE; i++)
		for(int j=0; j<IMG_SIZE; j++)
			mat[i][j] = rand()%256;
	
	//print_mat(mat, IMG_SIZE);
	
	// Apply Gaussian Smoothing
	for(int i=0; i<IMG_SIZE; i++)
		for(int j=0; j<IMG_SIZE; j++)
			new_mat[i][j] = smoothing(mat, IMG_SIZE, i, j); // smooth one position at a time
		
	
	//print_mat(new_mat, IMG_SIZE);
	
	for(int i=0; i<IMG_SIZE; i++){
		free(mat[i]);
		free(new_mat[i]);
	}
	free(mat);
	free(new_mat);
    return 0;

}