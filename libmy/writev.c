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
#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>

ssize_t writev( fd , iov , iovcnt )
	int fd ;
	const struct iovec * iov ;
	int iovcnt ;
{
	int count ;
	ssize_t total = 0 ;
	for ( count = 0 ; count < iovcnt ; count ++ ) {
		const struct iovec v = iov[ count ] ;
		ssize_t l = write( fd , v.iov_base , v.iov_len ) ;
		if ( l < 0 ) return l ;
		total += l ;
		if ( (size_t) l != v.iov_len ) {
			break ;
		}
	}
	return total ;
}

