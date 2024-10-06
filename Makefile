##
## This file is part of UEFI GPT fdisk.
##
## UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
## UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
## Ce fichier a été initié par Bernard Burette en janvier 2014.
##
## Original work is copyleft Bernard Burette.
## Modifications are copyleft Joseph Zeller.
##
## This program is distributed under the terms of the GNU GPL version 2.
## La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
##

##
## Makefile to build an UEFI version of gdisk.
##
## Not tried any 32 bits compilation.
## Not tried support of wchar_t here, all the code we use is only UTF-8.
##
## We build a reduced version of the libc that we use for final link.
## This "libmy.a" is built from bits taken from "dietlibc" but also bits of
## "glic" (mostly the "locale" bits because "dietlibc" doesn't have them and
## GNU stdlibc++ seems fond of gnulibc implementation).
## What a mess!
##


# Name of the directory where GNU EFI is built.
GNUEFI = $(shell ls -d gnu-efi-*)

# Debug this code. This should make the code be verbose or not depending
# on the value of the UEFI firmware variable EFIDebug.
#EFI_DEBUG_FLAGS = -DEFI_DEBUG -O0 -g -D_FORTIFY_SOURCE=2 -fstack-protector

# Architecture for target EFI.
# Note: 32 bits code does not build.
#ARCH = ia32
ARCH = x86_64


## End of the changeable section of this Makefile.


# Compiler switch for 32 or 64 bits.
ifeq ($(ARCH),ia32)
  # Build 32 bits binaries.
  CFLAGS += -m32
endif
ifeq ($(ARCH),x86_64)
  # Build 64 bits binaries.
  CFLAGS += -m64 -mno-red-zone
endif

# Access 64 bits for seek() et al.
CFLAGS += -D_FILE_OFFSET_BITS=64
# UEFI firmware doesn't initialize CPU for MMX et al (returning float or double
# using Linux x86_64 ABI is not possible: better break during compilation than
# crash during run).
#CFLAGS += -mno-mmx -mno-sse
# Optimize for size. We also need to use -fno-inline otherwise <stdlib.h> will
# try to define atof() as "inline" returning a "float" which is not available
# in UEFI using x86_64 ABI and is forbidden by -mno-mmx option.
CFLAGS += -Os -fno-inline
# Compile Position Independent Code and for foreign platform.
CFLAGS += -fPIC -DPIC -ffreestanding
# wchar_t for UEFI is 16 bits (we don't use UFT-16 here but standard libraries
# were built without this flag, so using this switch could be a problem).
CFLAGS += -fshort-wchar
# Code should be well debugged so don't need these.
CFLAGS += -fno-stack-protector -fno-stack-check -fomit-frame-pointer
# ? need stack "shadow space" in UEFI.
CFLAGS += -mno-red-zone
# ?
CFLAGS += -fno-asynchronous-unwind-tables -fno-unwind-tables
# Alloue tout l'espace de pile d'une fonction (plus rapide).
CFLAGS += -maccumulate-outgoing-args
# Be thorough.
CFLAGS += -Wall -Wextra
# Debug UEFI code flag.
CFLAGS += $(EFI_DEBUG_FLAGS)

# Same flags for assembler files.
ASFLAGS += $(CFLAGS)

# Same flags for GPT fdisk C++ files and add a special define to trigger
# code specific to UEFI version.
override CXXFLAGS += $(CFLAGS) -DEFI
# At least for the moment C++ exceptions and RTTI are not handled.
override CXXFLAGS += -fno-exceptions -fno-rtti

# Our UEFI glue will need additional includes from GNU EFI.
CFLAGS += -I$(GNUEFI)/inc/$(ARCH) -I$(GNUEFI)/inc

# We want to build a standalone executable (no dynamic link available with
# UEFI) but just using "-static" removes the relocation information in the
# resulting file and we need them when loading the UEFI image.
# Note that we use the -Bxx format because these options are also used with
# "g++" who wouldn't send them to "ld" otherwise and we also use additional
# link options for the Linux binary "gdisk".
LDFLAGS += -Bshareable -Bsymbolic -Bstatic
# Very important otherwise the relocation section will be emptied by ld.
LDFLAGS += -znocombreloc
# We don't use the .data.rel.ro magic here (make a segment read-only after
# relocation entries have been applied to it).
LDFLAGS += -zrelro

# Libs libstdc++.a and libgcc_eh.a are stored in same dir (is this luck or
# will it stay that way?)
GPPLIB = $(shell $(CXX) $(CXXFLAGS) -print-file-name=libstdc++.a)
GPPLIBSDIR = $(shell dirname $(GPPLIB))
LDFLAGS += -L$(GPPLIBSDIR) -lstdc++ -lgcc_eh

# Object files to be built for our glue with UEFI.
GLUE_OBJS = start-$(ARCH).o relocate-$(ARCH).o initfini.o \
	efi_main.o thread.o io.o alloc.o time.o

# Module names (copied from gptfdisk Makefile).
MODULES += crc32 support guid gptpart mbrpart basicmbr mbr gpt bsd \
        parttypes attributes diskio diskio-unix gdisk gpttext

# Object files names derived from module names.
OBJS = $(MODULES:=.o)

# Extra object files replacing libraries.
OBJS += debug.o dynamic_cast.o uuid.o


# Build only the UEFI version by default.
# Need to "make gdisk" to build the Linux one.
#
all: gdisk.efi


# Cleanup.
#
clean: #no pre-reqs
	$(MAKE) -C $(GNUEFI) clean
	$(MAKE) -C libmy clean
	-rm -f core *.o *~ gdisk.efi gdisk.so.s gdisk.so gdisk gdisk.map test


# Make a map of the link for debugging, delete gdisk.so if it exists then
# rebuild it with "-M" flag.
#
map: #no pre-reqs
	-rm -f gdisk.so
	$(MAKE) MAPOPT="--cref -Map=gdisk.map" gdisk.so


# Phoney targets (build them every time).
#
.PHONY: all clean map


# Build the GNU UEFI library.
#
libefi.a: #no pre-reqs
	$(MAKE) -C $(GNUEFI) ARCH="$(ARCH)" \
		EFI_DEBUG_FLAGS="$(EFI_DEBUG_FLAGS)" lib


# Build our extras replacing libc for the target architecture.
#
libmy.a: #no pre-reqs
	$(MAKE) -C libmy ARCH="$(ARCH)" \
		EFI_DEBUG_FLAGS="$(EFI_DEBUG_FLAGS)" libmy.a


# Build object files for UEFI from GPT fdisk C++ source files.
#
%.o: ../%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<


# Build gdisk.so as static ELF image by combining all bits.
# We carefully avoid linking against libc.a which has Linux system calls.
# We use a custom link script because the script from GNU EFI misses bits to
# successfully link a C++ program.
# Since version 3.0.2 of GNU EFI, libraries directories have moved so let's
# use both locations here.
#
gdisk.so: libefi.a libmy.a $(GLUE_OBJS) $(OBJS)
	$(LD) -nostdlib --no-undefined -Telf-$(ARCH).lds \
		$(GLUE_OBJS) $(OBJS) -Llibmy -lmy $(LDFLAGS) \
		-L$(GNUEFI)/$(ARCH)/lib -L$(GNUEFI)/lib -lefi \
		--entry=_start -lmy $(MAPOPT) -o $@


# Build .EFI from .SO by first removing symbols (otherwise .EFI will say he
# has symbols in its PE header when he doesn't), then copy the required
# sections from image file.
#
%.so.s: %.so
	strip -s -o $@ $<
#
%.efi: %.so.s
	objcopy -j .reloc -j .dynamic -j .rela -j .dynsym -j .text \
		-j .rdata -j .data -j .bss \
		--set-section-flags .dynamic=alloc,load,readonly,data \
		--target=efi-app-$(ARCH) $< $@


# For testing purposes, build this program for the Linux platform with our
# libmy.a but then link with standard libc library for system calls like
# write()/read()... for Linux and not from our glue modules.
# This program is good for debugging with the symbols et al.
#
gdisk: libmy.a $(OBJS) debug_linux.o
	$(CXX) -g $(OBJS) debug_linux.o -Llibmy -lmy $(LDFLAGS) \
		-static -lmy $(MAPOPT) -o $@

