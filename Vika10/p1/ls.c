/* 
 * Group number (on canvas): 21
 * Student 1 name: Hermann Helgi
 * Student 2 name: Ágúst Máni
 */

#define _DEFAULT_SOURCE

#include "ls.h"

// You may not need all these headers ...
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int list(const char* path, int recursive)
{
	(void) recursive;
	DIR *opened_directory;
	int error;
	opened_directory = opendir(path);
	if (opened_directory == NULL)
	{
		printf("\nHERE\n");
		return -1;
	}

	struct dirent *new_file;
	struct stat new_file_statistics;
	size_t size;
	char* full_path_and_name = malloc(sizeof(char) * (MAX_FILE_NAME_LENGTH + strlen(path)));
	char type_str[MAX_FILE_NAME_LENGTH];

	new_file = readdir(opened_directory);
	while (new_file != NULL)
	{
		strcpy(full_path_and_name, path);
		strcat(full_path_and_name, new_file->d_name);
		error = fstatat(dirfd(opened_directory), path, &new_file_statistics, 0);
		if (error != 0)
		{
			printf("\nHERE 2\n");
			free(full_path_and_name);
			return error;
		}
		size = new_file_statistics.st_size;
		
		if (new_file->d_type == DT_FIFO)
		{
			strcpy(type_str, "|");
		}
		else if (new_file->d_type == DT_DIR)
		{
			strcpy(type_str, "/");
		}
		else if (new_file->d_type == DT_LNK)
		{
			strcpy(type_str, " -> ");
			// Not really sure how i should do this...
		}
		else if (new_file_statistics.st_mode & 0111)
		{
			strcpy(type_str, "*");
		}
		
		_printLine(size, full_path_and_name, 0);
		new_file = readdir(opened_directory);
	}

	error = closedir(opened_directory);
	free(full_path_and_name);
	printf("\nHERE 3\n");
	return error;
}
