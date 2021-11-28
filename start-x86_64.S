/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en janvier 2014.
 *
 * All this work is copyleft Bernard Burette.
 * Gauche d'auteur Bernard Burette.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * start-x86_64.S - UEFI startup code for x86_64 architecture.
 *
 * This module contains the exported _start symbol, the entry
 * point of the UEFI image (program).
 *
 * This module also provides a fake .reloc section, because
 * some UEFI loaders need that to detect a program.
 *
 */


//
// Startup code.
// Entry point of program given to "ld" is "_start" in Makefile.
// It first processes the ELF relocation information (because the UEFI loader
// will not have loaded this program at the VMA specified in the file (use
// "objdump -h" to see those VMAs) and we do not use the .reloc section
// because it is defined in µ$ crap and we would have to build it manually.
// It then calls the efi_main() function using x86_64 ABI (arguments in %rdi
// and %rsi registers.
// Also note that since UEFI uses µ$ 64 bits ABI to call this entry point
// the two arguments from the UEFI loader are received here in %rcx and %rdx
// registers.
// This file also uses external symbol _DYNAMIC automatically defined by the
// linker.
//
	.section .text.startup, "ax"

	.global _start
_start:

	// make room in the stack
	subq $8 , %rsp
	// 1st arg (MS ABI %rcx) is ImageHandle
	pushq %rcx
	// 2nd arg (MS ABI %rdx) is SystemTable
	pushq %rdx

	// address of variable UEFI_ImageHandle
	lea UEFI_ImageHandle(%rip) , %rdi
	// save value of 1st arg in this location
	mov %rcx , (%rdi)
	// address of variable ST - from GNU EFI library
	lea ST(%rip) , %rdi
	// save value of 2nd arg in this location
	mov %rdx , (%rdi)

	// initialze FPU (will be needed later)
	fninit

	// VMA=0 adjusted with %rip to real loaded address
	lea ImageBase(%rip) , %rdi
	// VMA of .dynamic section adjusted with %rip to real loaded address
	lea _DYNAMIC(%rip) , %rsi

	// call with 3 args (x86_64 ABI %rdi, %rsi, %rdx)
	call _relocate@PLT

	// call with no args
	call _call_init@PLT

	// get SystemTable (pushed to stack by code above)
	popq %rsi
	// get ImageHandle (pushed to stack by code above)
	popq %rdi
	// call with 2 args (x86_64 ABI %rdi, %rsi)
	call efi_main@PLT

	// use return value for first arg (exit status)
	mov %rax , %rdi
	// call exit() function with 1 arg (x86_64 ABI %rdi)
	// instead of returning
	call exit@PLT

	// if we ever return from exit()
	addq $8 , %rsp
  	ret


//
// Une entrée .reloc bidon pour faire plaisir au loader EFI
//
 	.section .reloc, "al"

	.long 0/*VirtualAddress*/
 	.long 10/*SymbolTableIndex*/
	.word ( 0/*do nothing*/ << 12 ) + 0/*fixup*/


//
// Les variables UEFI_ImageHandle et ST
//
 	.bss

	.global UEFI_ImageHandle
	.comm UEFI_ImageHandle,8,8
	.global ST
	.comm ST,8,8

