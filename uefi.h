/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 *
 * Original work is copyleft Bernard Burette.
 * Modifications are copyleft Joseph Zeller.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 * Définitions pour notre colle entre UEFI et un programme Linux.
 * Définitions pour l'appel des fonctions UEFI.
 *
 * L'appel aux fonctions UEFI se fait via une macro C définie ici.
 * Le nombre d'arguments passé à la macro est calculé automatiquement
 * via un peu de magie des macros C pour appeller la fonction efi_callX
 * appropriée.
 * Celles-ci se trouvent dans le fichier efi_stub.S dans la librairie
 * gnuefi.
 */


/* Pas de double inclusion. */
#ifndef uefi_h_INCLUDED
#define uefi_h_INCLUDED


/* Pour obtenir la définition de "struct stat64" depuis sys/stat.h
   il faut faire cela en tout premier sinon le fichier sys/types.h
   aura déjà été inclus par un autre fichier d'en-tête mais sans
   cette définition. */
#define _LARGEFILE64_SOURCE 1
#include <sys/stat.h>


/* Pour éviter des inline qui retournent des "double" ce qui n'est pas
 * autorisé en mode UEFI où la partie MMS/SSE du processeur n'est pas
 * encore initialisée. */
#undef __USE_EXTERN_INLINES
#include <stdlib.h>


/* This file is included from C++ source files */
#if defined( __cplusplus )
extern "C" {
#endif


/* GNU EFI includes */
#include <efi.h>
#include <efilib.h>


/**
 * UEFI Image Handle provided on code entry.
 * Exporté du module start-$(ARCH).
 */
extern EFI_HANDLE UEFI_ImageHandle ;


/**
 * UEFI System Table provided on code entry.
 * Exporté des modules start-$(ARCH) et de la librairie GNU EFI.
 */
extern EFI_SYSTEM_TABLE * ST ;


/**
 * Une erreur qui abandonne l'exécution.
 * Affiche un message puis termine l'exécution.
 * La fonction est dans initfini.c.
 * @param mess Le message.
 */
extern void __fortify_fail( const char * msg ) __attribute__((noreturn)) ;


/**
 * Construit le nom d'un disque d'après son handle UEFI.
 * @param handle Le EFI_HANDLE dont on souhaite obtenir une représentation.
 * @return Un buffer "static" qui contient cela (ce code n'est pas
           multi-thread).
 * La fonction est dans io.c.
 */
const char * UEFI_disk_name( EFI_HANDLE handle ) ;


/**
 * Lit un caractère depuis la console.
 * @return Le caractère Unicode lu
 *         ou -1 en cas d'erreur (consulter errno).
 * La fonction est dans io.c.
 */
long UEFI_console_getchar() ;


/**
 * Écrit un caractère sur la console.
 * @param c Le caractère Unicode à écrire.
 * @param e Écrire sur StdErr pluôt que ConOut.
 * @return Le caractère Unicode écrit convertit en int
 *         ou -1 en cas d'erreur (consulter errno).
 * La fonction est dans io.c.
 */
int UEFI_console_putchar( long c , char e ) ;


/**
 * La macro UEFI_call() appelle une fonction du firmware UEFI.
 * Le premier argument est la fonction, les autres sont ses arguments.
 */
#define UEFI_call( fun , ... ) UEFI_call_count( __VA_ARGS__ ) \
	( fun , __VA_ARGS__ )

#define UEFI_call_count( ... ) UEFI_call_count2( \
	UEFI_call_count4( __VA_ARGS__ ) , __VA_ARGS__ )

#define UEFI_call_count2( quantite , ... ) \
	UEFI_call_count3( quantite , __VA_ARGS__ )

#define UEFI_call_count3( quantite , ... ) UEFI_call##quantite

#define UEFI_call_count4( ... ) UEFI_call_count5( __VA_ARGS__ , \
	10 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 , 1 , 0 , inutile )

#define UEFI_call_count5( un , deux , trois , quatre , cinq , six , sept , \
	huit , neuf , dix , onze , douze , ... ) onze

#define UEFI_call1( f , a1 ) efi_call1( (void*) ( f ) , \
	(UINTN) ( a1 ) )

#define UEFI_call2( f , a1 , a2 ) efi_call2( (void*) ( f ) , \
	(UINTN) ( a1 ) , (UINTN) ( a2 ) )

#define UEFI_call3( f , a1 , a2 , a3 ) efi_call3( (void*) ( f ) , \
	(UINTN) ( a1 ) , (UINTN) ( a2 ) , (UINTN) ( a3 ) )

#define UEFI_call4( f , a1 , a2 , a3 , a4 ) efi_call4( (void*) ( f ) , \
	(UINTN) ( a1 ) , (UINTN) ( a2 ) , (UINTN) ( a3 ) , (UINTN) ( a4 ) )

#define UEFI_call5( f , a1 , a2 , a3 , a4 , a5 ) efi_call5( (void*) ( f ) , \
	(UINTN) ( a1 ) , (UINTN) ( a2 ) , (UINTN) ( a3 ) , (UINTN) ( a4 ) , \
	(UINTN) ( a5 ) )
	
#define UEFI_call6( f , a1 , a2 , a3 , a4 , a5 , a6 ) efi_call6( (void*) ( f ) , \
	(UINTN) ( a1 ) , (UINTN) ( a2 ) , (UINTN) ( a3 ) , (UINTN) ( a4 ) , \
	(UINTN) ( a5 ) , (UINTN) ( a6 ) )

#if defined( __cplusplus )
}
#endif


#endif /* uefi_h_INCLUDED */

