#include <iostream>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <list>
#include <memory>


int g_Time = 0;

typedef int CacheData;
struct LRU_VALUE{
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

// f(key) -> memory 
class Lookup{

  private:


};


struct PriorityExpiryCache {
  int maxItems;
  // TODO(interviewee): implement this
  LRU_COMPONENTS_NAME m_nameLookup;
  LRU_COMPONENTS_NAME m_priorityLookup;

  // std::vector<std::shared_ptr<LRU_VALUES>> m_cache;
  
  CacheData* Get(std::string key) {
    // ... the interviewee does not need to implement this now.
    // Assume that this will return the value for the key in the cache
    return nullptr;
  }
  
  void Set(std::string key, CacheData value, int priority, int expiryInSecs) {
    // ... the interviewee does not need to implement this now.
    // Assume that this will add this key, value pair to the cache
    
    EvictItems();
  }
  
  void SetMaxItems(int numItems) {
    maxItems = numItems;
    EvictItems();
  }
  
  void DebugPrintKeys() {
    // ... the interviewee does not need to implement this now.
    // Prints all the keys in the cache for debugging purposes
  }
  
  void EvictItems() {
    // TODO(interviewee): implement this
    
  }
};