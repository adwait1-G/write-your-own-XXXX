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
```

**NOTE**: You can open up multiple files at once in a single program. A unique descriptor is given to each open file.





















