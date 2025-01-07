# CSC360 p1 assignment


# Simple Shell Interpreter

A simple command-line shell implemented in C, can do foreground and background execution of commands, changing directories, and listing background processes.

### Commands

- cd: Change the current working directory.
    - If no argument is provided, it returns to the home directory.
- bg: Run a command in the background.
- bglist: List all currently running background processes.
- q, quit or exit: Exit the shell