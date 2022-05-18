#include "gtest/gtest.h"
#include "LRU.hpp"

//Here were going to check we dont go over 
//the allowed size
TEST(LRU_CACHE, AddToZeroCache) {
  PriorityExpiryCache c(0);
  ASSERT_EQ(c.MaxSizeCache(), 0);

  c.Set("A", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);
  c.Set("B", 1, 5, 100);
  c.Set("C", 1, 5, 100);

};