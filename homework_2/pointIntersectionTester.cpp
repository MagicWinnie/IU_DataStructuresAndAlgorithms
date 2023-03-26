#include <iostream>
#include <cmath>
#include <random>
#define GENTESTS true

using namespace std;

const double EPS = 1e-9;

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
        template <typename V>
        double sqrDistance(const Point<V> &p) const
        {
            return pow((double(x) - double(p.getX())), 2.0) + pow(double(y) - double(p.getY()), 2.0);
        }
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
            out << "Point({" << p.x << ", " << p.y << "})";
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
            if (abs(left.getX() - right.getX()) < EPS)
                return left.getY();
            return left.getY() + (right.getY() - left.getY()) * (x - left.getX()) / (right.getX() - left.getX());
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
            return getY(x) < l.getY(x) - EPS;
        }
        friend std::ostream &operator<<(std::ostream &out, const LineSegment<T> &s)
        {
            Point<T> l = s.getLeft();
            Point<T> r = s.getRight();
            if (!s.isSwapped())
                out << "Segment(" << l << ", " << r << ")";
            else
                out << "Segment(" << r << ", " << l << ")";
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
            return (abs(pt.getX() - e.pt.getX()) <= EPS ? left > e.left : pt.getX() < e.pt.getX());
        }
        friend std::ostream &operator<<(std::ostream &out, const Event<T> &e)
        {
            out << "Event(" << e.pt << ", " << e.id << ", " << e.left << ')';
            return out;
        }
    };
}

template <typename T>
int orientation(Point::Point<T> a, Point::Point<T> b, Point::Point<T> c)
{
    T val = (b.getY() - a.getY()) * (c.getX() - b.getX()) - (b.getX() - a.getX()) * (c.getY() - b.getY());
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

// Task 2.1.1
// Returns true if two line segments intersect
// Inspired by https://e-maxx.ru/algo/segments_intersection_checking
template <typename T>
Point::Point<double> getIntersection(Point::LineSegment<T> a, Point::LineSegment<T> b)
{
    bool flag = intersect1d(a.getLeft().getX(), a.getRight().getX(), b.getLeft().getX(), b.getRight().getX()) and
                intersect1d(a.getLeft().getY(), a.getRight().getY(), b.getLeft().getY(), b.getRight().getY()) and
                orientation(a.getLeft(), a.getRight(), b.getLeft()) * orientation(a.getLeft(), a.getRight(), b.getRight()) <= EPS and
                orientation(b.getLeft(), b.getRight(), a.getLeft()) * orientation(b.getLeft(), b.getRight(), a.getRight()) <= EPS;
    if (!flag)
        return Point::Point<double>(0, 0);

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

template <typename T>
bool doIntersect(Point::LineSegment<T> a, Point::LineSegment<T> b)
{
    return intersect1d(a.getLeft().getX(), a.getRight().getX(), b.getLeft().getX(), b.getRight().getX()) and
           intersect1d(a.getLeft().getY(), a.getRight().getY(), b.getLeft().getY(), b.getRight().getY()) and
           orientation(a.getLeft(), a.getRight(), b.getLeft()) * orientation(a.getLeft(), a.getRight(), b.getRight()) <= EPS and
           orientation(b.getLeft(), b.getRight(), a.getLeft()) * orientation(b.getLeft(), b.getRight(), a.getRight()) <= EPS;
}

void testCases()
{
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> uni(-2097152, 2097152);

    while (true)
    {
        int x1 = uni(rng);
        int y1 = uni(rng);
        int x2 = uni(rng);
        int y2 = uni(rng);
        int x3 = uni(rng);
        int y3 = uni(rng);
        int x4 = uni(rng);
        int y4 = uni(rng);
        Point::LineSegment<int> l1(Point::Point<int>(x1, y1), Point::Point<int>(x2, y2), 1, false);
        Point::LineSegment<int> l2(Point::Point<int>(x3, y3), Point::Point<int>(x4, y4), 2, false);
        Point::Point<double> intersect = getIntersection(l1, l2);
        // bool a = doIntersect(l1, l2);
        // bool b = doIntersect(l1, l2, intersect);
        // if (a != b)
        // {
        //     cout << a << ' ' << b << endl;
        //     cout << l1 << '\n'
        //          << l2 << endl;
        //     return;
        // }
    }
}

int main(void)
{
    if (GENTESTS)
    {
        testCases();
    }
    else
    {
        double x1, y1, x2, y2, x3, y3, x4, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
        Point::Point<double> res;
        bool flag = doIntersect(Point::LineSegment<double>(Point::Point<double>(x1, y1), Point::Point<double>(x2, y2), 1, false), Point::LineSegment<double>(Point::Point<double>(x3, y3), Point::Point<double>(x4, y4), 2, false));
        cout << (flag ? "YES" : "NO");
    }
    return 0;
}