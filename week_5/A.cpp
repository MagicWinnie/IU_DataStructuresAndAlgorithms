#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int n, w;
    cin >> n >> w;
    vector<int> weights(n), costs(n);
    for (int i = 0; i < n; i++)
        cin >> weights[i];
    for (int i = 0; i < n; i++)
        cin >> costs[i];

    vector<vector<int>> dp(n + 1, vector<int>(w + 1, 0));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= w; j++)
        {
            if (weights[i - 1] > j)
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weights[i - 1]] + costs[i - 1]);
        }
    }
    vector<int> indices;
    int cost = dp[n][w];
    for (int i = n; i > 0 and cost > 0; i--)
    {
        if (cost != dp[i - 1][w])
        {
            indices.push_back(i);
            cost -= costs[i - 1];
            w -= weights[i - 1];
        }
    }
    cout << indices.size() << '\n';
    for (int i = indices.size() - 1; i >= 0; i--)
        cout << indices[i] << ' ';
    cout << '\n';

    return 0;
}