// TLB.h
#ifndef TLB_H
#define TLB_H

#include <map>
#include <list>
#include <utility>
#include <iostream>

class TLB {
private:
    int capacity;

    // Maps (pid, pageNum) → frameNum
    std::map<std::pair<int,int>, int> entries;

    // LRU list: front = most recently used, back = least recently used
    std::list<std::pair<int,int>> lru;

    // Stats
    int hits;
    int misses;

    void updateLRU(const std::pair<int,int>& key);
    void evictIfNeeded();

public:
    TLB(int size);

    bool lookup(int pid, int pageNum, int& frameOut);
    void insert(int pid, int pageNum, int frameNum);

    void printStats() const;
};

#endif
