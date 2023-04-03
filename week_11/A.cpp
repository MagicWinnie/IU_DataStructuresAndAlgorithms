// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Graph
{
private:
    size_t size;
    vector<vector<T>> adj;
    vector<bool> visited;
    void DFS_worker(size_t v)
    {
        visited[v] = true;
        for (size_t u = 0; u < size; u++)
            if (adj[v][u])
                if (!visited[u])
                    DFS_worker(u);
    }

public:
    Graph(size_t n)
    {
        size = n;
        adj.resize(size, vector<T>(size, T()));
        visited.resize(size, false);
    }
    void DFS(size_t v)
    {
        for (size_t i = 0; i < size; i++)
            visited[i] = false;
        DFS_worker(v);
    }
    bool isEveryNodeReachable()
    {
        for (size_t i = 0; i < size; i++)
        {
            DFS(i);
            for (size_t j = 0; j < visited.size(); j++)
                if (!visited[j])
                    return false;
            cout << endl;
        }
        return true;
    }
    friend istream &operator>>(istream &in, Graph<T> &g)
    {
        for (size_t i = 0; i < g.size; i++)
            for (size_t j = 0; j < g.size; j++)
                in >> g.adj[i][j];
        return in;
    }
    friend ostream &operator<<(ostream &out, Graph<T> &g)
    {
        for (size_t i = 0; i < g.size; i++)
        {
            for (size_t j = 0; j < g.size; j++)
            {
                out << g.adj[i][j];
                if (j != g.size - 1)
                    cout << ' ';
            }
            if (i != g.size - 1)
                out << '\n';
        }
        return out;
    }
};

int main(void)
{
    int n;
    cin >> n;
    Graph<int> g(n);
    cin >> g;
    cout << (g.isEveryNodeReachable() ? "YES" : "NO") << endl;
    return 0;
}