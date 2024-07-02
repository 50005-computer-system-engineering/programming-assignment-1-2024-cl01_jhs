#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <pwd.h>
#include <sys/resource.h>

// Maximum length of a command line
#define MAX_LINE 1024
// Maximum number of arguments
#define MAX_ARGS 64

// Builtin command declarations
const char *builtin_commands[] = {
    "cd",    // Changes the current directory of the shell to the specified path. If no path is given, it defaults to the user's home directory.
    "help",  // List all builtin commands in the shell
    "exit",  // Exits the shell
    "usage", // Provides a brief usage guide for the shell and its built-in command
    "env", // Lists all the environment variables currently set in the shell
    "setenv", // Sets or modifies an environment variable for this shell session
    "unsetenv", // Removes an environment variable from the shell
    "history"// Displays the command history
};

// Function declarations
void read_command(char **cmd);
void type_prompt();
void add_to_history(char *cmd);
void show_history();

// Builtin command handler declarations
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_usage(char **args);
int list_env(char **args);
int set_env_var(char **args);
int unset_env_var(char **args);
int shell_history(char **args);

// Array of function pointers for builtin commands
int (*builtin_command_func[])(char **) = {
    &shell_cd,     // builtin_command_func[0]: cd
    &shell_help,   // builtin_command_func[1]: help
    &shell_exit,   // builtin_command_func[2]: exit
    &shell_usage,  // builtin_command_func[3]: usage
    &list_env,     // builtin_command_func[4]: env
    &set_env_var,  // builtin_command_func[5]: setenv
    &unset_env_var, // builtin_command_func[6]: unsetenv
    &shell_history // builtin_command_func[7]: history
};

// Helper function to figure out how many builtin commands are supported by the shell
int num_builtin_functions()
{
    return sizeof(builtin_commands) / sizeof(char *);
}

#endif // SHELL_H
