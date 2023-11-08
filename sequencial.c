#include<stdio.h>
#include<stdlib.h>
#include <omp.h>


/*
 SSC0903 - Computação de Alto Desempenho (2023-2) BCC-Turma B
 
- Eduardo Garcia de Gáspari Valdejão, 11795676
- Henrico Lazuroz Moura de Almeida, 12543502
- Luca Gomes Urssi, 10425396
- Victor Lucas de Almeida Fernandes, 12675399
 
 Escolhemos gerar uma matriz aleatoria e 
 aplicar um filtro de suzvizacao Gaussiano.
 
 Apos aplicar o filtro buscamos pelos maiores e 
 menores valores para binarizar a imagem, 
 criando uma imagem de duas cores. (1 & 0)
 
*/

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
	printf("\n");
	fflush(stdout);
	return;
}

int main () {
	
	int IMG_SIZE = 10000;
	srand(1);
	
	int max =0;
	int min =256;
	int mean;
	
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
	//print_mat(new_mat, IMG_SIZE);

// -------     Image processing area begin here     ------- 
	
	double wtime;
	wtime = omp_get_wtime();
	
	
	// Apply Gaussian Smoothing
	for(int i=0; i<IMG_SIZE; i++)
		for(int j=0; j<IMG_SIZE; j++)
			new_mat[i][j] = smoothing(mat, IMG_SIZE, i, j); // smooth one position at a time
	
	//print_mat(new_mat, IMG_SIZE);



	// Fiding the max & min
	for(int i=0; i<IMG_SIZE; i++)
		for(int j=0; j<IMG_SIZE; j++)
			if(new_mat[i][j]>max) max = new_mat[i][j];
			else if(new_mat[i][j]<min) min = new_mat[i][j];

	mean = (int)(max+min)/2;

	// binary
	for(int i=0; i<IMG_SIZE; i++)
		for(int j=0; j<IMG_SIZE; j++)
			if(new_mat[i][j] <= mean) new_mat[i][j] = 0;
			else new_mat[i][j] = 1;
	
	//printf("mean: %d\n", mean);
	//print_mat(new_mat, IMG_SIZE);
	
	
	wtime = omp_get_wtime() - wtime; 
	printf("Sequential time = %.5f\n", wtime );
	
// -------     Image processing area end here     ------- 
	
	for(int i=0; i<IMG_SIZE; i++){
		free(mat[i]);
		free(new_mat[i]);
	}
	free(mat);
	free(new_mat);
    return 0;

}
