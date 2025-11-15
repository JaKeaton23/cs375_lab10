// TwoLevelPageTable.cpp
#include "TwoLevelPageTable.h"

TwoLevelPageTable::TwoLevelPageTable(int pageCount) {
    if (pageCount <= 0) {
        pagesPerInner = 1;
    } else {
        // Roughly split the pages across outer/inner levels
        int root = static_cast<int>(std::sqrt(static_cast<double>(pageCount)));
        pagesPerInner = (root > 0) ? root : 1;
    }
}

bool TwoLevelPageTable::isInMemory(int pid, int pageNum) const {
    std::pair<int,int> key(pid, pageNum);
    return memoryPages.find(key) != memoryPages.end();
}

int TwoLevelPageTable::getFrame(int pid, int pageNum) const {
    std::pair<int,int> key(pid, pageNum);
    auto it = secondLevel.find(key);
    if (it == secondLevel.end()) {
        return -1; // not present
    }
    return it->second;
}

void TwoLevelPageTable::loadPage(int pid, int pageNum, int frameNum) {
    int outer = getOuter(pageNum);

    // Mark that this (pid, outer) has an inner table allocated
    std::pair<int,int> outerKey(pid, outer);
    if (firstLevel.find(outerKey) == firstLevel.end()) {
        firstLevel[outerKey] = true;
    }

    // Store the mapping from (pid, full pageNum) -> frame
    std::pair<int,int> pageKey(pid, pageNum);
    secondLevel[pageKey] = frameNum;
    memoryPages.insert(pageKey);
}

void TwoLevelPageTable::printStats() const {
    std::cout << "Two-Level Page Table Stats\n";
    std::cout << "  Outer tables allocated : " << firstLevel.size() << "\n";
    std::cout << "  Pages mapped (2nd level): " << secondLevel.size() << "\n";
    std::cout << "  Pages currently in memory: " << memoryPages.size() << "\n";
}
