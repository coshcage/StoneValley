/*
 * Name:        c89bool.l
 * Description: C89 standard boolean type compliance tool.
 * Author:      cosh.cage#hotmail.com
 * File ID:     1126260909A1127261844L00179
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
 * This lex program is used to convert StoneValley library relevant files to comply with C89 standard boolean type.
 * Please put this file into StoneValley/src/ folder and use flex to compile it first.
 * Then use a C compiler to compile lex.yy.c which should appear after using flex.
 * Use the final executable file to convert C source files and header files that involve boolean type.
 * Here is a compound command line that can produce an executable file on Unix|GNU/Linux:
 * $ flex c89bool.l; cc lex.yy.c -o c89bool; rm lex.yy.c; ./c89bool *.c *.h;
 * Notice that flex is required on your operating system before you use the above command line.
 *
 */
%option noyywrap
%x SVDEFH

SPACE  [ |\t]
SYMBOL [\!-\?\[-\^\{-\~]

%{
#include <stdio.h>
#include <string.h>

#define COMPATIBLE_BOOL "BOOL"

FILE * fptmp = NULL;

char * ConvertStringBoolean(char * str);
%}

%%

<SVDEFH>^#{SPACE}*"include"{SPACE}+"<stdbool.h>".* {
	fprintf
	(
		fptmp,
"/* Boolean type and constants are used to replace stdbool including for C89 compliance. */\ntypedef unsigned int %s;\n#define true  1\n#define false 0",
		COMPATIBLE_BOOL
	);
}
^"bool"({SPACE}|{SYMBOL})+ { fprintf(fptmp, "%s", ConvertStringBoolean(yytext)); }
({SPACE}|{SYMBOL})+"bool"$ { fprintf(fptmp, "%s", ConvertStringBoolean(yytext)); }
({SPACE}|{SYMBOL})+"bool"({SPACE}|{SYMBOL})+ { fprintf(fptmp, "%s", ConvertStringBoolean(yytext)); }
" "|\t|\n { fprintf(fptmp, "%s", yytext); }
. { fprintf(fptmp, "%s", yytext); }

%%

#define FILE_FOUND(strfnam) (! strcmp(strlen(argv[i]) > strlen(strfnam) ? argv[i] + strlen(argv[i]) - strlen(strfnam) : argv[i], (strfnam)))

char strname[L_tmpnam];
char strbuff[BUFSIZ];

char * ConvertStringBoolean(char * str)
{
	char * p = str;
	
	p = strstr(str, "bool");
	
	if (NULL != p)
		memcpy(p, COMPATIBLE_BOOL, 4);
	
	return str;
}

int main(int argc, char * argv[])
{
	FILE * fp;
	register int c, i;
	register size_t j;
	
	tmpnam(strname);

	for (i = 1; i < argc; ++i)
	{
		if (FILE_FOUND("lex.yy.c"))
			continue;
		
		fptmp = NULL == fptmp ? fopen(strname, "w") : freopen(strname, "w", fptmp);
		
		if (NULL == fptmp)
		{
			printf("Can not open temporary file.\n");
			break;
		}
		
		yyout = fptmp;
		
		fp = fopen(argv[i], "r"); /* Convert and copy library file to temporary file. */
		
		if (NULL != fp)
		{
			BEGIN INITIAL;
			if (FILE_FOUND("svdef.h"))
				BEGIN SVDEFH;
			yyrestart(fp);
			yylex();
		}
		else
		{
			printf("Can not open file '%s' to read.\n", argv[i]);
			break;
		}
		
		fptmp = freopen(strname, "r", fptmp); /* Copy temporary file back to corresponding library file. */
		
		if (NULL != fptmp)
		{
			fclose(fp);

			fp = fopen(argv[i], "w");
			
			printf("Copying %s.\n", argv[i]);

			if (NULL != fp)
			{
				j = 0;
				while (! feof(fptmp))
				{
					c = fgetc(fptmp);
					
					if (EOF != c)
						strbuff[j++] = (char)c;
					
					if (j >= BUFSIZ)
					{
						fwrite(strbuff, j, 1, fp);
						j = 0;
					}
				}
				
				fwrite(strbuff, j, 1, fp);
				
				fclose(fp);
			}
			else
			{
				printf("Can not open file '%s' to write.\n", argv[i]);
				break;
			}
		}
		else
		{
			printf("Can not reopen temporary file.\n");
			break;
		}
	}
	
	if (i < 2)
		printf("This program requires at least one parameter to run.\nUsage: %s file.c file.h...\n", argv[0]);
	else if (argc == i)
		printf("Done!\n");
	
	if (NULL != fptmp)
	{
		fclose(fptmp);
		remove(strname);
	}
	
	return 0;
}

