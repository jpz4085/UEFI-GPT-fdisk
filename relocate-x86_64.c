/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en février 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/* reloc_x86_64.c - position independent x86_64 ELF shared object relocator
   Copyright (C) 1999 Hewlett-Packard Co.
	Contributed by David Mosberger <davidm@hpl.hp.com>.
   Copyright (C) 2005 Intel Co.
	Contributed by Fenghua Yu <fenghua.yu@intel.com>.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name of Hewlett-Packard Co. nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
    BE LIABLE FOR ANYDIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
    OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
    THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/


/* en premier */
#include "uefi.h"
#include "debug.h"
#include <efi.h>
#include <efilib.h>

#include <elf.h>


/**
 * Applique les informations de relocation ELF.
 * L'exécutable est construit en ELF (executable and linkable format), qui
 * n'est pas le format des programmes EFI : le PE (portable executable).
 * En particulier comme l'adresse de chargement effective n'est pas la VMA
 * (employer la commande "objdump -h" pour afficher les VMA) et que toutes les
 * adresses dans le fichier programme emploient les VMA, il faut les ajuster.
 * Le format PE prévoit une section .reloc pour cela, mais pour que le chargeur
 * de programmes UEFI l'interprète, il faudrait la construire d'après les
 * informations de relocation du format ELF (ou faire un programme pour cela)
 * au moment de la construction du binaire, ce qui est plus compliqué de
 * conserver les informations du format ELF dans les segments .dynamic
 * (description des tables), .rela (table des entrées de relocation), et
 * .dynsym (table des symboles employés dans la table .rela) et les appliquer
 * au tout début de l'exécution.
 * Cette fonction-ci s'en charge, elle est appelée depuis le point d'entrée
 * _start dans le module "start-x86_64.c".
 */
EFI_STATUS
_relocate( imabase , dyna , systab )
	unsigned long imabase ;
	Elf64_Dyn * dyna ;
	EFI_SYSTEM_TABLE * systab ;
{
	Elf64_Rel * rela = 0;
	Elf64_Sym * dynsym = 0 ;
	long relasz = 0, relaent = 0 , syment = 0 ;
	unsigned long * addr ;
	int trt , cnt = 0 ;

	/* debug */
	UEFI_dprintf( D_INIT | D_INFO ,
		"Entre dans _relocate( image_base=0x%lx , _DYNAMIC=%p , SystemTable=%p )\n" ,
		imabase , dyna , systab ) ;
	/* parcourt les entrées dans _DYNAMIC pour trouver les 5 informations nécessaires : VMA de la table de relocation (section
	   .rela du fichier), taille (en octets) de la table, taille (en octets) d'une entrée de cette table, VMA de la table des
	   symboles dynamiques (section .dynsym du fichier), taille (en octets) d'une entrée de cette table. Cette table-ci est
	   clôturée par une entrée de type DT_NULL */
	while ( dyna-> d_tag != DT_NULL ) {
		/* selon le type d'entrée */
		switch ( dyna-> d_tag ) {
			case DT_RELA : {
				/* le début de la table .rela (ajuste sa valeur avec imabase) */
				rela = (Elf64_Rel*) ( (unsigned long) dyna-> d_un. d_ptr
					+ imabase ) ;
				/* debug */
				UEFI_dprintf( D_INIT | D_INFO , "DT_RELA(%d) rela=0x%lx -> 0x%lx\n" ,
					(int) dyna-> d_tag , (long) dyna-> d_un. d_ptr , (long) rela ) ;
				/* break */
				break ;
			}
			case DT_RELASZ : {
				/* la taille de la table .rela */
				relasz = dyna-> d_un. d_val ;
				/* debug */
				UEFI_dprintf( D_INIT | D_INFO , "DT_RELAENT(%d) relasz=%ld\n" ,
					(int) dyna-> d_tag , relasz ) ;
				/* break */
				break ;
			}
			case DT_RELAENT : {
				/* la taille d'une entrée de la table .rela */
				relaent = dyna-> d_un. d_val ;
				/* debug */
				UEFI_dprintf( D_INIT | D_INFO , "DT_RELAENT(%d) relaent=%ld\n" ,
					(int) dyna-> d_tag , relaent ) ;
				/* break */
				break ;
			}
			case DT_SYMTAB : {
				/* le début de la table .dynsym (ajuste sa valeur avec imabase) */
				dynsym = (Elf64_Sym*) ( (unsigned long) dyna-> d_un. d_ptr
					+ imabase ) ;
				/* debug */
				UEFI_dprintf( D_INIT | D_INFO , "DT_SYMTAB(%d) dynsym=0x%lx -> 0x%lx\n" ,
					(int) dyna-> d_tag , (long) dyna-> d_un. d_ptr , (long) dynsym ) ;
				/* break */
				break ;
			}
			case DT_SYMENT : {
				/* la taille d'une entrée de la table .dynsym */
				syment = dyna-> d_un. d_val ;
				/* debug */
				UEFI_dprintf( D_INIT | D_INFO , "DT_SYMENT(%d) syment=%ld\n" ,
					(int) dyna-> d_tag , syment ) ;
				/* break */
				break ;
			}
			default :
				/* autre, debug */
				UEFI_dprintf( D_INIT | D_INFO | D_DEBUG , "DT_autre(%d)\n" ,
					(int) dyna-> d_tag ) ;
		}
		/* passe à l'entrée suivante */
		dyna ++ ;
	}

	if ( rela == NULL && relaent == 0 ) {
		/* debug */
		UEFI_dprintf( D_INIT | D_INFO , "Quitte _relocate() - rien a faire\n" ) ;
		/* rien à faire ici */
		return EFI_SUCCESS ;
	}

 	if ( rela == NULL || relaent == 0 ) {
		/* problème */
		UEFI_dprintf( D_ERROR , "Quitte _relocate() - erreur EFI_LOAD_ERROR\n" ) ;
		/* si notre appelant vérifie... */
 		return EFI_LOAD_ERROR;
	}

	/* le nombre d'entrée dans la table .rela */
	trt = relasz / relaent ;
	UEFI_dprintf( D_INIT | D_INFO , "Entrees a traiter : %d" , trt ) ;

	/* traite chaque entrée de .rela */
	while ( relasz > 0 ) {
		/* selon le type d'entrée */
		switch ( ELF64_R_TYPE( rela-> r_info ) ) {
			case R_X86_64_RELATIVE : {
				/* l'adresse à corriger (ajustée avec imabase) */
				addr = (unsigned long *) ( rela-> r_offset + imabase ) ;
				/* ajuste l'adresse contenue à cette adresse */
				* addr += imabase ;
				/* debug */
				cnt ++ ;
#if 0
				UEFI_dprintf( D_INIT | D_INFO | D_DEBUG , "R_X86_64_RELATIVE(%d) 0x%lx\n" ,
					(int) ELF64_R_TYPE( rela-> r_info ) , (long) addr ) ;
#endif
				/* break */
				break ;
			}
			case R_X86_64_GLOB_DAT :
			case R_X86_64_DTPOFF64 : {
				unsigned long sym , val ;
				/* contrôle que la table des symboles est utilisable */
				if ( dynsym == NULL || syment == 0 ) {
					/* problème */
					UEFI_dprintf( D_ERROR , "Quitte _relocate() - erreur EFI_LOAD_ERROR\n" ) ;
					/* si notre appelant vérifie... */
					return EFI_LOAD_ERROR;
				}
				/* l'adresse où placer l'adresse du symbole (ajustée avec imabase) */
				addr = (unsigned long *) ( rela-> r_offset + imabase ) ;
				/* le numéro du symbole à placer à cette adresse */
				sym = ELF64_R_SYM( rela-> r_info ) ;
				/* l'adresse de ce symbole (ajustée avec imabase) */
				val = ( (Elf64_Sym*) ( (char*) dynsym + ( sym * syment ) ) )-> st_value + imabase ;
				UEFI_dprintf( D_INIT | D_INFO , "%s(%d) 0x%lx : 0x%lx %ld 0x%lx\n" ,
					ELF64_R_TYPE( rela-> r_info ) == R_X86_64_GLOB_DAT ? "R_X86_64_GLOB_DAT" :
					"R_X86_64_DTPOFF64" , (int) ELF64_R_TYPE( rela-> r_info ) ,
					(long) addr , *addr , sym , val ) ;
				/* place l'adresse du symbole à l'adresse de l'entrée de relocation */
				* addr = val ;
				/* debug */
				cnt ++ ;
				/*
				UEFI_dprintf( D_INIT | D_INFO , "R_X86_64_GLOB_DAT(%d) 0x%lx : %ld 0x%lx\n" ,
					(int) ELF64_R_TYPE( rela-> r_info ) , (long) addr , sym , val ) ;
				*/
				/* break */
				break ;
			}
			default :
				/* autre, debug */
				UEFI_dprintf( D_INIT | D_INFO | D_DEBUG , "R_X86_64_?(d)\n" ,
					(int) ELF64_R_TYPE( rela-> r_info ) ) ;
		}
		/* passe à l'entrée suivante */
		rela = (Elf64_Rel*) ( (char*) rela + relaent ) ;
		relasz -= relaent ;
	}
	/* debug */
	UEFI_dprintf( D_INIT | D_INFO , "Quitte _relocate() - a traite %d entrees sur %d\n" , cnt , trt ) ;
	/* bien terminé */
	return EFI_SUCCESS;
}

