/*
 * File: elfp_phdr.c
 *
 * Description: All functions related to Program Headers, segments are defined
 * 	here.
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

#include <elf.h>
#include <string.h>
#include <stdlib.h>

#include "include/elfp.h"
#include "include/elfp_int.h"
#include "include/elfp_err.h"

elfp_phdr*
create_elfp_phdr(unsigned int num)
{
	elfp_phdr *phdr = NULL;
	
	phdr = calloc(num, sizeof(elfp_phdr));
	if(phdr == NULL)
	{
		elfp_err_warn("create_elfp_phdr", "calloc() failed");
		return NULL;
	}
	
	return phdr;
}


elfp_phdr_tbl*
create_elfp_phdr_tbl(elfp_main *main)
{
	elfp_phdr_tbl *phdr_tbl = NULL;

	phdr_tbl = calloc(1, sizeof(elfp_phdr_tbl));
	if(phdr_tbl == NULL)
	{
		elfp_err_warn("create_elfp_phdr_tbl", "calloc() failed");
		return NULL;
	}

	/* Now, we need an array of elfp_phdr structures in heap.
	 * Get them! */
	
	/* Get the number of Program Headers in the PHT */
	phdr_tbl->n = main->ehdr->data[ELFP_EHDR_PH_NUM];
	phdr_tbl->tbl = create_elfp_phdr(phdr_tbl->n);
	if(phdr_tbl->tbl == NULL)
	{
		elfp_err_warn("create_elfp_phdr_tbl",
				"create_elfp_phdr() failed");
		
		/* Free the memory allocated for phdr_tbl */
		free(phdr_tbl);

		return NULL;
	}
	
	/* Add the table to list of addresses to be freed */
	add_addr_to_main(main, phdr_tbl->tbl);

	/* All good */
	return phdr_tbl;
}

elfp_phdr_tbl*
elfp_get_p64hdr_tbl(elfp_main *main)
{
	elfp_phdr_tbl *phdr_tbl = NULL;
	elfp_phdr *phdr = NULL;

	Elf64_Phdr *seg_hdr = (Elf64_Phdr *)
		((char *)main->start_addr + main->ehdr->data[ELFP_EHDR_PHT_OFF]);

	phdr_tbl = create_elfp_phdr_tbl(main);
	if(phdr_tbl == NULL)
	{
		elfp_err_warn("elfp_get_p64hdr_tbl", "create_elfp_phdr_tbl() failed");
		return NULL;
	}

	/* Go through each Program Header and update phdr_tbl */
	for(int i = 0; i < phdr_tbl->n; i++)
	{
		phdr = phdr_tbl->tbl + i;

		phdr->data[ELFP_PHDR_TYPE] = seg_hdr->p_type;
		phdr->data[ELFP_PHDR_FLAGS] = seg_hdr->p_flags;
		phdr->data[ELFP_PHDR_OFFSET] = seg_hdr->p_offset;
		phdr->data[ELFP_PHDR_VIR_ADDR] = seg_hdr->p_vaddr;
		phdr->data[ELFP_PHDR_PHY_ADDR] = seg_hdr->p_paddr;
		phdr->data[ELFP_PHDR_FILE_SIZE] = seg_hdr->p_filesz;
		phdr->data[ELFP_PHDR_MEM_SIZE] = seg_hdr->p_memsz;
		phdr->data[ELFP_PHDR_ALIGNMENT] = seg_hdr->p_align;

		/* Now the data to be decoded! */
		/* Segment type */
		switch(phdr->data[ELFP_PHDR_TYPE])
		{
			case PT_PHDR:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "PHDR";
				break;

			case PT_LOAD:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "LOAD";
				break;

			case PT_DYNAMIC:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "DYNAMIC";
				break;

			case PT_INTERP:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "INTERP";
				break;

			case PT_NOTE:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "NOTE";
				break;

			case PT_SHLIB:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "SHLIB";
				break;

			case PT_TLS:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "TLS";
				break;

			case PT_GNU_STACK:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "GNU_STACK";
				break;

			case PT_GNU_RELRO:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "GNU_RELRO";
				break;

			default:
				phdr->decoded_data[ELFP_PHDR_TYPE] = "Unknown Segment type";
		}			

		/* Segment flags */
		switch(phdr->data[ELFP_PHDR_FLAGS])
		{
			case PF_R:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "r--";
				break;

			case PF_R | PF_W:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "rw-";
				break;
			
			case PF_R | PF_X:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "r-x";
			
			/* The case I am most excited about */
			case PF_R | PF_W | PF_X:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "rwx";
				break;

			/* May not encounter the following cases in real-life */
			case PF_W:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "-w-";
				break;

			case PF_X:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "--x";
				break;

			case PF_W | PF_X: 
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "-wx";
				break;
			
			/* No permissions */
			case 0:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = "---";
				break;
			
			default:
				phdr->decoded_data[ELFP_PHDR_FLAGS] = 
							"Unknown flags";
		}
		
		seg_hdr = seg_hdr + 1;
	}
	

	return phdr_tbl;
}


void
elfp_dump_phdr_tbl(elfp_phdr_tbl *phdr_tbl)
{
	if(phdr_tbl == NULL)
		return;

	int i;
	int sl_no;

	printf("Program Header Table: \n");
	for(i = 0; i < phdr_tbl->n; i++)
	{
		sl_no = 0;
		printf("Program Header %d: \n", i);
		printf("%d. Type:\t\t\t\t%s\n", sl_no++,
				phdr_tbl->tbl[i].decoded_data[ELFP_PHDR_TYPE]);
		printf("%d. Flags:\t\t\t\t%s\n", sl_no++,
				phdr_tbl->tbl[i].decoded_data[ELFP_PHDR_FLAGS]);
		printf("%d. Segment Offset (in bytes):\t\t%lu\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_OFFSET]);
		printf("%d. Virtual Address:\t\t\t0x%016lx\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_VIR_ADDR]);
		printf("%d. Physical Address:\t\t\t0x%016lx\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_PHY_ADDR]);
		printf("%d. Segment size in file (in bytes):\t%lu\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_FILE_SIZE]);
		printf("%d. Segment size in memory (in bytes):\t%lu\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_MEM_SIZE]);
		printf("%d. Segment Alignment:\t\t\t0x%lx\n", sl_no++,
				phdr_tbl->tbl[i].data[ELFP_PHDR_ALIGNMENT]);
		printf("----------------------------------------------------\n");
	}
	return;
}

elfp_phdr_tbl*
elfp_get_phdr_tbl(int handle)
{
	int ret;
	elfp_main *main = NULL;
	elfp_phdr_tbl *phdr_tbl = NULL;

	/* First, check it */
	ret = verify_elfp_handle(handle);
	if(ret == -1)
	{
		elfp_err_warn("elfp_get_phdr_tbl", 
				"Handle passed didn't pass the sanity check");
		return NULL;
	}

	main = elfp_main_vector.vec[handle];
	
	/* We part ways here because of class */
	if(main->ehdr->data[ELFP_EHDR_CLASS] == ELFCLASS64)
		phdr_tbl = elfp_get_p64hdr_tbl(main);
	/* Let us leave 32-bit for now.
	else if(main->ehdr->data[ELFP_EHDR_CLASS] == ELFCLASS32)
		phdr_tbl = elfp_get_p32hdr_tbl(main);
	*/

	else /* Think it is a 64-bit file and go ahead */
		phdr_tbl = elfp_get_p64hdr_tbl(main);

	/* At this point, we have parsed out the complete table.
	 * We'll let the internal data structures know */
	main->phdr_tbl = phdr_tbl;
	add_addr_to_main(main, phdr_tbl);

	return phdr_tbl;
}
















