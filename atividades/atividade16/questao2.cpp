// Correção: 0,5
/**
 * Instruções de compilação:
 * mpic++ -fopenmp <cppfile>
 * 
 * Instruções de execução
 * (--bind-to none) para que se posso usar as threads
 * (-x MPI_NUM_THREADS=<N>) caso se queira especificar o número de threads no openmp
 */

#include <bits/stdc++.h>
#include <omp.h>
#include <mpi.h>

#define ECHO 0
#define ECHO_FINAL 1

using namespace std;

constexpr unsigned int sqr(const int N) {
    return N*N;
}

void pmtx(vector<double>& mtx, const int N) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            cout << left << setw(10) << setprecision(3) << mtx[i*N + j];
        }
        cout << endl;
    }
    cout << endl;
}

void pmtx(vector<double>& mtx, const int M, const int N) {
    int i, j;
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            cout << left << setw(10) << setprecision(3) << mtx[i*N + j];
        }
        cout << endl;
    }
    cout << endl;
}

vector<double> mtxmult(vector<double>& a, vector<double>& b, const int ar, const int ac_br, const int bc) {
    int i, j, k;
    vector<double> c(ar * bc, 0.0);

    #pragma omp parallel for collapse(3)
    for (i = 0; i < ar; i++) {
        for (j = 0; j < bc; j++) {
            for (k = 0; k < ac_br; k++) {
                c[ i*bc + j ] += a[ i*ac_br + k ] * b[ k*ac_br + j ];
            }
        }
    }

    return c;
}

int main(int argc, char **argv)
{
    int rank, size, provided;
    const int ROOT = 0;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    #if ECHO
    cout << "rank: " << rank << " size: " << size << " provided: " << provided << endl;
    #endif

    int N; /* Tamanho da matriz */
    vector<double> mtxa, mtxb, mtxc;

    if (rank == ROOT) {
        N = atoi(argv[1]);
        mtxa.insert(mtxa.begin(), sqr(N), 1.0);
        mtxb.insert(mtxb.begin(), sqr(N), 1.0);
    }

    MPI_Bcast(&N, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (rank != ROOT) {
        mtxa.resize(sqr(N)/size);
        mtxb.resize(sqr(N));
    }

    MPI_Scatter(&mtxa[0], sqr(N)/size, MPI_DOUBLE, &mtxa[0], sqr(N)/size, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    MPI_Bcast(&mtxb[0], sqr(N), MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    
    /**
     * O processo ROOT pode remover o que enviado para os outros processo no Scatter
     * e manter apenas o que computará
     */
    if (rank == ROOT) mtxa.erase(mtxa.begin()+sqr(N)/size+1, mtxa.end()-1);

    #if ECHO
    pmtx(mtxa, N/size, N);
    pmtx(mtxb, N);
    #endif

    mtxc = mtxmult(mtxa, mtxb, N/size, N, N);

    #if ECHO
    pmtx(mtxc, N/size, N);
    #endif
    
    if (rank == ROOT) mtxc.resize(sqr(N));

    MPI_Gather(&mtxc[0], sqr(N)/size, MPI_DOUBLE, &mtxc[0], sqr(N)/size, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    #if ECHO_FINAL
    if (rank == ROOT) pmtx(mtxc, N);
    #endif

    MPI_Finalize();

    return 0;
}
