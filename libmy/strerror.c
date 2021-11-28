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

int strerror_r( errnum , buf , len )
	int errnum __attribute__((unused)) ;
	char * buf __attribute__((unused)) ;
	size_t len __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "strerror_r()" ) ;
        /*
	return 0 ;
	*/
}

static char * my_strerror __attribute__((unused)) ;

char * strerror( errnum )
	int errnum __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "strerror()" ) ;
	/*
        char *ret = strerror_r( errnum , NULL , 0 ) ;
        int saved_errno ;
        if ( ret != NULL ) {
                return ret ;
        }
        saved_errno = errno ;
        if ( my_strerror == NULL ) {
                my_strerror = malloc( 1024 ) ;
                errno = saved_errno ;
                if ( my_strerror == NULL ) {
                        return "Unknown error" ;
                }
        }
        return strerror_r( errnum , my_strerror , 1024 ) ;
	*/
}

