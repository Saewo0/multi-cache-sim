#include <iostream>

#include "policy.hh"
#include "multi_cache.hh"

using namespace std;

// usage: multi_cache num_external_storage first_storage_type:size second_storage_type:size ... eviction_policy
int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "invalid arguments" << endl;
    return -1;
  }
  int num_storages = atoi(argv[1]);
  uint64_t external_storages[num_storages];

  uint64_t multiplier = 0;
  int base_size = 0;

  int eviction_policy = 0;
  
  for (int i = 0; i < num_storages; i++) {
    char *b = NULL;
    char *external_storage = argv[i + 2];

    char *storage = strtok_r(external_storage, ":", &b);
    char *size = strtok_r(NULL, ":", &b);
    char unit = tolower(size[strlen(size)-1]);
    size[strlen(size)-1] = '\0';
    
    switch (unit) {
        case 'm':
          multiplier = 1024 * 1024;
          break;
        case 'g':
          multiplier = 1024 * 1024 * 1024;
          break;
        case 't':
          multiplier = 1024 * 1024;
          multiplier *= 1024 * 1024;
          break;
        case 'p':
          multiplier = 1024 * 1024;
          multiplier *= 1024 * 1024 * 1024;
          break;
        default:
          cout << "invalid size unit" << endl;
          return -1;
    }
    base_size = atoi(size);
    cout << base_size << "*" << multiplier << " " << unit << endl;

    if (strcmp(storage, "DRAM") == 0) {
      external_storages[0] = base_size * multiplier;
    } else if (strcmp(storage, "NVME") == 0) {
      external_storages[1] = base_size * multiplier;
    } else if (strcmp(storage, "FLASH") == 0) {
      external_storages[2] = base_size * multiplier;
    } else {
      cout << "invalid enternal storage type" << endl;
      return -1;
    }
  }

  /*
  for (int i = 0; i < num_storages; i++) {
    cout << external_storages[i] << endl;
  }
  */
  if (argc < 3 + num_storages || strcmp(argv[argc - 1], "lru") == 0) {
    eviction_policy = LRU;
  } else if (strcmp(argv[argc - 1], "lfu") == 0) {
    eviction_policy = LFU;
  } else {
    cout << "invalid eviction policy" << endl;
    return -1;
  }

  Multi_cache cache(external_storages, eviction_policy, 5);
  
  
  return 0;
}