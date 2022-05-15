
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>

#include "LRU.hpp"

PriorityExpiryCache::PriorityExpiryCache(){
};

PriorityExpiryCache::PriorityExpiryCache(unsigned int size): maxItems(size)
{
  m_nameLookup.reserve(maxItems);//grab memory ahead of time
  m_priorityLookup.reserve(maxItems);
  m_timeoutLookup.reserve(maxItems);
};

CacheData* PriorityExpiryCache::Get(std::string key) {
  // ... the interviewee does not need to implement this now.
  // Assume that this will return the value for the key in the cache
  return nullptr;
}

void PriorityExpiryCache::Set(std::string key, CacheData value, int priority, int expiryInSecs) {
  // ... the interviewee does not need to implement this now.
  // Assume that this will add this key, value pair to the cache
  
  EvictItems();
}

void PriorityExpiryCache::SetMaxItems(int numItems) {
  maxItems = numItems;
  EvictItems();
}

void PriorityExpiryCache::DebugPrintKeys() {
  // ... the interviewee does not need to implement this now.
  // Prints all the keys in the cache for debugging purposes
}

void PriorityExpiryCache::EvictItems() {
  // TODO(interviewee): implement this
  
}