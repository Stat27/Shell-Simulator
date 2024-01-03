#include <iostream>
#include <sys/wait.h> // for wait()
#include <signal.h>
#include <unistd.h>
#include <vector>
#include <sys/resource.h>
#include "process_table.h"

using namespace std;

void ProcessTable::addProcess(const string &cmd, pid_t pid, bool isRunning) {
    table.push_back({cmd, isRunning, pid});
}

void ProcessTable::cmd_jobs() {
    int running_jobs_count = 0;
    for (auto &process : table) {
        if (process.isRunning) {
            running_jobs_count++;
        }
    }
    cout << "Running processes: " << running_jobs_count << " Active" << endl;

    for (auto &process : table) {
        if(process.isRunning){
            cout << "PID: " << process.pid
                << " , Command: " << process.cmd
                << " , Is Running: " << (process.isRunning ? "Yes" : "No")
                << endl;
        }
    }
}

bool ProcessTable::suspendProcess(pid_t pid) {
    for (auto &process : table) {
        if (process.pid == pid) {
            if (kill(pid, SIGSTOP) == 0) {
                process.isRunning = false;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool ProcessTable::resumeProcess(pid_t pid) {
    for (auto &process : table) {
        if (process.pid == pid) {
            if (kill(pid, SIGCONT) == 0) {
                process.isRunning = true;
                return true;
            }
            return false;
        }
    }
    return false;
}

bool ProcessTable::killProcess(pid_t pid) {
    for (size_t i = 0; i < table.size(); ++i) {
        if (table[i].pid == pid) {
            if (kill(pid, SIGKILL) == 0) {
                table.erase(table.begin() + i);
                return true;
            }
            return false;
        }
    }
    return false;
}

void ProcessTable::setTable(const vector<Process>& newTable) {
    table = newTable;
}

vector<Process>& ProcessTable::getTable() {
    return table;
}

bool ProcessTable::waitForProcess(pid_t pid) {
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        // An error occurred.
        perror("waitpid");
        return false;
    }
    // If you reach here, the process has terminated.
    // Update its isRunning status in your table.
    for (auto &process : table) {
        if (process.pid == pid) {
            process.isRunning = false;
            break;
        }
    }
    return true;
}
