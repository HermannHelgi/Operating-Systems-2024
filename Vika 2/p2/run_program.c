#include "run_program.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define ERROR_CODE 255

int run_program(char *file_path, char *argv[])
{

    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Error creating fork.");
    }
    if (pid == 0)
    {
        printf("Child doing stuff");
        execv(file_path, argv);
    }
    else
    {
        printf("parent waiting");
        wait(NULL);
        printf("parent done waiting");
        return 1;
    }


    // remember to return ERROR_CODE on error.
    return ERROR_CODE;
}
