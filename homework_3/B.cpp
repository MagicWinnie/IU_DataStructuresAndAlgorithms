// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <unordered_map>
#define INF 10000000

// For debugging purposes for my implementation of priority queue
#define USE_BUILTIN_PQ false
#if USE_BUILTIN_PQ
    #include <queue>
#endif

using namespace std;

namespace BinaryHeap
{
    template <typename T>
    class MinHeap
    {
    private:
        vector<T> vec;
        int capacity;
        int curr_size;

        int parent(int i) const { return (i == 0) ? 0 : (i - 1) / 2; }
        int leftChild(int i) const { return 2 * i + 1; }
        int rightChild(int i) const { return 2 * i + 2; }

        void _swap(int i, int j)
        {
            swap(vec[i], vec[j]);
        }
        void decrease(int i, T val)
        {
            vec[i] = val;
            while (i != 0 and vec[i] < vec[parent(i)])
            {
                _swap(i, parent(i));
                i = parent(i);
            }
        }
        void minHeapify(int i)
        {
            int l = leftChild(i);
            int r = rightChild(i);
            int ind = i;
            if (l < curr_size and vec[l] < vec[i])
                ind = l;
            if (r < curr_size and vec[r] < vec[ind])
                ind = r;
            if (ind != i)
            {
                _swap(i, ind);
                minHeapify(ind);
            }
        }

    public:
        MinHeap() : capacity(), curr_size() {}
        MinHeap(int capacity) : capacity(capacity), curr_size()
        {
            vec.resize(capacity);
        }
        int size() const { return curr_size; }
        T top() const { return vec.front(); }
        void push(T val)
        {
            if (capacity == 0)
            {
                capacity = 1;
                vec.resize(capacity);
            }
            if (curr_size == capacity)
            {
                capacity *= 2;
                vec.resize(capacity);
            }
            curr_size++;
            decrease(curr_size - 1, val);
        }
        T pop()
        {
            if (curr_size == 0)
                throw runtime_error("Pop from empty heap!");
            T root = vec[0];
            vec[0] = vec[curr_size - 1];
            curr_size--;
            minHeapify(0);
            return root;
        }
        friend ostream &operator<<(ostream &out, const MinHeap<T> &h)
        {
            for (auto const &el : h.vec)
                out << el << "; ";
            return out;
        }
    };
}

// Pair class for storing the distance and vertex
template <typename K, typename V>
class Pair
{
public:
    K key;
    V val;
    Pair() : key(), val() {}
    Pair(K key, V val) : key(key), val(val) {}
    bool operator<(const Pair<K, V> &v) const { return key < v.key; }
    bool operator>(const Pair<K, V> &v) const { return key > v.key; }
    bool operator==(const Pair<K, V> &v) const { return key == v.key; }
    friend ostream &operator<<(ostream &out, const Pair<K, V> &v)
    {
        out << "Pair[" << v.key << ", " << v.val << "]";
        return out;
    }
};
// Custom hash using only the first value for pair
template <typename K, typename V>
struct hash<Pair<K, V>>
{
    size_t operator()(const Pair<K, V> &v) const
    {
        return hash<K>()(v.key);
    }
};

namespace Graph
{
    template <typename T>
    class Edge
    {
    private:
        T length;
        T bandwidth;

    public:
        Edge() : length(), bandwidth() {}
        Edge(T length, T bandwidth) : length(length), bandwidth(bandwidth) {}
        T getLength() const { return this->length; }
        T getBandwidth() const { return this->bandwidth; }
        bool operator==(const Edge<T> &e)
        {
            return length == e.getLength() and bandwidth == e.getBandwidth();
        }
        friend ostream &operator<<(ostream &out, const Edge<T> &e)
        {
            out << "Edge[" << e.getLength() << ", " << e.getBandwidth() << "]";
            return out;
        }
    };
    template <typename V, typename W>
    class Graph
    {
    private:
        int size = 0; // number of vertices
        int id = 0;   // temporary variable for assigning id to a new vertex

    protected:
        unordered_map<V, int> vertex2id;   // map from V to integer ids
        unordered_map<int, V> id2vertex;   // map integer ids to V
        vector<vector<Edge<W>>> adjMatrix; // adjacency matrix

    public:
        Graph(int size, Edge<W> infEdge)
        {
            this->size = size;
            adjMatrix.resize(size, vector<Edge<W>>(size, infEdge));
        }
        void insertVertex(V vertex)
        {
            vertex2id[vertex] = id;
            id2vertex[id] = vertex;
            id++;
        }
        virtual void insertEdge(V start, V end, Edge<W> edge)
        {
            adjMatrix[vertex2id[start]][vertex2id[end]] = edge;
        }
        W Dijsktra(V start, V finish, W minBandwidth, vector<V> &path, W &foundMinBand)
        {
            vector<W> dist(size, INF);    // storing minimum distances
            vector<W> bw(size, INF);      // storing minimum bandwidth
            vector<int> parent(size, -1); // storing parents

            // Create priority queue (USE_BUILTIN_PQ == true for debugging)
            #if USE_BUILTIN_PQ
                priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
            #else
                BinaryHeap::MinHeap<Pair<W, int>> pq;
            #endif
            
            // Push start vertex (USE_BUILTIN_PQ == true for debugging)
            #if USE_BUILTIN_PQ
                pq.push(make_pair(0, vertex2id[start]));
            #else
                pq.push(Pair<W, int>(0, vertex2id[start]));
            #endif

            // Set stuff for start vertex to zero
            dist[vertex2id[start]] = 0;
            bw[vertex2id[start]] = 0;

            while (pq.size() != 0)
            {
                // Get the vertex with minimum distance (USE_BUILTIN_PQ == true for debugging)
                #if USE_BUILTIN_PQ
                    int u = pq.top().second;
                    pq.pop();
                #else
                    int u = pq.pop().val;
                #endif

                // Traverse through adjacent vertices
                for (int v = 0; v < size; v++)
                {
                    // If edge exists
                    if (adjMatrix[u][v].getLength() == INF)
                        continue;

                    W weight = adjMatrix[u][v].getLength();
                    W bandwidth = adjMatrix[u][v].getBandwidth();

                    // If bandwidth is sufficient
                    if (bandwidth < minBandwidth)
                        continue;

                    // Choose the path with minimum bandwidth if the distance is equal
                    if (dist[u] + weight == dist[v] and bandwidth < bw[v])
                    {
                        parent[v] = u;
                        bw[v] = bandwidth;
                    }
                    // Update the minimum path
                    if (dist[u] + weight < dist[v])
                    {
                        parent[v] = u;
                        dist[v] = dist[u] + weight;
                        bw[v] = bandwidth;
                        
                        // Push new vertex (USE_BUILTIN_PQ == true for debugging)
                        #if USE_BUILTIN_PQ
                            pq.push(make_pair(dist[v], v));
                        #else
                            pq.push(Pair<W, int>(dist[v], v));
                        #endif
                    }
                }
            }

            // Backtrace the path
            if (dist[vertex2id[finish]] != INF)
            {
                int v = vertex2id[finish];
                while (true)
                {
                    // Update the minimum bandwidth
                    foundMinBand = min(foundMinBand, bw[v]);
                    path.push_back(id2vertex[v]);
                    v = parent[v];
                    if (v == vertex2id[start])
                        break;
                }
                path.push_back(start);
            }

            return dist[vertex2id[finish]];
        }
        void printMap()
        {
            for (auto const &p : vertex2id)
                cout << p.first << " -> " << p.second << '\n';
        }
        friend ostream &operator<<(ostream &out, const Graph<V, W> &g)
        {
            for (int i = 0; i < g.size; i++)
            {
                for (int j = 0; j < g.size; j++)
                    out << g.adjMatrix[i][j] << ' ';
                out << '\n';
            }
            return out;
        }
    };
}

int main(void)
{
    int n, m;
    cin >> n >> m;

    // Basically, Graph class has an adjacency matrix filled
    // with Edge classes. No edge == Edge with INF values.
    Graph::Graph<int, int> graph(n, Graph::Edge<int>(INF, INF));

    // Add all the vertices
    for (int i = 1; i <= n; i++)
        graph.insertVertex(i);
    // Add all the edges
    for (int k = 0; k < m; k++)
    {
        int i_k, j_k, l_k, b_k;
        cin >> i_k >> j_k >> l_k >> b_k;
        graph.insertEdge(i_k, j_k, Graph::Edge<int>(l_k, b_k));
    }

    int start, finish, minBand;
    cin >> start >> finish >> minBand;

    vector<int> path;       // vector for the shortest path
    int foundMinBand = INF; // the minimum bandwidth in the path
    int dist = graph.Dijsktra(start, finish, minBand, path, foundMinBand);
    if (dist == INF)
    {
        cout << "IMPOSSIBLE" << '\n';
    }
    else
    {
        cout << path.size() << ' ' << dist << ' ' << foundMinBand << '\n';
        // Print the path backwards
        for (auto it = path.rbegin(); it != path.rend(); it++)
            cout << *it << ' ';
        cout << '\n';
    }

    return 0;
}
