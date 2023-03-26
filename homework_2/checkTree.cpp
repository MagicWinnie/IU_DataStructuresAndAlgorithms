#include <iostream>
#include <cmath>
#include <string>

#include <cstdio>

namespace RBTree
{
    enum Color
    {
        BLACK,
        RED
    };
    template <typename T>
    class Node
    {
    public:
        Node<T> *parent;
        Node<T> *left;
        Node<T> *right;
        T data;
        Color color;

        Node() : parent(nullptr), left(nullptr), right(nullptr), data(), color() {}
        Node(Node<T> *_parent, Node<T> *_left, Node<T> *_right, T _data, Color _color) : parent(_parent), left(_left), right(_right), data(_data), color(_color) {}
    };
    template <typename T>
    class RBTree
    {
    private:
        Node<T> *root; // root node
        Node<T> *NIL;  // leaf node

        Node<T> *findWorker(Node<T> *tree, T data) const
        {
            if (tree == NIL)
                return nullptr;
            if (data == tree->data)
                return tree;
            else if (data < tree->data)
                return findWorker(tree->left, data);
            else
                return findWorker(tree->right, data);
        }
        Node<T> *leftRotate(Node<T> *tree, Node<T> *x)
        {
            Node<T> *y = x->right;
            x->right = y->left;
            if (y->left != NIL)
                y->left->parent = x;
            y->parent = x->parent;
            if (x->parent == nullptr)
                tree = y;
            else if (x == x->parent->left)
                x->parent->left = y;
            else
                x->parent->right = y;
            y->left = x;
            x->parent = y;
            return tree;
        }
        Node<T> *rightRotate(Node<T> *tree, Node<T> *x)
        {
            Node<T> *y = x->left;
            x->left = y->right;
            if (y->right != NIL)
                y->right->parent = x;
            y->parent = x->parent;
            if (x->parent == nullptr)
                tree = y;
            else if (x == x->parent->right)
                x->parent->right = y;
            else
                x->parent->left = y;
            y->right = x;
            x->parent = y;
            return tree;
        }
        Node<T> *fixInsert(Node<T> *tree, Node<T> *z)
        {
            Node<T> *y;
            while (z->parent->color == RED)
            {
                if (z->parent == z->parent->parent->left)
                {
                    y = z->parent->parent->right;

                    if (y->color == RED)
                    {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->right)
                        {
                            z = z->parent;
                            tree = leftRotate(tree, z);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        tree = rightRotate(tree, z->parent->parent);
                    }
                }
                else
                {
                    y = z->parent->parent->left;
                    if (y->color == RED)
                    {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->left)
                        {
                            z = z->parent;
                            tree = rightRotate(tree, z);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        tree = leftRotate(tree, z->parent->parent);
                    }
                }
                if (z == tree)
                    break;
            }
            tree->color = BLACK;
            return tree;
        }
        Node<T> *insertWorker(Node<T> *tree, T data)
        {
            Node<T> *z = new Node<T>(nullptr, NIL, NIL, data, RED);

            Node<T> *x = tree;
            Node<T> *y = nullptr;

            while (x != NIL)
            {
                y = x;
                if (z->data < x->data)
                    x = x->left;
                else
                    x = x->right;
            }

            z->parent = y;
            if (y == nullptr)
                tree = z;
            else if (z->data < y->data)
                y->left = z;
            else
                y->right = z;

            if (z->parent == nullptr)
            {
                z->color = BLACK;
                return tree;
            }
            if (z->parent->parent == nullptr)
                return tree;

            tree = fixInsert(tree, z);

            return tree;
        }
        Node<T> *RBTransplant(Node<T> *tree, Node<T> *u, Node<T> *v)
        {
            if (u->parent == nullptr)
                tree = v;
            else if (u == u->parent->left)
                u->parent->left = v;
            else
                u->parent->right = v;
            v->parent = u->parent;
            return tree;
        }
        Node<T> *fixDelete(Node<T> *tree, Node<T> *x)
        {
            Node<T> *w;
            while (x != tree and x->color == BLACK)
            {
                if (x == x->parent->left)
                {
                    w = x->parent->right;
                    if (w->color == RED)
                    {
                        w->color = BLACK;
                        x->parent->color = RED;
                        tree = leftRotate(tree, x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->color == BLACK and w->right->color == BLACK)
                    {
                        w->color = RED;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->right->color == BLACK)
                        {
                            w->left->color = BLACK;
                            w->color = RED;
                            tree = rightRotate(tree, w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->right->color = BLACK;
                        tree = leftRotate(tree, x->parent);
                        x = tree;
                    }
                }
                else
                {
                    w = x->parent->left;
                    if (w->color == RED)
                    {
                        w->color = BLACK;
                        x->parent->color = RED;
                        tree = rightRotate(tree, x->parent);
                        w = x->parent->left;
                    }

                    if (w->right->color == BLACK and w->left->color == BLACK)
                    {
                        w->color = RED;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->left->color == BLACK)
                        {
                            w->right->color = BLACK;
                            w->color = RED;
                            tree = leftRotate(tree, w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->left->color = BLACK;
                        tree = rightRotate(tree, x->parent);
                        x = tree;
                    }
                }
            }
            x->color = BLACK;
            return tree;
        }
        Node<T> *minimum(Node<T> *tree) const
        {
            while (tree->left != NIL)
                tree = tree->left;
            return tree;
        }
        Node<T> *maximum(Node<T> *tree) const
        {
            while (tree->right != NIL)
                tree = tree->right;
            return tree;
        }
        Node<T> *eraseWorker(Node<T> *tree, T data)
        {
            Node<T> *x, *y;
            Node<T> *z = findWorker(tree, data);
            if (z == NIL)
                return tree;

            y = z;
            Color y_original_color = y->color;
            if (z->left == NIL)
            {
                x = z->right;
                tree = RBTransplant(tree, z, z->right);
            }
            else if (z->right == NIL)
            {
                x = z->left;
                tree = RBTransplant(tree, z, z->left);
            }
            else
            {
                y = minimum(z->right);
                y_original_color = y->color;
                x = y->right;
                if (y != z->right)
                {
                    tree = RBTransplant(tree, y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                else
                    x->parent = y;
                tree = RBTransplant(tree, z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            delete z;
            if (y_original_color == BLACK)
                tree = fixDelete(tree, x);
            return tree;
        }
        void inOrderWorker(std::ostream &out, Node<T> *tree) const
        {
            if (tree == NIL)
                return;
            inOrderWorker(out, tree->left);
            out << tree->data << ' ';
            inOrderWorker(out, tree->right);
        }
        void printWorker(std::ostream &out, Node<T> *tree, std::string indent, bool last) const
        {
            if (tree != NIL)
            {
                out << indent;
                if (last)
                {
                    out << "R____";
                    indent += "     ";
                }
                else
                {
                    out << "L____";
                    indent += "|    ";
                }
                out << tree->data << "(" << (tree->color ? "RED" : "BLACK") << ")" << '\n';
                printWorker(out, tree->left, indent, false);
                printWorker(out, tree->right, indent, true);
            }
        }

    public:
        RBTree()
        {
            NIL = new Node<T>();
            root = NIL;
        }
        Node<T> *getRoot() const { return root; }
        Node<T> *find(T data) const { return findWorker(root, data); }
        void insert(T data) { root = insertWorker(root, data); }
        void erase(T data) { root = eraseWorker(root, data); }
        Node<T> *predecessor(T data)
        {
            bool existed = false;
            Node<T> *x = find(data);

            if (x == nullptr)
            {
                insert(data);
                x = find(data);
            }
            else
                existed = true;
            if (x->left != NIL)
            {
                Node<T> *tmp = maximum(x->left);
                if (!existed)
                    erase(data);
                return tmp;
            }
            Node<T> *y = x->parent;
            while (y != nullptr and x == y->left)
            {
                x = y;
                y = y->parent;
            }
            if (!existed)
                erase(data);
            return y;
        }
        Node<T> *successor(T data)
        {
            bool existed = false;
            Node<T> *x = find(data);
            if (x == nullptr)
            {
                insert(data);
                x = find(data);
            }
            else
                existed = true;
            if (x->right != NIL)
            {
                Node<T> *tmp = minimum(x->right);
                if (!existed)
                    erase(data);
                return tmp;
            }
            Node<T> *y = x->parent;
            while (y != nullptr and x == y->right)
            {
                x = y;
                y = y->parent;
            }
            if (!existed)
                erase(data);
            return y;
        }
        void inOrder(std::ostream &out) const
        {
            inOrderWorker(out, root);
        }
        friend std::ostream &operator<<(std::ostream &out, const RBTree<T> &rb)
        {
            rb.inOrder(out);
            return out;
        }
        void print() const
        {
            printWorker(std::cout, root, "", true);
        }
    };
}

using namespace std;

int main(void)
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // freopen("error.txt", "r", stdin);
    RBTree::RBTree<int> tree;
    string s;
    int x;
    int i = 1;
    while (cin >> s >> x)
    {
        // cout << "----------------------------" << '\n';
        // cout << "LINE: " << i << endl;
        if (s == "insert" and tree.find(x) == nullptr)
            tree.insert(x);
        else if (s == "delete" and tree.find(x) != nullptr)
            tree.erase(x);
        else if (s == "exists")
            cout << (tree.find(x) == nullptr ? "false" : "true") << '\n';
        else if (s == "next")
        {
            RBTree::Node<int> *tmp = tree.successor(x);
            if (tmp == nullptr)
                cout << "none" << '\n';
            else
                cout << tmp->data << '\n';
        }
        else if (s == "prev")
        {
            RBTree::Node<int> *tmp = tree.predecessor(x);
            if (tmp == nullptr)
                cout << "none" << '\n';
            else
                cout << tmp->data << '\n';
        }
        // tree.print();
        // cout << "----------------------------" << '\n';
        // i++;
    }
    return 0;
}