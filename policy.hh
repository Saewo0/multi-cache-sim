#ifndef __POLICY_H_
#define __POLICY_H_

#include <string>

#define DRAM  0
#define NVME  1
#define FLASH 2

#define LRU 0
#define LFU 1

#define LATENCY_NVME_LOWER_BOUND 10;
#define LATENCY_NVME_HIGHER_BOUND 20;
#define LATENCY_FLASH_LOWER_BOUND 100;
#define LATENCY_FLASH_HIGHER_BOUND 200;

struct Value {
  int size;
  std::string content;
};

class Policy {
protected:
  int storage_medium;
  int slab_class;
  int capacity;

public:
  virtual Value get(int key) = 0;
  virtual int put(int ket, Value val) = 0;
  int get_latency() {
    if (storage_medium == NVME) {
      int period = LATENCY_NVME_HIGHER_BOUND - LATENCY_NVME_LOWER_BOUND + 1;
      return rand() % period + LATENCY_NVME_LOWER_BOUND;
    } else if (storage_medium == FLASH) {
      int period = LATENCY_FLASH_HIGHER_BOUND - LATENCY_FLASH_LOWER_BOUND + 1;
      return rand() % period + LATENCY_FLASH_LOWER_BOUND;
    } else {
      return 0;
    }
  }
  virtual ~Policy() {}
};

extern Policy *get_lru(int slab_class, int default_size, int storage_type);
extern Policy *get_lfu(int slab_class, int default_size, int storage_type);

#endif
