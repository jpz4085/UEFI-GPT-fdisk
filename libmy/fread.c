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

size_t fread_unlocked( ptr , size , count , stream )
	void * ptr ;
	size_t size ;
	size_t count ;
	FILE * stream ;
{
	char * p ; /* l'adresse où stocker ce qui est lu */
	size_t n ; /* le nombre d'items lus */
	char premier = 1 ; /* la première lecture */
	/* jamais de buffering ici */
	setvbuf( stream , NULL , _IONBF , 0 ) ;
	p = (char *) ptr ;
	n = 0 ;
	while ( count ) {
		ssize_t s ; /* la taille de cet item */
		s = size ;
		while ( s ) {
			ssize_t r ;
			if ( premier ) {
				/* gestion du ungetc() */
				char c = fgetc( stream ) ;
				/* fin de fichier ou erreur */
				if ( c == EOF ) goto ret ;
				/* stocke l'octet et avance */
				* p = c ;
				p ++ ;
				s -- ;
				premier = 0 ;
			}
			/* lit un item */
			r = read( stream-> _fileno , p , s ) ;
			if ( r < 0 ) {
				/* note l'erreur */
				stream-> _flags |= _IO_ERR_SEEN ;
				goto ret ;
			}
			if ( r < s ) {
				/* note la fin de fichier */
				stream-> _flags |= _IO_EOF_SEEN ;
				goto ret ;
			}
			p += r ;
			s -= r ;
		}
		count -- ;
		n ++ ;
	}
	ret : ;
	return n ;
}

__typeof( fread_unlocked ) fread __attribute__((weak,alias("fread_unlocked"))) ;

