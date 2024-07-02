#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_BUFFER_SIZE 1024
#define RESULT_BUFFER_SIZE 4096

int main() {
    char cmd[CMD_BUFFER_SIZE];
    char result[RESULT_BUFFER_SIZE];
    FILE *fp;

    // Command to get the count of live dspawn daemons
    snprintf(cmd, sizeof(cmd), "ps -ef | grep dspawn | grep -Ev 'tty|pts' | grep -v grep | wc -l");

    // Open the command for reading
    fp = popen(cmd, "r");
    if (fp == NULL) {
        perror("popen");
        return 1;
    }

    // Read the output
    if (fgets(result, sizeof(result) - 1, fp) != NULL) {
        printf("Number of live dspawn daemons: %s", result);
    }

    // Close the command stream
    pclose(fp);

    return 0;
}

