/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 * Ce fichier est récupéré soit de "GNU libc" soit de "dietlibc"
 * soit encore il a été créé de toutes pièces.
 *
 * Original work is copyleft Bernard Burette.
 * Modifications are copyleft Joseph Zeller.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

#include "libmy.h"
#include <stddef.h>
#include <wctype.h>
#include <locale.h>

int __iswctype_l( wc , desc , locale )
        wint_t wc __attribute__((unused)) ;
        wctype_t desc __attribute__((unused)) ;
        __locale_t locale __attribute__((unused)) ;
{
        /* TODO: ? */
        __fortify_fail( "__iswctype_l()" ) ;
}

__typeof( __iswctype_l ) iswctype_l __attribute__((weak,alias("__iswctype_l"))) ;

int __iswctype( wc , desc )
        wint_t wc __attribute__((unused)) ;
        wctype_t desc __attribute__((unused)) ;
{
        return iswctype_l( wc , desc , NULL ) ;
}

__typeof( __iswctype ) iswctype __attribute__((weak,alias("__iswctype"))) ;

