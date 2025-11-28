#include <bits/stdc++.h>
using namespace std;

class LRUCache {
private:
    int capacity;
    list<pair<int,int>> dll;  // {key, value}
    unordered_map<int, list<pair<int,int>>::iterator> mp;

    void moveToFront(int key) {
        auto it = mp[key];
        auto kv = *it;
        dll.erase(it);
        dll.push_front(kv);
        mp[key] = dll.begin();
    }

public:
    LRUCache(int cap) : capacity(cap) {}

    int get(int key) {
        if (!mp.count(key)) return -1;
        moveToFront(key);
        return mp[key]->second;
    }

    void put(int key, int value) {
        // If key already exists → update + move to front
        if (mp.count(key)) {
            mp[key]->second = value;
            moveToFront(key);
            return;
        }

        // If cache full → remove LRU item (back of list)
        if ((int)dll.size() == capacity) {
            auto lru = dll.back();
            mp.erase(lru.first);
            dll.pop_back();
        }

        dll.push_front({key, value});
        mp[key] = dll.begin();
    }
};

int main() {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cout << cache.get(1) << endl; // 10
    cache.put(3, 30); // evicts key 2
    cout << cache.get(2) << endl; // -1
}
