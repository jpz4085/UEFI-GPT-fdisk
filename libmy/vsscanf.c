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
#define _GNU_SOURCE 1
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "dietstdio.h"

struct str_data {
  unsigned char* str;
};

static int sgetc(struct str_data* sd) {
  register unsigned int ret = *(sd->str++);
  return (ret)?(int)ret:-1;
}

static int sputc(int c, struct str_data* sd) {
  return (*(--sd->str)==c)?c:-1;
}

#ifdef __vsscanf
# undef __vsscanf
#endif

int __vsscanf(const char* str, const char* format, va_list arg_ptr)
{
  struct str_data  fdat = { (unsigned char*)str };
  struct arg_scanf farg = { (void*)&fdat, (int(*)(void*))sgetc, (int(*)(int,void*))sputc };
  return __v_scanf(&farg,format,arg_ptr);
}

