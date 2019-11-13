/*
 * File: main.c
 *
 * Description: Has definitions to a few functions defined in elfp.h
 *
 * License: 
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                  Version 2, December 2004
 *  
 * Copyright (C) 2019 Adwaith Gautham <adwait.gautham@gmail.com>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *          DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

#include "include/elfp.h"
#include "include/elfp_err.h"
#include "include/elfp_int.h"


void
elfp_init()
{	
	elfp_vector_init();
}

void
elfp_fini()
{
	/* Check if there is anything to do */
	if(elfp_main_vector.latest == 0)
		return;

	int i, j;
	elfp_main *main = NULL;
	int ret;

	/* For every ELF file in the vector, free everything related to that */
	for(i = 1; i <= elfp_main_vector.latest; i++)
	{	
		main = elfp_main_vector.vec[i];
		/* Check if the main structure in hand is already free */
		if(main == NULL)
			continue;
	
		else
			elfp_close(i);
	}
}

	
int
elfp_open(const char *elfp_elf_path)
{	
	/* All variables needed */
	int ret;
	elfp_main *main = NULL;
	int fd;
	int user_handle;
	unsigned char magic[4];
	struct stat st;
	void *mmap_addr = NULL;
	

	if(elfp_elf_path == NULL)
	{
		elfp_err_warn("elfp_open", "File Path is NULL");
		return -1;
	}
	
	/* See if the file can be read or not */
	ret = access(elfp_elf_path, R_OK);
	if(ret != 0)
	{
		elfp_err_warn("elfp_open", "access() to the file failed");
		return -1;
	}
	
	/* Let us open the file */
	ret = open(elfp_elf_path, O_RDONLY);
	if(ret == -1)
	{
		elfp_err_warn("elfp_open", "open() to the file failed");
		return -1;
	}
	fd = ret;

	/* Now, check if this is really an ELF file.
	 * Very basic magic number check! */
	ret = read(fd, magic, 4);
	if(ret == -1)
	{
		elfp_err_warn("elfp_open", "read() to the file failed");
		return -1;
	}
	
	ret = (magic[0] == 0x7f) && (magic[1] == 0x45) && (magic[2] == 0x4c)
							&& (magic[3] == 0x46);
	if(ret == 0)
	{
		elfp_err_warn("elfp_open", "ELF Magic characters not identified");
		return -1;
	}

	/* Create a new elfp_main object and update stuff */
	main = create_elfp_main();
	main->fd = fd;
	strncpy(main->path, elfp_elf_path, ELFP_FILEPATH_SIZE);

	/* 'main' should already have the user handle updated in it */
	user_handle = main->handle;

	/* File out the size of the file and update main */
	ret = fstat(fd, &st);
	if(ret == -1)
	{
		elfp_err_warn("elfp_open", "fstat() on the file failed");
		return -1;
	}
	main->file_size = st.st_size;

	/* Let us load the file */
	mmap_addr = mmap(NULL, main->file_size, PROT_READ, MAP_PRIVATE,
				main->fd, 0 /* Beginning of file */);

	if(mmap_addr == MAP_FAILED)
	{
		elfp_err_warn("elfp_open", "mmap() on the file failed");
		return -1;
	}
	main->start_addr = mmap_addr;

	/* We are all set now. Let us send back the user-handle */

	return user_handle;
}


int
elfp_close(int user_handle)
{	
	elfp_main *main = NULL;
	int ret;
	int j;
	
	ret = verify_elfp_handle(user_handle);
	if(ret == -1)
	{
		elfp_err_warn("elfp_close", 
				"The handle didn't pass the sanity check");
		return -1;
	}

	main = elfp_main_vector.vec[user_handle];
	
	/* Check if it already has been cleaned up */
	if(main == NULL)
		return 0;

	/* Unmap the mapped ELF File. 
         * If clean-up functions fail, then only OS should take care of it */
	
        ret = munmap(main->start_addr, main->file_size);
        if(ret == -1)
        {
                elfp_err_warn("elfp_fini", "munmap failed");
        }

        if(main->n_addrs > 0)
        {
                for(j = 0; j < main->n_addrs; j++)
                        free(main->addrs[j]);
        }
	
	/* addrs is an calloc'd chunk. Thanks valgrind! */
	free(main->addrs);

        /* Everything inside main is freed. Now eliminate main */
        free(main);
	elfp_main_vector.vec[user_handle] = NULL;
	
	return 0;
}
