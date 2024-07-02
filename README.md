[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/u16ttUuk)
# CSEShell

CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`find`, `ld`, `ldr`), and some test files.

## Compiling and Running the shell 

After Cloning the repo, cd to the repo.

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make
```

```bash
make clean
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

The shell supports several features : 

## Built in functions like : 

cd <directory_name>: Changes the current working directory.

help: Displays the list of built-in commands.

exit: Exits the shell.

usage <command>: Provides usage information for a specific command.

env: Lists all registered environment variables.

setenv KEY=VALUE: Sets a new environment variable.

unsetenv KEY: Removes an environment variable.

## System Programs

find.c - Searches for files in a directory.

ld.c - Lists the contents of the current directory.\

ldr.c - Lists the contents of the current directory recursively.

dspawn.c - Spawns a daemon process that logs messages to a file periodically.

dcheck.c - Checks if the daemon process is running and reports its status.

sys.c - Displays system information such as user, system, memory, and CPU details.

backup.c - Creates a zip archive of the directory specified by the BACKUP_DIR environment variable and moves it to the archive directory.

Each program can be executed from the CSEShell once it is running. This starter code only allows the shell to execute a command once before exiting because `execv` replace the entire process' address space. Students need to fix this and allow the shell to prompt for more commands in Programming Assignment 1.

## Additional Functions

## Sustainability and inlcusivit
