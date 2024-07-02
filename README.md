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



## Additional Functions
1. Decorate the Prompt :
Decorating the shell prompt involves enhancing the appearance and informational content displayed when the user is prompted to enter a command. The prompt decoration is handled by type_prompt function which displays useful information like the current user, hostname ,current working directory and the current time.

Example : 
![image](https://github.com/50005-computer-system-engineering/programming-assignment-1-2024-cl01_jhs/assets/140350660/c4dac3b4-67e1-404f-b3b1-fa4e2d6d5569)

2. Command History configuration :
Command history is a crucial feature for any shell, keeping the record of all commands entered by the user and displaying the history of entered commands.

Example : 
![image](https://github.com/50005-computer-system-engineering/programming-assignment-1-2024-cl01_jhs/assets/140350660/476de43b-4041-4b76-953e-20e5450dfd17)

## Sustainability Idea: Resource Usage Feedback

### Enhancing the Shell with Resource Statistics:

Purpose: This feature aims to make users aware of how much system resources their commands consume.

How it works: After each command is executed, the shell will display detailed statistics about:

CPU Time: How much processing power the command used.

Memory Usage: The amount of RAM the command required.

Disk I/O: The read and write operations performed on the disk.

Benefits: Users will be able to see the impact of their commands on system resources, encouraging more efficient usage and promoting overall sustainability.

### Displaying Resource Usage Feedback:

Purpose: To give users immediate feedback on the resource consumption of their commands.

How it works: After running a command, the shell will automatically show a summary of resource usage.

```bash
$$ ls
file1.txt  file2.txt  file3.txt
Resource Usage:
- CPU Time: 0.001 seconds
- Memory Usage: 2 MB
- Disk I/O: 1 read, 0 writes
```

Example :

![image](https://github.com/50005-computer-system-engineering/programming-assignment-1-2024-cl01_jhs/assets/140350660/5416dc41-e265-4172-b1a9-9e6313710180)


## Inclusivity Idea: Customizable Interface
### Customizable Appearance and Behavior:

Purpose: To make the shell more accessible and comfortable for a diverse range of users.

Features:

Change Text Size: Users can adjust the size of the text in the shell to suit their preferences.

Color Schemes: Users can choose from various color schemes to enhance readability and reduce eye strain.

How it works: Users will be able to customize these settings through simple shell commands or configuration files.

Example Commands:
```bash
$$ set text_size 16
Text size set to 16

$$ set color_scheme dark
Color scheme set to dark
```

## Benefits:

Improved Accessibility: Users with visual impairments or preferences for certain text sizes and colors will find the shell easier to use.
Enhanced Comfort: Customization options will make prolonged use of the shell more comfortable, reducing fatigue and improving productivity.
Inclusivity: By accommodating different needs and preferences, the shell becomes more inclusive and user-friendly for a wider audience.

## Future Suggestions 
1. Sustainability: Implement more energy-efficient algorithms and further optimize resource usage for various commands.
2. Inclusivity: Add more customization options, including support for different languages and enhanced accessibility features.
