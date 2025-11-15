// Process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <map>

class Process {
private:
    int pid;

    // Per-thread memory access logs
    std::map<int, std::vector<unsigned int>> threadAccesses;

    int totalAccesses;
    int cpuBursts;

public:
    Process() : pid(-1), totalAccesses(0), cpuBursts(0) {}
    
    Process(int pid, int initialBursts = 0);

    void addAccess(int tid, unsigned int address);

    const std::vector<unsigned int>& getThreadAccesses(int tid) const;

    int getPID() const { return pid; }

    int getTotalAccesses() const { return totalAccesses; }

    int getBurstCount() const { return cpuBursts; }

    void incrementBursts() { cpuBursts++; }
};

#endif
