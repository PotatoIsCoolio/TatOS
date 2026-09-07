/*
################################
#           [TatOS]            #
#         File: File.h         #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once

#include <stddef.h>
#include <stdint.h>

// POSIX USTAR 512-byte header structure matching the tar archive format
struct TarHeader // https://wiki.osdev.org/USTAR 
{  
    char name[100];     // File name
    char mode[8];       // File mode / permissions (Octal ASCII)
    char uid[8];        // User ID (Octal ASCII)
    char gid[8];        // Group ID (Octal ASCII)
    char size[12];      // File size in bytes (Octal ASCII)
    char mtime[12];     // Modification time (Octal ASCII)
    char chksum[8];     // Header checksum (Octal ASCII)
    char typeflag;      // File type ('0' = Normal file, '5' = Directory)
    char linkname[100]; // Name of linked file
    char magic[6];      // "ustar" magic string
    char version[2];    // USTAR version
    char uname[32];     // User name
    char gname[32];     // Group name
    char devmajor[8];   // Device major number
    char devminor[8];   // Device minor number
    char prefix[155];   // Prefix for long path names
    char pad[12];       // Padding to reach 512 bytes
} __attribute__((packed));

// Abstract handle used by TatOS to reference a loaded file in memory
struct File {
    const char* name;    // File path/name
    const uint8_t* data; // Pointer to raw file bytes in memory
    size_t size;         // File size in bytes
    bool is_directory;   // True if this entry is a folder
};