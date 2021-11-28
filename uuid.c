/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * Fonction de génération de nombres aléatoires et de UUID.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"
#include <efi.h>
#include <efilib.h>

#include <errno.h>
#include <stdlib.h>
#include <uuid/uuid.h>


/**
 * Racine pour les nombres aléatoires.
 */
static uint32_t holdrand = 1 ;

/**
 * Positionne la racine.
 */
void srand( seed )
	unsigned int seed ;
{
	UEFI_dprintf( D_INIT | D_DEBUG , "--> srand(%d)\n" , seed ) ;
	/* store this seed */
	holdrand = (uint32_t) seed ;
	UEFI_dprintf( D_INIT | D_DEBUG , "<-- srand()\n" ) ;
}

/*
 * Génère un nombre aléatoire.
 */
int rand()
{
	/* return a pseudo-random in range 0-0x7fff */
	return ( ( holdrand = holdrand * 214013L + 2531011L ) >> 16 )
		& 0x7fff ;
}

/*
 * Construit un UUID.
 */
void uuid_generate( out )
	uuid_t out ;
{
	size_t i ;
	/* generates 128 random bits for new UUID */
	for ( i = 0 ; i < 16 ; i ++ ) {
		int v ;
		/* if we didn't knew max rand() value and since the most
		   random bits are the most significant we would do
		   v=(int)(256.0f*((float)rand()/(RAND_MAX+1.0f)));
		   but here it is simpler beacuse RAND_MAX == 0x7fff  */
		v = rand() >> 7 ;
		out[ i ] = (unsigned char) v ;
	}
	/* set variant 10x and version 4 as required by RFC 4122 */
	out[ 8 ] = 0x80 | ( out[ 8 ] & 0x3f ) ;
	out[ 6 ] = 0x40 | ( out[ 6 ] & 0xf ) ;
}

