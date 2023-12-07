#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAM 16
 
int main( int argc, char **argv )
{
    int rank, size,rec_size, i,*vetor_env,*vetor_rec, root=0;
 
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    rec_size=TAM/size;
    vetor_rec=(int*)malloc(rec_size*sizeof(int));

    if(rank == root)
    {
		vetor_env=(int*)malloc(TAM*sizeof(int));

        for(i=0;i<TAM;i++)
        {
            vetor_env[i]=i;
        }
		for(i=0;i<TAM;i++)
		{
			printf("Processo %d: vetor_env[%d]=%d \n",rank, i, vetor_env[i]);
		}
    }

    MPI_Scatter (vetor_env,rec_size,MPI_INT,vetor_rec,rec_size,MPI_INT,root,MPI_COMM_WORLD);
    
    for(i=0;i<rec_size;i++)
    {
        printf("Rank :%d recebeu %d\n",rank, vetor_rec[i]);
    }

	if(rank == root)
    {
		free(vetor_env);
	}
	
    free(vetor_rec);
    MPI_Finalize();
}
