// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <unordered_map>
#define INF 100000

using namespace std;

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
    class Graph
    {
    private:
        int size = 0;

    protected:
        unordered_map<V, int> vertex2id;
        unordered_map<int, V> id2vertex;
        unordered_map<int, vector<Edge<int, W>>> adjList;

    public:
        Graph() {}
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
        bool BellmanFord(V start_raw, vector<V> &cycle)
        {
            int start = vertex2id[start_raw];
            vector<W> d(size, INF);
            vector<int> p(size, -1);
            d[start] = 0;

            int changed;
            for (int i = 0; i < size; i++)
            {
                changed = -1;
                for (auto const &startVertex : adjList)
                {
                    for (auto const &edge : startVertex.second)
                    {
                        if (d[edge.getEnd()] > d[edge.getStart()] + edge.getVal())
                        {
                            d[edge.getEnd()] = d[edge.getStart()] + edge.getVal();
                            p[edge.getEnd()] = edge.getStart();
                            changed = edge.getEnd();
                        }
                    }
                }
                if (changed == -1)
                    return false;
            }

            for (int i = 0; i < size; i++)
                changed = p[changed];

            int v = changed;
            while (true)
            {
                if (v == changed and cycle.size() > 1)
                    break;
                cycle.push_back(id2vertex[v]);
                v = p[v];
            }

            return true;
        }
        friend ostream &operator<<(ostream &out, const Graph<V, W> &g)
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
    class BiGraph : public Graph<V, W>
    {
    public:
        BiGraph() : Graph<V, W>() {}
        void insertEdge(V start, V end, W val) override
        {
            this->adjList[this->vertex2id[start]].push_back(Edge<int, W>(this->vertex2id[start], this->vertex2id[end], val));
            this->adjList[this->vertex2id[end]].push_back(Edge<int, W>(this->vertex2id[end], this->vertex2id[start], val));
        }
    };
}

int main(void)
{
    Graph::Graph<int, int> graph;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        graph.insertVertex(i);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int w;
            cin >> w;
            if (w != INF)
                graph.insertEdge(i, j, w);
        }
    }

    vector<int> cycle;
    if (!graph.BellmanFord(0, cycle))
    {
        cout << "NO" << '\n';
    }
    else
    {
        cout << "YES" << '\n';
        cout << cycle.size() << '\n';
        for (auto it = cycle.rbegin(); it != cycle.rend(); it++)
            cout << (*it) + 1 << ' ';
        cout << '\n';
    }

    return 0;
}
