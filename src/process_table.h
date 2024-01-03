#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

#include <string>
#include <vector>
#include <unistd.h>  // for pid_t
using namespace std;

// Process structure
struct Process {
    string cmd;       // Command string
    bool isRunning;   // Whether the process is running or not
    pid_t pid;        // Process ID
};

// ProcessTable class
class ProcessTable {
private:
    vector<Process> table;

public:
    // Add a process to the process table
    void addProcess(const string &cmd, pid_t pid, bool isRunning = true);

    // Display all the jobs currently running or suspended
    void cmd_jobs();

    // Suspend a process by its PID
    bool suspendProcess(pid_t pid);

    // Resume a suspended process by its PID
    bool resumeProcess(pid_t pid);

    // Kill a process by its PID
    bool killProcess(pid_t pid);

    // Function to set the process table using another vector
    void setTable(const vector<Process>& newTable);

    // Function to get the entire process table
    vector<Process>& getTable();

    bool waitForProcess(pid_t pid);
};


#endif // PROCESS_TABLE_H
