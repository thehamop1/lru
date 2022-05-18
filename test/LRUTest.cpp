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
  ASSERT_EQ(c.SizeOfCache(), 0);

  c.Set("C", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);
};

TEST(LRU_CACHE, NegativeSizedCache) {
  PriorityExpiryCache c(-5);
  ASSERT_EQ(c.MaxSizeCache(), 0);

  c.Set("A", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);

  c.Set("B", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);

  c.Set("C", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);
}

TEST(LRU_CACHE, EvictionToZero) {
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  for(int x=2;x>=0;x--){
    c.SetMaxItems(x);
    ASSERT_EQ(c.SizeOfCache(), x);
  }
}

// TEST(LRU_CACHE, EvictionToZero) {
//   PriorityExpiryCache c(3);
//   ASSERT_EQ(c.MaxSizeCache(), 3);

//   c.Set("A", 1, 5, 100);
//   ASSERT_EQ(c.SizeOfCache(), 1);

//   c.Set("B", 1, 5, 100);
//   ASSERT_EQ(c.SizeOfCache(), 2);

//   c.Set("C", 1, 5, 100);
//   ASSERT_EQ(c.SizeOfCache(), 3);

//   for(int x=2;x>=0;x--){
//     c.SetMaxItems(x);
//     ASSERT_EQ(c.SizeOfCache(), x);
//   }
// }