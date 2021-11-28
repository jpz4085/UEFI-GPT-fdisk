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
 * fopen : ouverture d'un flux.
 */
#undef fopen
FILE *
fopen( name , mode )
	const char * name __attribute__((unused)) ;
	const char * mode __attribute__((unused)) ;
{
	/* TODO: ? */
	__fortify_fail( "fopen()" ) ;
}

__typeof( fopen ) fopen64 __attribute__(( weak , alias( "fopen" ) )) ;

