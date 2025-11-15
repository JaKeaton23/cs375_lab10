// CPU.h
#ifndef CPU_H
#define CPU_H

#include "Process.h"
#include <iostream>

class CPU {
private:
    int cycles;             // total CPU cycles executed
    int contextSwitches;    // how many times CPU switched processes
    int lastPID;            // used to detect a context switch
    int lastTID;            // track thread-level switching (optional)

public:
    CPU();

    // Called for every memory access referenced in trace
    void execute(Process& p, int tid);

    void printStats() const;
};

#endif
