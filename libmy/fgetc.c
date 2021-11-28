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
#include <libio.h>
#include <unistd.h>

#ifdef getc
# undef getc
#endif

#define GETC_UNGETC 0x100
#if GETC_UNGETC == _IO_EOF_SEEN
# error GETC_UNGETC == _IO_EOF_SEEN
#endif

int ungetc( c , stream )
	int c ;
	FILE * stream ;
{
	/* jamais de buffering ici */
	/* inutile setvbuf( stream , NULL , _IONBF , 0 ) ; */
	/* on ne relit pas EOF */
	if ( c == EOF ) return EOF ;
	/* si déjà un UNGET */
	if ( stream-> _flags2 & GETC_UNGETC ) {
		return EOF ;
	}
	/* note la valeur du caractère à relire */
	stream-> _shortbuf[ 0 ] = (char) c ;
	/* passe EOF dans flags2 et note la présence du caractère à relire */
	stream-> _flags2 |= ( GETC_UNGETC |
		( stream-> _flags & _IO_EOF_SEEN ) ) ;
	/* enlève erreur et fin de fichier du flux */
	stream-> _flags &= ~ ( _IO_ERR_SEEN | _IO_EOF_SEEN ) ;
	/* retourne le caractère remis dans le flux */
	return c ;
}

int fgetc( stream )
	FILE * stream __attribute__((unused)) ;
{
	char c ;
	/* jamais de buffering ici */
	setvbuf( stream , NULL , _IONBF , 0 ) ;
	/* s'il y a un caractère à relire en attente */
	if ( stream-> _flags2 & GETC_UNGETC ) {	
		/* récupère le caractère */
		c = stream -> _shortbuf[ 0 ] ;
		/* repasse l'indicateur EOF dans _flags */
		stream-> _flags |= stream-> _flags2 & _IO_EOF_SEEN ;
		/* remet flags2 en état normal */
		stream-> _flags2 &= ~ ( GETC_UNGETC | _IO_EOF_SEEN ) ;
	} else {
		int n ;
		n = read( stream-> _fileno , & c , 1 ) ;
		if ( n < 0 ) {
			/* note l'erreur */
			stream-> _flags |= _IO_ERR_SEEN ;
			return EOF ;
		}
		if ( n != 1 ) {
			/* note la fin de fichier */
			stream-> _flags |= _IO_EOF_SEEN ;
			return EOF ;
		}
	}
	return (unsigned char) c ;
}

__typeof( fgetc ) getc __attribute__((weak,alias("fgetc"))) ;

