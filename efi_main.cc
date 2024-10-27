/*
 * This file is part of UEFI GPT fdisk.
 *
 * UEFI GPT fdisk is a port of GPT fdisk to UEFI/BIOS.
 * UEFI GPT fdisk est un portage de GPT fdisk vers UEFI/BIOS.
 * Ce fichier a été initié par Bernard Burette en février 2014.
 *
 * Original work is copyleft Bernard Burette.
 * Modifications are copyleft Joseph Zeller.
 *
 * This program is distributed under the terms of the GNU GPL version 2.
 * La diffusion de ce code est faite selon les termes de la GPL GNU version 2.
 */

/**
 *
 * Ce module contient la fonction efi_main() appellée depuis
 * _start du module start-$(ARCH). Cette fonction est en charge
 * d'énumérer les périphériques bloc disponibles, de faire
 * choisir l'utilisateur et d'appeller la fonction main() avec
 * le chemin vers ce disque comme argument.
 *
 */


/* en premier */
#include "uefi.h"
#include "debug.h"
#include <efi.h>
#include <efilib.h>
#include <efishell.h>
#include <efiprot.h>
#include "../support.h"

extern "C" {
#include "libmy/converter.h"
}

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

using namespace std ;


/**
 * La table de tous les périphériques disque.
 */
EFI_HANDLE * HandlePointer ;


/**
 * Le nombre d'entrées dans HandlePointer.
 */
UINTN HandleCount ;


/**
 * table: Table of fixed disks when no argument is present.
 * Device: Handle of the block device passed by UEFI Shell.
 */
static EFI_HANDLE * table ;
static EFI_HANDLE * Device;


/**
 * Le nombre d'entrées utiles dans "table".
 */
static int max_table ;


/**
 * Fait le ménage en fin d'exécution.
 */
static void fini( void )
{
	UINTN i ;
	EFI_STATUS status ;
	EFI_BLOCK_IO * block_io ;
	// force la relecture des tables de partitions
	for ( i = 0 ; i < HandleCount ; i ++ ) {
		status = UEFI_call( ST-> BootServices-> HandleProtocol ,
			HandlePointer[ i ] , & BlockIoProtocol , & block_io ) ;
		if ( EFI_ERROR( status ) ) continue ;
		UEFI_call( ST-> BootServices-> ReinstallProtocolInterface ,
			HandlePointer[ i ] , & BlockIoProtocol ,
			block_io , block_io ) ;
	}
	FreePool( HandlePointer ) ;
	free( table ) ;
}


/**
 * Détermine la liste des périphériques partitionnables.
 * @return EFI_SUCCESS si tout est bon, sinon une erreur.
 * Cette fonction est reprise du programme "diskpart" de
 * Intel.
 */
static EFI_STATUS FindPartitionableDevices( void )
{
    EFI_STATUS      status ;
    EFI_BLOCK_IO    *BlkIo;
    EFI_DEVICE_PATH *DevicePath;
    UINTN           PathSize;
    BOOLEAN         Partitionable;
    EFI_DEVICE_PATH *PathInstance;
    UINTN           i;

    /*
     Try to find all of the hard disks by finding all
     handles that support BlockIo protocol
    */
    status = LibLocateHandle(
        ByProtocol,
        &BlockIoProtocol,
        NULL,
        &HandleCount,
        &HandlePointer
        );

    if (EFI_ERROR(status)) {
        return status;
    }

    table = (EFI_HANDLE*) malloc( sizeof( EFI_HANDLE ) * HandleCount ) ;

    if (table == NULL) {
	FreePool( HandlePointer ) ;
        return EFI_OUT_OF_RESOURCES;
    }

    atexit( fini ) ;

    for (i = 0; i < HandleCount; i++) {
        Partitionable = TRUE;
        status = UEFI_call( ST-> BootServices-> HandleProtocol ,
		HandlePointer[ i ] , &BlockIoProtocol, &BlkIo);
        /* According to Windows GPT FAQ at
	   www.microsoft.com/hwdev/tech/storage/GPT_FAQ.ASP, Microsoft tools
	   will never partition removable media, whether it be floppy or
	   superfloppy.  */
        if (BlkIo->Media->RemovableMedia) {
            /* It's removable, it's not for us */
            Partitionable = FALSE;
        }
        if ( ! BlkIo->Media->MediaPresent) {
            /* It's still not for us */
            Partitionable = FALSE;
        }
        if (BlkIo->Media->ReadOnly) {
            /* Cannot partition a read-only device!  */
            Partitionable = FALSE;
        }
	/* OK, it seems to be a present, fixed, read/write, block device.
	   Now, make sure it's really the raw device by inspecting the
	   device path.  */
        DevicePath = DevicePathFromHandle(HandlePointer[i]);
        while (DevicePath != NULL) {
            PathInstance = DevicePathInstance(&DevicePath, &PathSize);

            while (!IsDevicePathEnd(PathInstance)) {
                if ((DevicePathType(PathInstance) == MEDIA_DEVICE_PATH)) {
                    Partitionable = FALSE;
                }

                PathInstance = NextDevicePathNode(PathInstance);
            }
        }

        if (Partitionable) {
		// l'affiche
		CHAR16 * str , * p ;
		EFI_BLOCK_IO * BlkIo ;
        	DevicePath = DevicePathFromHandle( HandlePointer[ i ] ) ;
		str = DevicePathToStr( DevicePath ) ;
		status = UEFI_call( ST-> BootServices-> HandleProtocol ,
			HandlePointer[ i ] , & BlockIoProtocol , & BlkIo ) ;
		cout << "  " << max_table + 1 << ": Disk " <<
			UEFI_disk_name( HandlePointer[ i ] ) << ": " <<
			BlkIo-> Media-> LastBlock + 1 << " sectors, " <<
			BytesToIeee( BlkIo-> Media-> LastBlock + 1 ,
				BlkIo-> Media-> BlockSize ) << "\n    " ;
		// pour convertir chaque CHAR16 en char
		p = str ;
		while ( * p ) {
			char c = * p ;
			cout << c ;
			p ++ ;
		}
		FreePool( str ) ;
		cout << '\n' ;
		// return this handle
		table[ max_table ] = HandlePointer[ i ] ;
		max_table ++ ;
	}
    }
    return EFI_SUCCESS;
}

/**
 * Read in arguments from the UEFI Shell command line.
 */
static EFI_STATUS
get_args(EFI_HANDLE image, UINTN *argc, CHAR16 ***argv)
{
    EFI_STATUS status;
    EFI_SHELL_PARAMETERS_PROTOCOL *ShellParameters;
    EFI_GUID ShellParametersProtocolGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;

    status = UEFI_call(ST->BootServices->OpenProtocol,
    	               image, &ShellParametersProtocolGuid,
                       (VOID **)&ShellParameters, image, NULL,
                       EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status)) {
    	return status;
    }

    *argc = ShellParameters->Argc;
    *argv = ShellParameters->Argv;

    status = UEFI_call(ST->BootServices->CloseProtocol, image,
                       &ShellParametersProtocolGuid, image, NULL);
    return status; 
}


/**
 * Get the handle of the block device passed through the command line.
 * Display an error message and return if unable to get device path or
 * handle or device seems to be a partition or DVD rather than a disk.
 */
static EFI_STATUS
get_device_handle(CONST CHAR16 *Mapping)
{
	EFI_STATUS status;
	EFI_BLOCK_IO *BlkIo;
	EFI_SHELL_PROTOCOL *Shell; 
	CONST EFI_DEVICE_PATH_PROTOCOL *EfiDevPath;
	EFI_GUID ShellProtocolGuid = EFI_SHELL_PROTOCOL_GUID;
	EFI_GUID BlockIoProtocolGuid = EFI_BLOCK_IO_PROTOCOL_GUID;
        
        status = UEFI_call(ST->BootServices->LocateProtocol,
                           &ShellProtocolGuid, NULL, (VOID **)&Shell);
        
        if (EFI_ERROR(status)) {
        	Print((CHAR16*)u"Unable to access shell protocol data. \n");
        	Print((CHAR16*)u"Run this program without arguments. \n");
        	return status;
        }
        
        EfiDevPath = (EFI_DEVICE_PATH_PROTOCOL *) UEFI_call(
                      Shell->GetDevicePathFromMap, Mapping);
        
        if (!EfiDevPath) {
        	Print((CHAR16*)u"Unable to get device path for %s. \n", Mapping);
        	status = EFI_ABORTED;
        	return status;
        }
        
        status = UEFI_call(ST->BootServices->LocateDevicePath,
                           &BlockIoProtocolGuid, &EfiDevPath, &Device);
        
        if (EFI_ERROR(status)) {
        	Print((CHAR16*)u"Unable to get device handle for %s. \n", Mapping);
        	return status;
        }
        
        status = UEFI_call(ST->BootServices->HandleProtocol,
                           Device, &BlockIoProtocolGuid, &BlkIo);
        
        if (EFI_ERROR(status)) {
        	Print((CHAR16*)u"Unable to get device data for %s. \n", Mapping);
        	return status;
        }
        
        if (BlkIo->Media->LogicalPartition) {
        	Print((CHAR16*)u"Device %s appears to be a partition. \n", Mapping);
        	status = EFI_ABORTED;
        	return status;
        }
        
        if (!BlkIo->Media->MediaPresent) {
        	Print((CHAR16*)u"Device %s does not have media present. \n", Mapping);
        	status = EFI_ABORTED;
        	return status;
        }
        
        if (BlkIo->Media->ReadOnly) {
        	Print((CHAR16*)u"Device %s can not be partitioned: Read Only. \n", Mapping);
        	status = EFI_ABORTED;
        	return status;
        }

	return status;
}

/**
 * Shift all characters to lower case then
 * get program name from end of file path.
 */
static CHAR16 *progname (CHAR16 *opt)
{
	CHAR16 *ptr;
	static BOOLEAN islower;
	
	if (!islower) {
		for (UINTN i = 0; opt[i]; i++)
			StrLwr(&opt[i]);
		islower = TRUE;
	}
	
	for (ptr = NULL;; opt++) {
		if (*opt == '\\'){
			opt++;
			ptr = (CHAR16 *)opt;
		}
		if (*opt == '\0')
			return (ptr);
	}
}

/**
 *  Format program name to UTF-8 then copy to proginvoshrtname.
 */
void set_shortname(CHAR16 *optname, char *pshort)
{
	size_t length = (size_t)StrLen(optname);
	size_t buffsz = utf16_to_utf8((utf16_t*)optname, length, NULL, 0);
	utf8_t* buffer = (utf8_t*) malloc(buffsz + 1);
	if (!buffer) {
		Print((CHAR16*)u"malloc: Failed to allocate buffer for name argument. \n");
		FreePool(buffer);
		exit(5);
    	}
    	ZeroMem(buffer, buffsz + 1);
	utf16_to_utf8((utf16_t*)optname, length, buffer, buffsz);
	buffer[buffsz + 1] = '\0';
	strcpy(pshort, (const char*)buffer);
	FreePool(buffer);
}

/**
 * Le nom de ce programme.
 */
static char proginvoshrtname[25];
char *shortname = (char*) proginvoshrtname;

/**
 * Les arguments passés à la fonction main().
 */
static const char* args[32];
static const char* envp[] = { NULL } ;


/**
 * La définition de la fonction main().
 */
extern "C" {
extern int main( int , const char ** , const char ** ) ;
}


/**
 * La fonction principale appelée depuis _start du module start-$(ARCH).
 * @param ImageHandle Le EFI_HANDLE de ce programme.
 * @param SystemTable La table des HANDLE et fonction UEFI.
 * @return Un EFI_STATUS de fin d'exécution.
 *
 * Cette fonction est le point d'entrée dans le programme
 * GPT fdisk (voir le module start-$(ARCH)). Elle fait des trucs
 * EFI pour récupérer la liste des disques puis appelle la
 * fonction main() du GPT fdisk classique.
 */
extern "C" {
EFI_STATUS
efi_main( EFI_HANDLE ImageHandle , EFI_SYSTEM_TABLE * SystemTable ) ;
}
EFI_STATUS
efi_main( EFI_HANDLE ImageHandle , EFI_SYSTEM_TABLE * SystemTable )
{
	EFI_STATUS status ;
	EFI_HANDLE Disk ;
	UINTN argc = 0;
	CHAR16 **argv;
	
	/* initialise la librairie GNU */
	InitializeLib( ImageHandle , SystemTable ) ;
	
	/* Get arguments from UEFI Shell */
	status = get_args(ImageHandle, &argc, &argv);
	
	/* Set the program name depending on the presence of auguments */
	if (status == EFI_SUCCESS) {
		set_shortname(progname(argv[0]), proginvoshrtname);
	} else {
		strcpy(proginvoshrtname, "gdisk.efi");
	}

#ifdef VBOXERR	
	/* Send errors/warnings to stdout under VirtualBox */
	cerr.rdbuf(cout.rdbuf());
#endif
	
	cout << "UEFI GPT fdisk (" << shortname << ") version " GPTFDISK_VERSION "\n\n";

	if (argc < 2) {
		cout << "List of hard disks found:\n" ;
		FindPartitionableDevices() ;
		cout << '\n' ;

		/* si un seul disque, le sélectionne, sinon... */
		if ( max_table == 1 ) {
			Disk = table[ 0 ] ;
			cout << "Using 1\n" ;
		} else {
			for(;;) {
				cout << "Disk number (1-" << max_table << "): " ;
				int n = 0 ;
				cin >> n ;
				if ( ! cin.good() ) {
					/* fin de fichier ? */
					exit( 5 ) ;
				}
				if ( n >= 1 && n <= max_table ) {
					Disk = table[ n - 1 ] ;
					break ;
				}
			}
		}
		cout << '\n' ;
		argc = 2;
		args[0] = proginvoshrtname;
		args[1] = UEFI_disk_name(Disk);
	} else {
		/* Read in arguments from UEFI Shell and format to UTF-8 */
		for (UINTN i = 0; i < argc; i++) {
			size_t len = (size_t)StrLen(argv[i]);
			size_t out_len = utf16_to_utf8((utf16_t*)argv[i], len, NULL, 0);
			utf8_t* out_buf = (utf8_t*) malloc(out_len + 1);
			if (!out_buf) {
				Print((CHAR16*)u"malloc: Failed to allocate buffer for arguments. \n");
				FreePool(out_buf);
				exit(5);
    			}
    			ZeroMem(out_buf, out_len + 1);
			utf16_to_utf8((utf16_t*)argv[i], len, out_buf, out_len);
			out_buf[out_len + 1] = '\0';
			if (!CompareMem(out_buf, "-", 1) ||
			    !CompareMem(out_buf, "--", 2)) {
			        args[i] = (const char*)out_buf;
			} else if (i == 0) {
				args[i] = proginvoshrtname;
			} else {
				status = get_device_handle(argv[i]);
				if (EFI_ERROR(status)) {
					FreePool(out_buf);
					exit(5);
				}
				args[i] = UEFI_disk_name(Device);
			}
			FreePool(out_buf);
		}
	}

	status = main( (int)argc , args , envp ) ;

	if ( status != EFI_SUCCESS ) status = EFI_ABORTED ;
	return status ;
}

