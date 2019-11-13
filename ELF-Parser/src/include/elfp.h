/*
 * File: elfp.h
 *
 * Description: API exposed to the user
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

#ifndef _ELFP_H
#define _ELFP_H

#include <elf.h>
#include <stdint.h>

/*
 * elfp_init: Initializes the library. 
 * 		* MUST be called before any other library functions are called.
 */
void
elfp_init();


/*
 * elfp_fini: Cleans up everything and deinits the library.
 */
void
elfp_fini();

/*
 * elfp_open: Opens the specfied ELF file and returns a handle.
 *
 * @arg0: Path / name of ELF File
 *
 * @return: A non-negative integer - handler on success.
 * 		(-1) on failure.
 */
int
elfp_open(const char *elfp_elf_path);

/*
 * elfp_close: Closes everything about the specified handle.
 *
 * @arg0: User handle obtained from an elfp_open call.
 */
int
elfp_close(int user_handle);


/*
 * Parsing the ELFHeader.
 *
 * 1. struct data: ELF is a package of encoded stuff. So, this structure
 * 	has both the encoded value and the string obtained after parsing
 * 	that value. The user can use either the value or string.
 *
 * 2. struct elfp_ehdr: Contains all the information about the ELF Header.
 *
 * 3. Necessary macros.
 *
 * 4. elfp_get_ehdr: Returns a pointer to elfp_ehdr structure.
 * 
 * 	@arg0: Handle returned in elfp_open
 * 	@return: Pointer to elf_hdr structure, belonging to this file.
 *
 * 5. elfp_dump_ehdr: Dumps the ehdr in a human-readable form.
 * 	
 * 	@arg0: Reference to a ehdr structure.
 *
 * You can get 'ehdr' and write your own dump functions, to suit your needs.
 */

enum
{
	ELFP_EHDR_CLASS = 0,
	ELFP_EHDR_DATA,
	ELFP_EHDR_OSABI,
	ELFP_EHDR_TYPE,
	ELFP_EHDR_MACH,
	ELFP_EHDR_VERSION,
	
	ELFP_EHDR_TOTAL1,

	ELFP_EHDR_ENTRY_ADDR = 6,
	ELFP_EHDR_PHT_OFF,
	ELFP_EHDR_SHT_OFF,
	ELFP_EHDR_FLAGS,
	ELFP_EHDR_SIZE,
	ELFP_EHDR_PH_SIZE,
	ELFP_EHDR_PH_NUM,
	ELFP_EHDR_SH_SIZE,
	ELFP_EHDR_SH_NUM,
	ELFP_EHDR_SH_STR_INDEX,
	
	ELFP_EHDR_TOTAL2,

	/* Total size */
	ELFP_EHDR_TOTAL = ELFP_EHDR_TOTAL1 + ELFP_EHDR_TOTAL2,
} elfp_ehdr_index;


typedef struct elfp_ehdr
{
	unsigned char e_ident[EI_NIDENT];

	/* All members are stored here */
	unsigned long int data[ELFP_EHDR_TOTAL];

	/* Only few members need to be decoded to understand it.
	 * They are decoded and stored as strings */
	const char *decoded_data[ELFP_EHDR_TOTAL1];
} elfp_ehdr;


elfp_ehdr*
elfp_get_ehdr(int elfp_handle);

void
elfp_dump_ehdr(elfp_ehdr *ehdr);

#endif /* _ELFP_H */
