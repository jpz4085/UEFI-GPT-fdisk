/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en février 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * Entrée sortie FILE de stdio.
 *
 */


/* en premier */
#include "libmy.h"
#include <stdio.h>


/**
 * fdopen() : ouverture d'après un descripteur.
 */
FILE *
fdopen( fd , mode )
	int fd __attribute__((unused)) ;
	const char * mode __attribute__((unused)) ;
{
	__fortify_fail( "fdopen()" ) ;
}

