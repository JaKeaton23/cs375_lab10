// Scheduler.cpp
#include "Scheduler.h"
#include <iostream>

Scheduler::Scheduler(SchedulingPolicy p, int quanta)
    : policy(p), globalTick(0), currentPID(-1)
{
    // Base quantum (TLB size passed in main.cpp)
    quantumQ0 = quanta;          // highest priority
    quantumQ1 = quanta * 2;
    quantumQ2 = quanta * 4;

    timeSliceRemaining = 0;
}

void Scheduler::addProcessIfNotExists(int pid) {
    if (processes.count(pid)) return;

    processes[pid] = { pid, 0, 0 };   // start at highest priority queue (0)
    q0.push(pid);
}

void Scheduler::demoteProcess(int pid) {
    auto &p = processes[pid];
    if (p.priority < 2) p.priority++;  // max = 2

    if (p.priority == 1) q1.push(pid);
    else if (p.priority == 2) q2.push(pid);
}

void Scheduler::promoteIfStarving() {
    for (auto &entry : processes) {
        auto &p = entry.second;

        p.age++;

        if (p.age >= 20 && p.priority > 0) {
            p.priority--;
            p.age = 0;

            if (p.priority == 0) q0.push(p.pid);
            else if (p.priority == 1) q1.push(p.pid);
        }
    }
}

void Scheduler::tick() {
    globalTick++;

    promoteIfStarving();

    // If no current process, select one
    if (currentPID == -1) {
        if (!q0.empty()) {
            currentPID = q0.front(); q0.pop();
            timeSliceRemaining = quantumQ0;
        }
        else if (!q1.empty()) {
            currentPID = q1.front(); q1.pop();
            timeSliceRemaining = quantumQ1;
        }
        else if (!q2.empty()) {
            currentPID = q2.front(); q2.pop();
            timeSliceRemaining = quantumQ2;
        }
        else {
            return; // no runnable processes
        }
    }

    // Running process consumes CPU time
    timeSliceRemaining--;
    processes[currentPID].age = 0; // reset starvation counter

    // If timeslice expires → demote + choose next
    if (timeSliceRemaining <= 0) {
        demoteProcess(currentPID);
        currentPID = -1; // force selecting a new one next tick
    }
}

void Scheduler::printQueues() const {
    std::cout << "MLFQ State:\n";

    std::cout << "Q0: ";
    std::queue<int> t0 = q0;
    while (!t0.empty()) { std::cout << t0.front() << " "; t0.pop(); }

    std::cout << "\nQ1: ";
    std::queue<int> t1 = q1;
    while (!t1.empty()) { std::cout << t1.front() << " "; t1.pop(); }

    std::cout << "\nQ2: ";
    std::queue<int> t2 = q2;
    while (!t2.empty()) { std::cout << t2.front() << " "; t2.pop(); }

    std::cout << "\nCurrent PID: " << currentPID << "\n\n";
}
