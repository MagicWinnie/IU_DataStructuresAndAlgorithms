// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

// Template class for HashMap key-value pair entry
template <typename K, typename V>
class Entry
{
private:
    K key;
    V value;
    typename list<Entry<K, V>>::iterator iterator; // iterator to the entry in HashMap's `bucket`

public:
    // Default constructor, initialize with default values
    Entry()
    {
        this->key = K();
        this->value = V();
    }
    // Constructor, initialize with key and value.
    Entry(K key, V value)
    {
        this->key = key;
        this->value = value;
    }
    // Method that returns the key.
    // Time complexity O(1)
    K &get_key()
    {
        return key;
    }
    // Method that returns the value.
    // Time complexity O(1)
    V &get_value()
    {
        return value;
    }
    // Method that sets the value.
    // Time complexity O(1)
    void set_value(V value)
    {
        this->value = value;
    }
    // Method that sets the iterator.
    // Time complexity O(1)
    void set_iterator(typename list<Entry<K, V>>::iterator iterator)
    {
        this->iterator = iterator;
    }
    // Method that returns the iterator.
    // Time complexity O(1)
    typename list<Entry<K, V>>::iterator& get_iterator()
    {
        return iterator;
    }
    // Method that overload the cout operator, so the entry is printed
    // in the following format: <key: value>
    friend ostream &operator<<(ostream &os, Entry<K, V> &entry)
    {
        os << "<" << entry.get_key() << ": " << entry.get_value() << ">";
        return os;
    }
};

// Template class for HashMap ADT
template <typename K, typename V>
class HashMap
{
private:
    vector<list<Entry<K, V>>> vec; // vector of `buckets`
    int CAPACITY = 1; // number of `buckets` in the table
    int _size = 0; // number of elements currently in table

    // Method that returns a hash in range [0, CAPACITY)
    // for the specified key. Uses the hash function from std
    int hash_function(K key)
    {
        size_t key_hash = hash<K>()(key);
        return key_hash % CAPACITY;
    }
    // Method that rehashes the HashMap.
    // It doubles the HashMap size, and copies the elements
    // from the old HashMap to the new one using the new hash function.
    // Time complexity: average case O(1 + alpha)
    void rehash()
    {   
        CAPACITY *= 2;
        vector<list<Entry<K, V>>> tmp = vec;
        vec.clear();
        vec.resize(CAPACITY, list<Entry<K, V>>(0));
        for (list<Entry<K, V>> &lst : tmp)
            for (Entry<K, V> &entry : lst)
                put(entry.get_key(), entry.get_value());
    }

public:
    // Default constructor.
    // Resizes the vector to size CAPACITY, filling it
    // with the default value
    HashMap()
    {
        vec.resize(CAPACITY, list<Entry<K, V>>(0));
    }
    // Method that returns the number of elements in HashMap.
    // Time complexity: worst case O(1)
    int size() const
    {
        return _size;
    }
    // Method that returns true if the HashMap is empty.
    // Time complexity: worst case O(1)
    bool isEmpty() const
    {
        return _size == 0;
    }
    // Method that returns a reference to an element by key.
    // Puts a new key with default value if it
    // does not exist and returns a reference to it.
    // Time complexity: average case O(1 + alpha)
    Entry<K, V> &get(K key)
    {
        int key_index = hash_function(key);
        for (auto &elem : vec[key_index])
        {
            if (elem.get_key() == key)
                return elem;
        }
        _size++;
        return put(key, V());
    }
    // Method that puts a value into the HashMap.
    // Rehashes if needed.
    // Returns a reference to the old/new entry.
    // Time complexity: average case O(1 + alpha)
    Entry<K, V> &put(K key, V value)
    {
        if (_size / CAPACITY > 8)
            rehash();
        int key_index = hash_function(key);
        for (auto &elem : vec[key_index])
        {
            if (elem.get_key() == key)
            {
                Entry<K, V> &prev_elem = elem;
                elem.set_value(value);
                return prev_elem;
            }
        }
        vec[key_index].push_back(Entry<K, V>(key, value));
        vec[key_index].back().set_iterator(prev(vec[key_index].end()));
        _size++;
        return vec[key_index].back();
    }
    // Method that removes an entry from HashMap.
    // Time complexity: worst case O(1)
    void remove(Entry<K, V> entry)
    {
        int key_index = hash_function(entry.get_key());
        vec[key_index].erase(entry.get_iterator());
        _size--;
    }
    // Method that returns a vector of entries from HashMap.
    // Time complexity: average case O(1 + alpha)
    vector<Entry<K, V>> get_entries()
    {
        vector<Entry<K, V>> res;
        for (auto &lst : vec)
            for (auto &entry : lst)
                res.push_back(entry);
        return res;
    }
};

// Function that reads the input and parses it into a HashMap
HashMap<string, HashMap<string, double>> readInput()
{
    // The key of the outer HashMap holds the date.
    // The value of the outer HashMap is an another HashMap
    // The inner HashMap has a key "cost" that has a sum of costs
    // and other keys corresponding to the IDs with a value of 1,
    // in order to keep the unique IDs
    HashMap<string, HashMap<string, double>> map;

    int numberOfEntries;
    cin >> numberOfEntries;
    for (int i = 0; i < numberOfEntries; i++)
    {
        string date, time, id, cost, title;
        cin >> date >> time >> id >> cost;
        cin.ignore(1, ' '); // remove unwanted space in front of the title
        getline(cin, title);

        double cost_double = stod(cost.substr(1)); // the first character of the cost is `$`
        double prev_cost = map.get(date).get_value().get("cost").get_value();

        map.get(date).get_value().put("cost", prev_cost + cost_double);
        map.get(date).get_value().put(id, 1);
    }

    return map;
}

// Function that prints the content of HashMap in specified format
void printEntries(HashMap<string, HashMap<string, double>> &map)
{
    for (Entry<string, HashMap<string, double>> &outer_entry : map.get_entries())
    {
        // The number of unique IDs == the number of keys in the inner map except the key "cost"
        int unique = 0;
        for (Entry<string, double> &entry : outer_entry.get_value().get_entries())
            if (entry.get_key() != "cost")
                unique++;
        cout << outer_entry.get_key() << " $" << outer_entry.get_value().get("cost").get_value() << ' ' << unique << endl;
    }
}

int main(void)
{
    // The code is tested using only the tasks' example tests on Codeforces:
    // https://codeforces.com/group/v3tYbkCHj3/contest/427350/problem/B
    HashMap<string, HashMap<string, double>> map = readInput();
    printEntries(map);
    return 0;
}
