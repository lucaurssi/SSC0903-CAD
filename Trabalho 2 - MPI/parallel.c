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
#include <mpi.h>
#include <time.h>
#incldue <string.h>

#define PRINT 0


int main(int argc, char *argv[]){
	
	int MAT_SIZE = atoi(argv[1]);
	
	MPI_Init(&argc, &argv);
	
	// create matrix - initialized with zeros
	int **mat = (int**)calloc (MAT_SIZE, sizeof (int*));
	for (int i=0; i < MAT_SIZE; i++)
		mat[i] = (int*)calloc (MAT_SIZE, sizeof (int));
	
	clock_t time = clock();
	double total_time;
	
	int npes, myrank, ret;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	
	MPI_Comm_size(MPI_COMM_WORLD, &npes); // retorna o numero de processos
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  // retonaro rank do processo	
	MPI_Get_processor_name(processor_name, &name_len);
	
	int mat_size_por_processo = MAT_SIZE  / npes;
    int inicio = myrank * mat_size_por_processo;	

	int max = 0;
	int min = MAT_SIZE*2;
	long int sum = 0;
	int *sum_lin = (int*)calloc (MAT_SIZE, sizeof (int));
	int *sum_col = (int*)calloc (MAT_SIZE, sizeof (int));
	
	int global_max=0;
	int global_min=0;
	long int global_sum=0;
	int *global_lin = (int*)calloc (MAT_SIZE, sizeof (int));
	int *global_col = (int*)calloc (MAT_SIZE, sizeof (int));
	
	for(int k=0; k<100; k++){ // multiple runs
	
		max = 0;
		min = MAT_SIZE*2;
		sum = 0;
		memset(sum_lin, 0, MAT_SIZE * sizeof(int));
		memset(sum_col, 0, MAT_SIZE * sizeof(int));
		
		global_max=0;
		global_min=0;
		global_sum=0;
		memset(global_lin, 0, MAT_SIZE * sizeof(int));
		memset(global_col, 0, MAT_SIZE * sizeof(int));
		
		// inside loop because its improved with parallelism
		for(int i=inicio; i<inicio + mat_size_por_processo; i++)
			for(int j=0; j<MAT_SIZE; j++)
				mat[i][j] = i + j;
		
		// actual loop
		for(int i=inicio; i<inicio + mat_size_por_processo; i++)
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
		
		MPI_Reduce(&max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
		MPI_Reduce(&min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
		MPI_Reduce(&sum, &global_sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(sum_lin, global_lin, MAT_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(sum_col, global_col, MAT_SIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	
	
	total_time = (double)(clock() - time)/CLOCKS_PER_SEC;
	if(myrank == 0) 
		printf("time: %f\n", total_time);	
	
	if(myrank == 0 && PRINT){
		
		printf("max : %d\n", global_max);
		printf("min : %d\n", global_min);
		printf("sum : %ld\n", global_sum);
		printf("global_lin :\n");
		for(int i=0; i<MAT_SIZE; i++)
			;//printf("%d ", global_lin[i]);
		printf("\nglobal_col :\n");
		for(int i=0; i<MAT_SIZE; i++)
			;//printf("%d ", global_col[i]);
	}
	
    ret = MPI_Finalize();
	
	for(int i=0; i<MAT_SIZE; i++)
		free(mat[i]);
	free(mat);
	free(sum_col);
	free(sum_lin);
	free(global_lin);
	free(global_col);
	
	
	if (ret == MPI_SUCCESS && PRINT)
		printf("MPI_Finalize success from myrank %d from host %s\n", myrank, processor_name);
    
	return 0;
}
