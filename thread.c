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
 * Gestion mulithread.
 * La librairie libstdc++ s'attend à un environnement habituel
 * dans lequel le multithread est possible, mais en UEFI ce
 * n'est pas le cas. Toutefois les fonctions dans ce modules sont
 * appelées via le mode @PLT et si elles ne sont définies nulle
 * part, le linker laisse la gestion via la GLOBAL_OFFSET_TABLE
 * ce qui fera crasher le programme.
 * Rechercher GOT / PLT sur Internet pour plus d'explication.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"

#include <pthread.h>


/**
 * pthread_once() : appelle une fonction une seule fois.
 */
int pthread_once( control , routine )
	pthread_once_t * control ;
	void ( * routine )() ;
{
	if ( * control == PTHREAD_ONCE_INIT ) {
		( * control ) ++ ;
		( * routine )() ;
	}
	return 0 ;
}


/**
 * dl_tls_index : argument passé à __tls_get_addr().
 */
typedef struct dl_tls_index
{
	uint64_t ti_module;
	uint64_t ti_offset;
} tls_index;


/**
 * __tls_get_addr() : retourne l'adresse d'une variable "thread local".
 * Cela n'a pas de sens en mode UEFI : un seul thread est possible donc
 * la fonction retourne juste l'adresse qui a été ajustée par _relocate
 * du module relocate.
 */
void *
__tls_get_addr ( ti )
	tls_index * ti ;
{
	UEFI_dprintf( D_DEBUG | D_INIT , "__tls_get_addr( mod=%ld , off=%p )\n" , ti-> ti_module , (void *) ti-> ti_offset ) ;
	/*
	__fortify_fail( "__tls_get_addr()" ) ;
	*/
	return (void*) ti-> ti_offset ;
}

