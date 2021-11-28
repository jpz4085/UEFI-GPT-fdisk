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
#include <xlocale.h>

size_t __strxfrm_l( dest , src , n , loc )
	char *dest __attribute__((unused)) ;
	const char *src __attribute__((unused)) ;
	size_t n __attribute__((unused)) ;
	__locale_t loc __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "__strxfrm_l()" ) ;
	/*
	size_t m = strlen( src ) ;
	if ( m < n ) m = n ;
	strncpy( dest , src , n ) ;
	return m ;
	*/
}

__typeof (__strxfrm_l) strxfrm_l __attribute__((weak, alias("__strxfrm_l"))) ;

/*
size_t strxfrm( dest , src , n )
	char *dest ;
	const char *src ;
	size_t n ;
{
	return strxfrm_l( dest , src , n , NULL ) ;
}
*/

