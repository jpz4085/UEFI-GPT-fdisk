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
#include "dietfeatures.h"

#ifdef WANT_LINKER_WARNINGS

#ifndef __ASSEMBLER__

#define link_warning(symbol,msg) \
  asm (".section .gnu.warning." symbol "\n\t.string \"" msg "\"\n\t.previous");

#else

#define link_warning(symbol,msg) \
  .section .gnu.warning.##symbol ;\
  .string msg ;\
  .previous

#endif

#else

#define link_warning(foo,bar)

#endif
