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
 * setvbuf
 *
 */


/* en premier */
#include "libmy.h"
#include <stdio.h>


/**
 * setvbuf() : choix du buffering.
 * Toutes les fonctions associées aux FILE de libmy commencent
 * par un setvbuf( f , NULL , _IONBF , 0 ) pour ne pas utiliser
 * de buffer du tout. L'appel est ici ignoré.
 */
int
setvbuf( stream , buffer , mode , size )
	FILE * stream __attribute__((unused)) ;
	char * buffer __attribute__((unused)) ;
	int mode __attribute__((unused)) ;
	size_t size __attribute__((unused)) ;
{
	return 0 ;
}

