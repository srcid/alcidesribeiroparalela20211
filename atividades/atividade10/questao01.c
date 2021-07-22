#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
                        
#define MAX 100
                        
int main(int argc, char *argv[]) {
    int rank, size, total_num, source, tag, numbers[MAX];

    source = 0;
    tag = 0;
                        
    MPI_Status status;
                        
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank != source) {
        srand(time(0)*rank);
        total_num = (rand() / (float) RAND_MAX) * MAX;

        printf("Sanding %d, from %d to 0.\n", total_num, rank);

        /* tag will be always 0 because all process only will executed once */
        MPI_Send((const void *)&numbers, total_num, MPI_INT, source, tag, MPI_COMM_WORLD);
    } else {

        for (int i=1; i < size; i++) {
            /* MPI_ANY_SOURCE is used because it is wanted to receive the results as they finish.
             * MPI_ANY_TAG for that such of case had only semantic meaning whereas it will be always 0.
             */
            MPI_Recv((void *)&numbers, MAX, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &total_num);
                            
            printf("Process 0 received %d numbers from %d\n", total_num, status.MPI_SOURCE, status.MPI_TAG);
        }

    }
                        
    MPI_Finalize();
                   
    return 0;
}