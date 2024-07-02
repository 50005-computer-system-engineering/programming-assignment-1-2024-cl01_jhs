#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (stat(argv[1], &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("Directory %s exists.\n", argv[1]);
    } else {
        printf("Directory %s does not exist.\n", argv[1]);
    }

    return 0;
}
