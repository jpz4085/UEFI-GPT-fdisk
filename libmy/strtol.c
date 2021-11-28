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
#include "dietfeatures.h"
#undef __USE_EXTERN_INLINES
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#if __WORDSIZE == 64
#define ABS_LONG_MIN 9223372036854775808UL
#else
#define ABS_LONG_MIN 2147483648UL
#endif

/* __ctype_b_loc returns the address in the image file which
   is not the address in memory: crash guaranteed we should
   do something with the relocations. Fixed since version 0.1 */
#ifdef isspace
# undef isspace
#endif
#define isspace( c ) ( ( c ) <= ' ' )
#ifdef isalnum
# undef isalnum
#endif
#define isalnum( c ) ( ( ( c ) >= '0' && ( c ) <= '9' ) || \
	( ( c ) >= 'A' && ( c ) <= 'Z' ) || \
	( ( c ) >= 'a' && ( c ) <= 'z' ) )

long int strtol(const char *nptr, char **endptr, int base)
{
  int neg=0;
  unsigned long int v;
  const char*orig=nptr;

  while(isspace(*nptr)) nptr++;

  if (*nptr == '-' && isalnum(nptr[1])) { neg=-1; ++nptr; }
  v=strtoul(nptr,endptr,base);
  if (endptr && *endptr==nptr) *endptr=(char *)orig;
  if (v>=ABS_LONG_MIN) {
    if (v==ABS_LONG_MIN && neg) {
      errno=0;
      return v;
    }
    errno=ERANGE;
    return (neg?LONG_MIN:LONG_MAX);
  }
  return (neg?-v:v);
}
