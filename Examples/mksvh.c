/*
 * Name:        mksvh.c
 * Description: Make one StoneValley header tool.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0709250909A0709250933L00168
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
 * Tips of usage:
 * Put mksvh.c(this file) under StoneValley/src/ and run this compound command line under GNU/Linux.
 * $ cc mksvh.c -o mksvh; ./mksvh;
 * You can see a file named sv.h at StoneValley/src/.
 * Please include sv.h whenever and wherever you want.
 *
 */
#include <stdio.h>  /* Using type FILE, function fopen, fputs, feof, fgetc, fputc and fclose. */
#include <stdlib.h> /* Using function exit. */

/* Make type boolean. */
typedef unsigned bool;

#ifndef true
#define true  1
#endif

#ifndef false
#define false 0
#endif

/* One header name. */
#define SZ_HEADER "sv.h"

/* StoneValley files array. */
const char * SZFILE[] = 
{
	"svdef.h",
	"svstring.h",
	"svatom.c",
	"svarray.c",
	"svlist.c",
	"svmatrix.c",
	"svmisc.c",
	"svstack.h",
	"svstack.c",
	"svqueue.h",
	"svqueue.c",
	"svtree.h",
	"svbytree.c",
	"svhtree.c",
	"svctree.c",
	"svgtree.c",
	"svstree.c",
	"svhash.h",
	"svhash.c",
	"svset.h",
	"svset.c",
	"svgraph.h",
	"svgraph.c"
};

/* Function name: GetFile
 * Description:   Fetch file pointer.
 * Parameter:
 *    szfile File name string.
 * Return value:  File pointer.
 */
FILE * GetFile(const char * szfile)
{
	FILE * fp;
	
	fp = fopen(szfile, "r");
	
	return fp;
}

/* Function name: PutFile
 * Description:   Copy a file into another file.
 * Parameters:
 *       fpin Pointer to the destined file.
 *      fpout Pointer to the file to be copied.
 * Return value:  true  Coping succeeded.
 *                false Coping failed.
 */
bool PutFile(FILE * fpin, FILE * fpout)
{
	if (NULL == fpin || NULL == fpout)
		return false;
	
	while (!feof(fpout))
	{
		register int c;
		c = fgetc(fpout);
		if (EOF != c)       /* Filter out EOF. */
			fputc(c, fpin); /* Move a char in the file. */
	}
	
	fclose(fpout);
	
	return true;
}

/* Function name: PAssert
 * Description:   Phony assert function.
 * Parameters:
 *        exp An expression that returns boolean.
 *         sz File name to locate error.
 * Return value:  N/A.
 */
void PAssert(bool exp, const char * sz)
{
	if (!exp)
	{
		printf("File operation failure at file \"%s\"!\n", sz);
		exit(2);
	}
}

/* Function name: main
 * Description:   Program entry.
 * Parameters:    N/A.
 * Return value:  0: No error.
 *                1: Can not open sv.h for writing.
 *                2: Can not open a StoneValley file for reading.
 */
int main()
{
	FILE * fpsvh; /* Open sv.h for writing. */
	
	fpsvh = fopen(SZ_HEADER, "w");
	
	if (NULL != fpsvh)
	{
		size_t i;
		
		/* Add pre-compiling header. */
		fputs("#ifndef _SV_H_\r\n", fpsvh);
		fputs("#define _SV_H_\r\n", fpsvh);
		
		/* Copy files. */
		for (i = 0; i < sizeof(SZFILE) / sizeof(SZFILE[0]); ++i)
		{
			PAssert(PutFile(fpsvh, GetFile(SZFILE[i])), SZFILE[i]);
		}
		
		/* Terminate pre-compiling header. */
		fputs("#endif\r\n\r\n", fpsvh);
		
		fclose(fpsvh);
	}
	else
		return 1;
	
	return 0;
}

