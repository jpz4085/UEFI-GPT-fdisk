#!/bin/bash
##
## This file is part of UEFI GPT fdisk.
##
## UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
## UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
## Ce fichier a été initié par Bernard Burette en mars 2014.
##
## All this work is copyleft Bernard Burette.
## Gauche d'auteur Bernard Burette.
##
## This program is distributed under the terms of the GNU GPL version 2.
## La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
##

## Patch section for version 1.0.5 and above of GPT fdisk by Joseph Zeller.

## Class variable PartType::unusedPartType is removed and replaced with the
## associated GUID value used in previous versions of gdisk to prevent hang.

version=$( grep '^#\w*define\>' ../*.h | grep 'VERSION' | awk '{print $NF}' | tr -d '"' )
if [ "$(printf '%s\n' "1.0.5" "$version" | sort -V | head -n1)" = "1.0.5" ]; then
	if grep PartType::unusedPartType ../gptpart.cc >/dev/null 2>&1 ; then
		echo "Replace unusedPartType variable with GUID in gptpart.cc"
		sed -i 's/PartType::unusedPartType/(GUIDData) "00000000-0000-0000-0000-000000000000"/' ../gptpart.cc
	else
		echo "File ../gptpart.cc is already patched."
	fi
	if grep unusedPartType ../parttypes.cc >/dev/null 2>&1 ; then
		echo "Remove unusedPartType variable from parttypes.cc"
		sed -i '/unusedPartType/d' ../parttypes.cc
	else
		echo "File ../parttypes.cc is already patched."
	fi
	if grep unusedPartType ../parttypes.h >/dev/null 2>&1 ; then
		echo "Remove unusedPartType variable from parttypes.h"
		sed -i '/unusedPartType/d' ../parttypes.h
		sed -i '/PartType with GUID/d' ../parttypes.h
	else
		echo "File ../parttypes.h is already patched."
	fi
exit 0
fi

# Patch section for version 0.8.10 and below of GPT fdisk.

# We add three small bits of code
# 1) in gdisk.cc we don't display the introduction banner because the
#    efi_main() function has done this before asking for the disk to edit.
# 2) in gpt.cc we compute log2 with integers (because in UEFI we can't call
#    math functions returning double.
# 3) in support.cc for a similar reason we convert sizes to IEEE without
#    using %f printf format.

case "${version}" in
	1* | 0.9* | 0.8.11 | 0.8.12 )
		echo "Version " ${version} " of GPT fdisk doesn't need patching."
		exit 0
		;;
esac

if grep -w EFI ../gdisk.cc >/dev/null 2>&1 ; then
	echo "File ../gdisk.cc is already patched."
else
	# line number of first cout
	n=$( grep -wn "cout *<<" ../gdisk.cc | head -n 1 | awk -F: '{print $1}' )
	# backup original
	mv ../gdisk.cc ../gdisk.cc~
	# info
	echo "Patching ../gdisk.cc, inserting EFI define at line $n."
	# copy first part
	head -n $(( $n - 1 )) ../gdisk.cc~ >../gdisk.cc
	# add our code
	echo "#ifndef EFI" >>../gdisk.cc
	# copy line
	tail -n +$n ../gdisk.cc~ | head -n 1 >>../gdisk.cc
	# add our code
	echo "#endif /*EFI*/" >>../gdisk.cc
	# copy rest
	tail -n +$(( $n + 1 )) ../gdisk.cc~ >>../gdisk.cc
fi

if grep -w EFI ../gpt.cc >/dev/null 2>&1 ; then
	echo "File ../gpt.cc is already patched."
else
	# line number of last #define log2
	n=$( grep -wn "define *log2" ../gpt.cc | tail -n 1 | awk -F: '{print $1}' )
	# find first empty line after this one
	while [ $( tail -n +$n ../gpt.cc | head -n 1 | wc -w ) -gt 0 ] ; do
		n=$(( $n + 1 ))
	done
	# backup original
	mv ../gpt.cc ../gpt.cc~
	# info
	echo "Patching ../gpt.cc, inserting EFI code at line $n."
	# copy first part
	head -n $(( $n - 1 )) ../gpt.cc~ >../gpt.cc
	# add our code
	echo "#ifdef EFI" >>../gpt.cc
	echo "// in UEFI mode MMX registers are not yet available so using the" >>../gpt.cc
	echo "// x86_64 ABI to move \"double\" values around is not an option." >>../gpt.cc
	echo "#ifdef log2" >>../gpt.cc
	echo "#  undef log2" >>../gpt.cc
	echo "#endif" >>../gpt.cc
	echo "#define log2(x) log2_32( x )" >>../gpt.cc
	echo "static inline uint32_t log2_32( uint32_t v ) {" >>../gpt.cc
	echo "  int r = -1 ;" >>../gpt.cc
	echo "  while ( v >= 1 ) {" >>../gpt.cc
	echo "    r ++ ;" >>../gpt.cc
	echo "    v >>= 1 ;" >>../gpt.cc
	echo "  }" >>../gpt.cc
	echo "  return r ;" >>../gpt.cc
	echo "}" >>../gpt.cc
	echo "#endif" >>../gpt.cc
	# copy rest
	tail -n +$n ../gpt.cc~ >>../gpt.cc
fi

if grep -w EFI ../support.cc >/dev/null 2>&1 ; then
	echo "File ../support.cc is already patched."
else
	# line number for begin of our code in ../support.cc
	n=$( grep -wn "^string BytesToIeee" ../support.cc | awk -F: '{print $1}' )
	# advance until first "}"
	m=$( tail -n +$n ../support.cc | grep -wn '}' | head -n 1 | awk -F: '{print $1}' )
	n=$(( $n + $m ))
	# now advance until end of function
	m=$( tail -n +$n ../support.cc | grep -wn '^}' | head -n 1 | awk -F: '{print $1}' )
	# we keep the "return" line
	m=$(( $m + $n - 2 ))
	# backup
	mv ../support.cc ../support.cc~
	# info
	echo "Patching ../support.cc, inserting EFI code at line $n."
	# copy first part
	head -n $(( $n - 1 )) ../support.cc~ >../support.cc
	# insert our code
	echo "#ifdef EFI" >>../support.cc
	echo "   unsigned int v = (unsigned int) sizeInIeee ;" >>../support.cc
	echo "   theValue << v ;" >>../support.cc
	echo "   if ( prefixes[index] == ' ' ) {" >>../support.cc
	echo "      theValue << \" bytes\" ;" >>../support.cc
	echo "   } else {" >>../support.cc
	echo "      int r = (unsigned int) ( ( sizeInIeee - ( (float) v ) ) * 1000.0f ) ;" >>../support.cc
	echo "      if ( sizeInIeee < 1000.0f ) {" >>../support.cc
	echo "         // add one decimal digit" >>../support.cc
	echo "         theValue << \".\" << (unsigned int) ( r / 100 ) ;" >>../support.cc
	echo "         if ( sizeInIeee < 100.0f ) {" >>../support.cc
	echo "            // add a second decimal digit" >>../support.cc
	echo "            theValue << (unsigned int) ( ( r / 10 ) % 10 ) ;" >>../support.cc
	echo "            if ( sizeInIeee < 10.0f ) {" >>../support.cc
	echo "               // add a third decimal digit" >>../support.cc
	echo "               theValue << (unsigned int) ( r % 10 ) ;" >>../support.cc
	echo "            }" >>../support.cc
	echo "         }" >>../support.cc
	echo "      }" >>../support.cc
	echo "      units = \"  iB\" ;" >>../support.cc
	echo "      units[ 1 ] = prefixes[ index ] ;" >>../support.cc
	echo "      theValue << units ;" >>../support.cc
	echo "   }" >>../support.cc
	echo "#else" >>../support.cc
	# copy middle part
	tail -n +$n ../support.cc~ | head -n $(( $m - $n )) >>../support.cc
	# insert code
	echo "#endif" >>../support.cc
	# copy rest
	tail -n +$m ../support.cc~ >>../support.cc
fi

