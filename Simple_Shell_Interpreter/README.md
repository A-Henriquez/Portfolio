# Simple Shell Interpreter

## Project Overview

The goal of the project is to implement a simple command-line shell interpreter in C, which supports running commands both in the foreground and in the background. This shell also supports basic directory traversal and process management, which are essential concepts in operating systems. This project was and assignment for the CSC 360 Operating Systems course.

### Key Features:
- **Foreground and Background Execution**: Run processes either in the foreground or in the background.
- **Directory Navigation**: Change the current working directory and return to the home directory when no argument is provided to the `cd` command.
- **Background Process Management**: List all running background processes with the `bglist` command.
- **Exit**: Exit the shell using `q`, `quit`, or `exit`.

### Commands:
- **cd**: Change the current working directory.
    - If no argument is provided, it returns to the home directory.
- **bg**: Run a command in the background.
- **bglist**: List all currently running background processes.
- **q, quit, or exit**: Exit the shell.

### Technologies Used:
- **C Programming Language**: The shell is implemented in C.
- **System Calls**: Key system calls such as `fork()`, `exec()`, `wait()`, `chdir()`, and `getcwd()` are utilized to manage processes and navigate the file system.
