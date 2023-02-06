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
    friend auto operator<<(std::ostream& os, Entry<K, V> const& entry) -> std::ostream& { 
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
        for (auto &elem: vec[key_index])
        {
            if (elem.get_key() == key)
                return elem;
        }
        return Entry<K, V>();
    }
    Entry<K, V> put(K key, V value)
    {
        int key_index = hash_function(key);
        for (auto &elem: vec[key_index])
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
};

int main()
{
    int n;
    cin >> n;
    Map<string, int> freq;
    for (int i = 0; i < n; i++)
    {
        string word;
        cin >> word;
        freq.put(word, 1);
    }
    int m, k = 0;
    cin >> m;
    vector<string> order_of_words(m);
    for (int i = 0; i < m; i++)
    {
        string word;
        cin >> word;
        if (freq.get(word).get_value() == 0)
        {
            freq.put(word, 1);
            order_of_words[k] = word;
            k++;
        }
    }
    cout << k << endl;
    for (int i = 0; i < k; i++)
        cout << order_of_words[i] << '\n';

    return 0;
}
