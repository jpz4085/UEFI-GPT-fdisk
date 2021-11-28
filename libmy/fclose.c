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

#define _XOPEN_SOURCE 600
#include "libmy.h"
#undef __USE_EXTERN_INLINES
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int fclose_unlocked(FILE *stream) {
  int res;
  /*FILE *f,*fl;*/
  /* no buffers, no flush needed
  res=fflush_unlocked(stream);
  */
  /*res|=__libc_close(stream->fd);*/
  res = close( stream-> _fileno ) ;
  if ( res < 0 ) res = EOF ;
  /*
  for (fl=0,f=__stdio_root; f; fl=f,f=f->next)
    if (f==stream) {
      if (fl)
	fl->next=f->next;
      else
	__stdio_root=f->next;
      break;
    }
  */
  /* no buffers
  if ((!(stream->flags&STATICBUF))&&(stream->buf))
    free(stream->buf);
  */
  if ( stream != stdin && stream != stdout && stream != stderr ) {
    free(stream);
  }
  return res;
}

int fclose(FILE *stream) __attribute__((weak,alias("fclose_unlocked")));

