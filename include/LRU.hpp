#pragma once
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <memory>
#include <atomic>

typedef int CacheData;
struct LRU_VALUE
{
    LRU_VALUE(std::string name, CacheData value, int priority, int expiryInSecs) : m_name(name),
                                                                                   m_value(value),
                                                                                   m_priority(priority),
                                                                                   m_timeout(expiryInSecs){};

    std::string m_name;
    CacheData m_value;
    int m_priority;
    int m_timeout;

    std::unordered_map<std::string, std::shared_ptr<LRU_VALUE>>::iterator m_nameIt;
    std::list<std::shared_ptr<LRU_VALUE>>::iterator m_priorityIt;
    std::list<std::shared_ptr<LRU_VALUE>>::iterator m_timeoutIt;
};

typedef std::unordered_map<std::string, std::shared_ptr<LRU_VALUE>> LRU_COMPONENTS_NAME;
typedef std::map<int, std::list<std::shared_ptr<LRU_VALUE>>> LRU_COMPONENTS_PRIORITY;
typedef std::map<int, std::list<std::shared_ptr<LRU_VALUE>>> LRU_COMPONENTS_TIMEOUT;

class PriorityExpiryCache
{ // I'm going to make this a class so we can add scope
public:
    PriorityExpiryCache();
    PriorityExpiryCache(int size);
    CacheData *Get(std::string key);
    int Set(std::string key, CacheData value, int priority, int expiryInSecs);
    void SetMaxItems(int numItems);
    void DebugPrintKeys();
    void EvictItems();
    int MaxSizeCache(){return m_maxItems;};
    int SizeOfCache(){return m_nameLookup.size();};

    int g_Time; // Changing this away from global for testing

private:
    void RemoveItem(std::shared_ptr<LRU_VALUE> item);
    void UpdateList(std::list<std::shared_ptr<LRU_VALUE>> &list, std::list<std::shared_ptr<LRU_VALUE>>::iterator &it, std::shared_ptr<LRU_VALUE> elem);
    void AddToMap(std::map<int, std::list<std::shared_ptr<LRU_VALUE>>> &map, std::list<std::shared_ptr<LRU_VALUE>>::iterator &it, std::shared_ptr<LRU_VALUE> ptr, int key);
    unsigned int m_maxItems;

    LRU_COMPONENTS_NAME m_nameLookup;
    LRU_COMPONENTS_PRIORITY m_priorityLookup;
    LRU_COMPONENTS_TIMEOUT m_timeoutLookup;
};