#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// Function to get user information
void get_user_info() {
    struct passwd *pw;
    uid_t uid;
    uid = geteuid();
    pw = getpwuid(uid);

    if (pw) {
        printf("User: %s\n", pw->pw_name);
    } else {
        printf("Unable to find username for UID %u\n", (unsigned) uid);
    }
}

// Function to get system information
void get_system_info() {
    struct utsname buffer;

    if (uname(&buffer) != 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    printf("System: %s\n", buffer.sysname);
    printf("Node Name: %s\n", buffer.nodename);
    printf("Release: %s\n", buffer.release);
    printf("Version: %s\n", buffer.version);
    printf("Machine: %s\n", buffer.machine);
}

// Function to get memory information
void get_memory_info() {
    struct sysinfo info;

    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }

    unsigned long total_memory = info.totalram / 1024 / 1024;
    printf("Total RAM: %lu MB\n", total_memory);
}

// Function to get CPU information
void get_cpu_info() {
    char buffer[128];
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), cpuinfo)) {
        if (strncmp(buffer, "model name", 10) == 0) {
            char *cpu_model = strchr(buffer, ':') + 2;
            printf("CPU: %s", cpu_model);
            break;
        }
    }

    fclose(cpuinfo);
}

int main() {
    get_user_info();
    get_system_info();
    get_memory_info();
    get_cpu_info();

    return 0;
}


