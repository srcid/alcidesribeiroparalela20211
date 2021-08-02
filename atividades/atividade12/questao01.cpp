#include <bits/stdc++.h>
#include <mpi.h>

#define ROOT 0

using namespace std;

int main(int argc, char const *argv[])
{
    int id, np;
    const int N = atoi(argv[3]);
    vector<double> arr1(N,atof(argv[1])),
                   arr2(N,atof(argv[2]));
    double res = 0;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    double begin_time, acc;
    int size, gap;

    if (id == ROOT) {
        begin_time = MPI_Wtime();
        size = N;
    }

    MPI_Bcast(&size, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    
    gap = size/np;
    acc = 0;

    for (int i=id*gap; i < id*gap+gap; i++) {
        acc += arr1[i]*arr2[i];
    }

    MPI_Reduce(&acc, &res, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);    

    if (id == ROOT) {
        cout << np << "," << MPI_Wtime() - begin_time << endl;
    }

    MPI_Finalize();

    return 0;
}
