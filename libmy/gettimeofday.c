/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en février 2014.
 *
 * Original work is copyleft Bernard Burette.
 * Modifications are copyleft Joseph Zeller.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/* en premier */
#include "libmy.h"
#include <sys/time.h>


/**
 * gettimeofday : date/heure locale.
 */
int
gettimeofday( tv , tz )
	struct timeval *tv __attribute__(( unused )) ;
	void *tz __attribute__(( unused )) ;
{
	/* TODO: ? */
	__fortify_fail( "gettimeofday()" ) ;
}

