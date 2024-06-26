// This changes the way some includes behave.
// This should stay before any include.
#define _GNU_SOURCE

#include "pipe.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork, pipe */
#include <stdlib.h> /* For exit */
#include <stdio.h>
#include <string.h>
          
#define buffer_size 1024

char *get_output(char *argv[])
{

    int pipefd[2];
    char buffer[buffer_size];

    char *ptr = malloc(buffer_size); //Return data

    if (ptr == NULL)
    {
        perror("malloc failed");
        return NULL;
    }

    if (pipe(pipefd) == -1) 
    {
        perror("pipe failed");
        return NULL;
    }

    int child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork failed");
        close(pipefd[0]);
        close(pipefd[1]);
        return NULL;
    } 

    else if (child_pid == 0) 
    {

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        if(execvp(argv[0], argv) == -1)
        {
            close(pipefd[0]);
	        perror("execvp failed");
            exit(255);
        }
    }
    else
    {
        //Close write to get so that operation called gets EOF 
        close(pipefd[1]);

        //Wait for child process
	    int status;
        waitpid(child_pid, &status, 0);
        
        ssize_t bytes_read = read(pipefd[0], buffer, buffer_size);
        
        close(pipefd[0]);
        close(pipefd[1]);
        
        if (bytes_read == -1) 
        {
            perror("read failed");
            return NULL;
        }
        //read data until new line or reach the end of the buffer_size (1024)
        else 
        {
            int i;
            for (i = 0; i < buffer_size && buffer[i] != '\n'; i++) 
            {
                ptr[i] = buffer[i];
            }
            ptr[i] = '\0'; // Null-terminate the string
        }
    }

    return ptr;
}
