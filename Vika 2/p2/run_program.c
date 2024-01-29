#include "run_program.h"
#include <stdio.h>
#include <stdlib.h>
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
        if(argv[0] == NULL)
        {
            char *argv_new[] = { file_path, NULL };
            execvp(file_path, argv_new);
        }
        else
        {
            char *argv_new[] = { file_path, NULL };
            int i = 0;
            while (argv[i] != NULL) {
                argv_new[i+1] = argv[i];
                i++;
            }
            argv_new[i+1] = '\0';
            execvp(file_path, argv_new);
        }

        exit(ERROR_CODE);
    }
    else
    {
        pid_t wait_result = wait(&child_status);

        if (wait_result == -1)
        {
            return ERROR_CODE;
        }

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
