#include "shell.h"
#include <time.h>
#include <unistd.h>


// Function to read a command from the user input
void read_command(char **cmd)
{
    char line[MAX_LINE];
    int count = 0, i = 0;
    char *array[MAX_ARGS], *command_token;

    for (;;)
    {
        int current_char = fgetc(stdin);
        line[count++] = (char)current_char;
        if (current_char == '\n')
            break;
        if (count >= MAX_LINE)
        {
            printf("Command is too long, unable to process\n");
            exit(1);
        }
    }
    line[count] = '\0';

    if (count == 1)
        return;

    add_to_history(line); // add this command to history 

    command_token = strtok(line, " \n");

    while (command_token != NULL)
    {
        array[i++] = strdup(command_token);
        command_token = strtok(NULL, " \n");
    }

    for (int j = 0; j < i; j++)
    {
        cmd[j] = array[j];
    }
    cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
    static int first_time = 1;
    if (first_time)
    {
    
  //Additional feature implementation - Decoration of prompt
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Get current time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        // Get current user
        struct passwd *pw = getpwuid(getuid());
        char *user = pw ? pw->pw_name : "unknown";

        printf("[%s@%s %02d:%02d:%02d %s]$$ ", user, cwd, tm.tm_hour, tm.tm_min, tm.tm_sec, cwd);
    } else {
        perror("getcwd");
    }
    fflush(stdout);

    }}

// Handler for the 'cd' command
int shell_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
    }
    return 1;
}

// Handler for the 'help' command
int shell_help(char **args)
{
    printf("Builtin commands:\n");
    for (int i = 0; i < num_builtin_functions(); i++)
    {
        printf("  %s\n", builtin_commands[i]);
    }
    return 1;
}

// Handler for the 'exit' command
int shell_exit(char **args)
{
    return 0;
}

// Handler for the 'usage' command
int shell_usage(char **args)
{
    if (args[1] == NULL)
    {
        printf("usage: command not given. Type usage <command>\n");
        printf("usage: cd directory_name to change the current working directory of the shell\n");
        printf("usage: help for supported commands\n");
        printf("usage: exit to terminate the shell gracefully\n");
        printf("usage: usage to show how to use shell commands\n");
        printf("usage: env to list all registered environment variables\n");
        printf("usage: setenv ENVNAME VALUE to set a new environment variable\n");
        printf("usage: unsetenv ENVNAME to remove this env from the list of env variables\n");
    }
    else
    {
        if (strcmp(args[1], "cd") == 0)
        {
            printf("usage: cd directory_name to change the current working directory of the shell\n");
        }
        else if (strcmp(args[1], "help") == 0)
        {
            printf("usage: help for supported commands\n");
        }
        else if (strcmp(args[1], "exit") == 0)
        {
            printf("usage: exit to terminate the shell gracefully\n");
        }
        else if (strcmp(args[1], "usage") == 0)
        {
            printf("usage: usage to show how to use shell commands\n");
        }
        else if (strcmp(args[1], "env") == 0)
        {
            printf("usage: env to list all registered environment variables\n");
        }
        else if (strcmp(args[1], "setenv") == 0)
        {
            printf("usage: setenv ENVNAME VALUE to set a new environment variable\n");
        }
        else if (strcmp(args[1], "unsetenv") == 0)
        {
            printf("usage: unsetenv ENVNAME to remove this env from the list of env variables\n");
        }
        else
        {
            printf("usage: command %s not found. Type usage <command>\n", args[1]);
        }
    }
    return 1;
}


// Handler for the 'env' command
int list_env(char **args)
{
    extern char **environ;
    char **env = environ;

    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    return 1;
}

// Handler for the 'setenv' command
int set_env_var(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: setenv <var> <value>\n");
    }
    else
    {
        // Check if the argument contains '='
        char *equal_sign = strchr(args[1], '=');

        if (equal_sign == NULL)
        {
            fprintf(stderr, "setenv: incorrect format. Use KEY=VALUE\n");
        }
        else
        {
            // Splitting the argument into key and value
            *equal_sign = '\0'; // Replace '=' with '\0' to separate key and value
            char *key = args[1];
            char *value = equal_sign + 1;

            // Remove quotes from the value if they exist
            int len = strlen(value);
            if (len >= 2 && value[0] == '"' && value[len - 1] == '"')
            {
                value[len - 1] = '\0'; // Remove the ending quote
                value++; // Move pointer past the starting quote
            }

            if (setenv(key, value, 1) != 0)
            {
                perror("setenv");
            }
        }
    }
    return 1;
}
//function to print resource usage of child processes 
void print_resource_usage() {
    struct rusage usage;
    if (getrusage(RUSAGE_CHILDREN, &usage) == 0) {
        printf("\nResource Usage:\n");
        printf("CPU Time:\n");
        printf("  User CPU time used: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("  System CPU time used: %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        
        printf("Memory Usage:\n");
        printf("  Maximum resident set size: %ld KB\n", usage.ru_maxrss);
        
        printf("Disk I/O:\n");
        printf("  Block input operations: %ld\n", usage.ru_inblock);
        printf("  Block output operations: %ld\n", usage.ru_oublock);
    } else {
        perror("getrusage");
    }
}

// Handler for the 'unsetenv' command
int unset_env_var(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: unsetenv <var>\n");
    }
    else
    {
        if (unsetenv(args[1]) != 0)
        {
            perror("unsetenv");
        }
    }
    return 1;
}

//History functionality 
#define HISTORY_SIZE 100
char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(char *cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(cmd);
    } else {
        // Shift history to make room for new command
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

// Handler for the 'history' command
int shell_history(char **args)
{
    show_history();
    return 1;
}

//function to read commands from a .cseshellrc file 
int readrc()
{   
    char line[MAX_LINE];
    const char *filePath = ".cseshellrc"; // Specify the path to your text file

    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    while (fgets(line, sizeof(line), file))
    {
        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\r')
            continue;

        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;

        char *args[MAX_ARGS]; 
        char *command_point;
        int i = 0;

        command_point = strtok(line, " \n");

        while (command_point != NULL)
        {
            args[i++] = command_point;  // Duplicate the token and store it
            command_point = strtok(NULL, " \n");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;

        if (strncmp(args[0], "PATH", 4) == 0)
        {
            set_env_var(args);
        }
        else
        {
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork failed");
                _exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                execvp(args[0], args);
                printf("cseshell: command not found: %s\n", args[0]);
                _exit(EXIT_SUCCESS); // Exit child process
            }
            else
            {
                // Parent process
                int status;
                waitpid(pid, &status, 0); // Wait for child process to finish
            }
        }
    }
    fclose(file); // Close the file
    return EXIT_SUCCESS;
}


// The main function where the shell's execution begins
int main(void)
{
    // Define an array to hold the command and its arguments
    char *cmd[MAX_ARGS];
    int child_status;

    readrc();

    
    pid_t pid;


    // Move cwd declaration outside the if block
    char cwd[1024]; // Current working directory

    while (1) // Infinite loop
    { 
        type_prompt();     // Display the prompt
        read_command(cmd); // Read a command from the user

        // If the command is empty, skip execution
        if (cmd[0] == NULL)
            continue;

        // Check if the command is a built-in command
        int is_builtin = 0;
        for (int i = 0; i < num_builtin_functions(); i++)
        {
            if (strcmp(cmd[0], builtin_commands[i]) == 0)
            {
                is_builtin = 1;
                if ((*builtin_command_func[i])(cmd) == 0)
                {
                    return 0; // Exit the shell
                }
                break;
            }
        }

        // If the command is not a built-in, execute it
        if (!is_builtin)
        {
            // Formulate the full path of the command to be executed
            char full_path[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                snprintf(full_path, sizeof(full_path), "%s/bin/%s", cwd, cmd[0]);
            }
            else
            {
                printf("Failed to get current working directory.");
                exit(1);
            }

            ////////////////////////////////////////////////////////////
            // Fork the shell process and execv in child process
            pid = fork();
            // printf("pid: %d\n", pid);

            if (pid == 0) // Child process
            {
                execv(full_path, cmd);

                // If execv returns, command execution has failed
                printf("Command %s not found\n", cmd[0]);
                exit(1);
            }
            else if (pid > 0) //Parent process
            {
                pid_t waited_pid = waitpid(pid, &child_status, WUNTRACED); 
                // waitpid(pid, *status, options) system call: wait until a child specified by pid argument has changed state.
                // wait(*status) system call: wait until one of its children terminates.
                if (waited_pid == -1)
                {
                    perror("waitpid failed");
                    exit(1);
                }

                // Inspect the child's exit status
                if (WIFEXITED(child_status))
                {
                    int child_exit_status = WEXITSTATUS(child_status);
                    if (child_exit_status == 0)
                    {
                        printf("Child process exited successfully\n");
                    }
                    else
                    {
                        printf("Child process exited with status %d\n", child_exit_status);
                        print_resource_usage();
                        // Print resource usage after the child process has exited
                        //print_resource_usage();
                        // Print resource usage after the child process has exited
                        
                    }
                }
            }
            else
            {
                printf("Failed to fork process.\n");
                exit(1);
            }
            ////////////////////////////////////////////////////////
        }

        // Free the allocated memory for the command arguments before exiting
        for (int i = 0; cmd[i] != NULL; i++)
        {
            print_resource_usage();
            free(cmd[i]);
        }
        memset(cwd, '\0', sizeof(cwd)); // clear the cwd array
    }
    return 0;
}





