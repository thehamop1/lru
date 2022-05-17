
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>

#include "LRU.hpp"

/**
 * @brief Default constructor with size 0 cache
 */
PriorityExpiryCache::PriorityExpiryCache():m_maxItems(0){
};

/**
 * @brief Constructor used to reserve data on construction
 * @param size The initial size of the LRU cache
 */
PriorityExpiryCache::PriorityExpiryCache(unsigned int size): m_maxItems(size)
{
  m_nameLookup.reserve(m_maxItems);//grab memory ahead of time
  // m_priorityLookup.reserve(m_maxItems);
  // m_timeoutLookup.reserve(m_maxItems);
};

/**
 * @brief Gets a pointer to a particular value in the lru cache
 * @param key The name of the value we're accessing
 */
CacheData* PriorityExpiryCache::Get(std::string key) {
  auto f = m_nameLookup.find(key);
  if(f==m_nameLookup.end()){
    std::cout << "Did not find the value in the cache!" << std::endl;
    return nullptr;
  }

  return &((f->second).get()->m_value);
};

/**
 * @brief Inserts a new data item into the cache
 * @param key The name of the value we're storing
 * @param value The actually value of the item
 * @param priority Priority of this data block
 * @param expiryInSecs The timeout for a particular variable to be considered stale
 */
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

/**
 * @brief Sets the size of the cache and evicts items if the size is set smaller
 * @param numItems The size to set the cache to
 */
void PriorityExpiryCache::SetMaxItems(int numItems) {
  m_maxItems = numItems;
  EvictItems();
};

/**
 * @brief Prints all the keys in the cache for debugging purposes
 */
void PriorityExpiryCache::DebugPrintKeys() {
  std::cout << "LRU Values: " << std::endl;
  for(const auto& val : m_nameLookup){
    std::cout << "\t{" << val.first << ", " << val.second->m_value << "}," << std::endl;
  }
};

/**
 * @brief Remove items from the LRU based on priority, timeout, and last accessed
 */
void PriorityExpiryCache::EvictItems() {
  //if we have enough space to hold everything dont evict anything
  if(m_nameLookup.size()<m_maxItems) return;

  //iterate over priority but only check the back

  LRU_COMPONENTS_TIMEOUT::reverse_iterator timeoutRiter = m_timeoutLookup.rbegin();
  while(m_nameLookup.size()>m_maxItems){//while the cache is too big
    while(timeoutRiter!=m_timeoutLookup.rend()){
      if(timeoutRiter->second.back()->m_timeout > g_Time){
        break;//start evicting by priority
      }else{
        
      } 
      timeoutRiter++;
    }




  }


  // for(auto& it : m_timeoutLookup){
  //   std::cout << "First: " << it.first << "\tTimeout: " << it.second.back()->m_timeout << std::endl;
  //   // auto& last accessed = it.second.back();
  //   // if(g_Time>it)
  // }

};

void PriorityExpiryCache::RemoveItem(std::shared_ptr<LRU_VALUE> item) {

};