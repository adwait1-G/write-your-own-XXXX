/*
 * File: elfp_int.h
 *
 * Description: Data structures needed to maintain open files, metadata 
 * 		related to files.
 *
 * 		* Internal to the tool. User should not touch these structures.
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

#ifndef _ELFP_INT_H
#define _ELFP_INT_H

#include <semaphore.h>

#include "elfp.h"

/*
 * Important macros.
 */
#define ELFP_FILEPATH_SIZE	256
#define ELFP_MAIN_VECTOR_SIZE	1000
#define ELFP_FREE_LIST_SIZE	100

/*
 * struct elfp_main: An open file's metadata.
 */

typedef struct elfp_main
{	
	/* File descriptor of the open file */
	int fd;

	/* File Path */
	char path[ELFP_FILEPATH_SIZE];

	/* Other details related to file */
	unsigned long int file_size;

	/* Starting address of mmap */
	unsigned char *start_addr;

	/* elfp_ehdr */
	elfp_ehdr *ehdr;

	/* elfp_phdr_tbl */
	elfp_phdr_tbl *phdr_tbl;

	/* Handle sent to the user */
	int handle;
	
	/* Many functions allocate objects in heap and return the pointer
	 * to it to the user.
	 *
	 * Need to keep track of all this memory and free all of it in the end.
	 */

	void **addrs;
	unsigned long int n_addrs;
} elfp_main;


/*
 * struct elfp_main_vector: A vector of elfp_main structure.
 *
 * Needed to handle multiple open files.
 */
typedef struct elfp_vector_main
{
	/* Array of pointer to elfp_main structure */
	elfp_main *vec[ELFP_MAIN_VECTOR_SIZE];

	/* Handle of the latest opened file.
	 * This is NOT the total number of open files. */
	unsigned long int latest;

	/* The above members can be accessed by multiple threads
	 * at the same time. There will be a race condition when that happens.
	 * So, a semaphore to take care of it. */

	sem_t *sem;

} elfp_vector_main;

extern elfp_vector_main elfp_main_vector;

/*
 * The following are the functions needed to manipulate the vector structure.
 */

/*
 * elfp_vector_init: Initializes the library.
 */

void
elfp_vector_init();

/*
 * elfp_main_vector_lock(): Locks the vector('s semaphore).
 */
void
elfp_main_vector_lock();

/*
 * elfp_main_vector_free(): Unlocks the vector.
 */
void
elfp_main_vector_free();




/*
 * The following functions are related to elfp_main structure.
 */

/*
 * create_elfp_main: Create a new elfp_main object and return the reference.
 *
 * @return: On success, returns a reference to a elfp_main instance.
 * 		NULL on failure.
 */

elfp_main*
create_elfp_main();

/*
 * add_elfp_main: Adds the new structure to the vector.
 *
 * @arg0: Reference to elfp_main
 * @return: Returns the user-handle.
 */
int
add_elfp_main(elfp_main *main);

/* add_addr_to_main: Adds the specified address to the main's list of 
 * 		"to-be-freed-later" addresses.
 *
 * @arg0: elfp_main reference
 * @arg1: elfp_ehdr reference
 */
void
add_addr_to_main(elfp_main *main, void *addr);

/*
 * get_elfp_main: Returns a reference to an elfp_main structure.
 *
 * @arg0: User handle
 * @return: Corresponding elfp_main structure
 */
elfp_main*
get_elfp_main(int handle);

/*
 * verify_elfp_handle: Sanity check on the user-handle before passing it to 
 * 		internal functions.
 *
 * @arg0: potential handle
 * 
 * @return: -1 if there are problems with handle. The caller MUST NOT proceed
 * 		with this handle value. 
 * 	    0 means the handle is valid.
 */
int
verify_elfp_handle(int handle);









#endif /* _ELFP_INT_H */
