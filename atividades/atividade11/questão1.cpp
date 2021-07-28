#include <bits/stdc++.h>
#include <mpi.h>

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

    switch (id)
    {
    case 0: {
        vector<int> arr1(N,1), arr2(N,1), workers(np-1);
        int tag, res, ans;

        tag = 0;
        ans = 0;

        iota(workers.begin(), workers.end(), 1);

        for (vector<int> arr : {arr1, arr2}) {
            int range, beg, end, tmp;

            range = ceildiv(arr.size(), np-1);

            for (int id : workers) {
                beg = (id-1)*range;
                end = beg + range;
                
                if (end > arr.size()) {
                    MPI_Send(&arr[beg], range-(end- arr.size()), MPI_INT, id, tag, MPI_COMM_WORLD);
                } else {
                    MPI_Send(&arr[beg], range, MPI_INT, id, tag, MPI_COMM_WORLD);
                }
            }

            res = 1;

            for (int id : workers) {
                MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, nullptr);
                res *= tmp;
            }

            ans += res;

            tag++;
        }

        cout << "ans: " << ans << endl;

        break;
    }
    default: {
        vector<int> buf;
        int acc, tag, count;
        MPI_Status status;
        
        count = ceildiv(N, np-1);
        tag = 0;

        buf.resize(count);

        while (tag < 2) {
            acc = 1;

            MPI_Recv(&buf[0], count, MPI_INT, root, tag, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_INT, &count);

            for_each(buf.begin(), buf.begin()+count, [&acc](int x) {
                acc *= x;
            });

            MPI_Send(&acc, 1, MPI_INT, root, tag, MPI_COMM_WORLD);

            tag++;
        }

        break;
    }
    }

    MPI_Finalize();
    return 0;
}
