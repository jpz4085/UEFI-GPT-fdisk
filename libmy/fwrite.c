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

#define _XOPEN_SOURCE 600
#include "libmy.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

#ifdef fwrite_unlocked
# undef fwrite_unlocked
#endif

size_t fwrite_unlocked( ptr , size , count , stream )
        const void * ptr ;
        size_t size ;
        size_t count ;
        FILE * stream ;
{
        const char * p ;
        size_t n ;
	/* jamais de buffering ici */
	setvbuf( stream , NULL , _IONBF , 0 ) ;
        p = (const char *) ptr ;
        n = 0 ;
        while ( count ) {
                if ( write( stream-> _fileno , p , size ) != (ssize_t) size ) {
			stream-> _flags |= _IO_ERR_SEEN ;
                        break ;
                }
                p += size ;
                count -- ;
                n ++ ;
        }
        return n ;
}

__typeof( fwrite_unlocked ) fwrite __attribute__((weak,alias("fwrite_unlocked"))) ;

