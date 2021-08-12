#include <bits/stdc++.h>

#define ECHO 0

using namespace std;

template <class T>
vector<vector<T>> multm(vector<vector<T>>& ma, vector<vector<T>>& mb, const int N) {
    int i, j, k;
    vector<vector<T>> mc(N, vector<T>(N, 0.0));

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            for (k=0; k<N; k++) {
                mc[i][j] += ma[i][k]*mb[k][j];
            }
        }
    }

    return mc;
}

void print_2d_array(vector<vector<double>>& arr) {
    for (auto row : arr) {
        for (auto cel : row) {
            cout << cel << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    const int msize = atoi(argv[1]);
    vector<vector<double>> mtxa(msize, vector<double>(msize, 1.0)), mtxb(msize, vector<double>(msize, 1.0));
    vector<vector<double>> res = multm(mtxa, mtxb, msize);

    if (ECHO) print_2d_array(res);

    return 0;
}
