// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Class for item containing:
// L -> current bid
// H -> max bid
// index -> original index
class Pair
{
public:
    int L;
    int H;
    int index;

    Pair()
    {
        this->L = 0;
        this->H = 0;
        this->index = 0;
    }

    Pair(int L, int H, int index)
    {
        this->L = L;
        this->H = H;
        this->index = index;
    }
};

// Counting sort for max bid
// Sorts the index-th digit
void counting_sort(vector<Pair> &vec, int index)
{
    const int k = 9; // max value for a digit
    const int n = vec.size();

    // save indices
    vector<vector<int>> map_indices(100001);
    for (int i = 0; i < n; i++)
        map_indices[vec[i].H].push_back(vec[i].index);

    vector<int> C(k + 1, 0), B(n);
    for (int i = 0; i < n; i++)
        C[(vec[i].H / index) % 10]++;
    for (int i = 1; i <= k; i++)
        C[i] += C[i - 1];
    for (int i = n - 1; i >= 0; i--)
    {
        B[C[(vec[i].H / index) % 10] - 1] = vec[i].H;
        C[(vec[i].H / index) % 10]--;
    }

    // reverse order as we pop in the outer counting sort from behind
    for (int i = n - 1; i >= 0; i--)
    {
        vec[i].H = B[i];
        // getting from the back as it is O(1) to remove from behind
        vec[i].index = map_indices[vec[i].H].back();
        map_indices[vec[i].H].pop_back();
    }
}

// Radix sort for max bid with indices
void radix_sort(vector<Pair> &vec)
{
    const int max_number = 100000; // maximum value of max bid
    for (int place = 1; max_number / place > 0; place *= 10)
        counting_sort(vec, place);
}

// Counting sort for the items
// Sorts the current bid using counting sort
// Sorts the max bid with indices using radix sort
void counting_sort(vector<Pair> &vec)
{
    const int k = 100;
    const int n = vec.size();

    // save max bid and indices
    vector<vector<Pair>> map_H_indices(k + 1);
    for (int i = 0; i < n; i++)
        map_H_indices[vec[i].L].push_back(Pair(0, vec[i].H, vec[i].index));

    // sort the max bid with indices
    for (int i = 0; i <= k; i++)
    {
        if (map_H_indices[i].size() > 1)
            radix_sort(map_H_indices[i]);
    }

    vector<int> C(k + 1, 0), B(n);
    for (int i = 0; i < n; i++)
        C[vec[i].L]++;
    for (int i = 1; i <= k; i++)
        C[i] += C[i - 1];
    for (int i = n - 1; i >= 0; i--)
    {
        B[C[vec[i].L] - 1] = vec[i].L;
        C[vec[i].L]--;
    }

    for (int i = 0; i < n; i++)
    {
        vec[i].L = B[i];
        // getting the last value as it is O(1) to remove from back
        vec[i].H = map_H_indices[vec[i].L].back().H;
        vec[i].index = map_H_indices[vec[i].L].back().index;
        map_H_indices[vec[i].L].pop_back();
    }
}

// Overall time complexity should be O(n)
int main(void)
{
    int n;
    cin >> n;

    vector<Pair> vec(n);
    for (int i = 0; i < n; i++)
    {
        cin >> vec[i].L >> vec[i].H;
        vec[i].index = i + 1;
    }

    if (vec.size() > 1)
        counting_sort(vec);

    for (int i = n - 1; i >= 0; i--)
        cout << vec[i].index << ' ';
    cout << endl;

    return 0;
}