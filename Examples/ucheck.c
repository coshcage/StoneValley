//
//  ucheck.c
//  This file is used to do before programming checklist for the library.
//  Created by cosh.cage#hotmail.com on 09/22/26.
//  License:  LGPLv3
//  Platform: Cross Platform.
//  Copyright (C) 2017-2026 John Cage
//
// This file is part of StoneValley.
//
// StoneValley is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// StoneValley is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with StoneValley.
// If not, see <https://www.gnu.org/licenses/>.
//
// Usage:
// Compile and use this file after you have just received StoneValley source before you start programming with StoneValley to justify the configuration of the library.
// It cannot guarantee the authenticity of StoneValley source code that you have received.
// Take care and good luck!
//
// Compilation guide:
// Put this file along with StoneValley source under 'StoneValley/src/' directory and run this following compound command line under Unix|GNU/Linux shell:
// $ cc *.c -o ucheck; ./ucheck; echo $?;
//
// Program return specification:
// 0: No error.
// 1: Library integrity check failed.
// 2: SV_OPTIMIZATION failed to configure.
// 3: SET_TREE_USING  failed to configure.
//
// Warning: COMPILE ucheck.c whenever you want to start to program StoneValley.
//          And then check the output of this program to test configurations.
//          Do NOT miss a compilation before you run this ucheck.
//          Cross compilers produce platform specific checks only after you compile and RUN ucheck.
// Briefing: Compile and run make you strong.
//
#include <stdio.h>
#include "svstring.h"
#include "svset.h"

#define FILE_NUM 23

const char * SZ_SV_FILE_ALL[FILE_NUM] =
{
	"svarray.c",
	"svatom.c",
	"svbytree.c",
	"svctree.c",
	"svdef.h",
	"svgraph.c",
	"svgraph.h",
	"svgtree.c",
	"svhash.c",
	"svhash.h",
	"svhtree.c",
	"svlist.c",
	"svmatrix.c",
	"svmisc.c",
	"svqueue.c",
	"svqueue.h",
	"svset.c",
	"svset.h",
	"svstack.c",
	"svstack.h",
	"svstree.c",
	"svstring.h",
	"svtree.h"
};

#define SZ_ALERT_SIZEOF_PLATFORM_INTEGER "\e[46m[ALERT]\e[m   Platform integer size                      = %zu\n"
#define SZ_ALERT_SIZEOF_UCHART           "\e[46m[ALERT]\e[m   Unsigned char size                         = %zu\n"
#define SZ_ALERT_SIZEOF_PUCHAR           "\e[46m[ALERT]\e[m   Unsigned char pointer size                 = %zu\n"
#define SZ_ALERT_SIZEOF_PFUNCTION        "\e[46m[ALERT]\e[m   Function pointer size                      = %zu\n"

#define SZ_WARNING_YES "\e[92mYES\e[m"
#define SZ_WARNING_NO  "\e[31mNO\e[m"

#define SZ_WARNING_CHAR_LENGTH           "\e[43m[WARNING]\e[m Char is one byte:                            %s\n"
#define SZ_WARNING_CHAR_BIT              "\e[43m[WARNING]\e[m Char bit is 8:                               %s\n"
#define SZ_WARNING_POINTER_LENGTH        "\e[43m[WARNING]\e[m Function pointer is as same as char pointer: %s\n"

#define SZ_CONFIG_ON  "\e[92mON\e[m"
#define SZ_CONFIG_OFF "\e[31mOFF\e[m"

#define SZ_CONFIG_DISUSE                 "\e[44m[CONFIG]\e[m  Macro DISUSE:                                %s\n"
#define SZ_CONFIG_REGISTER               "\e[44m[CONFIG]\e[m  Macro REGISTER:                              %s\n"
#define SZ_CONFIG_SV_OPTIMIZATION_STAT   "\e[44m[CONFIG]\e[m  Library optimization:                        %s\n"
#define SZ_CONFIG_SET_TREE_USING_STAT    "\e[44m[CONFIG]\e[m  Binary search tree for set:                  %s\n"
#define SZ_CONFIG_ERROR                  "\e[44m[CONFIG]\e[m  \e[31mError configuration:\e[m                         %s\n"

int main(int argc)
{
	FILE * fp;
	bool bff = false;
	REGISTER int i = 0;
	
	svPrintVersion();
	
	printf("\n");
	
	for (i = 0; i < FILE_NUM; ++i)
	{
		if (NULL != (fp = fopen(SZ_SV_FILE_ALL[i], "r")))
			fclose(fp);
		else
		{
			if (!bff)
				printf("Library integrity check:\n");
			bff = true;
			printf("File \"%s\" Does \e[31mNot Exists.\e[m\n", SZ_SV_FILE_ALL[i]);
		}
	}
	
	if (bff)
	{
		printf("Library integrity check \e[31mfailed.\e[m\n");
		return 1;
	}
	else
		printf("Library integrity check \e[92mpassed!\e[m\n");
	
	printf("\n");
	
	printf(SZ_ALERT_SIZEOF_PLATFORM_INTEGER, sizeof(size_t));
	printf(SZ_ALERT_SIZEOF_UCHART,           sizeof(UCHART));
	printf(SZ_ALERT_SIZEOF_PUCHAR,           sizeof(PUCHAR));
	printf(SZ_ALERT_SIZEOF_PFUNCTION,        sizeof(CBF_TRAVERSE));
	
	printf("\n");
	
	printf(SZ_WARNING_CHAR_LENGTH,    sizeof(UCHART) == 1                    ? SZ_WARNING_YES : SZ_WARNING_NO);
	printf(SZ_WARNING_CHAR_BIT,       CHAR_BIT == 8                          ? SZ_WARNING_YES : SZ_WARNING_NO);
	printf(SZ_WARNING_POINTER_LENGTH, sizeof(PUCHAR) == sizeof(CBF_TRAVERSE) ? SZ_WARNING_YES : SZ_WARNING_NO);
	
	printf("\n");
	
#ifndef DISUSE
	if (DISUSE(i) = 1)
		printf(SZ_CONFIG_DISUSE, SZ_CONFIG_OFF);
	else
		printf(SZ_CONFIG_DISUSE, SZ_CONFIG_ON);
#endif

#ifndef REGISTER
	if (*(int *)&i = 1)
		printf(SZ_CONFIG_REGISTER, SZ_CONFIG_OFF);
	else
		printf(SZ_CONFIG_REGISTER, SZ_CONFIG_ON);
#endif

	printf("\n");

	switch (SV_OPTIMIZATION)
	{
	case SV_OPT_MINISIZE:
		printf(SZ_CONFIG_SV_OPTIMIZATION_STAT, "SV_OPT_MINISIZE");
		break;
	case SV_OPT_MAXSPEED:
		printf(SZ_CONFIG_SV_OPTIMIZATION_STAT, "SV_OPT_MAXSPEED");
		break;
	case SV_OPT_FULLOPTM:
		printf(SZ_CONFIG_SV_OPTIMIZATION_STAT, "SV_OPT_FULLOPTM");
		break;
	case SV_OPT_DISABLED:
		printf(SZ_CONFIG_SV_OPTIMIZATION_STAT, "SV_OPT_DISABLED");
		break;
	default:
		printf(SZ_CONFIG_ERROR, "SV_OPTIMIZATION");
		return 2;
	}
	
	switch (SET_TREE_USING)
	{
	case SET_TREE_AA:
		printf(SZ_CONFIG_SET_TREE_USING_STAT, "SET_TREE_AA");
		break;
	case SET_TREE_AVL:
		printf(SZ_CONFIG_SET_TREE_USING_STAT, "SET_TREE_AVL");
		break;
	default:
		printf(SZ_CONFIG_ERROR, "SET_TREE_USING");
		return 3;
	}
	
	printf("\n");
	
	printf("Before using checklist \e[92mcomplete.\e[m\n");
	
	return 0;
}

