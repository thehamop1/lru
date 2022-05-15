
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>

#include "LRU.hpp"

PriorityExpiryCache::PriorityExpiryCache():m_maxItems(0){
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
  if(priorityCategory == m_priorityLookup.end()){
    //theres no current priority category for this type
    //list is empty in this case
    auto priorityIt = m_priorityLookup.insert({lruValue->m_priority, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = priorityIt->second.insert(priorityIt->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }else{
    //list is not empty here
    auto listIt = priorityCategory->second.insert(priorityCategory->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }


  auto timeoutCategory = m_timeoutLookup.find(lruValue->m_timeout);
  if(timeoutCategory == m_timeoutLookup.end()){
    auto timeoutIt = m_timeoutLookup.insert({lruValue->m_timeout, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = timeoutIt->second.insert(timeoutIt->second.begin(), lruValue);
    lruValue->m_timeoutIt = listIt;
  }else{
    //list is not empty here
    auto listIt = timeoutCategory->second.insert(priorityCategory->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }

  EvictItems();
};

void PriorityExpiryCache::SetMaxItems(int numItems) {
  m_maxItems = numItems;
  EvictItems();
};

/**
 * @brief Prints all the keys in the cache for debugging purposes
 * 
 */
void PriorityExpiryCache::DebugPrintKeys() {
  std::cout << "LRU Values: " << std::endl;
  for(const auto& val : m_nameLookup){
    std::cout << "\t{" << val.first << ", " << val.second->m_value << "}," << std::endl;
  }
};

void PriorityExpiryCache::EvictItems() {
  // TODO(interviewee): implement this
  if(m_nameLookup.size()<m_maxItems) return;

};