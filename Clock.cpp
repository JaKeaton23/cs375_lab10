// Clock.cpp
#include "Clock.h"

Clock::Clock(int frameCount)
    : frames(frameCount), hand(0), numFrames(frameCount), evictions(0)
{
    for (auto& f : frames) {
        f.pid = -1;
        f.pageNum = -1;
        f.reference = false;
        f.modified = false;
        f.valid = false;
    }
}

int Clock::evictAndReplace(int pid, int pageNum, int& evictedPageOut, int& wasModifiedOut) {
    // First try to find a free frame
    for (int i = 0; i < numFrames; i++) {
        if (!frames[i].valid) {
            frames[i].pid = pid;
            frames[i].pageNum = pageNum;
            frames[i].reference = true;
            frames[i].modified = false;
            frames[i].valid = true;

            evictedPageOut = -1;
            wasModifiedOut = 0;
            return i;
        }
    }

    // No free frames → Enhanced Clock search
    while (true) {
        FrameEntry& f = frames[hand];

        // Case 1: reference = 0 and modified = 0 → perfect eviction candidate
        if (!f.reference && !f.modified) {
            evictedPageOut = f.pageNum;
            wasModifiedOut = f.modified ? 1 : 0;

            // Replace
            f.pid = pid;
            f.pageNum = pageNum;
            f.reference = true;
            f.modified = false;
            f.valid = true;

            evictions++;
            int ret = hand;
            hand = (hand + 1) % numFrames;
            return ret;
        }

        // Case 2: reference = 0 but modified = 1 → second chance
        // Lower modified bit so next pass we can evict
        if (!f.reference && f.modified) {
            f.modified = false;
        }

        // Case 3: reference = 1 → clear and advance
        if (f.reference) {
            f.reference = false;
        }

        hand = (hand + 1) % numFrames;
    }
}

void Clock::printStats() const {
    std::cout << "\nClock Algorithm Stats:\n";
    std::cout << "  Total evictions: " << evictions << "\n";
}
