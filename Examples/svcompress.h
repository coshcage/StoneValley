/*
 * Name:        svcompress.h
 * Description: Compress files.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0120211637A0822261342L00068
 * License:     LGPLv3
 * Copyright (C) 2025-2026 John Cage
 *
 * This file is part of StoneValley.
 *
 * StoneValley is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * StoneValley is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with StoneValley.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef _SVCOMPRESS_H_
#define _SVCOMPRESS_H_

#include <stdio.h>

/* SV compressing error enumeration. */
typedef enum en_SVCERROR {
	SVC_NONE = 0,   /* No error. */
	SVC_FILE_OPEN,  /* File pointer is NULL. */
	SVC_FILE_IO,    /* File IO error. */
	SVC_ALLOCATION, /* Allocation failure. */
	SVC_COMPRESS,   /* Compressing error. */
	SVC_FILE_TYPE,  /* Data file error. */
	SVC_PLATFORM,   /* Platform integer length mismatch. */
	SVC_DECOMPRESS  /* Decompressing error. */
} SVCERROR;

/* Function declarations for interface. */
SVCERROR svcCompressFile  (FILE * fpout, FILE * fpin);
SVCERROR svcDecompressFile(FILE * fpout, FILE * fpin);

#endif

/* A file listed below to give users an example to launch the svcompress module.
// Name:        svcf.c
// Description: Launcher of compress/decompress files for the svcompress module.
// Usage:       $ cc *.c -o svcf; cat a.txt | ./svcf -c > b.bin; cat b.bin | ./svcf -d > c.txt; diff a.txt c.txt;
#include <stdio.h>
#include <string.h>
#include "svcompress.h"

int main(int argc, char ** argv)
{
	if (2 == argc)
	{
		if (0 == strcmp(argv[1], "-c") || 0 == strcmp(argv[1], "--compress"))
			return svcCompressFile(stdout, stdin);
		if (0 == strcmp(argv[1], "-d") || 0 == strcmp(argv[1], "--decompress"))
			return svcDecompressFile(stdout, stdin);
		if (0 == strcmp(argv[1], "-?") || 0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help"))
			printf("Usage:\nsvcf [-c|--compress] Compress stdin.\nsvcf [-d|--decompress] Decompress stdin as a svcf file.\nsvcf [-?|-h|--help] Show this help content.\n");
	}
	return 0;
}
*/

