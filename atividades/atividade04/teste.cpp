#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int parallel_sum(vector<int>& arr) {
    int nThreads = omp_get_max_threads(),
        subArrSize = ceil((double)arr.size() / nThreads),
        *res = (int*)calloc(nThreads, sizeof(int)),
        acc;

    #pragma omp parallel
    {
        int i, j, ID = omp_get_thread_num();

        for (i=ID*subArrSize, j=(i+subArrSize <= arr.size() ? i+subArrSize : arr.size()); i < j; i++) {
            res[ID] += arr[i];
        }
    }

    acc = accumulate(res, res+nThreads, 0);

    free(res);

    return acc;
}

int main(int argc, char const *argv[])
{
    vector<int> arr(atoi(argv[1]),1);
    
    cout << parallel_sum(arr) << endl;

    return 0;
}
