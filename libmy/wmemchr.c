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

extern wchar_t *wmemchr (const wchar_t *__s, wchar_t __c, size_t __n) ;

wchar_t * wmemchr(
        const wchar_t * s __attribute__((unused)) ,
        wchar_t c __attribute__((unused)) ,
        size_t n __attribute__((unused))
)
{
        __fortify_fail( "wmemchr()" ) ;
        /*
        size_t i ;
        for ( i = 0 ; i < n ; i++ ) {
                if ( * s == c ) {
                        return (wchar_t *) s ;
                }
                s ++ ;
        }
        return NULL ;
        */
}

