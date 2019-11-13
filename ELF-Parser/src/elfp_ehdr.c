/*
 * File: elfp_ehdr.c
 *
 * Description: Parses the ELFHeader.
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


elfp_ehdr*
create_elfp_ehdr()
{
	elfp_ehdr *ehdr = NULL;

	ehdr = calloc(1, sizeof(elfp_ehdr));
	if(ehdr == NULL)
	{
		elfp_err_warn("create_elfp_ehdr", "calloc() failed");
		return NULL;
	}

	return ehdr;
}


void
elfp_get_e64hdr(elfp_main *main, elfp_ehdr *ehdr)
{
	/* This is an internal function. Input sanitization is not being
	 * done because the caller should have taken care of this. */
	
	Elf64_Ehdr *elf_hdr = NULL;
	unsigned char *start_addr = NULL;

	/* Get the header */
	start_addr = main->start_addr;
	elf_hdr = (Elf64_Ehdr *)start_addr;

	/* From here, parsing starts */
	
	/* ELF File type */
	ehdr->data[ELFP_EHDR_TYPE] = (unsigned long int)elf_hdr->e_type;
	switch(ehdr->data[ELFP_EHDR_TYPE])
	{
		case ET_REL:
			ehdr->decoded_data[ELFP_EHDR_TYPE] = 
					"REL (Relocatable file)";
			break;

		case ET_EXEC:
			ehdr->decoded_data[ELFP_EHDR_TYPE] =
					"EXEC (Executable file)";
			break;

		case ET_DYN:
			ehdr->decoded_data[ELFP_EHDR_TYPE] =
					"DYN (Shared Object file)";
			break;

		case ET_CORE:
			ehdr->decoded_data[ELFP_EHDR_TYPE] =
					"CORE (Core file)";
			break;

		default:
			ehdr->decoded_data[ELFP_EHDR_TYPE] =
					"Unknown ELF type";
	}

	/* Machine (architecture) */
	ehdr->data[ELFP_EHDR_MACH] = elf_hdr->e_machine;
	switch(ehdr->data[ELFP_EHDR_MACH])
	{
		case EM_M32:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "AT&T WE 32100";
			break;

		case EM_SPARC:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "SUN SPARC";
			break;

		case EM_386:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Intel 80386";
			break;

		case EM_68K:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
							"Motorola m68k family";
			break;

		case  EM_88K:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
							"Motorola m88k family";
			break;

		case EM_860:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Intel 80860";
			break;

		case EM_MIPS:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
						"MIPS R3000 big-endian";
			break;

		case EM_S370:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "IBM System/370";
			break;

		case EM_MIPS_RS3_LE:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
						"MIPS R3000 little-endian";
			break;

		case EM_PARISC:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "HPPA";
			break;

		case EM_VPP500:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Fujitsu VPP500";
			break;

		case EM_SPARC32PLUS:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Sun's \"v8plus\"";
			break;

		case EM_960:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Intel 80960";
			break;

		case EM_PPC:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "PowerPC";
			break;

		case EM_PPC64:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "PowerPC 64-bit";
			break;

		case EM_S390:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "IBM S390";
			break;

		case EM_V800:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "NEC V800 series";
			break;

		case EM_FR20:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Fujitsu FR20";
			break;

		case EM_RH32:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "TRW RH-32";
			break;

		case EM_RCE:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Motorola RCE";
			break;

		case EM_ARM:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "ARM";
			break;

		case EM_FAKE_ALPHA:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Digital Alpha";
			break;
		
		case EM_SH:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Hitachi SH";
			break;

		case EM_SPARCV9:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "SPARC v9 64-bit";
			break;

		case EM_TRICORE:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Siemens Tricore";
			break;

		case EM_ARC:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
							"Argonaut RISC Core";
			break;

		case EM_H8_300:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Hitachi H8/300";
			break;

		case EM_H8_300H:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Hitachi H8/300H";
			break;

		case EM_H8S:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Hitachi H8S";
			break;

		case EM_H8_500:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Hitachi H8/500";
			break;

		case EM_IA_64:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Intel Merced";
			break;

		case EM_MIPS_X:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Stanford MIPS-X";
			break;

		case EM_COLDFIRE:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Motorola Coldfire";
			break;

		case EM_68HC12:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Motorola M68HC12";
			break;

		case EM_MMA:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
					"Fujitsu MMA Multimedia Accelerator";
			break;
		
		case EM_PCP:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Siemens PCP";
			break;

		case EM_NCPU:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
						"Sony nCPU embeeded RISC";
			break;

		case EM_NDR1:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"Denso NDR1 microprocessor";
			break;

		case EM_STARCORE:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"Motorola Start*Core processor";
			break;

		case EM_ME16:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"Toyota ME16 processor";
			break;

		case EM_ST100:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"STMicroelectronic ST100 processor";
			break;

		case EM_TINYJ:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Advanced Logic Corp. Tinyj emb.fam";
			break;

		case EM_X86_64:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
						"AMD x86_64 architecture";
			break;

		case EM_PDSP:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
							"Sony DSP Processor";
			break;

		case EM_FX66:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"Siemens FX66 microcontroller";
			break;

		case EM_ST9PLUS:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"STMicroelectronics ST9+ 8/16 mc";
			break;

		case EM_ST7:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"STmicroelectronics ST7 8 bit mc";
			break;

		case EM_68HC16:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Motorola MC68HC16 microcontroller";
			break;

		case EM_68HC11:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Motorola MC68HC11 microcontroller";
			break;

		case EM_68HC08:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Motorola MC68HC08 microcontroller";
			break;
		case EM_68HC05:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Motorola MC68HC05 microcontroller";
			break;
		
		case EM_SVX:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"Silicon Graphics SVx";
			break;
		
		case EM_VAX:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Digital VAX";
			break;
		
		case EM_CRIS:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
				"Axis Communications 32-bit embedded processor";
			break;
		
		case EM_JAVELIN:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
			"Infineon Technologies 32-bit embedded processor";
			break;

		case EM_FIREPATH:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Element 14 64-bit DSP Processor";
			break;

		case EM_ZSP:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"LSI Logic 16-bit DSP Processor";
			break;

		case EM_MMIX:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
				"Donald Knuth's educational 64-bit processor";
			break;
	
		case EM_HUANY:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
			"Harvard University machine-independent object files";
			break;
		
		case EM_PRISM:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "SiTera Prism";
			break;
		
		case EM_AVR:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
					"Atmel AVR 8-bit microcontroller";
			break;

		case EM_FR30:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Fujitsu FR30";
			break;
		
		case EM_D10V:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Mitsubishi D10V";
			break;

		case EM_D30V:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Mitsubishi D30V";
			break;

		case EM_V850:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "NEC v850";
			break;

		case EM_M32R:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
							"Mitsubishi M32R";
			break;

		case EM_MN10300:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
							"Matsushita MN10300";
			break;

		case EM_MN10200:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
                                                        "Matsushita MN10200";
			break;

		case EM_PJ:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "picoJava";
			break;
		
		case EM_OPENRISC:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
					"OpenRISC 32-bit embedded processor";
			break;

		case EM_ARC_A5:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
						"ARC Cores Tangent-A5";
			break;

		case EM_XTENSA:
			ehdr->decoded_data[ELFP_EHDR_MACH] =
					"Tensilica Xtensa Architecture";
			break;

		case EM_ALTERA_NIOS2:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Altera Nios II";
			break;

		case EM_AARCH64:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "ARM AARCH64";
			break;

		case EM_TILEPRO:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Tilera TILEPro";
			break;

		case EM_MICROBLAZE:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Xilinx MicroBlaze";
			break;

		case  EM_TILEGX:
			ehdr->decoded_data[ELFP_EHDR_MACH] = "Tilera TILE-Gx";
			break;

		default:
			ehdr->decoded_data[ELFP_EHDR_MACH] = 
						"Unknown machine / architecture";
	}

	/* Done with the encoded stuff. Now onto some normal members */

	ehdr->data[ELFP_EHDR_ENTRY_ADDR] = elf_hdr->e_entry;
	ehdr->data[ELFP_EHDR_PHT_OFF] = elf_hdr->e_phoff;
	ehdr->data[ELFP_EHDR_SHT_OFF] = elf_hdr->e_shoff;
	ehdr->data[ELFP_EHDR_FLAGS] = elf_hdr->e_flags;
	ehdr->data[ELFP_EHDR_SIZE] = elf_hdr->e_ehsize;
	ehdr->data[ELFP_EHDR_PH_SIZE] = elf_hdr->e_phentsize;
	ehdr->data[ELFP_EHDR_PH_NUM] = elf_hdr->e_phnum;
	ehdr->data[ELFP_EHDR_SH_SIZE] = elf_hdr->e_shentsize;
	ehdr->data[ELFP_EHDR_SH_NUM] = elf_hdr->e_shnum;
	ehdr->data[ELFP_EHDR_SH_STR_INDEX] = elf_hdr->e_shstrndx;

	/* With that, we are set!. Let us return */
	return;
}

elfp_ehdr*
elfp_get_ehdr(int elfp_handle)
{
	int ret;
	elfp_main *main = NULL;
	unsigned char *start_addr = NULL;
	unsigned char class;
	elfp_ehdr *ehdr = NULL;
	unsigned char e_ident[EI_NIDENT];

	/* Check if the handle is a valid one or not */
	ret = verify_elfp_handle(elfp_handle);
	if(ret == -1)
	{
		elfp_err_warn("elfp_get_ehdr", 
				"Handle didn't pass the sanity check");
		return NULL;
	}

	/* Get the reference to 'main' */
	main = get_elfp_main(elfp_handle);

	/* Check if the user has already called for ehdr before. If yes,
         * just send it out */
	if(main->ehdr != NULL)
		return main->ehdr;

	/* Get start address */
	start_addr = main->start_addr;

	/* Get a new elfp_ehdr instance */
	ehdr = create_elfp_ehdr();
	if(ehdr == NULL)
	{
		elfp_err_warn("elfp_ehdr", "Unable to create elfp_ehdr");
		return NULL;
	}

	/* Because we have already done the magic number test, skipping it */

	/* The first EI_NIDENT bytes of the ELF file says a lot about it.
	 * Further analysis depends on whether this object is a 32-bit or 64-bit
	 * object. It may also depend on the ABI */
	
	memcpy(ehdr->e_ident, start_addr, EI_NIDENT);
	
	/* Class */
	ehdr->data[ELFP_EHDR_CLASS] = ehdr->e_ident[EI_CLASS];
	switch(ehdr->data[ELFP_EHDR_CLASS])
	{
		case ELFCLASS32: 
			ehdr->decoded_data[ELFP_EHDR_CLASS] = "32-bit object";
			break;

		case ELFCLASS64:
			ehdr->decoded_data[ELFP_EHDR_CLASS] = "64-bit object";
			break;
		
		default: 
			ehdr->decoded_data[ELFP_EHDR_CLASS] =
			"Invalid Class (neither 32-bit nor 64-bit object)";
	}
	
	/* Data encoding */
	ehdr->data[ELFP_EHDR_DATA] = ehdr->e_ident[EI_DATA];
	switch(ehdr->data[ELFP_EHDR_DATA])
	{
		case ELFDATA2LSB:
			ehdr->decoded_data[ELFP_EHDR_DATA] =
				        "2's complement, little endian";
			break;

		case ELFDATA2MSB:
			ehdr->decoded_data[ELFP_EHDR_DATA] =
                                	"2's complement, big endian";
			break;

		default:
			ehdr->decoded_data[ELFP_EHDR_DATA] =
                                		"Invalid Data encoding";
	}

	/* OS ABI */
	ehdr->data[ELFP_EHDR_OSABI] = ehdr->e_ident[EI_OSABI];
	switch(ehdr->data[ELFP_EHDR_OSABI])
	{
		case ELFOSABI_NONE:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "UNIX System V";
			break;
		
		case ELFOSABI_HPUX:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "HP-UX";
			break;

		case ELFOSABI_NETBSD:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "NetBSD";
			break;
		
		case ELFOSABI_GNU:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = 
							"GNU ELF extensions";
			break;

		case ELFOSABI_SOLARIS:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "Sun Solaris";
			break;
		
		case ELFOSABI_AIX:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "IBM AIX";
			break;

		case ELFOSABI_IRIX:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "SGI Irix";
			break;
	
		case ELFOSABI_FREEBSD:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "FreeBSD";
			break;

		case ELFOSABI_TRU64:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = 
							"Compaq TRU64 UNIX";
			break;

		case ELFOSABI_MODESTO:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "Novell Modesto";
			break;

		case ELFOSABI_OPENBSD:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "OpenBSD";
			break;

		case ELFOSABI_ARM_AEABI:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "ARB EABI";
			break;

		case ELFOSABI_ARM:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "ARM";
			break;

		case ELFOSABI_STANDALONE:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = 
						"Standalone application";
			break;

		default:
			ehdr->decoded_data[ELFP_EHDR_OSABI] = "Unknown ABI";
	}

	
	/* Version */
	ehdr->data[ELFP_EHDR_VERSION] = ehdr->e_ident[EI_VERSION];
	if(ehdr->data[ELFP_EHDR_VERSION] == EV_CURRENT)
		ehdr->decoded_data[ELFP_EHDR_VERSION] = "Current";
	
	else
		ehdr->decoded_data[ELFP_EHDR_VERSION] = "Unknown ELF Version";

	/* This is where parsing e_ident array ends.
	 * At this point, we know it is a 32-bit or 64-bit object and we need
	 * to parse it accordingly. */

	/* I just realized why this parsing separately technique is useless
	 * and inefficient. After the class is determined, rest of the code
	 * is the exact same. 
	 *
	 * For 32-bit objects, we'll be doing Elf32_Ehdr->member and
	 * for 64-bit objects, Elf64_Ehdr->member. All members are the same,
	 * just the structure is different. If we somehow make a way around it,
	 * we don't have to write the same code twice */
	
	if(ehdr->data[ELFP_EHDR_CLASS] == ELFCLASS32)
		/* elfp_get_e32hdr(main, ehdr); */
		printf("elfp_get_e32hdr() not ready yet\n");

	else if(ehdr->data[ELFP_EHDR_CLASS] == ELFCLASS64)
		elfp_get_e64hdr(main, ehdr);
	
	else  /* Unknown class. Will parse it as 64-bit object for now */
		elfp_get_e64hdr(main, ehdr);
	
	/* At this point, ehdr is ready to be sent back to the user.
	 * Update the main structure and return. */
	main->ehdr = ehdr;

	/* Put it to the list of addresses we need to free at the end */
	add_addr_to_main(main, ehdr);

	return ehdr;
}

void
elfp_dump_ehdr(elfp_ehdr *ehdr)
{
	if(ehdr == NULL)
		return;
	
	int sl_no = 0;
	int i;
	
	printf("ELF Header: \n");
	
	/* Magic characters */
	printf("%d. Magic characters:\t\t", sl_no++);
	for(i = 0; i < EI_NIDENT; i++)
		printf("%02x ", ehdr->e_ident[i]);

	printf("\n");
	printf("%d. Class:\t\t\t%s\n", sl_no++, 
					ehdr->decoded_data[ELFP_EHDR_CLASS]);
	printf("%d. Data:\t\t\t%s\n", sl_no++,
					ehdr->decoded_data[ELFP_EHDR_DATA]);
	printf("%d. OSABI:\t\t\t%s\n", sl_no++,
					ehdr->decoded_data[ELFP_EHDR_OSABI]);
	printf("%d. ELF Type:\t\t\t%s\n", sl_no++,
					ehdr->decoded_data[ELFP_EHDR_TYPE]);
	printf("%d. Machine:\t\t\t%s\n", sl_no++,
					ehdr->decoded_data[ELFP_EHDR_MACH]);
	printf("%d. ELF Version:\t\t\t%s\n", sl_no++, 
					ehdr->decoded_data[ELFP_EHDR_VERSION]);

	printf("%d. Entry Point Address:\t\t0x%016lx\n", sl_no++,
					ehdr->data[ELFP_EHDR_ENTRY_ADDR]);
	printf("%d. Program Header Table Offset:\t%lu\n", sl_no++,
					ehdr->data[ELFP_EHDR_PHT_OFF]);
	printf("%d. Section Header Table Offset:\t%lu\n", sl_no++,
					ehdr->data[ELFP_EHDR_SHT_OFF]);
	printf("%d. Flags:\t\t\t%lu\n", sl_no++, ehdr->data[ELFP_EHDR_FLAGS]);
	printf("%d. ELF Header size:\t\t%lu bytes\n", sl_no++,
					ehdr->data[ELFP_EHDR_SIZE]);
	printf("%d. Program Header size:\t%lu bytes\n", sl_no++,
					ehdr->data[ELFP_EHDR_PH_SIZE]);

	printf("%d. Number of Program Headers:\t%lu\n", sl_no++,
					ehdr->data[ELFP_EHDR_PH_NUM]);

	printf("%d. Section Header size:\t%lu bytes\n", sl_no++,
					ehdr->data[ELFP_EHDR_SH_SIZE]);

	printf("%d. Number of Section Headers:\t%lu\n", sl_no++,
					ehdr->data[ELFP_EHDR_SH_NUM]);

	printf("%d. String Table Index:\t\t%lu\n", sl_no++,
					ehdr->data[ELFP_EHDR_SH_STR_INDEX]);

	return;
}













