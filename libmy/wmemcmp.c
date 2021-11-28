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

int wmemcmp( s1 , s2 , n )
        const wchar_t * s1 __attribute__((unused)) ;
        const wchar_t * s2 __attribute__((unused)) ;
        size_t n __attribute__((unused)) ;
{
        __fortify_fail( "wmemcmp()" ) ;
        /*
        size_t i ;
        for ( i = 0 ; i < n ; i ++ ) {
                if ( *s1 != *s2 ) {
                        return *s1 > *s2 ? 1 : -1 ;
                }
                s1 ++ ;
                s2 ++ ;
        }
        return 0 ;
        */
}

