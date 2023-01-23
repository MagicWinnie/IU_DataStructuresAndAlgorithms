// Dmitriy Okoneshnikov
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int n, k;
    cin >> n >> k;

    if (n < k)
        k = n;

    string top_players[k];
    int top_scores[k];
    for (int i = 0; i < k; i++)
    {
        top_players[i] = "none";
        top_scores[i] = -1;
    }
    
    for (int i = 0; i < n; i++)
    {
        string player;
        int score;
        cin >> player >> score;
        if (score < top_scores[0])
            continue;
        int index = -1;
        for (int j = k - 1; j >= 0; j--)
        {
            if (score > top_scores[j])
            {
                index = j;
                break;
            }
            else if (score == top_scores[j])
            {
                continue;
            }
        }
        for (int j = 0; j < index; j++)
        {
            top_players[j] = top_players[j + 1];
            top_scores[j] = top_scores[j + 1];
        }
        if (index != -1)
        {
            top_players[index] = player;
            top_scores[index] = score;
        }
    }

    for (int i = k - 1; i >= 0; i--)
        cout << top_players[i] << ' ' << top_scores[i] << '\n';

    return 0;
}
