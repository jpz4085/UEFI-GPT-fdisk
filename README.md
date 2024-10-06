# UEFI-GPT-fdisk

This is a port of the linux disk partitioning tool by Roderick W. Smith for use in a UEFI environment. It can be added as an option to the firmware boot menu like an operating system or run as an application from the UEFI Shell. When running from the shell it will accept all valid command line arguments including a block device. If run without arguments or as a firmware boot option the program will display a list of internal disks for selection. The original library can be found on SourceForge at [UEFI GPT fdisk](https://sourceforge.net/projects/uefigptfdisk/) by bub75.

<img align="center" src="https://raw.githubusercontent.com/jpz4085/UEFI-GPT-fdisk/main/.github/images/gdisk_create.png"/>

Example 1: Create three new partitions then write to block device.

<img align="center" src="https://raw.githubusercontent.com/jpz4085/UEFI-GPT-fdisk/main/.github/images/gdisk_list.png" width=623 height=470/>

Example 2: List partitions on the specified block device.

## Requirements

Download from Releases or clone the repository.
```
git clone https://github.com/jpz4085/UEFI-GPT-fdisk.git
```
Download or clone the current [GNU-EFI](https://github.com/ncroxon/gnu-efi) library.
```
git clone https://github.com/ncroxon/gnu-efi.git
```
Download the source code for a gdisk [release](https://sourceforge.net/projects/gptfdisk/files/gptfdisk/) or clone the repository.
```
git clone https://git.code.sf.net/p/gptfdisk/code gptfdisk-code
```

## Build Instructions

1. Install any dependencies for building gdisk: see it's README.
2. Extract the gdisk source files to a temporary location.
3. Move the GNU-EFI folder inside the UEFI-GPT-fdisk folder.
4. Move the UEFI-GPT-fdisk folder inside the gdisk source folder.
5. Now open a terminal at the UEFI-GPT-fdisk subdirectory.
6. Enter "PATCH.sh" at the prompt when using the following versions.
	- GPT fdisk: 1.0.5 and above.
	- GNU-EFI: 3.0.18 and above.
7. Enter "make" at the prompt to build the executable.

## Addendum

The only functions not available are the commands to save or load the partition layout to disk. This may be addressed in a future release. Apart from that all features should operate as expected. Refer to the GPT fdisk manual or [tutorial](http://www.rodsbooks.com/gdisk/) for more information on how to use the program. Additional information about source code used in this project is provided below.

- We link against "libstdc++" however this was compiled with option _GLIBCXX_USE_WCHAR_T under Ubuntu which adds a lot of useless bloat to the program. But only a full rewrite of GPT fdisk in plain C could alleviate this.
- A private "libc" library called "libmy.a" is built in the "libmy" subdirectory and is mostly built from bits imported from "dietlibc" but contains some code imported from "glibc" since GNU libstdc++ also calls __newlocale().
- EFI is UTF-16 while GPT fdisk uses UTF-8 internally and the original glue functions hand code conversions from UTF-8 to UTF-16. This process has been extended with Davipb's [UTF-8/UTF-16 converter](https://github.com/Davipb/utf8-utf16-converter) when processing command line arguments.
- No C++ exception is handled here. The stack unwinding is tricky to implement and fortunately GPT fdisk does not use exceptions at all.
- For all the system calls from the standard libray we build EFI versions of functions like write(), read() and so on, all these functions are in the ".c" source files in this "." directory, not in the "libmy" subdirectory.
- We have added a "dynamic_cast.c" module that doesn't check anything because the code should be fully debugged for all use cases and using libstdc++ doesn't work the way it should (remember we don't do C++ exceptions here, so throwing a "bad_cast" would crash the program with no explanation).
- The current build environment supports GCC-12 and glibc v2.39 running under Ubuntu 24.04 LTS with GPT fdisk versions up to 1.0.10. When building under other Linux distributions check your environment for compatibility.
