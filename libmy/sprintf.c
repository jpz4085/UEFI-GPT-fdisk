/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 * Ce fichier est récupéré soit de "GNU libc" soit de "dietlibc"
 * soit encore il a été créé de toutes pièces.
 *
 * Original work is copyleft Bernard Burette.
 * Modifications are copyleft Joseph Zeller.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

#define _XOPEN_SOURCE 600
#include "libmy.h"
#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>

#include "dietstdio.h"

#pragma GCC diagnostic ignored "-Wstringop-overflow"

int sprintf(char *dest,const char *format,...)
{
  int n;
  va_list arg_ptr;
  va_start(arg_ptr, format);
  n=__vsnprintf_chk(dest , (size_t) -1 , 0 , (size_t) -1 , format,arg_ptr);
  va_end (arg_ptr);
  return n;
}
