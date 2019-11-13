/*
 * File: elfp_err.h
 *
 * Description: elfparse's error handling infra.
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

#ifndef _ELFP_ERR_H
#define _ELFP_ERR_H

#include <stdio.h>

static void
elfp_err_warn(const char *function_name, const char *err_msg)
{
	fprintf(stderr, "%s: %s\n", function_name, err_msg);
}

static void
elfp_err_exit(const char *function_name, const char *err_msg)
{
	elfp_err_warn(function_name, err_msg);
	fprintf(stderr, "Exiting...\n");
}

enum
{
	/* System related errors */
	ELFP_INVALID_ARG = 0,
	ELFP_ACCESS_FAIL,
	ELFP_OPEN_FAIL,
	ELFP_READ_FAIL,
	ELFP_FSTAT_FAIL,
	ELFP_MMAP_FAIL,
	ELFP_CALLOC_FAIL,
	
	/* Tool specific errors */
	ELFP_VECTOR_OVERFLOW,

	/* ELFHeader specific error messages */
	ELFP_ELFH_MAGIC_MISMATCH,
	ELFP_ELFH_INV_CLASS,
	ELFP_ELFH_INV_DATA,
	ELFP_ELFH_INV_OSABI,
	ELFP_ELFH_INV_TYPE,
	ELFP_ELFH_INV_MACH,
	ELFP_ELFH_INV_VERSION,
	ELFP_ELFH_INV_ELFH_MEMBER,
} elfp_error_codes;


/*
const char*
elfp_error_msgs[] = 
{
	"Invalid Argument, probably NULL",
	"access() failed",
	"open() failed",
	"read() failed",
	"fstat() failed",
	"mmap() failed",
	"calloc() failed",
	"Magic Number mismatch",
	"Invalid Class (Neither 32-bit nor 64-bit)",
	"Invalid Data encoding",
	"Invalid OS ABI used",
	"Invalid ELF Type",
	"Invalid Machine (architecture)",
	"Invalid ELF version",
	"Invalid ELF Header member"	
};
*/


#endif /* _ELFP_ERR_H */
