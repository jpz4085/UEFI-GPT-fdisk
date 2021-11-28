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
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "dietstdio.h"

int __sprintf_chk(
        char * s ,
        int flag __attribute__((unused)) ,
        size_t slen __attribute__((unused)) ,
        const char * format ,
        ... )
{
        va_list arg ;
        int done ;
        va_start( arg , format ) ;
        done = __vsnprintf_chk( s , (size_t) -1 , 0 , (size_t) -1 , format , arg ) ;
        va_end( arg ) ;
        return done ;
}

