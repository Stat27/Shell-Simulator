# Shell379 - A Mini Shell Project

Shell379 is a minimalistic shell program that accepts and executes various commands. It allows you to manage processes, execute commands, and control their execution. This README provides an overview of the available commands and how to use them.

## Table of Contents

- [Commands](#commands)
  - [exit](#exit)
  - [jobs](#jobs)
  - [kill \<int\>](#kill-int)
  - [resume \<int\>](#resume-int)
  - [sleep \<int\>](#sleep-int)
  - [suspend \<int\>](#suspend-int)
  - [wait \<pid\>](#wait-pid)
  - [\<cmd\> \<arg\>*](#cmd-arg)
- [Special Arguments](#special-arguments)
- [Usage Examples](#usage-examples)

## Commands

### exit

End the execution of shell379. This command will wait until all processes initiated by the shell are complete. It will also print out the total user and system time for all processes run by the shell.

### jobs

Display the status of all running processes spawned by shell379. The format of the output is as follows:


### kill \<int\>

**Description:** Kill process \<int\>. This command allows you to terminate a specific process by providing its Process ID (PID).

### resume \<int\>

**Description:** Resume the execution of process \<int\>. This command undoes a suspend and allows a suspended process to continue its execution.

### sleep \<int\>

**Description:** Sleep for \<int\> seconds. This command will pause the shell's execution for the specified number of seconds.

### suspend \<int\>

**Description:** Suspend execution of process \<int\>. A resume command will reawaken the suspended process and allow it to continue its execution.

### wait \<pid\>

**Description:** Wait until process \<int\> has completed execution. This command will pause the shell's execution until the specified process has finished.

### \<cmd\> \<arg\>*

**Description:** Spawn a process to execute command \<cmd\> with 0 or more arguments \<arg\>. You can pass arguments to your commands as needed.

## Special Arguments

There are three special arguments that a command may have:

- `&`: If used, this must be the last argument and indicates that the command is to be executed in the background.
- `<fname`: This argument is the "<" character followed by a string of characters, indicating a file name to be used for program input.
- `>fname`: This argument is the ">" character followed by a string of characters, indicating a file name to be used for program output.

## Usage Examples

Here are some examples of how to use shell379 commands:

- To execute a command in the foreground:
ls -l


- To execute a command in the background(add & at the end):
gcc program.c -o program &


- To redirect input and output:
cat < input.txt > output.txt


- To check the status of running processes:
jobs


- To terminate a process:
kill 1234


- To wait for a specific process to complete:
wait 5678


