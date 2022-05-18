#include "LRU.hpp"

int main()
{
  PriorityExpiryCache c(5);
  // Name, value, priority, seconds
  c.Set("A", 1, 5, 100);
  c.Set("B", 2, 15, 3);
  c.Set("C", 3, 5, 10);
  c.Set("D", 4, 1, 15);
  c.Set("E", 5, 5, 150);

  std::cout << "Value at C: " << *(c.Get("C")) << std::endl; // edited line

  // Current time = 0
  c.SetMaxItems(5);
  // Keys in C = ["A", "B", "C", "D", "E"]
  // space for 5 keys, all 5 items are included
  c.DebugPrintKeys();

  // Sleep for 5 secs
  c.g_Time += 5;

  // Current time = 5
  c.SetMaxItems(4);
  // Keys in C = ["A", "C", "D", "E"]
  // "B" is removed because it is expired.  e3 < e5
  c.DebugPrintKeys();

  c.SetMaxItems(3);
  // Keys in C = ["A", "C", "E"]
  // "D" is removed because it the lowest priority
  // D's expire time is irrelevant.
  c.DebugPrintKeys();

  c.SetMaxItems(2);
  // Keys in C = ["C", "E"]
  // "A" is removed because it is least recently used."
  // A's expire time is irrelevant.
  c.DebugPrintKeys();

  c.SetMaxItems(1);
  // Keys in C = ["C"]
  // "E" is removed because C is more recently used (due to the Get("C") event).
  c.DebugPrintKeys();

  return 0;
}