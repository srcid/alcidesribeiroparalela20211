// Correção: 2,0
#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int id, np;
    const int N = atoi(argv[3]), ROOT = 0;
    vector<double> arr1(N,atof(argv[1])), arr2(N,atof(argv[2]));

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int size, range, i;
    double res, ans, begin;
    vector<double> buf1, buf2;

    if (id == ROOT) {
        size = N;
        begin = MPI_Wtime();
    }

    MPI_Bcast(&size, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

    range = size/np;
    buf1.resize(range);
    buf2.resize(range);

    MPI_Scatter(&arr1[0], range, MPI_DOUBLE, &buf1[0], range, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    MPI_Scatter(&arr2[0], range, MPI_DOUBLE, &buf2[0], range, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

    res = 0.0;

    for (i=0; i<range; i++) res += buf1[i]*buf2[i];

    ans = 0.0;

    MPI_Reduce(&res, &ans, 1, MPI_DOUBLE, MPI_SUM, ROOT, MPI_COMM_WORLD);

    if (id == ROOT) {
        //cout << ans << endl;
        cout << MPI_Wtime()-begin << endl;
    } 

    MPI_Finalize();

    return 0;
}
