# LRU Homework
## How to Build
- Dependencies:
  - CMake 3.18 or Newer
  - GCC 10 or newer (tested with 10 and 12)
  - Google Test

- Instructions:
  - Create build folder
    mkdir build
  - Change directory to build folder:
    cd build
  - Run this command for configuring a debug build:
    cmake -DCMAKE_BUILD_TYPE=Debug ..
  - Run this command for configuring a release build:
    cmake -DCMAKE_BUILD_TYPE=Release ..
  - Build the code like this:
    make -j

- Binaries can be found in:  lru/build/bin
- The Binary called MAIN is the coderpad example
- The Binary called LRUTEST is the gtest set from the test folder

## Problem Description
Implement an LRU cache which includes the following features:
  Expire Time - after which an entry in the cache is invalid
  Priority - lower priority entries should be evicted before higher priority entries

The Cache eviction strategy should be as follows:
  Evict expired entries first
  If there are no expired items to evict then evict the lowest priority entries
    Tie breaking among entries with the same priority is done via least recently used.

You can use any language.

Your task is to implement a PriorityExpiryCache cache with a max capacity.  Specifically please fill out the data structures on the PriorityExpiryCache object and implement the entry eviction method.

You do NOT need to implement the get or set methods.

It should support these operations:
  Get: Get the value of the key if the key exists in the cache and is not expired.
  Set: Update or insert the value of the key with a priority value and expiretime.
    Set should never ever allow more items than maxItems to be in the cache.
    When evicting we need to evict the lowest priority item(s) which are least recently used.

Example:
p5 => priority 5
e10 => expires at 10 seconds since epoch

## Notes
For each item Priority, Timeout, Hits

1. Timeout
2. if priority
3. use hits