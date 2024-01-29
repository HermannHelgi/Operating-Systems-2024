#include "run_program.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERROR_CODE 255

int run_program(char *file_path, char *argv[])
{
    int child_status;
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Error creating fork.");
        return ERROR_CODE;
    }
    if (pid == 0)
    {
        execvp(file_path, argv);

        exit(ERROR_CODE);
    }
    else
    {
        wait(&child_status);
        if (WIFEXITED(child_status))
        {
            return WEXITSTATUS(child_status);
        }
        else
        {
            return ERROR_CODE;
        }
    }
}
