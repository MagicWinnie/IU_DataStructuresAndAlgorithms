// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

template<typename T>
class Stack
{
    private:
    int _size;
    struct list 
    {
        T val;
        struct list* next = nullptr;
    };
    struct list* head;

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
            delete[] curr;
            curr = head;
        }
        delete[] curr;
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
    friend auto operator<<(std::ostream& os, Stack const& s) -> std::ostream& { 
        struct list *temp = s.head;
        while (temp != nullptr)
        {
            os << temp->val << " ";
            temp = temp->next;
        }
        return os;
    }
};

template<typename T>
class Queue
{
    private:
    int _size;
    struct list 
    {
        T val;
        struct list* next = nullptr;
    };
    struct list* head;
    struct list* tail;

    public:
    Queue()
    {
        _size = 0;
        head = nullptr;
        tail = nullptr;
    }
    ~Queue()
    {
        struct list *curr = head;
        while (head != nullptr)
        {
            head = head->next;
            delete[] curr;
            curr = head;
        }
        delete[] curr;
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
            tail = temp;
            _size++;
            return;
        }
        temp->val = e;
        temp->next = nullptr;
        tail->next = temp;
        tail = temp;
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
    friend auto operator<<(std::ostream& os, Queue const& q) -> std::ostream& { 
        struct list *temp = q.head;
        while (temp != nullptr)
        {
            os << temp->val << " ";
            temp = temp->next;
        }
        return os;
    }
};

int precedence(string exp)
{
    if (exp == "*" or exp == "/")
        return 2;
    else if (exp == "+" or exp == "-")
        return 1;
    else
        return 0;
}

int main()
{
    Stack<string> stack = Stack<string>();
    Queue<string> queue = Queue<string>();
    string exp;
    while (cin >> exp)
    {
        if (isdigit(exp[0]))
        {
            queue.push(exp);
        }
        else if (exp == "min" or exp == "max")
        {
            stack.push(exp);
        }
        else if (exp == ",")
        {
            while (!stack.isEmpty() and stack.peek() != "(")
            {
                queue.push(stack.pop());
            }
        }
        else if (exp == "-" or exp == "/" or exp == "*" or exp == "+")
        {
            while (!stack.isEmpty() and (stack.peek() != "(" and precedence(exp) <= precedence(stack.peek())))
            {
                queue.push(stack.pop());
            }
            stack.push(exp);
        }
        else if (exp == "(")
        {
            stack.push(exp);
        }
        else if (exp == ")")
        {
            while (stack.peek() != "(")
            {
                if (stack.isEmpty())
                {
                    cout << "Parentheses are mismatched" << endl;
                    return -1;    
                }
                queue.push(stack.pop());
            }
            if (stack.isEmpty() or stack.peek() != "(")
            {
                cout << "Parentheses are mismatched" << endl;
                return -1;
            }
            stack.pop();
            if (!stack.isEmpty() and (stack.peek() == "min" or stack.peek() == "max"))
            {
                queue.push(stack.pop());
            }
        }
    }
    while (!stack.isEmpty())
    {
        if (stack.peek() == "(")
        {
            cout << "Parentheses are mismatched" << endl;
            return -1;
        }
        queue.push(stack.pop());
    }
    Stack<int> stack_calculate = Stack<int>();
    while (!queue.isEmpty())
    {
        string elem = queue.pop();
        if (isdigit(elem[0]))
        {
            stack_calculate.push(stoi(elem));
        }
        else
        {
            int val1 = stack_calculate.pop();
            int val2 = stack_calculate.pop();
            if (elem == "+")
            {
                stack_calculate.push(val2 + val1);
            }
            else if (elem == "-")
            {
                stack_calculate.push(val2 - val1);
            }
            else if (elem == "*")
            {
                stack_calculate.push(val2 * val1);
            }
            else if (elem == "/")
            {
                stack_calculate.push(floor((double)val2 / (double)val1));
            }
            else if (elem == "min")
            {
                stack_calculate.push((val2 < val1 ? val2 : val1));
            }
            else if (elem == "max")
            {
                stack_calculate.push((val2 > val1 ? val2 : val1));
            }
        }
    }
    cout << stack_calculate.pop() << endl;
    return 0;
}