#include <iostream>
#include <sys/wait.h> // for wait()
#include <signal.h>
#include <unistd.h>
#include <vector>
#include "cmd.h"
#include "process_table.h"
using namespace std;


int main(){
    signal(SIGCHLD, sigchld_handler);
    while (true){
        cout << "shell379> ";
        string cmd;
        getline(std::cin, cmd);


        if(!commands(cmd)){
            break;
        };

    }
    return 0;
}