#include "system_program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND "ps -efj | grep dspawn | grep -Ev 'tty|pts' "

int main()
{
    char buffer[128];
    int count = 0;
    FILE *fp; // file pointer 

    // Open the command for reading
    fp = popen(COMMAND, "r");
    if (fp == NULL)
    {
        //error message if command fails
        fprintf(stderr, "Failed to run command\n");
        exit(EXIT_FAILURE);
    }

    // Read the output a line at a time and count the lines
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        count++;
    }

    // Close the command stream
    pclose(fp);

    // Print the number of 'dspawn' processes found
    printf("Number of live dspawn daemon processes: %d\n", count);

    return EXIT_SUCCESS;
}

