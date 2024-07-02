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

static int daemon_work()
{
    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // Write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // Then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
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

        if (num == 10) // We just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int main()
{
    pid_t pid, sid; //process id, session id
    int i;

    // Setup path
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log");

    // Fork the parent process
    pid = fork();
    if (pid < 0)
    {
        perror("First fork failed\n");
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then we can exit the parent process
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // At this point we are executing as the child process (intermediate process)
    // Create a new session
    sid = setsid(); //becomes new session leader, loses controlling TTY - setsid() syscall
    if (sid < 0)
    {
        perror("setsid failed\n");
        exit(EXIT_FAILURE);
    }

    // Ignore SIGCHLD and SIGHUP
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // Fork off for the second time
    pid = fork();
    if (pid < 0)
    {
        perror("Second fork failed\n");
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then we can exit the parent process
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // At this point we are executing as the grandchild (daemon) process
    printf("\n -> Daemon process created successfully\n");

    // Change the file mode mask
    umask(0);

    // Change the working directory to the root directory
    if ((chdir("/")) < 0)
    {
        perror("chdir failed\n");
        exit(EXIT_FAILURE);
    }

    // Close all open file descriptors
    for (i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
    {
        close(i);
    }

    // Redirect file descriptors 0, 1, 2 to /dev/null
    open("/dev/null", O_RDWR); // stdin
    dup(0); // stdout
    dup(0); // stderr

    // Execute the daemon work
    if (daemon_work() != EXIT_SUCCESS)
    {
        perror("daemon_work failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
