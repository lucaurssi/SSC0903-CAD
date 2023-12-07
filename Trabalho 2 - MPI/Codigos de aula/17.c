//compilar: mpicc modo-com.c -o modo-com
//executar: mpirun -np 2 modo-com

// exemplifica comunica��o ponto-a-ponto n�o bloqueante
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char **argv)  {
    int my_rank, num_procs;
    int tag, src, dst, test_flag;
    int buffersize, sizemsgsnd, nrmsgs;

    char *buffersnd;
    char msgsnd[30], msgrcv[30], msgsync[30];

    MPI_Status  status;
    MPI_Request mpirequest_mr, mpirequest_mr_ready;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    tag = 0;
    if (my_rank == 0)
    {
        //****************************************
        src = 1;  // determina o processo de origem de uma msg recebida como nao bloqueante
        //****************************************
        dst = 1;  // determina o processo de destino das msgs enviadas pelo processo com rank 0

        // recebe msg com primitiva nao bloqueante standard
        MPI_Irecv(msgrcv, sizeof(msgrcv), MPI_CHAR, src, tag, MPI_COMM_WORLD, &mpirequest_mr);
        // testa para ver se a msg ja foi recebida
        MPI_Test(&mpirequest_mr, &test_flag, &status);
        if(test_flag)
            printf("Rank 0: MPI_Test flag = %d, msgrcv=%s. \n", test_flag, msgrcv);
        else
            printf("Rank 0: MPI_Test flag = %d, nao recebeu a msg ainda. \n", test_flag);
    
        //****************************************
        strcpy(msgsnd, "Blocking standard send");
        MPI_Send(msgsnd, strlen(msgsnd)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD);
        //****************************************
        strcpy(msgsnd, "Blocking buffered send");
        //determina o tamanho basico do buffer para este tipo de dado
        //mais tipos de dados poderiam ser incluidos em outras funcoes Pack_size
        MPI_Pack_size(strlen(msgsnd)+1, MPI_CHAR, MPI_COMM_WORLD, &sizemsgsnd);
        // apenas uma msg bsend sera enviada
        nrmsgs = 1; 
        // calcula o tamanho final do buffer contando todos os dados a serem empacotados
        // insere overhead por msg a ser enviada com bsend
        buffersize = sizemsgsnd + nrmsgs * MPI_BSEND_OVERHEAD;
        // aloca o buffer considerando o tamanho total de todas as msgs bsend a serem enviadas
        buffersnd = (char*) malloc(buffersize);
        // fornece ao MPI um buffer na memoria do usu�rio para ser usado no bsend
        MPI_Buffer_attach(buffersnd, buffersize);
        // envia a msg bsend usando o buffer criado pelo usuario
        MPI_Bsend(msgsnd, strlen(msgsnd)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD);
        MPI_Buffer_detach(buffersnd, &buffersize);
        //****************************************

        strcpy(msgsnd, "Blocking synchronous send");
        MPI_Ssend(msgsnd, strlen(msgsnd)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD);
    
        //****************************************
        // printf for debbuging
        // printf("Rank 0: msg sync sizeof=%ld, src=%d, tag=%d\n", sizeof(msgsync), src, tag);
        // fflush(0);
    
        // tem que trocar de tag. A tag == 0 j� � usada para o MPI_Irecv do processo 0.
        // Se este MPI_Recv abaixo usar a tag 0 entao esta msg poder� ser recebida pelo MPI_Irecv
        // executado antes de todos os sends neste processo de rank 0.
        tag = 1;
    
        // recebe msg do processo 1 para garantir a execu��o previa do recv para MPI_Rsend abaixo
        // o recv evita um sleep(x)
        MPI_Recv(msgsync, sizeof(msgsync), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
    
        // printf for debbuging
        // printf("Rank 0: msg sync recebida: %s \n", msgsync);
        //fflush(0);
    
        // retorna para a tag = 0 usada nos sends e receives
        tag = 0;
        
        strcpy(msgsnd, "Blocking ready send");
        MPI_Rsend(msgsnd, strlen(msgsnd)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD);
    
        //****************************************
        // aguarda termino do MPI_Isend, caso nao tenha recebido ainda a msg        
        if(!test_flag)
        {
            MPI_Wait(&mpirequest_mr, &status);
            printf("Rank 0: MPI_Wait liberou: msgrcv=%s. \n", msgrcv);
        }  
    
    } // fim do if (my_rank == 0)
    else   // my_rank == 1
    {
        // determina o destino da msg a ser enviada
        dst = 0; 
        // determina o processo de origem da msg recebida
        src = 0;
        
        //****************************************
        // recebe do send bloqueante standard
        MPI_Recv(msgrcv, sizeof(msgrcv), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
        printf("Rank 1: msg 01 recebida: %s \n", msgrcv);
        fflush(0);
    
        //****************************************
        // recebe do send bloqueante com buffer
        MPI_Recv(msgrcv, sizeof(msgrcv), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
        printf("Rank 1: msg 02 recebida: %s \n", msgrcv);
        fflush(0);
    
        //****************************************
        // recebe do send bloqueante s�ncrono
        MPI_Recv(msgrcv, sizeof(msgrcv), MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
        printf("Rank 1: msg 03 recebida: %s \n", msgrcv);
        fflush(0);
    
        //****************************************
        // dispara previamente o MPI_IRecv para o send bloqueante ready existente no processo 0
        MPI_Irecv(msgrcv, sizeof(msgrcv), MPI_CHAR, src, tag, MPI_COMM_WORLD, &mpirequest_mr_ready);
    
        // este send abaixo garante que o MPI_IRecv acima ser� executado antes que seu MPI_Rsend do processo 0
        // ele libera a execu��o do MPI_Rsend
    
        // tem que trocar de tag. A tag == 0 coincide j� � usada para o MPI_Irecv do processo 0.
        // Se este MPI_Send abaixo usar a tag 0 entao esta msg ser� a primeira a chegar no processo 0
        // e ser� recebida pelo MPI_Irecv executado antes de todos os sends.
        tag = 1;
        
        strcpy(msgsync, "unlock ready send");
        
        // printf for debbuging
        // printf("Rank 1: msg sync a enviar: %s, strlen=%ld, dst=%d, tag=%d \n", msgsync, strlen(msgsync)+1, dst, tag );
        // fflush(0);
        MPI_Send(msgsync, strlen(msgsync)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD);
        
        // aguarda pelo recebimento da msg do MPI_Rsend. Ir� finalizar o MPI_Irecv acima.
        MPI_Wait(&mpirequest_mr_ready, &status);
        
        printf("Rank 1: msg 04 recebida: %s \n", msgrcv);
        fflush(0);
        
        // retorna para a tag = 0 usada nos sends e receives
        tag = 0;
        
        //****************************************
        // conteudo da msg de envio
        strcpy(msgsnd, "Nonblocking standard send");  
        // send nao bloqueante
        MPI_Isend(msgsnd, strlen(msgsnd)+1, MPI_CHAR, dst, tag, MPI_COMM_WORLD, &mpirequest_mr);
        // aguarda termino do MPI_Isend
        MPI_Wait(&mpirequest_mr, &status);
        
    } // fim do else my_rank == 1
    
    MPI_Finalize();
	
    exit(0);

}

