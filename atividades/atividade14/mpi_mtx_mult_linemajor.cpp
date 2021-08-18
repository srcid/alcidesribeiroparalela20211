#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

constexpr int sqr(int a) {
    return a*a;
}

template <class T>
vector<T> multm(vector<T>& ma, vector<T>& mb, const int NROWS, const int N) {
    int i, j, k;
    vector<T> mc(NROWS*N, 0.0);

    for (i=0; i<NROWS; i++) {
        for (j=0; j<N; j++) {
            for (k=0; k<N; k++) {
                mc[i*N+j] += ma[i*N+k]*mb[k*N+j];
            }
        }
    }

    return mc;
}

int main(int argc, char *argv[])
{
    int rank, size;
    const int msize = atoi(argv[1]);
    vector<double> mtxa, mtxb, mtxc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    double wtimebeg;
    
    if (!rank) wtimebeg = MPI_Wtime();

    const int RANGE = sqr(msize)/size, /* Número de elementos a serem processados */
              LINES = RANGE/msize;     /* Número de linhas que cada processo pegou */

    vector<double> abuf, res;

    abuf.resize(RANGE);

    /* Apenas 0 aloca a matriz A a inteiramente, e, depois de enviá-la, a desaloca */
    if (!rank) mtxa.insert(mtxa.begin(), sqr(msize), 1.0);
    
    MPI_Scatter(&mtxa[0], RANGE, MPI_DOUBLE, &abuf[0], RANGE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (!rank) mtxa.clear();

    /* 0 carrega a matriz B e a copia pelos demais processos */
    if (!rank) mtxb.insert(mtxb.begin(), sqr(msize), 1.0);
    else mtxb.reserve(sqr(msize));
    
    MPI_Bcast(&mtxb[0], sqr(msize), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    res = multm(abuf, mtxb, LINES, msize);

    mtxb.clear();

    if (!rank) mtxc.resize(sqr(msize));
    
    MPI_Gather(&res[0], RANGE, MPI_DOUBLE, &mtxc[0], RANGE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* A barreira é usada para assegurar que 0 não terminará antes dos outros impactando
     * a medida de tempo para baixo 
     */
    MPI_Barrier(MPI_COMM_WORLD);

    if (!rank) cout << MPI_Wtime() - wtimebeg << endl;

    MPI_Finalize();

    return 0;
}
