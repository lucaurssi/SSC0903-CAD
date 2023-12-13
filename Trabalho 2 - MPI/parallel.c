// para compilar: mpicc hello.c -o hello -Wall

// para rodar experimente uma destas:
// mpirun -np 2 hello   (supondo 04 cores)
// mpirun --oversubscribe -np 2 hello (sobreposicao)
// mpirun -np 2 --hostfile hosts hello (segue o hostfile e deve obedecer limitacoes)
// mpirun -np 2 --host hostname1:slots,hostname2:slots hello (segue -host e segue limitacoes. slots � opcional (default 1))
// mpirun -np 2 --host hal02:4,hal03:4,hal05:4,hal07:4 hello

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
#include <string.h>
#include <mpi.h>
#define MESSAGE "Hello World running!!!! " 

int  main(int argc, char *argv[])  
{
	int npes, myrank, src, dest, msgtag, ret;
	char *bufrecv, *bufsend;
	MPI_Status status;
	
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &npes); // retorna 2
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  // retonar 0 ou 1 a depender do processo
	
	MPI_Get_processor_name(processor_name, &name_len);

	bufrecv = (char *) malloc (strlen(MESSAGE)+1);
   	msgtag = 1;  // dois processos executam aqui
	if ( myrank == 0) 
	{ // para o processo 0
        printf("There are %d processes.\n", npes);
		printf("I am process %d. Message with %d char. \n", myrank, (int)strlen(MESSAGE));
		fflush(0);
		
		bufsend = (char *) malloc (strlen(MESSAGE)+1);
        strcpy(bufsend,MESSAGE);
		
		src = dest = 1;
		MPI_Send(bufsend, strlen(bufsend)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		
		msgtag = 2;
		MPI_Recv(bufrecv, strlen(MESSAGE)+1, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
		
		printf("Master received message from slave: %s with %d chars. From host %s \n", bufrecv, (int)strlen(bufrecv), processor_name);
	
	}
	else 
	{   // para o processo 1
         printf("There are %d processes. I am process %d \n", npes, myrank);
		// fflush(0);
		bufsend = (char *) malloc (strlen(MESSAGE)+1);
		src = dest = 0;
		MPI_Recv(bufrecv, strlen(MESSAGE)+1, MPI_CHAR, src, msgtag, MPI_COMM_WORLD, &status);
		msgtag = 2;
		strcpy(bufsend, bufrecv);
		MPI_Send(bufsend, strlen(MESSAGE)+1, MPI_CHAR, dest, msgtag, MPI_COMM_WORLD);
		free(bufsend);
	} 
	free(bufrecv);
	fflush(0);
	ret = MPI_Finalize();
	if (ret == MPI_SUCCESS)
		printf("MPI_Finalize success from myrank %d from host %s\n", myrank, processor_name);
	return(0);
}
