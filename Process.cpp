// Process.cpp
#include "Process.h"

Process::Process(int pid, int initialBursts)
    : pid(pid), totalAccesses(0), cpuBursts(initialBursts) {}

void Process::addAccess(int tid, unsigned int address) {
    threadAccesses[tid].push_back(address);
    totalAccesses++;
}

const std::vector<unsigned int>& Process::getThreadAccesses(int tid) const {
    static std::vector<unsigned int> empty;

    auto it = threadAccesses.find(tid);
    if (it == threadAccesses.end())
        return empty;

    return it->second;
}
