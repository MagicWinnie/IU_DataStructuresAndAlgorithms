#include <iostream>

using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node *left;
    Node *right;

    Node() : data(), left(nullptr), right(nullptr) {}
    Node(T _data) : data(_data), left(nullptr), right(nullptr) {}
    Node(T _data, Node *_left, Node *_right) : data(_data), left(_left), right(_right) {}
    friend ostream &operator<<(std::ostream &os, Node const &node)
    {
        os << node.data;
        return os;
    }
};

template <typename T>
class BinarySearchTree : public Node<T>
{
private:
    Node<T> *tree;
    size_t size;
    Node<T> *searchWorker(T data, Node<T> *subtree, bool exitOnFirstFound)
    {
        if (subtree == nullptr)
            return subtree;
        else if (data == subtree->data and exitOnFirstFound)
            return subtree;
        else if (data < subtree->data and subtree->left != nullptr)
            return searchWorker(data, subtree->left, exitOnFirstFound);
        else if (data >= subtree->data and subtree->right != nullptr)
            return searchWorker(data, subtree->right, exitOnFirstFound);
        else
            return subtree;
    }
    void infixWorker(Node<T> *subtree, void (*ptrf)(Node<T> *node))
    {
        if (subtree == nullptr)
            return;
        infixWorker(subtree->left, ptrf);
        ptrf(subtree);
        infixWorker(subtree->right, ptrf);
    }

public:
    BinarySearchTree() : tree(nullptr), size() {}
    Node<T> *search(T data, bool exitOnFirstFound) { return searchWorker(data, tree, exitOnFirstFound); }
    Node<T> *getRoot() { return tree; }
    size_t getSize() { return size; }
    void infix(void (*ptrf)(Node<T> *node)) { infixWorker(tree, ptrf); }
    void add(T data)
    {
        if (tree == nullptr)
        {
            tree = new Node<T>(data);
        }
        else
        {
            Node<T> *subtree = searchWorker(data, tree, false);
            if (data < subtree->data)
                subtree->left = new Node(data);
            else
                subtree->right = new Node(data);
        }
        size++;
    }
};

int global_index = 1;

class Pair
{
public:
    int index;
    int value;
    Pair() : index(), value() {}
    Pair(int _index, int _value) : index(_index), value(_value) {}
    bool operator==(const Pair &p2) { return value == p2.value; }
    bool operator>=(const Pair &p2) { return value >= p2.value; }
    bool operator<(const Pair &p2) { return value < p2.value; }
};

void putIndicesInfix(Node<Pair> *subtree)
{
    subtree->data.index = global_index;
    global_index++;
}

void printInfix(Node<Pair> *subtree)
{
    cout << subtree->data.index << ' ';
    if (subtree->left == nullptr)
        cout << -1 << ' ';
    else
        cout << subtree->left->data.index << ' ';
    if (subtree->right == nullptr)
        cout << -1 << '\n';
    else
        cout << subtree->right->data.index << '\n';
}

int main(void)
{
    BinarySearchTree<Pair> bst;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int tmp;
        cin >> tmp;
        Pair p(i + 1, tmp);
        bst.add(p);
    }
    bst.infix(&putIndicesInfix);
    cout << bst.getSize() << '\n';
    bst.infix(&printInfix);
    cout << bst.getRoot()->data.index << endl;

    return 0;
}