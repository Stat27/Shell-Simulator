#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <cstring>

void sigchld_handler(int signo);
// Splits the string 's' based on the delimiter 'delim' and returns a vector of tokens
std::vector<std::string> split(const std::string &s, char delim);

// Clean up the memory allocated for the command arguments
void cleanup(char** tokens, int size);

// Clean up the memory allocated for the command action
void command_action_cleanup(char* token);

// Executes the provided command and handles special commands like "exit", "jobs", etc.
// Returns 'false' if the "exit" command is given, otherwise 'true'
bool commands(std::string cmd);

#endif // CMD_H
