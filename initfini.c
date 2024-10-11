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

/**
 * Initialization and finalization.
 */


/* en premier */
#include "uefi.h"
#include "debug.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/*
 * Arguments à passer aux fonctions d'init.
 */
static char* argv[] = { "gdisk" , NULL , NULL } ;
static char* envp[] = { NULL } ;


/**
 * Initialization symbols defined in the elf-$(ARCH).lds script.
 * These have been renamed to avoid matching the GNU-EFI defaults.
 */
extern void ( * init_array_start[] ) ( int , char ** , char ** ) ;
extern void ( * init_array_end[] ) ( int , char ** , char ** ) ;
extern void ( * fini_array_start[] ) ( void ) ;
extern void ( * fini_array_end[] ) ( void ) ;


/**
 * Fonction interne pour enregistrer une fonction de fin d'exécution.
 */
static void __internal_atexit( void (*func) (void) , void *arg ) ;


/**
 * Initialization function called from _start in start-x86_64.S before
 * passing control to efi_main(). _call_init will call all functions in
 * the .init_array table where the start and end are provided by the
 * init_array_start and init_array_end symbols.
 */
void
_call_init()
{
	size_t size ;
	size_t i;
	UEFI_dprintf( D_INIT | D_INFO , "Entre dans _call_init()\n" ) ;
	size = init_array_end - init_array_start;
	UEFI_dprintf( D_INIT | D_INFO , "__init_array[%ld]\n" , size ) ;
	/* debug : affiche les adresses des premières fonctions */
	for (i = 0; i < size; i++) {
		UEFI_dprintf( D_INIT | D_INFO , " __init_array[%ld] = %p\n" ,
			i , init_array_start[ i ] ) ;
		if ( i >= 4 ) {
			UEFI_dprintf( D_INIT | D_INFO , " ...\n" ) ;
			break ;
		}
	}
	for (i = 0; i < size; i++) {
		void (* appel) ( int , char ** , char ** ) ;
		appel = init_array_start[ i ] ;
		UEFI_dprintf( D_INIT | D_INFO | D_DEBUG ,
			"Appelle __init_array[%ld] = %p -" , i , appel ) ;
#ifdef EFI_DEBUG
		/* le début du code de cette fonction sur 12 octets */
		size_t j ;
		if ( uefi_debug_mask( D_INIT | D_INFO ) ) {
			for ( j = 0 ; j < 12 ; j ++ ) {
				unsigned char * x = (unsigned char *) appel ;
				UEFI_dprintf( D_INIT | D_INFO , " %02x" ,
					x[ j ] ) ;
			}
			UEFI_dprintf( D_INIT | D_INFO , "\n--> Touche : " ) ;
			UEFI_console_getchar() ;
			UEFI_console_putchar( '\n' , 1 ) ;
		}
#endif
		/* l'appelle */
		( * appel )( 1 , argv , envp ) ;
		UEFI_dprintf( D_INIT | D_INFO , "<--\n" ) ;
	}
	/* enregistre les fonctions de fin */
	size = fini_array_end - fini_array_start;
	UEFI_dprintf( D_INIT | D_INFO , "__fini_array[%ld]\n" , size ) ;
	for (i = 0; i < size; i++) {
		void (* to_call) (void) ;
		to_call = fini_array_start[ i ] ;
		__internal_atexit( to_call , 0 ) ;
	}
	
	UEFI_dprintf( D_INIT | D_INFO , "Quitte _call_init()\n" ) ;
}


/**
 * Un truc lié à C++.
 */
int __dso_handle ;


/**
 * Types des fonctions de fin d'excution.
 */
enum {
	ef_free = 0 /* `ef_free' MUST be zero! */ ,
	ef_us , ef_on , ef_at , ef_cxa
} ;


/**
 * La structure qui enregistre une fonction de fin d'exécution.
 */
struct exit_function {
	long int flavor ;
	union {
		void (*at) (void) ;
		struct {
			void (*fn) (int status, void *arg) ;
			void *arg ;
		} on ;
		struct {
			void (*fn) (void *arg, int status) ;
			void *arg ;
			void *dso_handle ;
		} cxa ;
	} func ;
} ;


/**
 * La structure qui enregistre une fonction de fin d'exécution.
 * Cette structure permet d'en créer une liste chaînée.
 */
struct exit_function_list {
	struct exit_function_list *next ;
	struct exit_function f ;
} ;


/**
 * La liste des fonctions de fin d'exécution enregistrées.
 */
static struct exit_function_list * liste = NULL ;


/**
 * Enregistre une fonction de fin d'exécution.
 */
static void __internal_atexit( func , arg )
	void (*func) (void) ;
	void *arg ;
{
	/* crée une nouvelle structure */
	struct exit_function_list * n ;
	n = malloc( sizeof( struct exit_function_list ) ) ;
	if ( n == NULL ) __fortify_fail( "__internal_atexit() malloc()" ) ;
	memset( n , 0 , sizeof( n ) ) ;
	/* l'insère au tout début de la chaîne */
	n-> next = liste ;
	liste = n ;
	/* met les bonnes valeurs */
	n-> f. func. at = func ;
	n-> f. func. cxa. arg = arg ;
	/* debug */
	UEFI_dprintf( D_INIT | D_INFO , "__internal_atexit( %p )\n" , func ) ;
}


/**
 * Enregistre une fonction C de fin d'exécution.
 */
int atexit( func )
	void (*func) (void) ;
{
	__internal_atexit( func , NULL ) ;
	return 0 ;
}


/**
 * Enregistre une fonction C++ de fin d'exécution.
 */
int __cxa_atexit( func , arg , d )
	void (*func) (void *) ;
	void *arg ;
	void *d __attribute__((unused)) ;
{
	__internal_atexit( (void(*)(void)) func , arg ) ;
	return 0 ;
}


/**
 * __fortify_fail() : grave panne.
 */
void __fortify_fail( msg )
	const char * msg ;
{
	int ret ;
	ret = write( 2 , "** " , 3 ) ;
	/*                123 */
	ret += write( 2 , (void *) msg , strlen( msg ) ) ;
	/* car on espère que strlen() fonctionne encore... */
	ret += write( 2 , " **: terminating.\n" , 18 ) ;
	/*                 12345678901234567 8 */
	/* abandonne le programme avec un statut EFI_ABORTED */
	exit( 8 ) ;
}


/**
 * abort() : abandonne le programme avec un petit message.
 */
void abort()
{
	__fortify_fail( "abort()" ) ;
}


/**
 * __stack_chk_fail() : employée en cas de compilation avec -fstack-check.
 */
void __stack_chk_fail()
{
	__fortify_fail( "stack smashing detected" ) ;
}


/**
 * La fonction _call_fini() appelée depuis _start en fin d'exécution ou
 * depuis exit() ci-dessous.
 */
void
_call_fini( stat )
	int stat ;
{
	/* décroche une par une les fonctions et les appelle */
	while ( liste != NULL ) {
		struct exit_function_list * elem ;
		elem = liste ;
		liste = elem-> next ;
		UEFI_dprintf( D_INIT | D_INFO | D_DEBUG , "Appelle %p" ,
			elem-> f. func. cxa. fn ) ;
		elem-> f. func. cxa. fn( elem-> f. func. cxa. arg , stat ) ;
	}
}


/*
 * La fonction exit() classique.
 */
void exit( stat )
	int stat ;
{
	/* le status envoyé à UEFI:Exit */
	EFI_STATUS status ;
	/* mappe le status */
	status = stat ;
	switch ( stat ) {
		case 1 : {
			/* new() a retourné NULL
			   c++ lèvera plutôt une exception */
			status = EFI_OUT_OF_RESOURCES ;
			break ;
		}
		case 2 :
			/* erreur fatale dans gpt.cc */
		case 5 : {
			/* cin ne fonctionne plus */
			status = EFI_DEVICE_ERROR ;
			break ;
		}
		case 8 : {
			/* quitte le programme sans sauver */
			status = EFI_ABORTED ;
			break ;
		}
#if 0
		case 25 : {
			/* le test des tailles mémoire SizesOK() a raté */
			status = EFI_INCOMPATIBLE_VERSION ;
			break ;
		}
#endif
	}
	/* avant de terminer, fait le ménage */
	_call_fini( stat ) ;
	/* debug */
	UEFI_dprintf( D_INIT | D_INFO , "Fin du programme : %lx\n" , status ) ;
	/* appelle UEFI:Exit */
	UEFI_call( ST-> BootServices-> Exit , UEFI_ImageHandle ,
		status , 0 , 0 ) ;
	/* ne revient normalement pas mais sait-on jamais... */
	__fortify_fail( "BootServices:Exit()" ) ;
}

