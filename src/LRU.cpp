
#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>

#include "LRU.hpp"

int PriorityExpiryCache::g_Time = 0; // this is like a global timer

/**
 * @brief Default constructor with size 0 cache
 */
PriorityExpiryCache::PriorityExpiryCache() : m_maxItems(0){};

/**
 * @brief Constructor used to reserve data on construction
 * @param size The initial size of the LRU cache
 */
PriorityExpiryCache::PriorityExpiryCache(unsigned int size) : m_maxItems(size)
{
  m_nameLookup.reserve(m_maxItems); // grab memory ahead of time
};

/**
 * @brief Gets a pointer to a particular value in the lru cache and also updates
 * the list in order to account for least recently used.
 * @param key The name of the value we're accessing
 */
CacheData *PriorityExpiryCache::Get(std::string key)
{
  auto f = m_nameLookup.find(key);
  if (f == m_nameLookup.end())
  {
    std::cout << "Did not find the value in the cache!" << std::endl;
    return nullptr;
  }
  auto& priorityList = m_priorityLookup.find(f->second->m_priority)->second;
  priorityList.erase(f->second->m_priorityIt);
  auto newPriorityIt = priorityList.insert(priorityList.begin(), f->second);
  f->second->m_priorityIt = newPriorityIt;

  auto& timeoutList = m_timeoutLookup.find(f->second->m_timeout)->second;
  timeoutList.erase(f->second->m_timeoutIt);
  auto newTimeoutIt = priorityList.insert(timeoutList.begin(), f->second);
  f->second->m_timeoutIt = newTimeoutIt;

  return &((f->second).get()->m_value);
};

/**
 * @brief Inserts a new data item into the cache
 * @param key The name of the value we're storing
 * @param value The actually value of the item
 * @param priority Priority of this data block
 * @param expiryInSecs The timeout for a particular variable to be considered stale
 */
void PriorityExpiryCache::Set(std::string key, CacheData value, int priority, int expiryInSecs)
{
  // Assume that this will add this key, value pair to the cache
  std::shared_ptr<LRU_VALUE> lruValue = std::make_shared<LRU_VALUE>(key, value, priority, expiryInSecs);
  m_nameLookup.insert({key, lruValue});

  auto priorityCategory = m_priorityLookup.find(lruValue->m_priority);
  if (priorityCategory == m_priorityLookup.end())
  {
    // theres no current priority category for this type
    // list is empty in this case
    auto priorityIt = m_priorityLookup.insert({lruValue->m_priority, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = priorityIt->second.insert(priorityIt->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }
  else
  {
    // list is not empty here
    auto listIt = priorityCategory->second.insert(priorityCategory->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }

  auto timeoutCategory = m_timeoutLookup.find(lruValue->m_timeout);
  if (timeoutCategory == m_timeoutLookup.end())
  {
    auto timeoutIt = m_timeoutLookup.insert({lruValue->m_timeout, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = timeoutIt->second.insert(timeoutIt->second.begin(), lruValue);
    lruValue->m_timeoutIt = listIt;
  }
  else
  {
    // list is not empty here
    auto listIt = timeoutCategory->second.insert(priorityCategory->second.begin(), lruValue);
    lruValue->m_priorityIt = listIt;
  }

  EvictItems();
};

/**
 * @brief Sets the size of the cache and evicts items if the size is set smaller
 * @param numItems The size to set the cache to
 */
void PriorityExpiryCache::SetMaxItems(int numItems)
{
  m_maxItems = numItems;
  EvictItems();
};

/**
 * @brief Prints all the keys in the cache for debugging purposes
 */
void PriorityExpiryCache::DebugPrintKeys()
{
  std::cout << "LRU Values: " << std::endl;
  for (const auto &val : m_nameLookup)
  {
    std::cout << "\t{" << val.first << ", " << val.second->m_value << "}," << std::endl;
  }
};

/**
 * @brief Remove items from the LRU based on priority, timeout, and last accessed
 */
void PriorityExpiryCache::EvictItems()
{
  // if we have enough space to hold everything dont evict anything
  if (m_nameLookup.size() <= m_maxItems)
    return;

  // iterate over priority but only check the back

  LRU_COMPONENTS_TIMEOUT::iterator timeoutIter = m_timeoutLookup.begin();
  while (timeoutIter != m_timeoutLookup.end() && m_nameLookup.size() > m_maxItems)
  {
    std::cout << "List ID: " << timeoutIter->first << "SIZE OF LIST: " << timeoutIter->second.size() << std::endl;
    if (timeoutIter->second.back()->m_timeout > g_Time)
    {
      break; // start evicting by priority
    }
    else
    {
      std::cout << "removing an item" << std::endl;
      RemoveItem(timeoutIter->second.back());
    }
    timeoutIter++;
  }

  LRU_COMPONENTS_PRIORITY::iterator priorityIter = m_priorityLookup.begin();
  while (priorityIter != m_priorityLookup.end() && m_nameLookup.size() > m_maxItems)
  {
    std::cout << "List ID: " << priorityIter->first << "SIZE OF LIST: " << priorityIter->second.size() << std::endl;
    std::cout << "removing an item" << std::endl;
    RemoveItem(priorityIter->second.back());
    priorityIter++;
  }

};

/**
 * @brief Remove items from the LRU data structures and also cleans up
 * empty lists in the map
 * @param item A shared pointer to the value to be removed from the cache
 */
void PriorityExpiryCache::RemoveItem(std::shared_ptr<LRU_VALUE> item)
{
  int priority = item->m_priority;
  int timeout = item->m_timeout;

  LRU_COMPONENTS_PRIORITY::iterator priorityIt = m_priorityLookup.find(priority);
  LRU_COMPONENTS_TIMEOUT::iterator timeoutIt = m_timeoutLookup.find(timeout);

  m_nameLookup.erase(item->m_name);
  priorityIt->second.erase(item->m_priorityIt);
  timeoutIt->second.erase(item->m_timeoutIt);

  if (priorityIt->second.empty())
    m_priorityLookup.erase(priorityIt);
  if (timeoutIt->second.empty())
    m_timeoutLookup.erase(timeoutIt);
};