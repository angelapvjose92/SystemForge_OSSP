# ForgeOS – Linux Process Creation, Execution and Termination System

ForgeOS is a C-based command-line application developed for **Linux/Ubuntu** that demonstrates the complete life cycle of a Linux process, including process creation, execution, synchronization, and termination.

The application uses Linux/POSIX system calls to create child processes, execute selected Linux commands, wait for their completion, and retrieve their termination status.

> **Project:** Operating Systems and Systems Programming (25CS2104E)  
> **Section:** 1  
> **Team:** Team 7  
> **Team Members:**
> - Ili Harshika — `2520030607`
> - Gurrapu Nikshitha — `2520030512`
> - Angela Jose — `2520030365`

## Abstract

Linux operating systems support multiple processes running simultaneously. Understanding how processes are created, executed, synchronized, and terminated is an important concept in Operating Systems.

ForgeOS demonstrates these operations through a C-based application that uses fundamental Linux system calls. The program creates a child process using `fork()`, executes a selected command in the child process using `execvp()`, and makes the parent process wait for the child to finish using `wait()`.

After execution, the parent retrieves the child's termination status using `WIFEXITED()` and `WEXITSTATUS()`. The application also displays process information, including Process ID (PID) and Parent Process ID (PPID), to demonstrate the relationship between parent and child processes.

## Features

1. **Process Creation** – creates a new child process using the `fork()` system call.
2. **Parent-Child Relationship** – displays the PID and PPID to demonstrate the relationship between the parent and child processes.
3. **Process Execution** – executes selected Linux commands using the `execvp()` system call.
4. **Process Synchronization** – uses the `wait()` system call to make the parent process wait until the child process finishes execution.
5. **Process Termination** – demonstrates the termination of the child process after command execution.
6. **Exit Status Retrieval** – uses:
   - `WIFEXITED()` – checks whether the child process terminated normally.
   - `WEXITSTATUS()` – retrieves the child's exit status.
7. **Execution Result** – displays whether the selected command executed successfully.

## Technology Stack

| Component | Technology |
|---|---|
| Programming language | C |
| Operating system | Linux / Ubuntu |
| Compiler | GCC |
| Development environment | Visual Studio Code |
| Debugging tool | GDB |
| Process creation | `fork()` |
| Process execution | `execvp()` |
| Process synchronization | `wait()` |
| Termination status | `WIFEXITED()` and `WEXITSTATUS()` |

## Repository Structure

```text
ForgeOS/
├── Shellforge/
│   ├── main.c
│   └── README.md
└── docs/
    └── screenshots/
```

*Note: Update the source filename and directory structure above to match the actual files in your repository.*

## Setup and Execution

### 1. Open the project directory

```bash
cd SystemForge_OSSP/ForgeOS/Shellforge
```

### 2. Compile

```bash
gcc main.c -o forgeos
```

### 3. Run

```bash
./forgeos
```

The application allows the user to select Linux commands for execution through the process creation and execution mechanism.

Some commands used for testing include:

```bash
pwd
ls
ps
```

## Screenshots and Observed Output

The following examples describe the command executions observed during project testing.

### 1. Process Creation and Execution

The application creates a child process using `fork()` and executes the selected Linux command using `execvp()`.

### 2. Execution of pwd Command

The `pwd` command was executed and displayed the working directory.

The recorded execution demonstrated:

- Parent PID: `608`
- Child PID: `632`
- Execution function: `execvp()`
- Normal termination: `YES`
- Exit status: `0`
- Execution result: `SUCCESS`

### 3. Execution of ls Command

The `ls` command was executed through the child process, demonstrating that the application can execute Linux commands.

### 4. Execution of ps Command

The `ps` command displayed running processes, including:

- `bash` – PID 549
- `forgeos` – PID 608
- `ps` – PID 706

*Note: Process IDs and command outputs may differ between executions.*

## Test Cases and Results

| Test ID | Feature | Test input / action | Expected / observed result |
|---|---|---|---|
| TC01 | Process Creation | Execute `fork()` | Child process is created |
| TC02 | Process Execution | Execute `pwd` using `execvp()` | Working directory is displayed |
| TC03 | Process Execution | Execute `ls` | Directory contents are displayed |
| TC04 | Process Information | Execute `ps` | Running processes are displayed |
| TC05 | Process Synchronization | Execute `wait()` | Parent waits for child completion |
| TC06 | Normal Termination | Check using `WIFEXITED()` | Normal termination is identified |
| TC07 | Exit Status Retrieval | Check using `WEXITSTATUS()` | Exit status `0` is retrieved for successful execution |

The recorded successful execution of `pwd` returned exit status `0`.

## How It Works

### 1. Process Creation using fork()

The `fork()` system call creates a new child process from the existing parent process.

- The parent process continues its execution.
- The child process gets a separate Process ID.
- Both processes have their own execution paths.

### 2. Process Execution using execvp()

The `execvp()` system call replaces the child process's existing program with the selected Linux command.

For example, commands such as `pwd`, `ls`, and `ps` can be executed.

### 3. Process Synchronization using wait()

The `wait()` system call makes the parent process wait until the child process terminates.

This ensures that the parent does not proceed before the child finishes execution.

### 4. Process Termination

After executing the selected command, the child process terminates.

The parent process receives information about the child's termination through the wait status.

### 5. Exit Status Retrieval

Two macros are used to check the termination status:

- `WIFEXITED(status)` checks whether the child terminated normally.
- `WEXITSTATUS(status)` retrieves the exit status if the child terminated normally.

An exit status of `0` generally indicates successful execution.

## Process Execution Workflow

```text
          START
            |
            v
    User selects command
            |
            v
      fork() system call
            |
            v
    Child process created
            |
            v
       execvp() called
            |
            v
    Selected command executes
            |
            v
      Child terminates
            |
            v
       wait() called
            |
            v
   Retrieve termination status
            |
            v
       WIFEXITED()
            |
            v
      WEXITSTATUS()
            |
            v
    Display execution result
            |
            v
           END
```

## Limitations

- Designed specifically for Linux-based operating systems.
- Uses Linux/POSIX process-management system calls.
- Command execution depends on the availability of the selected command.
- Execution may fail if the selected command is invalid or unavailable.
- The application uses a command-line interface rather than a graphical interface.

## Future Enhancements

- Add support for executing multiple commands.
- Provide a graphical user interface.
- Display detailed process information.
- Add better error handling for invalid commands.
- Support additional process-management operations.
- Maintain execution history and process details.

## Conclusion

ForgeOS demonstrates the fundamental concepts of Linux process management through a C-based application. By implementing `fork()`, `execvp()`, and `wait()`, the project illustrates how processes are created, executed, synchronized, and terminated.

The use of `WIFEXITED()` and `WEXITSTATUS()` further demonstrates how a parent process can retrieve and understand the termination status of its child process.

Overall, the project provides practical experience with Linux system calls and helps develop an understanding of process management in operating systems.

## References

- Linux Manual Page: `fork(2)`
- Linux Manual Page: `exec(3)`
- Linux Manual Page: `wait(2)`
- Linux Manual Page: `exit(3)`
- GNU GCC Documentation
- Operating Systems and Systems Programming course materials
