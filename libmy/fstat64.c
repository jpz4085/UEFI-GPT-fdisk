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

/* needed to get definition of "struct stat64" from sys/stat.h
   do this first otherwise file "sys/types.h" will have already
   been included by others. */
#define _LARGEFILE64_SOURCE 1
#include <sys/stat.h>
#include "libmy.h"
#include <sys/types.h>
#include <unistd.h>

int fstat64( fd , buf )
        int fd ;
        struct stat64 * buf ;
{
        return __fxstat64( 1 , fd , buf ) ;
}

