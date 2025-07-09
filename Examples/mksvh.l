/*
 * Name:        mksvh.l
 * Description: Make one StoneValley header tool.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0709250909A0709251044L00128
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
 * Put mksvh.l(this file) under StoneValley/src/ and run this compound command line under GNU/Linux.
 * $ flex mksvh.l; cc lex.yy.c -o mksvh; rm lex.yy.c; ./mksvh;
 * You can see a file named sv.h at StoneValley/src/.
 * Please include sv.h whenever and wherever you want.
 *
 */
%option noyywrap

%{
#include <stdio.h> /* Using type FILE, function fopen, fputs, fprintf and fclose. */

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

FILE * fpout = NULL;

%}

%%

(" "|\t)*#include(" "|\t)+\"sv.*
. { fprintf(fpout, "%s", yytext); }

%%

/* Function name: main
 * Description:   Program entry.
 * Parameters:    N/A.
 * Return value:  0: No error.
 *                1: Can not open sv.h for writing.
 *                2: Can not open a StoneValley file for reading.
 */
int main()
{
	size_t i;
	FILE * fpin = NULL;
	
	/* Open sv.h for writing. */
	fpout = fopen(SZ_HEADER, "w");
	
	if (NULL != fpout)
	{
		/* Add pre-compiling header. */
		fputs("#ifndef _SV_H_\r\n", fpout);
		fputs("#define _SV_H_\r\n", fpout);
		
		for (i = 0; i < sizeof(SZFILE) / sizeof(SZFILE[0]); ++i)
		{
			fpin = fopen(SZFILE[i], "r");
			
			if (NULL != fpin)
			{
				yyrestart(fpin);
				yylex();
				fclose(fpin);
				fpin = NULL;
			}
			else
			{
				printf("File operation failure at file \"%s\"!\n", SZFILE[i]);
				if (fpout)
					fclose(fpout);
				return 2;
			}
		}
		
		/* Terminate pre-compiling header. */
		fputs("#endif\r\n\r\n", fpout);
		
		fclose(fpout);
		fpout = NULL;
	}
	else
		return 1;

	return 0;
}

