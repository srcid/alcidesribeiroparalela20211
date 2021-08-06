// Correção: 1,0
#include <bits/stdc++.h>
#include <mpi.h>

#define ECHO 0

using namespace std;

inline int ceildiv(int a, int b)
{
    return (a + b - 1) / b;
}

int main(int argc, char const *argv[])
{
    const int N = atoi(argv[1]), root = 0;
    int id, np;

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (id == root) {
        vector<double> arr1(N,0.0666), arr2(N,0.01);
        int i, tag, rg, beg, end;
        double ans, tmp, wtime_begin, wtime_end;

        wtime_begin = MPI_Wtime();

        tag = 0;
        ans = 0;
        rg = ceildiv(N, np-1);

        for (auto arr : {&arr1, &arr2}) {
            for (i = 1; i < np; i++) {
                beg = (i-1)*rg;
                end = beg + rg;

                if (end > arr->size()) {
                    if (ECHO) cout << id << ": enviando " << rg - (end - arr->size()) << " to " << i << endl;
                    MPI_Send(&(*arr)[beg], rg - (end - arr->size()), MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
                }
                else {
                    if (ECHO) cout << id << ": enviando " << rg << " to " << i << endl;
                    MPI_Send(&(*arr)[beg], rg, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
                }
            }
            tag++;
        }

        tag = 0;

        for (i = 1; i < np; i++) {
            MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, nullptr);
            ans += tmp;
        }

        wtime_end = MPI_Wtime();

        if (ECHO) cout << "ans: " << ans << endl;

        cout << "size: " << np << " | elapsed: " << wtime_end - wtime_begin << endl;

    } else {
        vector<double> buf1, buf2;
        int i, size, tag, count;
        double acc;
        MPI_Status status;

        acc = 0;
        tag = 0;
        count = ceildiv(N, np-1);
        buf1.resize(count);
        buf2.resize(count);
        
        for (auto buf : {&buf1, &buf2}) {
            if (ECHO) cout << id << ": recebeu de root" << endl;
            
            MPI_Recv(&(*buf)[0], count, MPI_DOUBLE, root, tag, MPI_COMM_WORLD, &status);
            tag++;
        }
        
        MPI_Get_count(&status, MPI_DOUBLE, &size);

        for (i=0; i<size; i++) {
            acc += buf1[i]*buf2[i];
        }

        tag = 0;

        if (ECHO) cout << id << ": enviando " << acc << endl;

        MPI_Send(&acc, 1, MPI_DOUBLE, root, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
