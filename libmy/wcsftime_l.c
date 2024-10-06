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

#include "libmy.h"
#include <wctype.h>
#include <locale.h>

size_t __wcsftime_l( s , maxsize , format , tp , loc )
	char *s __attribute__((unused)) ;
	size_t maxsize __attribute__((unused)) ;
	const char *format __attribute__((unused)) ;
	const struct tm *tp __attribute__((unused)) ;
	__locale_t loc __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "__wcsftime_l()" ) ;
}

__typeof (__wcsftime_l) wcsftime_l __attribute__((weak, alias("__wcsftime_l"))) ;

