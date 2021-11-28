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
 * Definitions for module debug.c.
 *
 * This mainly defines the macro UEFI_dprintf() to print a debug message.
 * This module queries the UEFI firmware to get the value of its EFIDebug
 * variable and know what messages to print or not.
 * The debug code is only included in the program if the macro EFI_DEBUG
 * was defined at compile-time, the value of the EFIDebug variable is
 * ignored otherwise.
 *
 * The first argument is the bit mask giving the reason and optional
 * severity of the message. The second argument is a printf() format
 * string and remaining arguments are the values to be put in the
 * formatted string. Ex:
 *   UEFI_dprintf( D_POOL , "--> malloc(%d)\n" , count ) ;
 * Of course, when the code calls this macro no side effect is allowed, no:
 *   UEFI_dprintf( D_INIT , "%d" , x ++ ) ;
 * or alike!
 *
 */

#ifndef debug_h_INCLUDED
#define debug_h_INCLUDED

#include <stdio.h>

/* This file is included from C++ source files. */
#if defined( __cplusplus )
extern "C" {
#endif

/* Bitmasks defining what to debug. Copied from GNU EFI efidebug.h */
#define D_INIT          0x00000001          /* Initialization style messages */
#define D_WARN          0x00000002          /* Warnings */
#define D_LOAD          0x00000004          /* Load events */
#define D_FS            0x00000008          /* EFI File system */
#define D_POOL          0x00000010          /* Alloc & Free's */
#define D_PAGE          0x00000020          /* Alloc & Free's */
#define D_INFO          0x00000040          /* Verbose */
#define D_VAR           0x00000100          /* Variable */
#define D_PARSE         0x00000200          /* Command parsing */
#define D_BM            0x00000400          /* Boot manager */
#define D_BLKIO         0x00001000          /* BlkIo Driver */
#define D_BLKIO_ULTRA   0x00002000          /* BlkIo Driver */
#define D_TXTIN         0x00010000          /* Simple Input Driver */
#define D_TXTOUT        0x00020000          /* Simple Text Output Driver */
#define D_ERROR         0x80000000          /* Error */

/* Special bitmask for extra debug options. */
#define D_DEBUG         0x00000080          /* Verbose for debug */

/* A little macro magic to be able to call all the same way:
   UEFI_dprintf( mask , "text" )
   UEFI_dprintf( mask , "text %d" , i )
   UEFI_dprintf( mask , "text %d %s" , i , s )
   and so on...
*/

/* retourne le premier argument de ... */
#define UEFI_dprintf_PREMS( ... ) UEFI_dprintf_PREMS_( __VA_ARGS__ , ignore )

/* macro d'aide pour UEFI_dprintf_PREMS() */
#define UEFI_dprintf_PREMS_( un , ... ) un

/* retourne ", x" si ... a plus de deux argument, rien sinon */
#define UEFI_dprintf_RESTE( ... ) UEFI_dprintf_RESTE_( \
	UEFI_dprintf_NBR( __VA_ARGS__ ) , __VA_ARGS__ )

/* macro d'aide pour UEFI_dprintf_RESTE() */
#define UEFI_dprintf_RESTE_( quantite , ... ) \
	UEFI_dprintf_RESTE__( quantite , __VA_ARGS__ )

/* seconde macro d'aide pour UEFI_dprintf_RESTE() quantite est ici un mot */
#define UEFI_dprintf_RESTE__( quantite , ... ) \
	UEFI_dprintf_RESTE_##quantite( __VA_ARGS__ )

/* ignore si un seul argument */
#define UEFI_dprintf_RESTE_UN( un )

/* sinon insère une virgule avant le reste */
#define UEFI_dprintf_RESTE_PLUS( un , ... ) , __VA_ARGS__

/* retourne "UN" ou "PLUS" si ... a un ou plus d'arguments (max 15) */
#define UEFI_dprintf_NBR( ... ) UEFI_dprintf_NBR16( __VA_ARGS__ , \
	PLUS , PLUS , PLUS , PLUS , PLUS , PLUS , PLUS , PLUS ,   \
	PLUS , PLUS , PLUS , PLUS , PLUS , PLUS , UN , ignore )

/* retourne le seizième de ses (au moins) dix-sept arguments */
#define UEFI_dprintf_NBR16( un , deux , trois , quatre , cinq ,   \
	six , sept , huit , neuf , dix , onze , douze , treize ,  \
	quatorze , quinze , seize , ... ) seize

#ifdef EFI_DEBUG

extern unsigned UEFI_debug_mask ;

extern int uefi_debug_mask( unsigned ) ;

/* fonction interne qui écrit la chaîne sur la console */
extern void uefi_dprint_str( const char * ) ;

/* Appeler snprintf() directement dans cette macro permet au compilateur
   de contrôler les arguments par rapport au format passé: le compilateur
   GNU produit un warning s'il détecte une incohérence. */
#define UEFI_dprintf( mask , ... ) do {                           \
	if ( UEFI_debug_mask & ( mask ) &&                        \
		uefi_debug_mask( mask ) )                         \
	{                                                         \
		char text[ 1000 ] ;                               \
		sprintf( text , UEFI_dprintf_PREMS( __VA_ARGS__ ) \
			UEFI_dprintf_RESTE( __VA_ARGS__ ) ) ;     \
		uefi_dprint_str( text ) ;                         \
	}                                                         \
} while ( 0 )

#else

/* Cette version en mode non-debug sera enlevée du code par l'optimiseur
   mais permet de faire un contrôle de la syntaxe de l'appel. */
#define UEFI_dprintf( mask , ... ) while ( 0 ) {                  \
	char text[ 1000 ] ;                                       \
	sprintf( text , UEFI_dprintf_PREMS( __VA_ARGS__ )         \
		UEFI_dprintf_RESTE( __VA_ARGS__ ) ) ;             \
}

#endif /* EFI_DEBUG */

#if defined( __cplusplus )
}
#endif

#endif /* debug_h_INCLUDED */

