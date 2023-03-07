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
    K get_key() const
    {
        return key;
    }
    V get_value() const
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
    typename list<Entry<K, V>>::iterator get_iterator()
    {
        return iterator;
    }
    friend ostream &operator<<(std::ostream &os, Entry<K, V> const &entry)
    {
        os << "<" << entry.get_key() << ": " << entry.get_value() << ">";
        return os;
    }
};

template <typename K, typename V>
class Map
{
private:
    int CAPACITY = 52127;
    vector<list<Entry<K, V>>> vec;
    list<K> keys;
    int _size = 0;

    int hash_function(K key)
    {
        size_t key_hash = hash<K>()(key);
        return key_hash % CAPACITY;
    }

public:
    Map()
    {
        vec.resize(CAPACITY, list<Entry<K, V>>(0, Entry<K, V>()));
    }
    int size()
    {
        return _size;
    }
    bool isEmpty()
    {
        return _size == 0;
    }
    Entry<K, V> get(K key)
    {
        int key_index = hash_function(key);
        for (auto &elem : vec[key_index])
        {
            if (elem.get_key() == key)
                return elem;
        }
        return Entry<K, V>();
    }
    Entry<K, V> put(K key, V value)
    {
        int key_index = hash_function(key);
        for (auto &elem : vec[key_index])
        {
            if (elem.get_key() == key)
            {
                Entry<K, V> prev_elem = elem;
                elem.set_value(value);
                return prev_elem;
            }
        }
        vec[key_index].push_back(Entry<K, V>(key, value));
        vec[key_index].back().set_iterator(prev(vec[key_index].end()));
        keys.push_back(key);
        _size++;
        return Entry<K, V>();
    }
    void remove(Entry<K, V> entry)
    {
        int key_index = hash_function(entry.get_key());
        vec[key_index].erase(entry.get_iterator());
        _size--;
    }
    list<K> get_keys()
    {
        return keys;
    }
    vector<Entry<K, V>> get_entries()
    {
        vector<Entry<K, V>> res;
        for (auto &x : keys)
            res.push_back(get(x));
        return res;
    }
};

void sort_entries(vector<Entry<string, int>> &vec)
{
    for (int i = 1; i < vec.size(); i++)
    {
        auto key = vec[i];
        int j = i - 1;
        while (j >= 0 and (vec[j].get_value() < key.get_value() || vec[j].get_value() == key.get_value() && vec[j].get_key() > key.get_key()))
        {
            vec[j + 1] = vec[j];
            j -= 1;
        }
        vec[j + 1] = key;
    }
}

int main()
{
    Map<string, int> freq;

    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string word;
        cin >> word;
        freq.put(word, freq.get(word).get_value() + 1);
    }
    vector<Entry<string, int>> entries = freq.get_entries();
    sort_entries(entries);
    for (auto &x : entries)
        cout << x.get_key() << ' ' << x.get_value() << '\n';

    return 0;
}
