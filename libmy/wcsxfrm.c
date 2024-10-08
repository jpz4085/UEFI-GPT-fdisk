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
#include <wchar.h>
#include <locale.h>

size_t __wcsxfrm_l( dest , src , n , loc )
	wchar_t *dest __attribute__((unused)) ;
	const wchar_t *src __attribute__((unused)) ;
	size_t n __attribute__((unused)) ;
	__locale_t loc __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "__wcsxfrm_l()" ) ;
	/*
	size_t m = wcslen( src ) ;
	if ( m < n ) m = n ;
	wcsncpy( dest , src , n ) ;
	return m ;
	*/
}

__typeof( __wcsxfrm_l ) wcsxfrm_l __attribute__((weak,alias("__wcsxfrm_l"))) ;

size_t wcsxfrm( dest , src , n )
	wchar_t *dest ;
	const wchar_t *src ;
	size_t n ;
{
	return wcsxfrm_l( dest , src , n , NULL ) ;
}

