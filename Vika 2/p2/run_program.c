#include "run_program.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ERROR_CODE 255

int run_program(char *file_path, char *argv[])
{
    int error_code;
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf("Error creating fork.");
    }
    if (pid == 0)
    {
        if (argv[0] == NULL)
        {
            execv(file_path, argv);
        }

    }
    else
    {
        wait(&wait);
        if (WEXITSTATUS(error_code) != NULL)
        {
            return WEXITSTATUS(error_code)
        }
        else
        {
            return 0;
        }
    }


    // remember to return ERROR_CODE on error.
    return ERROR_CODE;
}
