/*
 * name_conv.l
 * This lex program is used to convert function names to Linux style.
 * Please put this file into StoneValley/src/ folder and use flex to compile it first.
 * Then use a C compiler to compile lex.yy.c which should appear after using flex.
 * Use the final executable file to convert C source files and header files.
 * Here is a compound command line that can produce an executable file on Unix|GNU/Linux:
 *     flex name_conv.l; cc lex.yy.c; ./a.out *.c *.h; rm lex.yy.c; rm ./a.out;
 * Notice that flex is required on your platform before you use the above command line.
 * Names of generated files are each original file name that headed with a '~' symbol.
 * Then original files will be renamed as '#'s add their file names.
 *
 * Created by cosh.cage#hotmail.com on 10/03/18.
 * License:  LGPLv3
 * Platform: Unix|GNU/Linux
 * Copyright (C) 2018-2024 John Cage
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
%option noyywrap

%{
#include <stdio.h>
#include <string.h>
FILE * pfout = NULL;
void EscapeFromHungary(char * pstr);
%}

%%

" "|\t|\n { fprintf(pfout, "%s", yytext); }
_?("sv"|"grp"|"tre"|"stk"|"str"|"set"|"hsh"|"que")+[a-zA-Z0-9_]* { EscapeFromHungary(yytext); }
. { fprintf(pfout, "%s", yytext); }

%%

void EscapeFromHungary(char * pstr)
{
	if ('_' == *pstr)
		fprintf(pfout, "%s", pstr); /* Ignore internal functions. */
	else
	{
		static char buf[BUFSIZ];
		char * pbuf = buf;
		register int i;
		for (; *pstr; ++pstr, ++pbuf)
		{
			if (*pstr >= 'A' && *pstr <= 'Z') /* Uppercase letter. */
			{
				if (*(pstr - 1) != '_' && *(pstr - 1) > 'Z')
				{
					*pbuf = '_';
					*(++pbuf) = *pstr + 32;
				}
				else
					*pbuf = *pstr + 32;
			}
			else
				*pbuf = *pstr;
		}
		*pbuf = '\0';
		fprintf(pfout, "%s", buf);
	}
}

int main(int argc, char * argv[])
{
	pfout = stdout;
	if (argc < 2) /* Read stdin. */
		yylex();
	else
	{
		FILE * pfin;
		register int i;
		char buf[FILENAME_MAX];
		for (i = 1; strcpy(buf, "~"), i < argc; ++i)
		{
			/* Ignore lex.yy.c */
			if (0 == strcmp(argv[i], "lex.yy.c"))
				continue;
			pfin = fopen(argv[i], "r");
			pfout = fopen(strcat(buf, argv[i]), "wb");
			if (NULL == pfin || NULL == pfout)
			{
				fprintf(stderr, "Can not convert file: \'%s\'.\n", argv[i]);
				return 1;
			}
			yyrestart(pfin);
			yylex();
			fclose(pfin);
			fclose(pfout);
			pfout = NULL;
			/* Backup original files. */
			strcpy(buf, "#");
			strcat(buf, argv[i]);
			if (0 != rename(argv[i], buf))
				fprintf(stderr, "Can not backup file: \'%s\'.\n", argv[i]);
			/* Rename new files. */
			strcpy(buf, "~");
			strcat(buf, argv[i]);
			if (0 != rename(buf, argv[i]))
				fprintf(stderr, "Can not rename file: \'%s\'.\n", buf);
		}
	}
	return 0;
}
