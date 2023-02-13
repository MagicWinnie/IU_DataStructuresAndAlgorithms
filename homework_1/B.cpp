// Dmitriy Okoneshnikov
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

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
    K& get_key()
    {
        return key;
    }
    V& get_value()
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
    friend auto operator<<(std::ostream &os, Entry<K, V> &entry) -> std::ostream &
    {
        os << "<" << entry.get_key() << ": " << entry.get_value() << ">";
        return os;
    }
};

template <typename K, typename V>
class HashMap
{
private:
    int CAPACITY = 10079;
    vector<list<Entry<K, V>>> vec;
    int _size = 0;

    int hash_function(K key)
    {
        size_t key_hash = hash<K>()(key);
        return key_hash % CAPACITY;
    }
    void rehash()
    {
        CAPACITY *= 2;
        vector<list<Entry<K, V>>> tmp = vec;
        vec.clear();
        vec.resize(CAPACITY, list<Entry<K, V>>(0, Entry<K, V>()));
        for (list<Entry<K, V>> &lst : vec)
            for (Entry<K, V> &entry : lst)
                put(entry.get_key(), entry.get_value());
    }

public:
    HashMap()
    {
        vec.resize(CAPACITY, list<Entry<K, V>>(0, Entry<K, V>()));
    }
    int size() const
    {
        return _size;
    }
    bool isEmpty() const
    {
        return _size == 0;
    }
    Entry<K, V>& get(K key)
    {
        int key_index = hash_function(key);
        for (auto &elem : vec[key_index])
        {
            if (elem.get_key() == key)
                return elem;
        }
        return put(key, V());
    }
    Entry<K, V>& put(K key, V value)
    {
        if (_size / CAPACITY > 0.75)
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
    HashMap<string, HashMap<string, double>> map;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string date, time, id, cost, title;
        cin >> date >> time >> id >> cost;
        cin.ignore(1, ' ');
        getline(cin, title);
        
        map.get(date).get_value().put("cost", map.get(date).get_value().get("cost").get_value() + stod(cost.substr(1)));
        map.get(date).get_value().put(id, 1);
    }

    return map;
}

void printEntries(HashMap<string, HashMap<string, double>> &map)
{
    for (Entry<string, HashMap<string, double>> &outer_entry : map.get_entries())
    {
        int unique = 0;
        for (Entry<string, double> &entry : outer_entry.get_value().get_entries())
            if (entry.get_key() != "cost")
                unique++;
        cout << outer_entry.get_key() << " $" << outer_entry.get_value().get("cost").get_value() << ' ' << unique << endl;
    }
}

int main()
{
    HashMap<string, HashMap<string, double>> map = readInput();
    printEntries(map);    
    return 0;
}
