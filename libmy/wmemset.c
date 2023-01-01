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
#include <wchar.h>

wchar_t * wmemset(s, c, n)
        wchar_t * s __attribute__((unused)) ;
        wchar_t c __attribute__((unused)) ;
        size_t n __attribute__((unused)) ;
{
        __fortify_fail( "wmemset()" ) ;
        /*
        size_t i;
        wchar_t * p ;
        p = s ;
        for ( i = 0 ; i < n ; i ++ ) {
                *p = c ;
                p ++ ;
        }
        return s ;
        */
}

wchar_t * __wmemset_chk (s, c, n, dstlen)
	wchar_t *s __attribute__((unused)) ;
	wchar_t c __attribute__((unused)) ;
	size_t n __attribute__((unused)) ;
	size_t dstlen __attribute__((unused)) ;
{
	__fortify_fail( "__wmemset_chk()" ) ;
}
