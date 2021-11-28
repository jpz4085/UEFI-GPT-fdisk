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

#include "libmy.h"
#include "libc-tsd.h"
#include "locale.h"
#include "localeinfo.h"
#include "ctype.h"
/* Define a constant for the name of the standard C locale, so the string
   constant is not repeated in dozens of object files.  */

#include "localeinfo.h"

/* Name of our standard locale.  */
const char _nl_C_name[] = "C";
const char _nl_POSIX_name[] = "POSIX";

/* The standard codeset.  */
const char _nl_C_codeset[] = "ANSI_X3.4-1968";
