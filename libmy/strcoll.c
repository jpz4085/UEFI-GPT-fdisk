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
#include <string.h>
#include <locale.h>

int __strcoll_l( s1 , s2 , loc )
	const char *s1 ;
	const char *s2 ;
	__locale_t loc __attribute__((unused)) ;
{
	return strcmp( s1 , s2 ) ;
}

__typeof( __strcoll_l ) strcoll_l __attribute__((weak,alias("__strcoll_l"))) ;

/*
int strcoll( s1 , s2 )
	const char *s1 ;
	const char *s2 ;
{
	return strcoll_l( s1 , s2 , NULL ) ;
}
*/

