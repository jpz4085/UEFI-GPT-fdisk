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
#ifndef	_LOCALE_H
#include <locale.h>
#include <xlocale.h>

extern locale_t uselocale (locale_t newloc) ;

#ifndef _ISOMAC
extern __typeof (uselocale) __uselocale;

libc_hidden_proto (setlocale)
libc_hidden_proto (__uselocale)

/* This has to be changed whenever a new locale is defined.  */
#define __LC_LAST	13

extern struct loaded_l10nfile *_nl_locale_file_list[] attribute_hidden;

/* Locale object for C locale.  */
extern const struct __locale_struct _nl_C_locobj attribute_hidden;
#define _nl_C_locobj_ptr ((struct __locale_struct *) &_nl_C_locobj)

/* Now define the internal interfaces.  */
extern struct lconv *__localeconv (void);

/* Fetch the name of the current locale set in the given category.  */
extern const char *__current_locale_name (int category) attribute_hidden;

#endif
#endif
