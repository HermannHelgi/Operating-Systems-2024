/* 
 * Group number (on canvas): 21
 * Student 1 name: Hermann Helgi
 * Student 2 name: Ágúst Máni
 */

#define _POSIX_C_SOURCE 2
#include "copy.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

// No need to change this. Parses argc into the CopyArgs structure
int parseCopyArgs(int argc, char * const argv[], CopyArgs* args)
{
	if (args == NULL) {
		return -1;
	}

	// Initialize with default values
	args->blocksize  = 4096;

	int opt;
	while ((opt = getopt(argc, argv, "b:")) != -1) {
		switch (opt) {
			case 'b':
				args->blocksize = atoi(optarg);

				if ((errno != 0) || (args->blocksize <= 0)) {
					return -1; // Invalid blocksize argument.
				}

				break;

			default: /* '?' */
				return -1;
		}
	}

	if (optind != argc - 2) {
		return -1; // We expect two parameters after the options.
	}

	args->from = argv[optind];
	args->to   = argv[optind + 1];

	return 0;
}


int doCopy(CopyArgs* args)
{

	char my_buffer[args->blocksize];
	int bytes_read;
	int bytes_written;

	struct stat current_status;
	
	if (args == NULL) {
		return -1;
	}

	int source_file = open(args->from, O_RDONLY);
	if ((fstat(source_file,&current_status)) == -1) // Geting permissions failed
	{
		close(source_file);
		return -1;
	}
	if (source_file == -1) // Fails to open original file
	{
		return -1;
	};
	

	int new_file = open(args->to, O_WRONLY | O_CREAT | O_EXCL, current_status.st_mode, current_status.st_mode);
	if (new_file == -1) // Fails to create new file
	{
		close(source_file);
		return -1;
	}

	bool sparse_file = false;
	while((bytes_read = read(source_file,my_buffer,args->blocksize)) > 0)
	{
		printf("%s",my_buffer);

		int empty_block = 1;
        for (int i = 0; i < bytes_read; i++) 
		{
            if (my_buffer[i] != 0) 
			{
                empty_block = 0;
				break;
            }
        }
		if (empty_block) 
		{
			sparse_file = true;
            if (lseek(new_file, args->blocksize, SEEK_CUR) == -1) 
			{
                close(source_file);
                close(new_file);
                return -1;
            }
			
		}
		else
		{
			bytes_written = write(new_file,my_buffer,bytes_read);
			if(bytes_read != bytes_written) //Write failed
			{
				close(source_file);
				close(new_file);
				return -1;
			}
		}

	}

	if (sparse_file)
	{
		write(new_file, 0, 1);
	}

	if (close(source_file) == -1 || close(new_file) == -1) // Failed to close files
	{
		return -1;
	}

	return 0;
}
