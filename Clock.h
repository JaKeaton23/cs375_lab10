// Clock.h
#ifndef CLOCK_H
#define CLOCK_H

#include <vector>
#include <iostream>

class Clock {
private:
    struct FrameEntry {
        int pid;        // owner process
        int pageNum;    // page number
        bool reference; // R bit
        bool modified;  // M bit
        bool valid;     // whether this slot holds a page
    };

    std::vector<FrameEntry> frames;
    int hand;               // circular pointer
    int numFrames;

    // Stats
    int evictions;

public:
    Clock(int frameCount);

    // Returns the frame number used for this pid/page load
    // If eviction happens, returns replaced frame
    int evictAndReplace(int pid, int pageNum, int& evictedPageOut, int& wasModifiedOut);

    void printStats() const;

};

#endif
