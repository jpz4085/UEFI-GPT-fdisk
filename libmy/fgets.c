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
#include <stdio.h>

#include "dietstdio.h"

char *fgets_unlocked(char *s, int size, FILE *stream) {
  int l;
  for (l=0; l<size; ) {
    register int c;
      c=fgetc(stream);
      if (c==EOF) {
	if (!l) return 0;
	goto fini;
      }
    s[l]=c;
    ++l;
    if (c=='\n') {
fini:
      s[l]=0;
      return s;
    }
  }
  return 0;
}

char*fgets(char*s,int size,FILE*stream) __attribute__((weak,alias("fgets_unlocked")));

