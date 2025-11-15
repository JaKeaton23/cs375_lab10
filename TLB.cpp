// TLB.cpp
#include "TLB.h"

TLB::TLB(int size)
    : capacity(size), hits(0), misses(0) {}

void TLB::updateLRU(const std::pair<int,int>& key) {
    // Remove if already exists
    lru.remove(key);
    // Put at front (most recent)
    lru.push_front(key);
}

void TLB::evictIfNeeded() {
    if ((int)entries.size() < capacity) return;

    // Evict least recently used (back of list)
    auto evictKey = lru.back();
    lru.pop_back();
    entries.erase(evictKey);
}

bool TLB::lookup(int pid, int pageNum, int& frameOut) {
    std::pair<int,int> key(pid, pageNum);

    auto it = entries.find(key);
    if (it == entries.end()) {
        misses++;
        return false;
    }

    hits++;
    frameOut = it->second;

    updateLRU(key);
    return true;
}

void TLB::insert(int pid, int pageNum, int frameNum) {
    std::pair<int,int> key(pid, pageNum);

    evictIfNeeded();
    entries[key] = frameNum;

    updateLRU(key);
}

void TLB::printStats() const {
    std::cout << "\nTLB Statistics:\n";
    std::cout << "  TLB hits:   " << hits << "\n";
    std::cout << "  TLB misses: " << misses << "\n";
    std::cout << "  Hit rate:   "
              << (hits + misses == 0
                  ? 0.0
                  : (double)hits / (hits + misses))
              << "\n";
}
