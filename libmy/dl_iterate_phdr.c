/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 * Ce fichier est récupéré soit de "GNU libc" soit de "dietlibc"
 * soit encore il a été créé de toutes pièces.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

#include "libmy.h"
/* support function when linking against libgcc_eh.a
 *
 * see gcc sources _Unwind_Find_FDE() in unwind-dw2-fde-glibc.c
 *
 * Copyright (C) 2005 Markus F.X.J. Oberhumer
 * License: GNU GPL
 */

#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <elf.h>

#if __WORDSIZE == 64
#  define ElfW(type) Elf64_##type
#else
#  define ElfW(type) Elf32_##type
#endif

struct dl_phdr_info
{
    ElfW(Addr) dlpi_addr;
    const char *dlpi_name;
    const ElfW(Phdr) *dlpi_phdr;
    ElfW(Half) dlpi_phnum;
};


/* dl_support.c defines these and initializes them early on.  */
/*
extern ElfW(Phdr) *_dl_phdr;
extern size_t _dl_phnum;
*/


int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info, size_t size, void *data),
                    void *data);
int dl_iterate_phdr(int (*callback) (struct dl_phdr_info *info, size_t size, void *data) __attribute((unused)) ,
                    void *data __attribute((unused)) )
{
/*
    int ret;
    struct dl_phdr_info info;

    if (_dl_phnum != 0)
    {
*/
        /* This entry describes this statically-linked program itself.  */
/*
        info.dlpi_addr = 0;
        info.dlpi_name = "";
        info.dlpi_phdr = _dl_phdr;
        info.dlpi_phnum = _dl_phnum;
        ret = (*callback) (&info, sizeof(info), data);
        if (ret != 0)
            return ret;
    }
*/
    return 0;
}


