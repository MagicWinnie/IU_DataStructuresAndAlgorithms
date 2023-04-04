// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

template <typename V, typename E>
class Graph
{
private:
    size_t size;
    unordered_map<V, size_t> vertex_mapping;
    vector<vector<E>> adj_matrix;
    vector<bool> visited;
    void DFS_worker(size_t v)
    {
        visited[v] = true;
        for (size_t u = 0; u < size; u++)
            if (adj_matrix[v][u])
                if (!visited[u])
                    DFS_worker(u);
    }
    void clear_visited()
    {
        for (size_t i = 0; i < size; i++)
            visited[i] = false;
    }
    void DFS(size_t v)
    {
        clear_visited();
        DFS_worker(v);
    }

public:
    Graph(size_t n)
    {
        size = n;
        adj_matrix.resize(size, vector<E>(size, E()));
        visited.resize(size, false);
    }
    void DFS(V v)
    {
        clear_visited();
        DFS_worker(vertex_mapping[v]);
    }
    bool isEveryNodeReachable()
    {
        for (size_t i = 0; i < size; i++)
        {
            DFS(i);
            for (size_t j = 0; j < visited.size(); j++)
                if (!visited[j])
                    return false;
        }
        return true;
    }
    friend istream &operator>>(istream &in, Graph<V, E> &g)
    {
        for (size_t i = 0; i < g.size; i++)
        {
            for (size_t j = 0; j < g.size; j++)
                in >> g.adj_matrix[i][j];
            g.vertex_mapping[i] = i;
        }
        return in;
    }
    friend ostream &operator<<(ostream &out, Graph<V, E> &g)
    {
        for (size_t i = 0; i < g.size; i++)
        {
            for (size_t j = 0; j < g.size; j++)
            {
                out << g.adj_matrix[i][j];
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
    Graph<int, int> g(n); // not <int, bool> because we can't read into bool vector due to the features of the language
    cin >> g;
    cout << (g.isEveryNodeReachable() ? "YES" : "NO") << endl;
    return 0;
}