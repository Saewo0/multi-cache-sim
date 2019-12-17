#ifndef __MULTI_CACHE_H_
#define __MULTI_CACHE_H_

#include <string>
#include <vector>
#include "policy.hh"

class Multi_cache {
private:
  int k;
  uint64_t dram = 0;
  uint64_t nvme = 0;
  uint64_t flash = 0;
  std::vector<Policy *> dram_queues;
  std::vector<Policy *> nvme_queues;
  std::vector<Policy *> flash_queues;
public:
  Multi_cache(uint64_t external_storages[], int _eviction_policy, int _k) {
    dram = external_storages[0];
    nvme = external_storages[1];
    flash = external_storages[2];
    k = _k;

    switch(_eviction_policy) {
      case 0:
        for (int i = 0; i < k; i++) {
          dram_queues.push_back(get_lru(i, 64 * 1024, DRAM));
          nvme_queues.push_back(get_lru(i, 64 * 1024, NVME));
          flash_queues.push_back(get_lru(i, 64 * 1024, FLASH));
        }
        break;
      case 1:
        for (int i = 0; i < k; i++) {
          dram_queues.push_back(get_lfu(i, 64 * 1024, DRAM));
          nvme_queues.push_back(get_lfu(i, 64 * 1024, NVME));
          flash_queues.push_back(get_lfu(i, 64 * 1024, FLASH));
        }
        break;
    }
  }

  Value get(int key);
  int put(int key, Value val);
  void move(Policy *p, int src_medium, int tgt_medium);
  double get_gain(Policy *new_item, Policy *victim);
};

#endif
