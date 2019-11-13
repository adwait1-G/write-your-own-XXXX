/*
 * File: elfparse.c
 *
 * A very simple program, to demonstrate elfp's API.
 *
 * Do the following to get the program running: 
 *
 * 1. Build and install library (Refer to this project's README)
 * 2. Compile this source-file in the following manner.
 * ```
 * $ gcc elfparse.c -o elfparse -lelfp
 * ```
 * 
 * > elfparse is ready to run!
 */

#include <elfp/elfp.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: $ %s <ELF-file-path>\n", argv[0]);
		return -1;
	}

	int ret;
	int elfpd;
	char *file_path = argv[1];	/* This can be any ELF file you want */

	/* Initialize the library */
	elfp_init();

	/* Open the ELF file, get the descriptor */
	ret = elfp_open(file_path);
	if(ret == -1)
	{
		fprintf(stderr, "elfp_open() failed\n");
		return -1;
	}
	elfpd = ret;

	/* Let us now get all details present in the ELF Header of the 
	 * specified file */
	elfp_ehdr *ehdr = elfp_get_ehdr(elfpd);
	if(ehdr == NULL)
	{
		fprintf(stderr, "elfp_get_ehdr() failed\n");
		return -1;
	}

	/* You can write your own dump function or use the following in-built
	 * function to dump ELF Header's details */
	elfp_dump_ehdr(ehdr);
	
	printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

	/* Let us now get details about the segments present */
	elfp_phdr_tbl *phdr_tbl = elfp_get_phdr_tbl(elfpd);
	if(phdr_tbl == NULL)
	{
		fprintf(stderr, "elfp_get_phdr_tbl() failed");
		return -1;
	}

	elfp_dump_phdr_tbl(phdr_tbl);

	/* Once you are done, de-initialize the library.
	 * You don't have to call elfp_close() . This will take care of it */
	elfp_fini();

	  return 0;
}
