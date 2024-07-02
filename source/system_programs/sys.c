#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/sysinfo.h>

int main() {
    struct utsname uname_data;
    struct sysinfo sys_info;
    struct passwd *pw;

    // Get system information
    if (uname(&uname_data) != 0) {
        perror("uname");
        return 1;
    }

    // Get system uptime, total RAM, and load averages
    if (sysinfo(&sys_info) != 0) {
        perror("sysinfo");
        return 1;
    }

    // Get current user
    pw = getpwuid(getuid());
    if (!pw) {
        perror("getpwuid");
        return 1;
    }

    printf("System Information:\n");
    printf("Operating System: %s\n", uname_data.sysname);
    printf("Node Name: %s\n", uname_data.nodename);
    printf("Kernel Version: %s\n", uname_data.release);
    printf("Architecture: %s\n", uname_data.machine);
    printf("Total RAM: %ld MB\n", sys_info.totalram / 1024 / 1024);
    printf("Uptime: %ld days, %ld hours, %ld minutes\n",
           sys_info.uptime / 86400,
           (sys_info.uptime % 86400) / 3600,
           (sys_info.uptime % 3600) / 60);
    printf("User: %s\n", pw->pw_name);

    return 0;
}
