#include <unordered_map>
#include <list>
#include <unistd.h>

#include "policy.hh"

class LRUCache : public Policy {
private:
  int size;
  typedef std::list<int> LI;
  typedef std::pair<Value, LI::iterator> PII;
  typedef std::unordered_map<int, PII> HIPII;
  HIPII cache;
  LI used;

  void touch(HIPII::iterator it) {
    int key = it->first;
    used.erase(it->second.second);
    used.push_front(key);
    it->second.second = used.begin();
  }
  
public:
  LRUCache(int _slab_class, int _capacity, int _storage) {
    slab_class = _slab_class;
    capacity = _capacity;
    storage_medium = _storage;
    size = 0;
  }
  Value get(int key);
  int put(int key, Value val);
};

Policy *get_lru(int slab_class, int default_size, int storage_type) {
  Policy *new_lru = new LRUCache(slab_class, default_size, storage_type);
  return new_lru;
}

Value LRUCache::get(int key) {
  int latency = get_latency();
  usleep(latency);
  auto it = cache.find(key);
  if (it == cache.end()) return Value{-1, ""};
  touch(it);
  return it->second.first;
}

int LRUCache::put(int key, Value val) {
  int latency = get_latency();
  usleep(latency);
  auto it = cache.find(key);
  if (it != cache.end()) {
    touch(it);
  }
  else {
    if (cache.size() == capacity) {
      cache.erase(used.back());
      used.pop_back();
    }
    used.push_front(key);
  }
  cache[key] = {val, used.begin()};
}


class LFUCache : public Policy {
private:
  int size;
  int minFreq;
  std::unordered_map<int, std::pair<Value, int>> m;         //key to {value,freq};
  std::unordered_map<int, std::list<int>::iterator> mIter;  //key to list iterator;
  std::unordered_map<int, std::list<int>> fm;               //freq to key list;
public:
  LFUCache(int _slab_class, int _capacity, int _storage) {
    slab_class = _slab_class;
    capacity = _capacity;
    storage_medium = _storage;
    size = 0;
  }

  Value get(int key);
  int put(int key, Value val);
};

Policy *get_lfu(int slab_class, int default_size, int storage_type) {
  Policy *new_lfu = new LFUCache(slab_class, default_size, storage_type);
  return new_lfu;
}

Value LFUCache::get(int key) {
  int latency = get_latency();
  usleep(latency);
  if (m.count(key) == 0) return Value{-1, ""};
  
  fm[m[key].second].erase(mIter[key]);
  m[key].second++;
  fm[m[key].second].push_back(key);
  mIter[key] = --fm[m[key].second].end();
  
  if (fm[minFreq].size() == 0) {
    minFreq++;
  }
  
  return m[key].first;
}
    
int LFUCache::put(int key, Value val) {
  int latency = get_latency();
  usleep(latency);
  if (capacity <= 0) return;
  
  int storedValue = get(key).size;
  if (storedValue != -1) {
    m[key].first = val;
    return;
  }
  
  if (size >= capacity) {
    m.erase(fm[minFreq].front());
    mIter.erase(fm[minFreq].front());
    fm[minFreq].pop_front();
    size--;
  }
  
  m[key] = {val, 1};
  fm[1].push_back(key);
  mIter[key] = --fm[1].end();
  minFreq = 1;
  size++;
}
