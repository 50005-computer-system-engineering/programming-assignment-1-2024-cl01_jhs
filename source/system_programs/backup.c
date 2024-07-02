#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "cp -r %s %s", argv[1], argv[2]);

    if (system(command) == -1) {
        perror("system");
        return 1;
    }

    printf("Backup from %s to %s completed.\n", argv[1], argv[2]);
    return 0;
}

