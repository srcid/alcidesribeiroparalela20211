#include <stdio.h>
#include <mpi.h>

enum Color {
    M2,     /* Multiplo de 2 */
    M3,     /* Multiplo de 3 */
    M2_3,   /* Multiplo de 2 e 3 */
    M_OTHER /* Multiplo de outro número */
};

char *ctos(enum Color c) {
    switch (c) {
    case M2: 
        return "M2";
        break;
    case M3:
        return "M3";
        break;
    case M2_3:
        return "M2_3";
        break;
    case M_OTHER:
        return "M_OTHER";
        break;
    }
}

int main(int argc, char const *argv[])
{
    int rank, nrank, size, nsize;
    MPI_Comm ncomm;
    enum Color c;
    char* s;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    c = rank % 2 == 0 && rank % 3 == 0 ? M2_3 : rank % 2 == 0 ? M2 : rank % 3 == 0 ? M3 : M_OTHER;
    s = ctos(c);

    MPI_Comm_split(MPI_COMM_WORLD, c, 0, &ncomm);

    MPI_Comm_rank(ncomm, &nrank);
    MPI_Comm_size(ncomm, &nsize);

    printf("COMM_WORLD(%d/%d) ➜ %s(%d/%d)\n", rank, size, s, nrank, nsize);

    MPI_Comm_free(&ncomm);
    
    MPI_Finalize();

    return 0;
}
