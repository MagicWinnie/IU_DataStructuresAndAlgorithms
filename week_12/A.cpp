// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#define INF 1E10

using namespace std;

namespace BinaryHeap
{
    template <typename T>
    class MinHeap
    {
    private:
        unordered_map<T, int> key2index; // index starts from 1
        vector<T> vec;
        int capacity;
        int curr_size;

        int parent(int i) const { return (i == 0) ? 0 : (i - 1) / 2; }
        int leftChild(int i) const { return 2 * i + 1; }
        int rightChild(int i) const { return 2 * i + 2; }

        void _swap(int i, int j)
        {
            swap(vec[i], vec[j]);
            key2index[vec[i]] = i + 1;
            key2index[vec[j]] = j + 1;
        }
        void decrease(int i, T val)
        {
            vec[i] = val;
            key2index[val] = i + 1;
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
            int index = key2index[val];
            if (index == 0)
            {
                curr_size++;
                decrease(curr_size - 1, val);
            }
            else
            {
                decrease(index - 1, val);
            }
        }
        T pop()
        {
            if (curr_size == 0)
                throw runtime_error("Pop from empty heap!");
            T root = vec[0];
            key2index[root] = 0;
            vec[0] = vec[curr_size - 1];
            key2index[vec[0]] = 0 + 1;
            curr_size--;
            minHeapify(0);
            return root;
        }
        void printMap() const
        {
            for (auto const &p : key2index)
                cout << p.first << " : " << p.second << '\n';
        }
        friend ostream &operator<<(ostream &out, const MinHeap<T> &h)
        {
            for (auto const &el : h.vec)
                out << el << "; ";
            return out;
        }
    };
}

template <typename K, typename V>
class Vertex
{
public:
    K key;
    V val;
    Vertex() : key(), val() {}
    Vertex(K key, V val) : key(key), val(val) {}
    bool operator<(const Vertex<K, V> &v) const { return key < v.key; }
    bool operator>(const Vertex<K, V> &v) const { return key > v.key; }
    bool operator==(const Vertex<K, V> &v) const { return key == v.key; }
    friend ostream &operator<<(ostream &out, const Vertex<K, V> &v)
    {
        out << "Vertex[" << v.key << ", " << v.val << "]";
        return out;
    }
};
template <typename K, typename V>
struct hash<Vertex<K, V>>
{
    size_t operator()(const Vertex<K, V> &v) const
    {
        return hash<K>()(v.key);
    }
};

namespace Graph
{
    template <typename V, typename W>
    class Edge
    {
    private:
        V start, end;
        W val;

    public:
        Edge() : start(), end(), val() {}
        Edge(V start, V end, W val) : start(start), end(end), val(val) {}
        void setStart(V start) { this->start = start; }
        void setEnd(V end) { this->end = end; }
        void setVal(W val) { this->val = val; }
        V getStart() const { return this->start; }
        V getEnd() const { return this->end; }
        W getVal() const { return this->val; }
        bool operator==(const Edge<V, W> &e)
        {
            return start == e.getStart() and end == e.getEnd();
        }
        friend ostream &operator<<(ostream &out, const Edge<V, W> &e)
        {
            out << "Edge[" << e.getStart() << ", " << e.getVal() << ", " << e.getEnd() << "]";
            return out;
        }
    };
    template <typename V, typename W>
    class DiGraph
    {
    private:
        int size = 0;

    protected:
        unordered_map<V, int> vertex2id;
        unordered_map<int, V> id2vertex;
        unordered_map<int, vector<Edge<int, W>>> adjList;

    public:
        DiGraph() {}
        void insertVertex(V vertex)
        {
            vertex2id[vertex] = size;
            id2vertex[size] = vertex;
            size++;
        }
        virtual void insertEdge(V start, V end, W val)
        {
            adjList[vertex2id[start]].push_back(Edge<int, W>(vertex2id[start], vertex2id[end], val));
        }
        void primMST()
        {
            vector<bool> visited(size, false);
            for (int i = 0; i < size; i++)
            {
                if (visited[i])
                    continue;

                vector<W> key(size, INF);
                key[i] = 0;

                vector<int> parent(size, -1);

                BinaryHeap::MinHeap<Vertex<int, W>> pq;
                pq.push(Vertex<int, W>(i, key[i]));
                // for (Edge<int, W> const &el : adjList[i])
                //     pq.push(Vertex<int, W>(el.getEnd(), key[el.getEnd()]));

                while (pq.size() != 0)
                {
                    Vertex<int, W> u = pq.pop();
                    visited[u.key] = true;
                    for (Edge<int, W> const &e : adjList[u.key])
                    {
                        int v = e.getEnd();
                        visited[v] = true;
                        if (e.getVal() < key[v])
                        {
                            parent[v] = u.key;
                            key[v] = e.getVal();
                            pq.push(Vertex<int, W>(v, key[v]));
                        }
                    }
                }

                // for (int i = 0; i < size; i++)
                //     cout << parent[i] << ':' << i << ' ';
                // cout << '\n';
                for (int j = 0; j < size; j++)
                    if (parent[j] != -1)
                        cout << id2vertex[parent[j]] << ':' << id2vertex[j] << ' ';
            }
            cout << '\n';
        }
        friend ostream &operator<<(ostream &out, const DiGraph<V, W> &g)
        {
            for (auto const &p : g.adjList)
            {
                out << p.first << " : ";
                for (auto const &el : p.second)
                    out << el << "; ";
                out << '\n';
            }
            return out;
        }
    };
    template <typename V, typename W>
    class BiGraph : public DiGraph<V, W>
    {
    public:
        BiGraph() : DiGraph<V, W>() {}
        void insertEdge(V start, V end, W val) override
        {
            this->adjList[this->vertex2id[start]].push_back(Edge<int, W>(this->vertex2id[start], this->vertex2id[end], val));
            this->adjList[this->vertex2id[end]].push_back(Edge<int, W>(this->vertex2id[end], this->vertex2id[start], val));
        }
    };
}

int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    unordered_map<string, int> vertexSatData;
    Graph::BiGraph<string, double> graph;

    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        string cmd;
        cin >> cmd;

        if (cmd == "ADD")
        {
            string name;
            int penalty;
            cin >> name >> penalty;

            vertexSatData[name] = penalty;
            graph.insertVertex(name);
        }
        else if (cmd == "CONNECT")
        {
            string name1, name2;
            int distance;
            cin >> name1 >> name2 >> distance;

            graph.insertEdge(name1, name2, double(distance) / (vertexSatData[name1] + vertexSatData[name2]));
        }
        else if (cmd == "PRINT_MIN")
        {
            graph.primMST();
        }
    }

    // cout << graph << '\n';

    return 0;
}