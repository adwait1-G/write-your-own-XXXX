/*
 * File: elfp_int.c
 *
 * Description: Definitions to all functions declared in elfp_int.h
 *
 * 		* Internal to the tool. Users should not use these functions.
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

#include "include/elfp_int.h"
#include "include/elfp.h"
#include "include/elfp_err.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

elfp_vector_main elfp_main_vector;

void
elfp_vector_init()
{
	int ret;
	
	/* NULLify all the pointers */
	for(int i = 0; i < ELFP_MAIN_VECTOR_SIZE; i++)
		elfp_main_vector.vec[i] = NULL;

	/* Let us start with 0 */
	elfp_main_vector.latest = 0;
}

int
add_elfp_main(elfp_main *main)
{
	if(main == NULL)
	{
		elfp_err_warn("add_elfp_main", "elfp_main passed is NULL");
		return -1;
	}

	int user_handle;

	/* Lock it */
	/* elfp_main_vector_lock(); */
	
	/* Addition code */
	/* Check for limit first */
	if(elfp_main_vector.latest == ELFP_MAIN_VECTOR_SIZE - 1)
	{
		elfp_err_warn("add_elfp_main", "Tool has reached it's limit of handling files");
		return -1;
	}

	elfp_main_vector.latest += 1;
	elfp_main_vector.vec[elfp_main_vector.latest] = main;
	
	user_handle = elfp_main_vector.latest;

	/* All done, let go of it! */
	/* elfp_main_vector_free(); */

	return user_handle;
}

elfp_main*
create_elfp_main()
{
	int ret;
	elfp_main *main = NULL;

	/* Allocate memory for it */
	main = calloc(1, sizeof(elfp_main));
	if(main == NULL)
	{
		elfp_err_warn("create_elfp_main", "calloc failed");
		return NULL;
	}

	/* Add it to the vector and get the user handle */
	ret = add_elfp_main(main);
	if(ret == -1)
	{
		elfp_err_warn("create_elfp_main", "add_elfp_main failed");
		return NULL;
	}

	/* Update the user handle */
	main->handle = ret;
	
	/* Allocate some memory to store the need-to-free addresses.
	 * I am guessing we won't have more than 30 objects to store */
	main->addrs = calloc(30, sizeof(void *));

	return main;
}

void
add_addr_to_main(elfp_main *main, elfp_ehdr *ehdr)
{
	main->addrs[main->n_addrs] = ehdr;
	main->n_addrs += 1;
}

elfp_main*
get_elfp_main(int handle)
{
	return (elfp_main_vector.vec[handle]);
}

int
verify_elfp_handle(int handle)
{
	/* Basic boundary checks */
	if(handle <= 0 || handle >= ELFP_MAIN_VECTOR_SIZE)
		return -1;
	
	/* A stronger boundary */
	if(handle > elfp_main_vector.latest)
		return -1;

	/* A handle inside boundaries doesn't mean it is a valid handle.*/
	elfp_main *main = elfp_main_vector.vec[handle];
	
	/* If the file has already been freed? */
	if(main == NULL)
		return -1;

	/* As of now, I can think of these checks. The checks only increase
	 * as the library becomes complex */

	return 0;
}











