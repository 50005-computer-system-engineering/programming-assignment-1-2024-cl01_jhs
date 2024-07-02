#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "system_program.h"
#include <linux/limits.h>

char output_file_path[PATH_MAX];

static int create_daemon()
{
    pid_t pid;

    // Step 1: Fork the parent process
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    // Step 2: Exit the parent process
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Step 3: On child process (intermediate process), call setsid() to lose controlling TTY
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    // Step 4: Ignore SIGCHLD and SIGHUP
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Step 5: Fork again, parent (intermediate) process terminates
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Step 6: Set new file permissions
    umask(0);

    // Step 7: Change the working directory to root
    if (chdir("/") < 0)
        exit(EXIT_FAILURE);

    // Step 8: Close all open file descriptors
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    {
        close(x);
    }

    // Attach file descriptors 0, 1, and 2 to /dev/null
    open("/dev/null", O_RDWR); // stdin
    dup(0); // stdout
    dup(0); // stderr

    return 0;
}


static int daemon_work()
{
    int num = 0;
    FILE *fptr;

    // Write PID of daemon at the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon started with PID %d\n", getpid());
    fclose(fptr);

    while (1)
    {
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d: Daemon writing line %d to the file.\n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // Terminate after 10 iterations
            break;
    }

    return EXIT_SUCCESS;
}



int main(int argc, char **args)
{
    // Setup path to the log file
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log");

    // Create daemon process
    if (create_daemon() < 0)
    {
        perror("Failed to create daemon");
        return 1;
    }

    // Open the log file and log daemon start message
    openlog("summond", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Daemon started.");
    closelog();

    // Start the daemon work
    return daemon_work();
}