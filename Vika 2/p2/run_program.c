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

    printf("file_path (argv[0]): %s\n", argv[0]);
    printf("file_path variable: %s\n", file_path);

    for (int i = 0; argv[i] != NULL; ++i) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }



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
