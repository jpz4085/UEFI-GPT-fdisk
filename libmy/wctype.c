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
#include <stddef.h>
#include <wctype.h>
#include <locale.h>

wctype_t __wctype_l( property , locale )
	const char *property __attribute__((unused)) ;
	__locale_t locale __attribute__((unused)) ;
{
	/* TODO: ? */
	/*
	__fortify_fail( "__wctype_l()" ) ;
	*/
	return 0 ;
}

__typeof( __wctype_l ) wctype_l __attribute__((weak,alias("__wctype_l"))) ;

wctype_t wctype( property )
	const char *property __attribute__((unused)) ;
{
	return wctype_l( property , NULL ) ;
}

