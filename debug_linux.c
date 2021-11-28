/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en mais 2015.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * Compléments pour un link correct de gdisk.
 * Le module debug.c emploie des fonctions qui ne sont disponibles que dans
 * GNU EFI ou io.c, ce module apporte les mêmes fonctionnalités pour Linux.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"
#include <efi.h>
#include <efilib.h>

#include <stdio.h>


#ifdef EFI_DEBUG


/**
 * Lit un caractère depuis la console.
 * @return le caractère lu (valeur unicode)
 *         ou -1 en cas d'erreur (consulter errno)
 * Note : pour simplifier ce code ne gère pas tout Unicode mais seulement
 *        la partie qui se code en un CHAR16 en UTF-16
 * Note : ce code est appelé par les macros de debug donc pour éviter
 *        une récursion infinie, il n'appelle aucun code de debug.
 */
long UEFI_console_getchar()
{
	return getchar() ;
}


/**
 * Écrit un caractère sur la console.
 * @param c Le caractère Unicode à écrire.
 * @param e Écrire sur StdErr plutôt que ConOut.
 * @return Le caractère Unicode écrit convertit en int
 *         ou -1 en cas d'erreur (consulter errno).
 * Note : ce code est appelé par les macros de debug donc pour éviter
 *        une récursion infinie, il n'appelle aucun code de debug.
 */
int UEFI_console_putchar( c , e )
	long c ;
	char e ;
{
	FILE * f ;
	f = e ? stderr : stdout ;
	if ( fputc( c , f ) == EOF ) return -1 ;
	return (unsigned int) c ;
}


/**
 * Autres élémens employés dans le module debug.c.
 */

static EFI_RUNTIME_SERVICES leRT ;
EFI_RUNTIME_SERVICES * RT = & leRT ;
static EFI_SYSTEM_TABLE leST = { .RuntimeServices = & leRT } ;
EFI_SYSTEM_TABLE * ST = & leST ;
EFI_GUID EfiGlobalVariable ;
UINT64 efi_call5( f , a1 , a2 , a3 , a4 , a5 )
	void * f __attribute__((unused)) ;
	UINT64 a1 __attribute__((unused)) ;
	UINT64 a2 __attribute__((unused)) ;
	UINT64 a3 __attribute__((unused)) ;
	UINT64 a4 __attribute__((unused)) ;
	UINT64 a5 __attribute__((unused)) ;
{
	return EFI_UNSUPPORTED ;
}


#endif /* EFI_DEBUG */

