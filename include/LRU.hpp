#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>
#include <atomic>

static std::atomic<int> g_Time = 0; //this is like a global timer//lets also add just a bit of safety to it

typedef int CacheData;
struct LRU_VALUE
{
    LRU_VALUE(std::string name, CacheData value, int priority, int expiryInSecs):
        m_name(name),
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
typedef std::unordered_map<int, std::list<std::shared_ptr<LRU_VALUE>>> LRU_COMPONENTS_PRIORITY;
typedef std::unordered_map<int, std::list<std::shared_ptr<LRU_VALUE>>> LRU_COMPONENTS_TIMEOUT;

class PriorityExpiryCache
{ // I'm going to make this a class so we can add scope
public:
    PriorityExpiryCache();
    PriorityExpiryCache(unsigned int size);
    CacheData *Get(std::string key);
    void Set(std::string key, CacheData value, int priority, int expiryInSecs);
    void SetMaxItems(int numItems);
    void DebugPrintKeys();
    void EvictItems();

private:
    unsigned int m_maxItems;

    LRU_COMPONENTS_NAME m_nameLookup;
    LRU_COMPONENTS_PRIORITY m_priorityLookup;
    LRU_COMPONENTS_TIMEOUT m_timeoutLookup;
};