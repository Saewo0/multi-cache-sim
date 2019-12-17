#include "multi_cache.hh"



Value Multi_cache::get(int key) {
  return Value{1, ""};
}

int Multi_cache::put(int key, Value val) {

}

void Multi_cache::move(Policy *p, int src_medium, int tgt_medium) {

}

double Multi_cache::get_gain(Policy *new_item, Policy *victim) {
  
  return 0;
}