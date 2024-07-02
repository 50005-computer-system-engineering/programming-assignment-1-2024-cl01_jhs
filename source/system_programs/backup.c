#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    // Get the BACKUP_DIR environment variable
    char *backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL) {
        fprintf(stderr, "Error: BACKUP_DIR environment variable is not set.\n");
        return 1;
    }

    // Set project_dir to the current directory
    char *project_dir = "./";

    // Get the current datetime
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char datetime[20];
    strftime(datetime, sizeof(datetime), "%Y%m%d_%H%M%S", t);

    // Create the archive directory if it doesn't exist
    char archive_dir[1024];
    snprintf(archive_dir, sizeof(archive_dir), "%sarchive", project_dir);
    struct stat st = {0};
    if (stat(archive_dir, &st) == -1) {
        mkdir(archive_dir, 0700);
    }

    // Create the zip filename
    char zip_filename[1024];
    snprintf(zip_filename, sizeof(zip_filename), "%s_%s.zip", strrchr(backup_dir, '/') + 1, datetime);

    // Create the full path for the zip file
    char zip_filepath[1024];
    snprintf(zip_filepath, sizeof(zip_filepath), "%s/%s", archive_dir, zip_filename);

    // Create the zip command
    char command[2048];
    snprintf(command, sizeof(command), "zip -r %s %s", zip_filepath, backup_dir);

    // Execute the zip command
    int result = system(command);
    if (result == 0) {
        printf("Backup successful: %s\n", zip_filepath);
    } else {
        fprintf(stderr, "Error: Backup failed.\n");
        return 1;
    }

    return 0;
}



