// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Template class for Stack ADT.
// Implemented using singly linked list
template <typename T>
class Stack
{
private:
    // Number of elements in Stack
    int _size;
    // Struct for a one node
    struct list
    {
        T val;
        struct list *next = nullptr;
    };
    struct list *head;

public:
    // Default constructor
    Stack()
    {
        _size = 0;
        head = nullptr;
    }
    // Destructor.
    // Traverse through the Stack and delete every node,
    // finally delete the head.
    // Time complexity: worst case O(n)
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
    // Method that returns the size of Stack.
    // Time complexity: worst case O(1)
    int size()
    {
        return _size;
    }
    // Method that returns true if Stack is empty.
    // Time complexity: worst case O(1)
    bool isEmpty()
    {
        return _size == 0;
    }
    // Method that adds an element to head of Stack.
    // Time complexity: worst case O(1)
    void push(T e)
    {
        struct list *temp = new struct list();
        // Special case: stack is empty
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
    // Method that removes and returns an element from head of Stack.
    // Time complexity: worst case O(1)
    T pop()
    {
        if (head == nullptr)
            return T(); // return default value of type T
        struct list *temp = head->next;
        T val = head->val;
        head = temp;
        _size--;
        return val;
    }
    // Method that returns an element from head of Stack.
    // Time complexity: worst case O(1)
    T peek()
    {
        if (head == nullptr)
            return T(); // return default value of type T
        return head->val;
    }
    // Method that overloads cout operator for Stack.
    // Prints elements in one line, separated by spaces
    friend ostream &operator<<(ostream &out, Stack const &stack)
    {
        struct list *element = stack.head;
        while (element != nullptr)
        {
            out << element->val << " ";
            element = element->next;
        }
        return out;
    }
};

// Function that returns the index of a string in an array of strings.
// If not found returns -1
int findElement(string *arrayOfDelimiters, int lengthOfArray, string element)
{
    for (int i = 0; i < lengthOfArray; i++)
    {
        if (arrayOfDelimiters[i] == element)
            return i;
    }
    return -1;
}

// Function that does the reading and processing
void solver()
{
    int numberOfDelimiters, numberOfTokenLines;
    cin >> numberOfDelimiters >> numberOfTokenLines;

    // Input delimiters
    string openingDelimiters[numberOfDelimiters], closingDelimiters[numberOfDelimiters];
    for (int i = 0; i < numberOfDelimiters; i++)
    {
        string openDelimiter, closeDelimiter;
        cin >> openDelimiter >> closeDelimiter;

        openingDelimiters[i] = openDelimiter;
        closingDelimiters[i] = closeDelimiter;
    }
    cin.ignore(1, '\n'); // remove \n to use getline after cin

    // Read line of tokens and process them
    int lineNumber = 0, columnNumber = 0;
    Stack<string> stack;
    string line, token;
    for (lineNumber = 0; lineNumber < numberOfTokenLines; lineNumber++)
    {
        // Read the whole line
        getline(cin, line);
        istringstream iss(line);

        // Traverse through each token (spaces not included)
        columnNumber = 0;
        while (iss >> token)
        {
            int type = 0; // -1: open; 1: close
            int index = 0;

            // To solve the task a stack is used. When we find an opening delimiter
            // we push onto the stack. Then when we find a closing delimiter we
            // compare if both opening and closing delimiters are of the same type.
            // If so we remove the opening delimiter. If anything goes wrong or
            // at the end stack is not empty, then the delimiters are not balanced
            index = findElement(openingDelimiters, numberOfDelimiters, token);
            if (index != -1)
            {
                type = -1;
            }
            else
            {
                index = findElement(closingDelimiters, numberOfDelimiters, token);
                if (index != -1)
                {
                    type = 1;
                }
            }
            
            if (type == -1)
            {
                stack.push(token);
            }
            else if (type == 1 and stack.isEmpty())
            {
                cout << "Error in line " << lineNumber + 1 << ", column " << columnNumber + 1 <<
                    ": unexpected closing token " << token << "." << endl;
                return;
            }
            else if (type == 1 and closingDelimiters[findElement(openingDelimiters, numberOfDelimiters, stack.peek())] != token)
            {
                cout << "Error in line " << lineNumber + 1 << ", column " << columnNumber + 1 <<
                    ": expected " << closingDelimiters[findElement(openingDelimiters, numberOfDelimiters, stack.peek())] <<
                    " but got " << token << "." << endl;
                return;
            }
            else if (type == 1)
            {
                stack.pop();
            }

            columnNumber += token.length() + 1; // + 1 for space between words
        }
    }

    if (!stack.isEmpty())
    {
        cout << "Error in line " << lineNumber << ", column " << columnNumber + 1 << ": expected " <<
            closingDelimiters[findElement(openingDelimiters, numberOfDelimiters, stack.peek())] << " but got end of input." << endl;
        return;
    }
    
    cout << "The input is properly balanced." << endl;
}

int main(void)
{
    // The code is tested using only the tasks' example tests on Codeforces:
    // https://codeforces.com/group/v3tYbkCHj3/contest/427350/problem/A
    solver();
    return 0;
}