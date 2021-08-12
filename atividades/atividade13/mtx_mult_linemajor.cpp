#include <bits/stdc++.h>

#define ECHO 0

using namespace std;

constexpr int sqr(int n) {
    return n*n;
}

template <class T>
vector<T> multm(vector<T>& ma, vector<T>& mb, const int N) {
    int i, j, k;
    vector<T> mc(sqr(N), 0.0);

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            for (k=0; k<N; k++) {
                mc[i*N+j] += ma[i*N+k]*mb[k*N+j];
            }
        }
    }

    return mc;
}

void print_array(vector<double>& arr, const int N) {
    int i = 1;
    for (auto n : arr) {
        cout << n << " ";
        i = (i < N ? i+1 : 0);
        if (!i) cout << endl;
    }
    cout << endl;
}


int main(int argc, char *argv[])
{
    const int msize = atoi(argv[1]);
    vector<double> mtxa(sqr(msize), 1.0), mtxb(sqr(msize), 1.0);
    vector<double> res = multm(mtxa, mtxb, msize);

    if (ECHO) print_array(res, msize);

    return 0;
}
