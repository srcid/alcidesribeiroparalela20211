#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

#include <mpi.h>

#define TAM_MAX 65536
#define REQ_SIZE 2
#define ECHO 0

int main(int argc, char const *argv[])
{
    int meu_ranque, num_procs;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    double tbeg;
    int sucessor, predecessor, a, ir, i;
    int vetor[TAM_MAX];

    if (!meu_ranque) tbeg = MPI_Wtime();

    for (i = 0; i < TAM_MAX; i++) _rdrand32_step((unsigned int *)&vetor[i]);
    _rdrand32_step((unsigned int *)&a);
    ir = 0;
    sucessor = (meu_ranque + 1) % num_procs;
    predecessor = (meu_ranque - 1 + num_procs) % num_procs;
    MPI_Request reqs[REQ_SIZE];
    
    if (ECHO) {
        printf("%d: send a == %d to %d\n", meu_ranque, a, sucessor);
        printf("%d: send vetor == {%d", meu_ranque, vetor[0]);
        for (i = 1; i < TAM_MAX; i++) printf(",%d", vetor[i]);
        printf("} to %d\n", sucessor);
    }

    MPI_Isend(&a, 1, MPI_FLOAT, sucessor, 1, MPI_COMM_WORLD, &reqs[ir++]);
    MPI_Isend(vetor, TAM_MAX, MPI_FLOAT, sucessor, 1, MPI_COMM_WORLD, &reqs[ir++]);

    ir = 0;

    MPI_Irecv(&a, 1, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &reqs[ir++]);
    MPI_Irecv(vetor, TAM_MAX, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &reqs[ir++]);

    MPI_Waitall(REQ_SIZE, reqs, MPI_STATUS_IGNORE);
    
    if (ECHO) { 
        printf("%d: got a == %d from %d\n", meu_ranque, a, predecessor);
        printf("%d: got vetor == {%d", meu_ranque, vetor[0]);
        for (i = 1; i < TAM_MAX; i++) printf(",%d", vetor[i]);
        printf("} from %d\n", predecessor);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (!meu_ranque) printf("%f\n", MPI_Wtime()-tbeg);

    MPI_Finalize();
    
    return 0;
}
