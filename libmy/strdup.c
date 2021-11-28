/*
 *
 * This file is part of UEFI fdisk.
 *
 * UEFI fdisk is a port of util-linux fdisk to UEFI/BIOS.
 * UEFI fdisk est un portage de util-linux fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU General Public
 *  License version 2 or more.
 * La diffusion de ce code est faite selon les termes de la licence publique
 *  générale GNU version 2 ou plus.
 *
 */
#define _GNU_SOURCE
#include "libmy.h"
#include <stdlib.h>
#include <string.h>
/* Duplicate S, returning an identical malloc'd string.  */
char *
strdup (const char *s)
{
	size_t len = strlen (s) + 1;
	void *new = malloc (len);
	if (new == NULL)
		return NULL;
	return (char *) memcpy (new, s, len);
}
