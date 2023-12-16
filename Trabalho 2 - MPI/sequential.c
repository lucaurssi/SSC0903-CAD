/*
SSC0903 - Computação de Alto Desempenho (2023-2) BCC-Turma B
Trabolho 2 

Membros:

- Eduardo Garcia de Gáspari Valdejão, 11795676
- Henrico Lazuroz Moura de Almeida, 12543502
- Luca Gomes Urssi, 10425396
- Victor Lucas de Almeida Fernandes, 12675399
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#inclue <string.h>

#define PRINT 0

int main(int argc, char *argv[]){

	int MAT_SIZE = atoi(argv[1]);
	
	// create matrix - initialized with zeros
	int **mat = (int**)calloc (MAT_SIZE, sizeof (int*));
	for (int i=0; i < MAT_SIZE; i++)
		mat[i] = (int*)calloc (MAT_SIZE, sizeof (int));
	
	clock_t time = clock();
	double total_time;
	
	int max = 0;
	int min = MAT_SIZE*2;
	long int sum = 0;
	int *sum_lin = (int*)calloc (MAT_SIZE, sizeof (int));
	int *sum_col = (int*)calloc (MAT_SIZE, sizeof (int));
    
	for(int k=0; k<100; k++){ // multiple runs
		
		max = 0;
		min = MAT_SIZE*2;
		sum = 0;
		memset(sum_lin, 0, MAT_SIZE * sizeof(int));
		memset(sum_col, 0, MAT_SIZE * sizeof(int));
		
		
		// populating 'mat' with i + j
		for(int i=0; i<MAT_SIZE; i++)
			for(int j=0; j<MAT_SIZE; j++)
				mat[i][j] = i + j;
		
		// actual loop
		for(int i=0; i<MAT_SIZE; i++)
			for(int j=0; j<MAT_SIZE; j++){
				if(mat[i][j] > max) 
					max = mat[i][j];
				else 
					if(mat[i][j] < min) 
						min = mat[i][j];
				
				sum += mat[i][j];
				
				sum_lin[j] += mat[i][j];
				sum_col[i] += mat[i][j];
			}
	}

    total_time = (double)(clock() - time)/CLOCKS_PER_SEC;
    printf("time: %f\n", total_time);	

    if(PRINT){
	    printf("max : %d\n", max);
	    printf("min : %d\n", min);
	    printf("sum : %ld\n", sum);
	    printf("sum_lin :\n");
	    for(int i=0; i<MAT_SIZE; i++)
		    ;//printf("%d ", sum_lin[i]);
	    printf("\nsum_col :\n");
	    for(int i=0; i<MAT_SIZE; i++)
		    ;//printf("%d ", sum_col[i]);
	}
	
	
	
	for(int i=0; i<MAT_SIZE; i++)
		free(mat[i]);
	free(mat);
	free(sum_col);
	free(sum_lin);
	
	return 0;
}
