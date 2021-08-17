#include <iostream>
#include <vector>
#include <mpi.h>

#define ECHORES 0

using namespace std;

constexpr int sqr(int a) {
    return a*a;
}

template <class T>
inline void transverse(vector<T>& arr, const int m) {
    int i, j;
    for (i=0; i<m; i++) {
        for (j=0; j<m; j++) {
            swap(arr[i*m+j], arr[j*m+i]);
        }
    }
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

template <class T>
void parr(vector<T>& arr) {
    cout << '[';
    if (!arr.empty()) {
        cout << arr.front();
        for_each(arr.begin()+1, arr.end(), [](auto e) {
            cout << ',' << e;
        });
    }
    cout << ']' << endl;
}

template <class T>
void print_mtx_array(vector<T>& arr, const int N) {
    int i = 0;
    for (auto n : arr) {
        cout << n << " ";
        i = (i < N-1 ? i+1 : 0);
        if (!i) cout << endl;
    }
    cout << endl;
}


int main(int argc, char *argv[])
{
    int rank, size;
    const int msize = atoi(argv[1]);

    vector<double> mtxa(sqr(msize), 1.0),
                   mtxb(sqr(msize), 1.0),
                   mtxc(sqr(msize));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    double wtimebeg;

    const int RANGE = sqr(msize)/size,
              LINES = RANGE/msize;

    vector<double> abuf(RANGE),
                   bbuf(sqr(msize)),
                   res;

    if (!rank) {
        wtimebeg = MPI_Wtime();
        bbuf.insert(bbuf.begin(), mtxb.begin(), mtxb.end());
    }

    MPI_Scatter(&mtxa[0], RANGE, MPI_DOUBLE, &abuf[0], RANGE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&bbuf[0], sqr(msize), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    res = multm(abuf, bbuf, LINES, msize);
    
    MPI_Gather(&res[0], RANGE, MPI_DOUBLE, &mtxc[0], RANGE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (!rank) {
        if (ECHORES) print_mtx_array(mtxc, msize);
        cout << MPI_Wtime() - wtimebeg << endl;
    }

    MPI_Finalize();

    return 0;
}
