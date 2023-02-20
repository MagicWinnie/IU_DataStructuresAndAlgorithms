// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(void)
{
    int n, k;
    cin >> n >> k;

    vector<string> words(n);
    for (int i = 0; i < n; i++)
        cin >> words[i];

    string text;
    cin >> text;

    vector<bool> dp(k + 1, false);
    dp[0] = true;
    vector<int> prev(k + 1, -1);
    for (int i = 1; i <= k; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int len = words[j].size();
            if (i - len >= 0 and text.substr(i - len, len) == words[j] and dp[i - len])
            {
                dp[i] = true;
                prev[i] = i - len;
                break;
            }
        }
    }

    string output;
    for (int i = k; i > 0; i = prev[i])
        output = text.substr(prev[i], i - prev[i]) + " " + output;
    cout << output << endl;

    return 0;
}
