// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <cmath>
#define ll long long
#define USE_BUCKETSORT true

const double EPS = 1e-9;

using namespace std;

namespace sorting
{
    namespace quickSortInternal
    {
        template <typename T>
        void quickSort(vector<T> &vec, size_t l, size_t r)
        {
            if (l >= r)
                return;
            size_t p = (l + r) / 2;
            swap(vec[l], vec[p]);
            T k = vec[l];
            size_t i = l + 1;
            size_t j = r;
            while (i <= j)
            {
                while (i <= r and vec[i] <= k)
                    i++;
                while (l <= j and vec[j] > k)
                    j--;
                if (i < j)
                    swap(vec[i], vec[j]);
            }
            swap(vec[l], vec[j]);
            if (j > 0)
                quickSort(vec, l, j - 1);
            quickSort(vec, j + 1, r);
        }
    }

    template <typename T>
    void quickSort(vector<T> &vec)
    {
        if (vec.size() == 0)
            return;
        quickSortInternal::quickSort(vec, 0, vec.size() - 1);
    }

    template <typename T>
    void insertionSort(vector<T> &vec)
    {
        for (size_t i = 1; i < vec.size(); i++)
        {
            T val = vec[i];
            ll j = i - 1;

            while (j >= 0 and vec[j] > val)
            {
                vec[j + 1] = vec[j];
                j--;
            }
            vec[j + 1] = val;
        }
    }

    template <typename T>
    void bucketSort(vector<T> &vec)
    {
        size_t n = vec.size();
        vector<vector<T>> B(n + 1);
        for (size_t i = 0; i < n; i++)
            B[size_t(n * vec[i])].push_back(vec[i]);
        for (size_t i = 0; i <= n; i++)
            insertionSort(B[i]);
        size_t ind = 0;
        for (size_t i = 0; i <= n; i++)
            for (size_t j = 0; j < B[i].size(); j++)
            {
                vec[ind] = B[i][j];
                ind++;
            }
    }
}

int main(void)
{
    ll a, c, m, seed;
    size_t n;
    cin >> a >> c >> m >> seed >> n;

    vector<ll> X(n);
    X[0] = (a * seed + c) % m;
    for (size_t i = 1; i < n; i++)
        X[i] = (a * X[i - 1] + c) % m;

    vector<double> Y(n);
    for (size_t i = 0; i < n; i++)
        Y[i] = abs(2.0 * double(X[i]) / m - 1.0);

    // The code was sent to CodeForces using bucket sort (199384647) and using quick sort (199384572).
    // The bucket sort has maximal execution time of 7128 ms and the quick sort -- 3166 ms.
    // It is obvious that quick sort is indeed quick and ran twice as fast.
    if (USE_BUCKETSORT)
        sorting::bucketSort(Y);
    else
        sorting::quickSort(Y);

    size_t j = n / 2;
    double value = Y[j];
    for (size_t i = 0; i < n; i++)
        if (abs(abs(2.0 * double(X[i]) / m - 1.0) - value) < EPS)
        {
            cout << i + 1 << endl;
            return 0;
        }

    return 0;
}