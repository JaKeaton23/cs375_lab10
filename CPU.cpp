// CPU.cpp
#include "CPU.h"

CPU::CPU()
    : cycles(0), contextSwitches(0), lastPID(-1), lastTID(-1) {}

void CPU::execute(Process& p, int tid) {
    cycles++; // every access counts as one CPU "tick"

    // Detect a context switch
    if (p.getPID() != lastPID || tid != lastTID) {
        if (lastPID != -1) {
            contextSwitches++;
        }
        lastPID = p.getPID();
        lastTID = tid;
    }

    // For reporting CPU bursts per process
    p.incrementBursts();
}

void CPU::printStats() const {
    std::cout << "\nCPU Statistics:\n";
    std::cout << "  Total CPU cycles:       " << cycles << "\n";
    std::cout << "  Context switches:       " << contextSwitches << "\n";
}
