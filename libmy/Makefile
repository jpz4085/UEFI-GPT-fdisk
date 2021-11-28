##
## This file is part of UEFI GPT fdisk.
##
## UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
## UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
## Ce fichier a été initié par Bernard Burette en janvier 2014.
##
## All this work is copyleft Bernard Burette.
## Gauche d'auteur Bernard Burette.
##
## This program is distributed under the terms of the GNU GPL version 2.
## La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
##

##
## Makefile to build a reduced version of the libc.
## This lib is built from bits taken from "dietlibc" but also
## bits of "glic" (mostly the locale bits because "dietlibc"
## doesn't have those bits). Not tried to support wchar_t here,
## all the code we use is only UTF-8.
##
## What a mess!
##

# Host/target identification
ARCH ?= $(shell uname -m|sed s,i[3-9]86,ia32,)
# FreeBSD (and possibly others) reports amd64 instead of x86_64
ifeq ($(ARCH),amd64)
  override ARCH = x86_64
endif

# Compiler switch for 32 or 64 bits.
ifeq ($(ARCH),ia32)
  # Build 32 bits binaries (not tested).
  CFLAGS += -m32 -mtune=i386
endif
ifeq ($(ARCH),x86_64)
  # Build 64 bits binaries.
  CFLAGS += -m64
endif

# UEFI firmware doesn't initialize CPU for MMX et al (returning float or double
# using Linux x86_64 ABI is not possible: better break during compilation than
# crash during run).
CFLAGS += -mno-mmx -mno-sse
# Optimize for size. We also need to use -fno-inline otherwise <stdlib.h> will
# try to define atof() as "inline" returning a "float" which is not available
# in UEFI using x86_64 ABI and is forbidden by -mno-mmx option.
CFLAGS += -Os -fno-inline
# Compile Position Independent Code and for foreign platform.
CFLAGS += -fPIC -DPIC -ffreestanding
# wchar_t for UEFI is 16 bits (we don't use UFT-16 here and standard libraries
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
CFLAGS += -Wall -Wextra -Werror
# Debug UEFI code flag.
CFLAGS += $(EFI_DEBUG_FLAGS)

# List of object files required to link a nice gdisk.so.
OBJS += abs.o dl_iterate_phdr.o syscall.o sleep.o errno.o
OBJS += strlen.o strcmp.o strcoll.o strxfrm.o strcpy.o
OBJS += wcslen.o wcscmp.o wcscoll.o wcsxfrm.o
OBJS += memset.o memcmp.o memchr.o memcpy.o memmove.o
OBJS += wmemset.o wmemcmp.o wmemchr.o wmemcpy.o wmemmove.o
OBJS += mbrtowc.o mbsnrtowcs.o mbsrtowcs.o
OBJS += tolower.o toupper.o towlower.o towupper.o
OBJS += ltostr.o lltostr.o strtol.o strtoul.o strtof.o strtod.o strtold.o
OBJS += wcrtomb.o wcsnrtombs.o
OBJS += stdio.o fopen.o fdopen.o setvbuf.o fileno.o fflush.o fclose.o
OBJS += fseeko64.o ftello64.o fstat64.o
OBJS += fgetc.o fgetwc.o fgets.o fread.o
OBJS += fputc.o fputwc.o fputs.o fwrite.o writev.o
OBJS += printf.o sprintf.o vfprintf.o vsnprintf.o
OBJS += printf_chk.o sprintf_chk.o vfprintf_chk.o vsnprintf_chk.o
OBJS += v_printf.o
OBJS += sscanf.o vsscanf.o
OBJS += v_scanf.o
OBJS += textdomain.o bindtextdomain.o
OBJS += nl_langinfo.o global-locale.o ctype-info.o setlocale.o
OBJS += xlocale.o C-ctype.o C_name.o lc-ctype.o
OBJS += newlocale.o uselocale.o duplocale.o freelocale.o
OBJS += strftime_l.o wcsftime_l.o strerror.o gettext.o dgettext.o
OBJS += btowc.o wctob.o iswctype.o wctype.o ctype_get_mb_cur_max.o
OBJS += gettimeofday.o


# Build the library by default.
#
all: libmy.a


# Cleanup.
#
clean: #no pre-reqs
	-rm -f core *.o *~ libmy.a


# Phony targets (build them every time).
#
.PHONY: all clean


# The library is made of all object files.
#
libmy.a: $(OBJS)
	ar -rs $@ $?
	-ranlib $@

