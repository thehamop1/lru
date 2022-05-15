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
    uint32_t id;
    CacheData value;
    int priority;
    int timeout;
    std::list<LRU_VALUE>::iterator it1;
    std::list<LRU_VALUE>::iterator it2;
};

typedef std::unordered_map<std::string, LRU_VALUE> LRU_COMPONENTS_NAME;
typedef std::unordered_map<int, std::list<std::shared_ptr<LRU_VALUE>>> LRU_COMPONENTS_PRIORITY;
typedef std::unordered_map<int, std::list<LRU_VALUE>> LRU_COMPONENTS_TIMEOUT;

class PriorityExpiryCache
{ // I'm going to make this a class so we can add scope
public:
    CacheData *Get(std::string key);
    void Set(std::string key, CacheData value, int priority, int expiryInSecs);
    void SetMaxItems(int numItems);
    void DebugPrintKeys();
    void EvictItems();

private:
    int maxItems;

    LRU_COMPONENTS_NAME m_nameLookup;
    LRU_COMPONENTS_PRIORITY m_priorityLookup;
    LRU_COMPONENTS_TIMEOUT m_timeoutLookup;
};