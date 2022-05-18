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
PriorityExpiryCache::PriorityExpiryCache() :g_Time(0), m_maxItems(0){};

/**
 * @brief Constructor used to reserve data on construction
 * @param size The initial size of the LRU cache
 */
PriorityExpiryCache::PriorityExpiryCache(int size):g_Time(0)
{
  m_maxItems = (size<0) ? 0 : size; // If we get a negative set it to 0
  m_nameLookup.reserve(m_maxItems); // grab memory ahead of time
};

/**
 * @brief Gets a pointer to a particular value in the lru cache and also updates
 * the lists in order to account for least recently used.
 * @param key The name of the value we're accessing
 */
CacheData *PriorityExpiryCache::Get(std::string key)
{
  auto mapIterator = m_nameLookup.find(key);
  if (mapIterator == m_nameLookup.end())
  {
    std::cout << "Did not find the value in the cache!" << std::endl;
    return nullptr;
  }
  
  auto lruElement = mapIterator->second;

  auto &timeoutList = m_timeoutLookup.find(lruElement->m_timeout)->second;
  UpdateList(timeoutList, lruElement->m_timeoutIt, lruElement);

  auto &priorityList = m_priorityLookup.find(lruElement->m_priority)->second;
  UpdateList(priorityList, lruElement->m_priorityIt, lruElement);

  return &(lruElement->m_value);
};

/**
 * @brief Updates a list based on access
 * @param list The list we're updating
 * @param it The lru value iterator we need to set
 * @param elem The lru element
 */
void PriorityExpiryCache::UpdateList(std::list<std::shared_ptr<LRU_VALUE>> &list, std::list<std::shared_ptr<LRU_VALUE>>::iterator &it, std::shared_ptr<LRU_VALUE> elem)
{
  list.erase(it);
  auto newPriorityIt = list.insert(list.begin(), elem);
  it = newPriorityIt;
}

/**
 * @brief Inserts a new data item into the cache
 * @param key The name of the value we're storing
 * @param value The actual value of the item
 * @param priority Priority of this lru element
 * @param expiryInSecs The timeout for a particular variable to be considered stale
 */
int PriorityExpiryCache::Set(std::string key, CacheData value, int priority, int expiryInSecs)
{

  //Check if value exists and if it does reject
  if(m_nameLookup.find(key)!=m_nameLookup.end()) return -1;

  // Assume that this will add this key, value pair to the cache
  std::shared_ptr<LRU_VALUE> lruValue = std::make_shared<LRU_VALUE>(key, value, priority, expiryInSecs);
  m_nameLookup.insert({key, lruValue});

  AddToMap(m_priorityLookup, lruValue->m_priorityIt, lruValue, lruValue->m_priority);

  AddToMap(m_timeoutLookup, lruValue->m_timeoutIt, lruValue, lruValue->m_timeout);

  EvictItems();

  return 0;
};

/**
 * @brief Adds a new value to the cache if no category exists for its map then it will create a list. Otherwise
 * prepend to the existing list.
 * @param map The map to add the value
 * @param it The corresponsing iterator for the list in the maps value
 * @param ptr A pointer to the lru value
 * @param key The value of the category (can be priority or timeout epoch)
 */
void PriorityExpiryCache::AddToMap(std::map<int, std::list<std::shared_ptr<LRU_VALUE>>> &map, std::list<std::shared_ptr<LRU_VALUE>>::iterator &it, std::shared_ptr<LRU_VALUE> ptr, int key)
{
  auto category = map.find(key);
  if (category == map.end())
  {
    // theres no current category for this type
    // list is empty in this case
    auto mapIt = map.insert({key, std::list<std::shared_ptr<LRU_VALUE>>()}).first;
    auto listIt = mapIt->second.insert(mapIt->second.begin(), ptr);
    it = listIt;
  }
  else
  {
    // list is not empty here
    auto listIt = category->second.insert(category->second.begin(), ptr);
    it = listIt;
  }
};

/**
 * @brief Sets the size of the cache and evicts items if the size is set smaller
 * @param numItems The size to set the cache
 */
void PriorityExpiryCache::SetMaxItems(int numItems)
{
  m_maxItems = (numItems<0) ? 0 : numItems;
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
 * @brief Remove items from the LRU based on timeout, priority, and least accessed
 */
void PriorityExpiryCache::EvictItems()
{
  // if we have enough space to hold everything dont evict anything
  if (m_nameLookup.size() <= m_maxItems)
    return;

  // iterate over timeout
  while (m_nameLookup.size() > m_maxItems)
  {
    auto firstToExpire = m_timeoutLookup.begin();
    if (firstToExpire->first > g_Time || firstToExpire == m_timeoutLookup.end()){
      break;
    }

    auto &list = firstToExpire->second;
    RemoveItem(list.back());
  }

  // iterate over priority
  while (m_nameLookup.size() > m_maxItems)
  {
    auto lowestPriority = m_priorityLookup.begin();
    if (lowestPriority == m_priorityLookup.end()){
      break;
    }

    auto &list = lowestPriority->second;
    RemoveItem(list.back());
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

  if (priorityIt->second.empty()){
    m_priorityLookup.erase(priorityIt);
  }
  if (timeoutIt->second.empty()){
    m_timeoutLookup.erase(timeoutIt);
  }
};