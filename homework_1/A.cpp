// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

template <typename T>
class Stack
{
private:
    int _size;
    struct list
    {
        T val;
        struct list *next = nullptr;
    };
    struct list *head;

public:
    Stack()
    {
        _size = 0;
        head = nullptr;
    }
    ~Stack()
    {
        struct list *curr = head;
        while (head != nullptr)
        {
            head = head->next;
            delete curr;
            curr = head;
        }
        delete curr;
    }
    int size()
    {
        return _size;
    }
    bool isEmpty()
    {
        return _size == 0;
    }
    void push(T e)
    {
        struct list *temp = new struct list();
        if (_size == 0)
        {
            temp->val = e;
            temp->next = nullptr;
            head = temp;
            _size++;
            return;
        }
        temp->val = e;
        temp->next = head;
        head = temp;
        _size++;
    }
    T pop()
    {
        struct list *temp = head->next;
        T val = head->val;
        head = temp;
        _size--;
        return val;
    }
    T peek()
    {
        return head->val;
    }
    friend auto operator<<(std::ostream &os, Stack const &s) -> std::ostream &
    {
        struct list *temp = s.head;
        while (temp != nullptr)
        {
            os << temp->val << " ";
            temp = temp->next;
        }
        return os;
    }
};

int findElement(string *arr, int n, string x)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == x)
            return i;
    }
    return -1;
}

void readInput()
{

    int n, k;
    cin >> n >> k;

    string opening[n], closing[n];
    for (int i = 0; i < n; i++)
    {
        string open, close;
        cin >> open >> close;
        opening[i] = open;
        closing[i] = close;
    }
    cin.ignore(1, '\n');
    Stack<string> stack;
    int i = 0, j = 0;
    string line, tmp;
    for (i = 0; i < k; i++)
    {
        getline(cin, line);
        istringstream iss(line);

        j = 0;
        while (iss >> tmp)
        {
            int type = 0; // -1: open; 1: close
            int index = 0;

            index = findElement(opening, n, tmp);
            if (index != -1)
            {
                type = -1;
            }
            else
            {
                index = findElement(closing, n, tmp);
                if (index != -1)
                {
                    type = 1;
                }
            }
            if (type == -1)
            {
                stack.push(tmp);
            }
            else if (type == 1 and stack.isEmpty())
            {
                cout << "Error in line " << i + 1 << ", column " << j + 1 << ": unexpected closing token " << tmp << "." << endl;
                return;
            }
            else if (type == 1 and closing[findElement(opening, n, stack.peek())] != tmp)
            {
                cout << "Error in line " << i + 1 << ", column " << j + 1 << ": expected " << closing[findElement(opening, n, stack.peek())] << " but got " << tmp << "." << endl;
                return;
            }
            else if (type == 1)
            {
                stack.pop();
            }
            j += tmp.length() + 1;
        }
    }
    if (!stack.isEmpty())
    {
        cout << "Error in line " << i << ", column " << j + 1 << ": expected " << closing[findElement(opening, n, stack.peek())] << " but got end of input." << endl;
        return;
    }
    cout << "The input is properly balanced." << endl;
}

int main(void)
{
    readInput();
    return 0;
}