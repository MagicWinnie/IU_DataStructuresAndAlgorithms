// Dmitriy Okoneshnikov
#include <iostream>
#include <vector>
#include <cmath>

const double EPS = 1e-9;

// Task 2.1.2
// Answer:
// The problem of finding an intersection of two line segments is important because it is a fundamental geometric operation.
// It is used, for example, in computer graphics and robotics.
// In computer graphics, it is used to, e.g, detect collisions between objects or draw lines on the screen.
// In robotics, it is used for path planning and collision avoidance.

// Task 2.1.3
// Answer:
// The idea of Sweep Line algorithms is to "move" a vertical line through the xy plane from left to right and do the necessary operations while moving the line.
// In this task we need to sort the line segments based on their x coordinates and traverse through the line segments.
// While traversing we check if the relative position of lines has changed (using a tree), if so then there is an intersection.
// Looked up at https://cp-algorithms.com/geometry/intersecting_segments.html

// Task 2.1.4
// Merge-Sort
// * Worst time complexity: O(n*log(n))
// * Out-of-place
// * Stable
namespace MergeSort
{
    namespace internal
    {
        template <typename T>
        void merge(std::vector<T> &vec, size_t left, size_t mid, size_t right)
        {
            size_t leftLength = mid - left + 1;
            size_t rightLength = right - mid;

            std::vector<T> leftVec(leftLength);
            std::vector<T> rightVec(rightLength);

            for (size_t i = 0; i < leftLength; i++)
                leftVec[i] = vec[left + i];
            for (size_t i = 0; i < rightLength; i++)
                rightVec[i] = vec[mid + 1 + i];

            size_t i = 0, j = 0, k = left;

            while (i < leftLength and j < rightLength)
            {
                if (leftVec[i] < rightVec[j])
                {
                    vec[k] = leftVec[i];
                    i++;
                }
                else
                {
                    vec[k] = rightVec[j];
                    j++;
                }
                k++;
            }

            while (i < leftLength)
            {
                vec[k] = leftVec[i];
                i++;
                k++;
            }
            while (j < rightLength)
            {
                vec[k] = rightVec[j];
                j++;
                k++;
            }
        }
        template <typename T>
        void sort(std::vector<T> &vec, size_t left, size_t right)
        {
            if (left >= right)
                return;
            size_t mid = (left + right) / 2;
            sort(vec, left, mid);
            sort(vec, mid + 1, right);
            merge(vec, left, mid, right);
        }
    }
    template <typename T>
    void sort(std::vector<T> &vec)
    {
        if (vec.size() == 0)
            return;
        internal::sort(vec, 0, vec.size() - 1);
    }
}

// Task 2.1.5
// Red-Black tree
// Implemented by using Cormen, Chapter 13
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

        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(1)
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
        // Worst-time complexity O(1)
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
        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(1)
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
        // Worst-time complexity O(log(n))
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

                    if (w->left->color == BLACK and w->right->color == BLACK)
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
        // Worst-time complexity O(log(n))
        Node<T> *minimum(Node<T> *tree) const
        {
            while (tree->left != NIL)
                tree = tree->left;
            return tree;
        }
        // Worst-time complexity O(log(n))
        Node<T> *maximum(Node<T> *tree) const
        {
            while (tree->right != NIL)
                tree = tree->right;
            return tree;
        }
        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(n)
        void inOrderWorker(std::ostream &out, Node<T> *tree) const
        {
            if (tree == NIL)
                return;
            inOrderWorker(out, tree->left);
            out << tree->data << ' ';
            inOrderWorker(out, tree->right);
        }

    public:
        RBTree()
        {
            NIL = new Node<T>();
            root = NIL;
        }
        // Worst-time complexity O(1)
        Node<T> *getRoot() const { return root; }
        // Worst-time complexity O(log(n))
        Node<T> *find(T data) const { return findWorker(root, data); }
        // Worst-time complexity O(log(n))
        void insert(T data) { root = insertWorker(root, data); }
        // Worst-time complexity O(log(n))
        void erase(T data) { root = eraseWorker(root, data); }
        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(log(n))
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
        // Worst-time complexity O(n)
        void inOrder(std::ostream &out) const
        {
            inOrderWorker(out, root);
        }
        // Worst-time complexity O(n)
        friend std::ostream &operator<<(std::ostream &out, const RBTree<T> &rb)
        {
            rb.inOrder(out);
            return out;
        }
    };
}

namespace Point
{
    // Data structure to store a point:
    template <typename T>
    class Point
    {
    protected:
        T x;
        T y;

    public:
        Point() : x(), y() {}
        Point(T _x, T _y) : x(_x), y(_y) {}
        T getX() const { return x; }
        T getY() const { return y; }
        bool operator==(const Point<T> &p) const
        {
            return abs(x - p.getX()) < EPS and abs(y - p.getY()) < EPS;
        }
        operator Point<double>() const
        {
            return Point<double>(double(x), double(y));
        }
        friend std::ostream &operator<<(std::ostream &out, const Point<T> &p)
        {
            out << "Point(" << p.x << ", " << p.y << ')';
            return out;
        }
    };

    // Data structure to store left and right ends of LineSegment
    template <typename T>
    class LineSegment
    {
    private:
        Point<T> left;  // end of segment with minimal x coordinate
        Point<T> right; // end of segment with maximal x coordinate
        size_t id;      // unique id of line segment
        bool swapped;   // if left end point is the right end point
        double getY(double x) const
        {
            double denominator = right.getX() - left.getX();
            if (abs(denominator) < EPS)
                return right.getY();
            return left.getY() + (right.getY() - left.getY()) * (x - left.getX()) / denominator;
        }

    public:
        LineSegment() : left(), right(), id(), swapped(false) {}
        LineSegment(Point<T> _left, Point<T> _right, size_t _id, bool _swapped) : left(_left), right(_right), id(_id), swapped(_swapped) {}
        void setLeft(Point<T> _left) { left = _left; }
        void setRight(Point<T> _right) { right = _right; }
        Point<T> getLeft() const { return left; }
        Point<T> getRight() const { return right; }
        size_t getID() const { return id; }
        bool isSwapped() const { return swapped; }
        bool operator<(const LineSegment<T> &l) const
        {
            double x = std::max(std::min(left.getX(), right.getX()),
                                std::min(l.getLeft().getX(), l.getRight().getX()));
            return getY(x) < l.getY(x);
        }
        bool operator==(const LineSegment<T> &l) const
        {
            return id == l.getID();
        }
        friend std::ostream &operator<<(std::ostream &out, const LineSegment<T> &s)
        {
            Point<T> l = s.getLeft();
            Point<T> r = s.getRight();
            if (!s.isSwapped())
                out << l.getX() << ' ' << l.getY() << ' ' << r.getX() << ' ' << r.getY();
            else
                out << r.getX() << ' ' << r.getY() << ' ' << l.getX() << ' ' << l.getY();
            return out;
        }
    };

    template <typename T>
    class Event
    {
    private:
        Point<T> pt;
        size_t id;
        bool left;

    public:
        Event() : id(), left() {}
        Event(Point<T> _pt, size_t _id, bool _left) : pt(_pt), id(_id), left(_left) {}
        size_t getID() const { return id; }
        bool isLeft() const { return left; }
        bool operator<(const Event<T> &e) const
        {
            return abs(pt.getX() - e.pt.getX()) < EPS
                       ? left > e.left
                       : pt.getX() < e.pt.getX();
        }
        friend std::ostream &operator<<(std::ostream &out, const Event<T> &e)
        {
            out << "Event(" << e.pt << ", " << e.id << ", " << e.left << ')';
            return out;
        }
    };
}

namespace Geometry
{
    template <typename T>
    int orientation(Point::Point<T> a, Point::Point<T> b, Point::Point<T> c)
    {
        T val = (b.getY() - a.getY()) * (c.getX() - b.getX()) -
                (b.getX() - a.getX()) * (c.getY() - b.getY());
        return abs(val) < EPS ? 0 : (val > 0 ? 1 : -1); // 1: clockwise, -1: counterclockwise, 0: collinear
    }

    template <typename T>
    bool intersect1d(T left1, T right1, T left2, T right2)
    {
        if (left1 > right1)
            std::swap(left1, right1);
        if (left2 > right2)
            std::swap(left2, right2);
        return std::max(left1, left2) <= std::min(right1, right2) + EPS;
    }

    template <typename T>
    bool doIntersect(Point::LineSegment<T> a, Point::LineSegment<T> b)
    {
        return intersect1d(a.getLeft().getX(), a.getRight().getX(), b.getLeft().getX(), b.getRight().getX()) and
               intersect1d(a.getLeft().getY(), a.getRight().getY(), b.getLeft().getY(), b.getRight().getY()) and
               orientation(a.getLeft(), a.getRight(), b.getLeft()) * orientation(a.getLeft(), a.getRight(), b.getRight()) < EPS and
               orientation(b.getLeft(), b.getRight(), a.getLeft()) * orientation(b.getLeft(), b.getRight(), a.getRight()) < EPS;
    }

    // Task 2.1.1
    // Returns true if two line segments intersect
    // Inspired by https://e-maxx.ru/algo/segments_intersection_checking
    template <typename T>
    Point::Point<double> getIntersection(Point::LineSegment<T> a, Point::LineSegment<T> b)
    {
        // if this check is passed then the denominator != 0
        if (!doIntersect(a, b))
            throw std::runtime_error("Line segments do not intersect!");

        double denominator = (a.getLeft().getX() - a.getRight().getX()) * (b.getLeft().getY() - b.getRight().getY()) -
                             (a.getLeft().getY() - a.getRight().getY()) * (b.getLeft().getX() - b.getRight().getX());

        double x_nominator = (a.getLeft().getX() * a.getRight().getY() - a.getLeft().getY() * a.getRight().getX()) * (b.getLeft().getX() - b.getRight().getX()) -
                             (a.getLeft().getX() - a.getRight().getX()) * (b.getLeft().getX() * b.getRight().getY() - b.getLeft().getY() * b.getRight().getX());
        double y_nominator = (a.getLeft().getX() * a.getRight().getY() - a.getLeft().getY() * a.getRight().getX()) * (b.getLeft().getY() - b.getRight().getY()) -
                             (a.getLeft().getY() - a.getRight().getY()) * (b.getLeft().getX() * b.getRight().getY() - b.getLeft().getY() * b.getRight().getX());

        double x = x_nominator / denominator;
        double y = y_nominator / denominator;

        return Point::Point<double>(x, y);
    }
}

using namespace std;

// Task 2.1.6
// Solves the problem using Sweep Line algorithm
// Returns the ids of leftmost intersecting line segments in vec
// The solution is inspired by https://cp-algorithms.com/geometry/intersecting_segments.html
pair<size_t, size_t> solve(vector<Point::LineSegment<int>> &lineSegments)
{
    vector<Point::Event<int>> events(2 * lineSegments.size());
    for (size_t i = 0; i < lineSegments.size(); i++)
    {
        events[i] = Point::Event<int>(lineSegments[i].getLeft(), i + 1, true);
        events[lineSegments.size() + i] = Point::Event<int>(lineSegments[i].getRight(), i + 1, false);
    }
    MergeSort::sort(events);

    RBTree::RBTree<Point::LineSegment<int>> tree;
    for (auto event : events)
    {
        RBTree::Node<Point::LineSegment<int>> *pre, *suc;
        Point::LineSegment<int> currSegment = lineSegments[event.getID() - 1];
        if (event.isLeft())
        {
            tree.insert(currSegment);
            suc = tree.successor(currSegment);
            pre = tree.predecessor(currSegment);
            if (suc != nullptr and Geometry::doIntersect(currSegment, suc->data))
                return make_pair(event.getID(), suc->data.getID());
            if (pre != nullptr and Geometry::doIntersect(currSegment, pre->data))
                return make_pair(event.getID(), pre->data.getID());
        }
        else
        {
            suc = tree.successor(currSegment);
            pre = tree.predecessor(currSegment);
            if (pre != nullptr and suc != nullptr and Geometry::doIntersect(pre->data, suc->data))
                return make_pair(pre->data.getID(), suc->data.getID());
            tree.erase(currSegment);
        }
    }
    return make_pair(0, 0);
}

int main(void)
{
    // The code was not tested on custom tests, only CodeForces was used

    // Accelerate input stream by turning off synchronization
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // Input
    size_t n;
    cin >> n;
    vector<Point::LineSegment<int>> lineSegments(n);
    for (size_t i = 0; i < n; i++)
    {
        int x_p, y_p, x_q, y_q;
        cin >> x_p >> y_p >> x_q >> y_q;
        Point::LineSegment<int> lineSegment;
        if (x_p <= x_q)
            lineSegment = Point::LineSegment<int>(Point::Point<int>(x_p, y_p), Point::Point<int>(x_q, y_q), i + 1, false);
        else
            lineSegment = Point::LineSegment<int>(Point::Point<int>(x_q, y_q), Point::Point<int>(x_p, y_p), i + 1, true);
        lineSegments[i] = lineSegment;
    }

    // Solution
    auto [id1, id2] = solve(lineSegments);

    // Output
    if (id1 != 0 and id2 != 0)
    {
        cout << "INTERSECTION" << '\n'
             << lineSegments[id1 - 1] << '\n'
             << lineSegments[id2 - 1] << '\n';
    }
    else
        cout << "NO INTERSECTIONS" << '\n';

    return 0;
}