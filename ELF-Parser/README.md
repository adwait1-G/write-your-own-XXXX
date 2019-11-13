# elfp: ELF-Parser library

## 0. Description

A simple ELF-Parser library in C. A very simple and easy to use API is provided to parse and inspect ELF files.

## 1. Building and installing the library

```
ELF-Parser$ gcc elfp_*.c main.c -c -fPIC
ELF-Parser$ gcc *.o -shared -o libelfp.so
ELF-Parser$ sudo cp libelfp.so /usr/lib
ELF-Parser$ sudo mkdir /usr/include/elfp
ELF-Parser$ sudo cp include/elfp.h /usr/include/elfp
```

With that, the library is ready to use.

## 2. Writing your own ELF-Parser using elfp!

Let the name of our ELF-Parser be ```elfparse```. Open up ```elfparse.c``` in an editor.

**1.** Adding the necessary header files.

```c
#include <elfp/elfp.h>
#include <stdio.h>

```

**2.** Let us start the ```main``` function with some basic variables.

```c
int main()
{
	int ret;
	int elfpd;

}
```

**3.** Initializing the library.

```c
	elfp_init();
```

* ```elfp_init()``` initializes the library and will make it ready to use. It **MUST** be called before any other elfp-library function is called.

**4.** Opening the ELF file you want to inspect.

```
	char *file_path = "./a.out";	/* This can be any file you want */
	elfpd = elfp_open(file_path);
```

* On **success**, ```elfp_open``` returns a descriptor(a positive integer) which you can use to request the library for data related to that particular file. On **failure**, ```elfp_open``` returns **-1**.

**5.** Now, you can start asking whatever you want.

```c
	elfp_ehdr *ehdr = elfp_get_ehdr(elfpd);
```
* Here, you are requesting for ELFHeader's details (**ehdr**). The structure ```elfp_ehdr``` has ALL details extracted from the ELF Header. Refer to ```elfp/elfp.h``` to understand the structure.

**6.** You can write your own ```print``` / ```dump``` function to print ELF Header's details from ```elfp_ehdr```. You can also use an in-built ehdr dump function.

```c
	elfp_dump_ehdr(ehdr);
```

**7.** Now, let us inspect the Program Header Table, arguably the most important structure needed for a program to run properly.
```c
	elfp_phdr_tbl *phdr_tbl = elfp_get_phdr_tbl(elfpd);
```
* On success, you should have a reference to a ```elfp_phdr_tbl``` structure.
* Refer [elfp.h](./include/elfp.h) to know the structure.
* This structure has everything about the table, everything about each of the table entries(each of Program Headers).

**8.** Again, you can write a dump function of your own or you can use the in-built dump function.
```c
	elfp_dump_phdr_tbl(phdr_tbl);
```


**7.** Suppose you are done inspecting the file, you can close it using the ```elfp_close()``` function.
```c
	elfp_close(elfpd);
```

**8.** Once you are done with your program, de-initialize the library.
```c
	elfp_fini();
```

**9.** The complete program is listed below.

```c
$ cat elfparse.c
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
```

**10.** Compile the program!
```
$ gcc elfparse.c -o elfparse -lelfp
```

**11.** Run the program
```
$ ./elfparse 
Usage: $ ./elfparse <ELF-file-path>
$
$ ./elfparse elfparse
ELF Header: 
0. Magic characters:		7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00 
1. Class:			64-bit object
2. Data:			2's complement, little endian
3. OSABI:			UNIX System V
4. ELF Type:			EXEC (Executable file)
5. Machine:			AMD x86_64 architecture
6. ELF Version:			Current
7. Entry Point Address:		0x0000000000400790
8. Program Header Table Offset:	64
9. Section Header Table Offset:	6824
10. Flags:			0
11. ELF Header size:		64 bytes
12. Program Header size:	56 bytes
13. Number of Program Headers:	9
14. Section Header size:	64 bytes
15. Number of Section Headers:	30
16. String Table Index:		27

++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Program Header Table: 
Program Header 0: 
0. Type:				PHDR
1. Flags:				rwx
2. Segment Offset (in bytes):		64
3. Virtual Address:			0x0000000000400040
4. Physical Address:			0x0000000000400040
5. Segment size in file (in bytes):	504
6. Segment size in memory (in bytes):	504
7. Segment Alignment:			0x8
----------------------------------------------------
Program Header 1: 
0. Type:				INTERP
1. Flags:				r--
2. Segment Offset (in bytes):		568
3. Virtual Address:			0x0000000000400238
4. Physical Address:			0x0000000000400238
5. Segment size in file (in bytes):	28
6. Segment size in memory (in bytes):	28
7. Segment Alignment:			0x1
----------------------------------------------------
Program Header 2: 
0. Type:				LOAD
1. Flags:				rwx
2. Segment Offset (in bytes):		0
3. Virtual Address:			0x0000000000400000
4. Physical Address:			0x0000000000400000
5. Segment size in file (in bytes):	3356
6. Segment size in memory (in bytes):	3356
7. Segment Alignment:			0x200000
----------------------------------------------------
Program Header 3: 
0. Type:				LOAD
1. Flags:				rw-
2. Segment Offset (in bytes):		3592
3. Virtual Address:			0x0000000000600e08
4. Physical Address:			0x0000000000600e08
5. Segment size in file (in bytes):	632
6. Segment size in memory (in bytes):	656
7. Segment Alignment:			0x200000
----------------------------------------------------
Program Header 4: 
0. Type:				DYNAMIC
1. Flags:				rw-
2. Segment Offset (in bytes):		3608
3. Virtual Address:			0x0000000000600e18
4. Physical Address:			0x0000000000600e18
5. Segment size in file (in bytes):	480
6. Segment size in memory (in bytes):	480
7. Segment Alignment:			0x8
----------------------------------------------------
Program Header 5: 
0. Type:				NOTE
1. Flags:				r--
2. Segment Offset (in bytes):		596
3. Virtual Address:			0x0000000000400254
4. Physical Address:			0x0000000000400254
5. Segment size in file (in bytes):	68
6. Segment size in memory (in bytes):	68
7. Segment Alignment:			0x4
----------------------------------------------------
Program Header 6: 
0. Type:				Unknown Segment type
1. Flags:				r--
2. Segment Offset (in bytes):		3056
3. Virtual Address:			0x0000000000400bf0
4. Physical Address:			0x0000000000400bf0
5. Segment size in file (in bytes):	52
6. Segment size in memory (in bytes):	52
7. Segment Alignment:			0x4
----------------------------------------------------
Program Header 7: 
0. Type:				GNU_STACK
1. Flags:				rw-
2. Segment Offset (in bytes):		0
3. Virtual Address:			0x0000000000000000
4. Physical Address:			0x0000000000000000
5. Segment size in file (in bytes):	0
6. Segment size in memory (in bytes):	0
7. Segment Alignment:			0x10
----------------------------------------------------
Program Header 8: 
0. Type:				GNU_RELRO
1. Flags:				r--
2. Segment Offset (in bytes):		3592
3. Virtual Address:			0x0000000000600e08
4. Physical Address:			0x0000000000600e08
5. Segment size in file (in bytes):	504
6. Segment size in memory (in bytes):	504
7. Segment Alignment:			0x1
----------------------------------------------------
$
```

**NOTE**: You can open up multiple files at once in a single program. A unique descriptor is given to each open file.





















