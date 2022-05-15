
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>

#include "LRU.hpp"

PriorityExpiryCache::PriorityExpiryCache(){
};

PriorityExpiryCache::PriorityExpiryCache(unsigned int size): m_maxItems(size)
{
  m_nameLookup.reserve(m_maxItems);//grab memory ahead of time
  m_priorityLookup.reserve(m_maxItems);
  m_timeoutLookup.reserve(m_maxItems);
};

CacheData* PriorityExpiryCache::Get(std::string key) {
  // ... the interviewee does not need to implement this now.
  // Assume that this will return the value for the key in the cache
  auto f = m_nameLookup.find(key);
  if(f==m_nameLookup.end()){
    std::cout << "Did not find the value in the cache!" << std::endl;
    return nullptr;
  }

  return &((f->second).get()->m_value);
};

void PriorityExpiryCache::Set(std::string key, CacheData value, int priority, int expiryInSecs) {
  // Assume that this will add this key, value pair to the cache
  std::shared_ptr<LRU_VALUE> lruValue = std::make_shared<LRU_VALUE>(key, value, priority, expiryInSecs);
  m_nameLookup.insert({key, lruValue});

  auto priorityCategory = m_priorityLookup.find(lruValue->m_priority);

  //theres no current priority category for this type
  if(priorityCategory == m_priorityLookup.end()){
    //list is empty in this case
    auto priorityIt = m_priorityLookup.insert({lruValue->m_priority, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = priorityIt->second.insert(priorityIt->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }else{
    
  }

  EvictItems();
};

void PriorityExpiryCache::SetMaxItems(int numItems) {
  m_maxItems = numItems;
  EvictItems();
};

void PriorityExpiryCache::DebugPrintKeys() {
  // ... the interviewee does not need to implement this now.
  // Prints all the keys in the cache for debugging purposes
  std::cout << "LRU Values: " << std::endl;
  for(const auto& val : m_nameLookup){
    std::cout << "{" << val.first << ", " << val.second << "}," << std::endl;
  }
};

void PriorityExpiryCache::EvictItems() {
  // TODO(interviewee): implement this
};