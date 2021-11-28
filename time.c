/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en mars 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * Gestion du temps.
 * En fait la fonction time() n'est pas utilisée ici pour avoir une
 * référence à l'heure mais seulment pour initialiser le générateur
 * de nombres aléatoires ainsi que par libmy pour sa fonction delay().
 * La fonction time() codée ici ne retourne par la bonne valeur (nombre
 * de secondes depuis le 1er janvier 1970, 0h GMT) mais celle-ci
 * s'incrémente tout de même à chaque seconde sauf peut-être aux moments
 * spéciaux de bascule heure d'hiver / heure d'été.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"

#include <time.h>


/**
 * time() : Retourne une valeur bidon qui change à chaque seconde.
 */
time_t time( ptr )
	time_t * ptr ;
{
	EFI_STATUS status ;
	EFI_TIME tim ;
	EFI_TIME_CAPABILITIES cap ;
	time_t val ;
	status = UEFI_call( RT-> GetTime , & tim , & cap ) ;
	if ( status == EFI_SUCCESS ) val = 0 ;
	val = val + tim. Year - 1970 ;
	val = val * 12 + tim. Month - 1 ;
	val = val * 31 + tim. Day - 1 ;
	val = val * 24 + tim. Hour ;
	val = val * 60 + tim. Minute + tim. TimeZone ;
	val = val * 60 + tim. Second ;
	if ( ptr ) * ptr = val ;
	return val ;
}

