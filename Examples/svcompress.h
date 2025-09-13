/*
 * Name:        svcompress.h
 * Description: Compress files.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0120211637A0513250000L00044
 * License:     LGPLv3
 * Copyright (C) 2025 John Cage
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
	SVC_NONE,       /* No error. */
	SVC_FILE_OPEN,  /* File pointer is NULL. */
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

