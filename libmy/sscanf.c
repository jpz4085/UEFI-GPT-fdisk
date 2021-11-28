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

extern int sscanf( const char * s , const char * format , ... ) ;

#include "libmy.h"
#include <stdarg.h>
/*
 on n'inclut surtout pas stdio.h
 car il transforme la définition de sscanf() en __iso99_...
 qui ne va pas du tout ! */

#ifdef __sscanf
# undef __sscanf
#endif

#ifdef vsscanf
# undef vsscanf
#endif

#ifdef __vsscanf
# undef __vsscanf
#endif

#ifdef sscanf
# undef sscanf
#endif

extern int __vsscanf(const char* str, const char* format, va_list arg_ptr) ;

int __sscanf( const char * s , const char * format , ... )
{
        va_list arg ;
        int done ;
        va_start( arg , format ) ;
        done = __vsscanf( s , format , arg ) ;
        va_end( arg ) ;
        return done ;
}

__typeof (__sscanf) sscanf __attribute__((weak, alias("__sscanf"))) ;

