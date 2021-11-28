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

wchar_t * wmemcpy( dest , src , n )
        wchar_t * dest __attribute__((unused)) ;
        const wchar_t * src __attribute__((unused)) ;
        size_t n __attribute__((unused)) ;
{
        __fortify_fail( "wmemcpy()" ) ;
        /*
        return (wchar_t *) memcpy( dest , src , n * sizeof( wchar_t ) ) ;
        */
}

