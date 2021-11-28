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
 * Affichage de messages pour le déverminage.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"
#include <efi.h>
#include <efilib.h>
#include <efidebug.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>


#ifdef EFI_DEBUG


/**
 * Le masque de debug actuel.
 * Tous les bits ne sont pas utilisés ici donc la valeur -1 indique que ce
 * module n'a pas encore été initialisé.
 * De plus cela forcera l'appel à la fonction uefi_debug_mask() lors du
 * premier emploi de la macro UEFI_dprintf().
 */
unsigned UEFI_debug_mask = (unsigned) -1 ;


/*
 * Récupère la valeur de la variable globale UEFI EFIDebug.
 * Ce code n'utilise pas la variable globale EFIDebug exportée par GNU EFI
 * car il a besoin de la valeur bien avant l'appel à InitializeLib().
 */
static void get_EFIDebug()
{
	static const char varname[] = { 'E' , 0 , 'F' , 0 , 'I' , 0 ,
		'D' , 0 , 'e' , 0 , 'b' , 0 , 'u' , 0 , 'g' , 0 , 0 , 0 } ;
	EFI_STATUS status ;
	UINT32 attribs ;
	UINTN size ;
	UINTN efidebug ;
	size = sizeof( efidebug ) ;
	status = UEFI_call( ST-> RuntimeServices-> GetVariable , varname ,
		& EfiGlobalVariable , & attribs , & size , & efidebug ) ;
	if ( status == EFI_SUCCESS ) {
		UEFI_debug_mask = efidebug ;
	} else if ( status != EFI_UNSUPPORTED ) {
		/* cas spécial pour la version Linux */
		UEFI_debug_mask = 0 ;
	}
}


/*
 * Contrôle le masque passé avec le masque de debug actuel.
 * Retourne vrai uniquement si tous les bits du masque passé sont
 * présents dans le masque.
 */
int uefi_debug_mask( mask )
	unsigned mask ;
{
	/* la valeur est -1 au tout premier appel */
	if ( UEFI_debug_mask == (unsigned) -1 ) {
		/* recupère la variable */
		get_EFIDebug() ;
		/* initialize */
		if ( UEFI_debug_mask & D_DEBUG ) UEFI_debug_mask |= D_INFO ;
		if ( UEFI_debug_mask & D_INFO ) UEFI_debug_mask |= D_WARN ;
		if ( UEFI_debug_mask & D_WARN ) UEFI_debug_mask |= D_ERROR ;
		if ( ! ( UEFI_debug_mask & D_ERROR ) ) UEFI_debug_mask = 0 ;
		/* chargement du code ça va */
		UEFI_debug_mask &= ~D_INIT ;
		/* malloc et free ça va */
		UEFI_debug_mask &= ~D_POOL ;
	}
	/* la macro a contrôlé qu'un bit au moins est présent
	   il reste à contrôler que le niveau est le bon */
	return ( UEFI_debug_mask | mask ) == UEFI_debug_mask ;
}


/*
 * Écrit un décimal sans signe sur la console.
 */
static void uefi_dprint_uint( v )
	uint64_t v ;
{
	/* le plus grand uint64_t est 18_446_744_073_709_551_615 de 20
	   caractères de long, on met les chiffres en commençant par le
	   dernier dans un buffer terminé par un \0 */
	char out[ 21 ] ;
	size_t p = sizeof( out ) ;
	out[ -- p ] = '\0' ;
	while ( v ) {
		int val = '0' + ( v % 10 ) ;
		out[ -- p ] = val ;
		v /= 10 ;
	}
	uefi_dprint_str( out + p ) ;
}


/**
 * Compteur du nombre de "Entre dans "
 */
static int compt ;


/**
 * Écrit la chaîne passée sur la console.
 */
void uefi_dprint_str( s )
	const char * s ;
{
	int l , i ;
	if ( s[ 0 ] == 'E' && s[ 1 ] == 'n' && s[ 2 ] == 't' && s[ 3 ] == 'r'
		&& s[ 4 ] == 'e' && s[ 5 ] == ' ' && s[ 6 ] == 'd'
		&& s[ 7 ] == 'a' && s[ 8 ] == 'n' && s[ 9 ] == 's'
		&& s[ 10 ] == ' ' )
	{
		compt ++ ;
		uefi_dprint_uint( compt ) ;
		UEFI_console_putchar( ']' , 1 ) ;
	}
	/* écrit sur StdErr */
	l = strlen( s ) ;
	for ( i = 0 ; i < l ; i ++ ) {
		/* message de debug, ne teste pas la valeur de retour */
		UEFI_console_putchar( s[ i ] , 1 ) ;
	}
}


#endif /* EFI_DEBUG */

