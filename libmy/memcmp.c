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
#include <sys/types.h>
#include <string.h>

int memcmp( s1 , s2 , n )
        const void * s1 ;
        const void * s2 ;
        size_t n ;
{
        const unsigned char * p1 = (const unsigned char *) s1 ;
        const unsigned char * p2 = (const unsigned char *) s2 ;
        size_t i ;
        
        for ( i = 0 ; i < n ; i ++ ) {
                if ( * p1 != * p2 ) {
                        return * p1 > * p2 ? 1 : -1 ;
                }
                p1 ++ ;
                p2 ++ ;
        }
        return 0 ;
}

