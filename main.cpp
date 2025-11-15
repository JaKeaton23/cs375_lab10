// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Process.h"
#include "Scheduler.h"
#include "CPU.h"
#include "TLB.h"
#include "TwoLevelPageTable.h"
#include "Clock.h"

using namespace std;

struct TraceEntry {
    int pid;
    int tid;
    unsigned int address;
    char access;
};

vector<TraceEntry> loadTrace(const string& filename) {
    vector<TraceEntry> trace;
    ifstream in(filename);
    if (!in) {
        cerr << "ERROR: Cannot open trace file " << filename << endl;
        exit(1);
    }

    int pid, tid;
    string addrHex;
    char accessType;

    while (in >> pid >> tid >> addrHex >> accessType) {
        unsigned int addr;
        stringstream ss;
        ss << hex << addrHex;
        ss >> addr;

        trace.push_back({pid, tid, addr, accessType});
    }
    return trace;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Usage: ./simulator <trace_file> <tlb_size> <num_frames>\n";
        return 1;
    }

    string traceFile = argv[1];
    int tlbSize = stoi(argv[2]);
    int numFrames = stoi(argv[3]);

    vector<TraceEntry> trace = loadTrace(traceFile);

    TLB tlb(tlbSize);
    TwoLevelPageTable table(1024);
    Clock clock(numFrames);
    Scheduler scheduler(SchedulingPolicy::MLFQ, tlbSize);
    CPU cpu;

    map<int, Process> processes;

    // Create processes dynamically during trace load
    for (auto& t : trace) {
        if (processes.count(t.pid) == 0) {
            processes[t.pid] = Process(t.pid, 0);
        }
        processes[t.pid].addAccess(t.tid, t.address);
    }

    // Run simulation
    for (auto& t : trace) {
        Process& p = processes[t.pid];

        int pageNum = t.address >> 12;      // 4KB pages
        int frameNum;

        bool tlbHit = tlb.lookup(t.pid, pageNum, frameNum);
        if (!tlbHit) {
            // Page table check
            if (!table.isInMemory(t.pid, pageNum)) {
                int evPage, isMod;
                frameNum = clock.evictAndReplace(t.pid, pageNum, evPage, isMod);
                table.loadPage(t.pid, pageNum, frameNum);
            } else {
                frameNum = table.getFrame(t.pid, pageNum);
            }
            tlb.insert(t.pid, pageNum, frameNum);
        }

        cpu.execute(p, t.tid);
        scheduler.addProcessIfNotExists(t.pid);
        scheduler.tick();
        scheduler.printQueues();
    }

    // Print stats
    cout << "\n==== FINAL STATISTICS ====\n";
    tlb.printStats();
    clock.printStats();
    table.printStats();
    cpu.printStats();

    return 0;
}
