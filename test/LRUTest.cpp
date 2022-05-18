#include "gtest/gtest.h"
#include "LRU.hpp"

//Here were going to check we dont go over 
//the allowed size
TEST(LRU_CACHE, AddToZeroCache) {
  PriorityExpiryCache c;
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

  c.Set("B", -1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);

  c.Set("C", 0, 50, 100);
  ASSERT_EQ(c.SizeOfCache(), 0);
}

//Not checking much here but just to
//check against runtime errors
TEST(LRU_CACHE, PrintValues) {
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", -9, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 4, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  //we can see the difference here
  //but no current way to test could overload
  //with ostream pass by ref
  c.DebugPrintKeys();
  c.EvictItems();
  c.DebugPrintKeys();
}

TEST(LRU_CACHE, EvictionToZero) {
  CacheData* val=nullptr;
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 1, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", -9, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 4, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  for(int x=2;x>=0;x--){
    c.SetMaxItems(x);
    ASSERT_EQ(c.SizeOfCache(), x);
  }

  val = c.Get("B");
  ASSERT_EQ(val, nullptr);
}

TEST(LRU_CACHE, CheckTimeout) {
  CacheData* val=nullptr;
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 1, 5, 1);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", -9, 5, 2);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 4, 5, 3);
  ASSERT_EQ(c.SizeOfCache(), 3);

  c.g_Time += 1;
  c.SetMaxItems(2);
  ASSERT_EQ(c.SizeOfCache(), 2);

  val=c.Get("A");
  ASSERT_EQ(val, nullptr);
  val=c.Get("B");
  ASSERT_EQ(*val, -9);
  val=c.Get("C");
  ASSERT_EQ(*val, 4);

  c.g_Time += 1;
  c.SetMaxItems(1);
  ASSERT_EQ(c.SizeOfCache(), 1);

  val=c.Get("A");
  ASSERT_EQ(val, nullptr);
  val=c.Get("B");
  ASSERT_EQ(val, nullptr);
  val=c.Get("C");
  ASSERT_EQ(*val, 4);

  c.g_Time += 1;
  c.SetMaxItems(0);
  ASSERT_EQ(c.SizeOfCache(), 0);

  val=c.Get("A");
  ASSERT_EQ(val, nullptr);
  val=c.Get("B");
  ASSERT_EQ(val, nullptr);
  val=c.Get("C");
  ASSERT_EQ(val, nullptr);
}

TEST(LRU_CACHE, CheckValuesInCache) {
  CacheData* val=nullptr;
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 100, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", 99, 8, 100);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 98, 5, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  val = c.Get("A");
  ASSERT_EQ(*val, 100);

  val = c.Get("B");
  ASSERT_EQ(*val, 99);

  val = c.Get("C");
  ASSERT_EQ(*val, 98);

  for(int x=2;x>=0;x--){
    c.SetMaxItems(x);
    ASSERT_EQ(c.SizeOfCache(), x);
  }

  val = c.Get("A");
  ASSERT_EQ(val, nullptr);

  val = c.Get("B");
  ASSERT_EQ(val, nullptr);

  val = c.Get("C");
  ASSERT_EQ(val, nullptr);
}

//D is not inserted into the cache
//since it is a lower priority than A,B,C
TEST(LRU_CACHE, EvictNewItem) {
  PriorityExpiryCache c(3);
  ASSERT_EQ(c.MaxSizeCache(), 3);

  c.Set("A", 10, 100, 100);
  ASSERT_EQ(c.SizeOfCache(), 1);

  c.Set("B", 10, 100, 100);
  ASSERT_EQ(c.SizeOfCache(), 2);

  c.Set("C", 10, 100, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  c.Set("D", 1, 1, 100);
  ASSERT_EQ(c.SizeOfCache(), 3);

  c.DebugPrintKeys();
  CacheData* val=c.Get("D");
  ASSERT_EQ(val, nullptr);
}