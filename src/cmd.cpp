#include <iostream>
#include <sys/wait.h> // for wait()
#include "process_table.h"
#include <unistd.h>
#include <vector>
#include <cstring>
#include <sys/resource.h>
using namespace std;

ProcessTable Allprocesses;

void child_processes_usage(){
    struct rusage usage;
    if (getrusage(RUSAGE_CHILDREN, &usage) == -1) {
        perror("getrusage");
        }
    printf("User time (children) = %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System time (children) = %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

}

void sigchld_handler(int signo) {

    (void)signo;  // prevent unused variable warning
    int status;
    pid_t child_pid;

    while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0) {

        for (auto &process : Allprocesses.getTable()) {
            if (process.pid ==child_pid){
                process.isRunning = false;
                break;
            }
        }
        
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> tokens;
    string token;
    for (char ch : s) {
        if (ch == delim) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }

    if (!token.empty()){
        tokens.push_back(token);
        } 
    return tokens;
}


// Clean up the memory allocated by the split function
void cleanup(char** tokens,int size) {
    for (int i = 0; i < size; i++) {
        delete[] tokens[i];
    }
    delete[] tokens;
}

void command_action_cleanup(char* token){
    delete[] token;
}


bool commands(string cmd){

    char delim = ' ';
        

    vector<string> full_command = split(cmd,delim);

    // check whether should run in background
    bool background = false;
    if (full_command.back() == "&") {
        background = true;
        full_command.pop_back();
    }

   string inputFileName, outputFileName;

    for (int i = 1; i < full_command.size(); i++) {
        if (full_command[i].find('<') != string::npos) {
            inputFileName = full_command[i].erase(0,1).c_str();


        }
            
        if (full_command[i].find('>') != string::npos) {
            outputFileName = full_command[i].erase(0,1).c_str();

        }
    }
        // cout <<(outputFileName)<<endl;
    bool exist_inputfile = (cmd.find('<') != string::npos);
    bool exist_outputfile = (cmd.find('>') != string::npos);

        // convert all command arguments to char* to fit in execvp()
    char* command_action = new char[full_command[0].size() + 1];

    strcpy(command_action, full_command[0].c_str());

    char** command_args = new char*[full_command.size()+1];

    if (full_command.size()!=1){
        // char** command_args[full_command.size()];6
            
        for (int i = 0; i<full_command.size(); i++){
            command_args[i] = new char[full_command[i].length() + 1];
            strcpy(command_args[i], full_command[i].c_str());
        }
        command_args[full_command.size()] = NULL;
    }
    
    
    // pid_t pid = fork();
    // if (pid < 0) {
    //     cerr << "Fork failed!" << std::endl;
    //     return true;
    // }
    if (cmd == ""){
        
    }
    else if (full_command[0] == "exit"){
        while (waitpid(-1, NULL, 0) > 0);

        child_processes_usage();
        cout << "Exiting" << endl;
        _exit(0);
        return false;
    }
    
    else if (full_command[0] == "jobs"){
        Allprocesses.cmd_jobs();
        child_processes_usage();
    }

    else if (full_command[0] == "kill"){
        if (full_command.size() < 2) {
            cerr << "Error: Please provide a PID to kill." << endl;
            // Go to the next iteration in the while loop
            return true;
        }
        pid_t pidToKill = stoi(full_command[1]);
        
        if (Allprocesses.killProcess(pidToKill)) {
            cout << "Successfully killed process with PID " << pidToKill << endl;
            } else {
            cerr << "Error: Failed to kill process with PID " << pidToKill 
            << ". It might not exist or you may not have the necessary permissions." << endl;
            }

    }
    else if (full_command[0] == "resume"){
        if (full_command.size() < 2) {
            cerr << "Error: Please provide a PID to kill." << endl;
            // Go to the next iteration in the while loop
            return true;
        }
        pid_t pidToKill = stoi(full_command[1]);
        
        if (Allprocesses.resumeProcess(pidToKill)) {
            cout << "Successfully resume process with PID " << pidToKill << endl;
            } else {
            cerr << "Error: Failed to kill process with PID " << pidToKill 
            << ". It might not exist or you may not have the necessary permissions." << endl;
            }
    }
    // else if (full_command[0] == "sleep"){
        
    // }
    else if (full_command[0] == "wait"){
        if (full_command.size() < 2) {
            cerr << "Error: Please provide a PID to kill." << endl;
            // Go to the next iteration in the while loop
            return true;
        }
        pid_t pidToKill = stoi(full_command[1]);
        
        if (Allprocesses.waitForProcess(pidToKill)) {
            cout << "Successfully wait for process with PID " << pidToKill << endl;
            } else {
            cerr << "Error: Failed to kill process with PID " << pidToKill 
            << ". It might not exist or you may not have the necessary permissions." << endl;
            }
    }
    else if (full_command[0] == "suspend"){
        if (full_command.size() < 2) {
            cerr << "Error: Please provide a PID to kill." << endl;
            // Go to the next iteration in the while loop
            return true;
        }
        pid_t pidToKill = stoi(full_command[1]);
        
        if (Allprocesses.suspendProcess(pidToKill)) {
            cout << "Successfully suspend process with PID " << pidToKill << endl;
            } else {
            cerr << "Error: Failed to kill process with PID " << pidToKill 
            << ". It might not exist or you may not have the necessary permissions." << endl;
            }
    }
    else {

        pid_t pid = fork();

        if (pid < 0) {
            cout << "Fork Failed" << endl;
            exit(1);
        } 
        
        if (pid == 0) {

            string completeCommand = full_command[0];

            for (int i = 1; i < full_command.size(); i++) {
                completeCommand += " " + full_command[i];
                }

            if (exist_inputfile) {
            // Redirecting input from file
                if(freopen(inputFileName.c_str(), "r", stdin) == nullptr){
                    perror("freopen for input");
                    exit(EXIT_FAILURE);
                }
                
            }
            
            if (exist_outputfile) {
                // Redirecting output to file
                if(freopen((outputFileName).c_str(), "w",stdout)==nullptr){
                    perror("freopen for output");
                    exit(EXIT_FAILURE);
                };
            } 

            execvp(command_action,command_args);
            
            perror("execvp");
            exit(EXIT_FAILURE);
        } 
        else {
            
            if (!background){
                Allprocesses.addProcess(cmd,pid,false);
                waitpid(pid,nullptr,0);
            }else{
                Allprocesses.addProcess(cmd,pid,true);
            }

            
            command_action_cleanup(command_action);
            if (full_command.size()>1){
                cleanup(command_args, full_command.size()); 
            }
            
        }
    } 

    return true;
}
