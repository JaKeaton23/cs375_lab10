// TwoLevelPageTable.h
#ifndef TWO_LEVEL_PAGE_TABLE_H
#define TWO_LEVEL_PAGE_TABLE_H

#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <cmath>

class TwoLevelPageTable {
private:
    // How many pages are in each inner table (used to compute outer/inner index)
    int pagesPerInner;

    // Outer level: (pid, outer_page) -> true if an inner table exists
    std::map<std::pair<int,int>, bool> firstLevel;

    // Second level: (pid, full_pageNum) -> frame number
    std::map<std::pair<int,int>, int> secondLevel;

    // Convenience set to quickly check if a page is currently in memory
    // key = (pid, pageNum)
    std::set<std::pair<int,int>> memoryPages;

    // Helper to compute outer page index from pageNum
    int getOuter(int pageNum) const {
        return pageNum / pagesPerInner;
    }

public:
    // pageCount = total number of virtual pages per process
    explicit TwoLevelPageTable(int pageCount);

    // Returns true if (pid, pageNum) is currently loaded in memory
    bool isInMemory(int pid, int pageNum) const;

    // Returns frame number if present, or -1 if not in memory
    int getFrame(int pid, int pageNum) const;

    // Load a page into a frame (handles allocating the "second-level" logically)
    void loadPage(int pid, int pageNum, int frameNum);

    // Print some simple stats for debugging / report
    void printStats() const;
};

#endif // TWO_LEVEL_PAGE_TABLE_H
