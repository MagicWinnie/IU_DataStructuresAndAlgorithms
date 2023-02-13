// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

// Template for HashMap key-value pair entry
template <typename K, typename V>
class Entry
{
private:
    K key;
    V value;
    typename list<Entry<K, V>>::iterator iterator;

public:
    Entry()
    {
        this->key = K();
        this->value = V();
    }
    Entry(K key, V value)
    {
        this->key = key;
        this->value = value;
    }
    K &get_key()
    {
        return key;
    }
    V &get_value()
    {
        return value;
    }
    void set_value(V value)
    {
        this->value = value;
    }
    void set_iterator(typename list<Entry<K, V>>::iterator iterator)
    {
        this->iterator = iterator;
    }
    typename list<Entry<K, V>>::iterator& get_iterator()
    {
        return iterator;
    }
    // Method to overload the cout operator, so the entry is printed
    // in the following format: <key: value>
    friend auto operator<<(std::ostream &os, Entry<K, V> &entry) -> std::ostream &
    {
        os << "<" << entry.get_key() << ": " << entry.get_value() << ">";
        return os;
    }
};

// Template for HashMap ADT
template <typename K, typename V>
class HashMap
{
private:
    vector<list<Entry<K, V>>> vec;
    int CAPACITY = 1; // number of `buckets` in the table
    int _size = 0; // number of elements currently in table

    int hash_function(K key)
    {
        size_t key_hash = hash<K>()(key);
        return key_hash % CAPACITY;
    }
    // Method to rehash the hashmap
    // Double the table size, and copy the elements
    // from old table to a new one using new hash function
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
    HashMap()
    {
        vec.resize(CAPACITY, list<Entry<K, V>>(0));
    }
    int size() const
    {
        return _size;
    }
    bool isEmpty() const
    {
        return _size == 0;
    }
    // Method to get a reference to an element by key
    // Puts a new key with default value if it
    // does not exist and returns a reference to it
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
    // Method to put a value into the hashmap
    // Rehashes if needed
    // Returns a reference to the old/new entry
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
    void remove(Entry<K, V> entry)
    {
        int key_index = hash_function(entry.get_key());
        vec[key_index].erase(entry.get_iterator());
        _size--;
    }
    vector<Entry<K, V>> get_entries()
    {
        vector<Entry<K, V>> res;
        for (auto &lst : vec)
            for (auto &entry : lst)
                res.push_back(entry);
        return res;
    }
};

HashMap<string, HashMap<string, double>> readInput()
{
    // The key of the outer hashmap holds the date.
    // The value of the outer hashmap is an another hashmap
    // The inner hashmap has a key "cost" that has a sum of costs
    // and other keys corresponding to the IDs with a value of 1,
    // in order to keep the unique IDs
    HashMap<string, HashMap<string, double>> map;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
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
    HashMap<string, HashMap<string, double>> map = readInput();
    printEntries(map);
    return 0;
}
