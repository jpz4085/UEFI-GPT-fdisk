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

size_t mbsnrtowcs( dest , src , nms , len , ps )
	wchar_t * dest __attribute__((unused)) ;
	const char** src __attribute__((unused)) ;
	size_t nms __attribute__((unused)) ;
	size_t len __attribute__((unused)) ;
	mbstate_t* ps __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "mbsnrtowcs()" ) ;
}

size_t __mbsnrtowcs_chk (dst, src, nmc, len, ps, dstlen)
	wchar_t *dst __attribute__((unused)) ;
	const char **src __attribute__((unused)) ;
	size_t nmc __attribute__((unused)) ;
	size_t len __attribute__((unused)) ;
	mbstate_t *ps __attribute__((unused)) ;
	size_t dstlen __attribute__((unused)) ;
{
	__fortify_fail( "__mbsnrtowcs_chk()" ) ;
}
