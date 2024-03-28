#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#include "multi_mutex.h"

int multi_mutex_unlock(pthread_mutex_t **mutexv, int mutexc)
{
    int error;
    for (int i = 0; i < mutexc; i++)
    {
        error = pthread_mutex_unlock(mutexv[i]);
        if (error != 0)
        {
            return -1;
        }
    }

    return 0;
}

int multi_mutex_trylock(pthread_mutex_t **mutexv, int mutexc)
{
    int result;
    for (int i = 0; i < mutexc; i++)
    {
        result = pthread_mutex_trylock(mutexv[i]);

        if (result != 0) {
            for (int j = 0; j < i; j++)
            {
                pthread_mutex_unlock(mutexv[j]);
            }
            return -1;
        }
    }

    return 0;
}