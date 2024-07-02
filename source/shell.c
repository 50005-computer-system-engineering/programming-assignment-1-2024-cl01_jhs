#include "shell.h"

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
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        first_time = 0;
    }
    fflush(stdout);
    printf("$$ ");
}

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
// Handler for the 'setenv' command
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

// The main function where the shell's execution begins
int main(void)
{
    char *cmd[MAX_ARGS];
    int status;

    // Get the initial working directory
    char initial_cwd[PATH_MAX];
    if (getcwd(initial_cwd, sizeof(initial_cwd)) == NULL)
    {
        perror("getcwd");
        exit(1);
    }

    do
    {
        type_prompt();
        read_command(cmd);

        if (cmd[0] == NULL)
        {
            continue;
        }

        int is_builtin = 0;
        for (int i = 0; i < num_builtin_functions(); i++)
        {
            if (strcmp(cmd[0], builtin_commands[i]) == 0)
            {
                status = (*builtin_command_func[i])(cmd);
                is_builtin = 1;
                break;
            }
        }

        if (!is_builtin)
        {
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
            if (pid == 0)
            {
                char full_path[PATH_MAX];
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL)
                {
                    char full_path[PATH_MAX];
                    snprintf(full_path, sizeof(full_path), "%s/source/system_programs/%s", initial_cwd, cmd[0]);
                    printf("Trying to execute: %s\n", full_path); // Debug print
                    execv(full_path, cmd);
                    perror("execv");
                    exit(1);
                }
                else
                {
                    printf("Failed to get current working directory.");
                }
                exit(1);
            }
            else
            {
                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                {
                    int exit_status = WEXITSTATUS(status);
                    printf("Child process exited with status %d\n", exit_status);
                }
            }
        }

        for (int i = 0; cmd[i] != NULL; i++)
        {
            free(cmd[i]);
        }
    } while (status);

    return 0;
}

