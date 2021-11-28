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
#ifndef _CTYPE_H

#ifndef _ISOMAC
/* Initialize ctype locale data.  */
extern void __ctype_init (void);
libc_hidden_proto (__ctype_init)

extern int __isctype (int __c, int __mask);

# ifndef NOT_IN_libc

/* These accessors are used by the optimized macros to find the
   thread-local cache of ctype information from the current thread's
   locale.  For inside libc, define them as inlines using the _NL_CURRENT
   accessors.  We don't use _NL_CURRENT_LOCALE->__ctype_b here because we
   want to cause a link-time ref to _nl_current_LC_CTYPE under
   NL_CURRENT_INDIRECT.  */

#  include "localeinfo.h"
#  include "libc-tsd.h"

#  ifndef CTYPE_EXTERN_INLINE	/* Used by ctype/ctype-info.c, which see.  */
#   define CTYPE_EXTERN_INLINE extern inline
#  endif

__libc_tsd_define (extern, const uint16_t *, CTYPE_B)
__libc_tsd_define (extern, const int32_t *, CTYPE_TOUPPER)
__libc_tsd_define (extern, const int32_t *, CTYPE_TOLOWER)


static inline const uint16_t ** __attribute__(( const ))
__ctype_b_loc (void)
{
  return __libc_tsd_address (const uint16_t *, CTYPE_B);
}

static inline const int32_t ** __attribute__(( const ))
__ctype_toupper_loc (void)
{
  return __libc_tsd_address (const int32_t *, CTYPE_TOUPPER);
}

static inline const int32_t ** __attribute__(( const ))
__ctype_tolower_loc (void)
{
  return __libc_tsd_address (const int32_t *, CTYPE_TOLOWER);
}

# endif	/* Not NOT_IN_libc.  */
#endif

#include <ctype.h>

#ifndef _ISOMAC
# if !defined __NO_CTYPE && !defined NOT_IN_libc
/* The spec says that isdigit must only match the decimal digits.  We
   can check this without a memory access.  */
#  undef isdigit
#  define isdigit(c) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#  undef isdigit_l
#  define isdigit_l(c, l) ({ int __c = (c); __c >= '0' && __c <= '9'; })
#  undef __isdigit_l
#  define __isdigit_l(c, l) ({ int __c = (c); __c >= '0' && __c <= '9'; })
# endif
#endif

#endif /* ctype.h */
