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
	
	return(0);
}
