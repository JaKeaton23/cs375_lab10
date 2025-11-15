// Scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <map>
#include <vector>

enum class SchedulingPolicy {
    MLFQ
};

struct SchedProcess {
    int pid;
    int priority;
    int age;     // tracks starving processes
};

class Scheduler {
private:
    SchedulingPolicy policy;

    // Three queues: 0 = high, 1 = medium, 2 = low
    std::queue<int> q0;
    std::queue<int> q1;
    std::queue<int> q2;

    // Track per-process metadata
    std::map<int, SchedProcess> processes;

    // Time
    int globalTick;

    // Quantum for each level
    int quantumQ0;
    int quantumQ1;
    int quantumQ2;

    // Track current running process
    int currentPID;
    int timeSliceRemaining;

    void promoteIfStarving();
    void demoteProcess(int pid);

public:
    Scheduler(SchedulingPolicy p, int quanta);

    void addProcessIfNotExists(int pid);

    void tick();   // called each CPU cycle

    int getCurrentProcess() const { return currentPID; }

    void printQueues() const;
};

#endif
