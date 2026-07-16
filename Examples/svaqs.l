/*
 * Name:        svaqs.l
 * Description: StoneValley API Query System.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0608260445A0715262324L00629
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
 * Tips of usage:
 * SVAPS is a tool to help StoneValley users query library APIs efficiently.
 * Launch svaps after compiling it and type help to show more apropos contents for usage.
 * Put svaqs.l(this file) under StoneValley/src/ and run this compound command line under GNU/Linux.
 * $ flex svaqs.l; cc *.c -o svaqs; rm lex.yy.c; ./svaqs;
 * You will get an executable file named svaqs.
 * Usages of svaps:
 * $ ./svaps help
 * $ ./svaps version
 * $ ./svaps list 5 str
 * $ ./svaps list strCreate*
 * $ ./svaps list strCreateArrayZ
 *
 */
%option noyywrap

D [0-9]
L [a-zA-Z_]
S [ |\t|\n|\r]
I ({L}+|{D}+)

%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "svstring.h"
#include "svtree.h"
#include "svstack.h"
#include "svset.h"

bool bIsInComment = false;
bool bIsDesc      = false;
bool bIsFunc      = false;

ARRAY_Z arrbuf;
PUCHAR  pbuf;

int len;

typedef struct st_FunRecord {
	char * fname;
	char * fdesc;
	char * record;
	char * prototype;
} FunRecord, * P_FunRecord;

FunRecord gfc;

P_RBT pdb;

char cmdbuf[BUFSIZ];

#define MATSIZ 38

static void CheckBufferSize(int bflen)
{
	if ((size_t)(pbuf - arrbuf.pdata) >= strLevelArrayZ(&arrbuf))
		pbuf = (PUCHAR)strResizeBufferedArrayZ(&arrbuf, sizeof(char), +BUFSIZ);
	if (bflen > 0)
		pbuf += bflen;
}

static void * DuplicateASubstring(char * start, char * end)
{
	char * str = (char *) malloc(end - start + 1);
	size_t i = 0;
	while (start + i != end)
	{
		str[i] = start[i];
		++i;
	}
	str[i] = '\0';
	return str;
}

static char * strtolower(char * str)
{
	char * p = str;
	while (*p)
	{
		*p = tolower(*p);
		++p;
	}
	return str;
}

static void DissectAComment(void)
{
	char * str1, * str2 = NULL;
	/* Extract function name. */
	str1 = strstr((const char *)arrbuf.pdata, "Function name: ");
	if (NULL != str1)
	{
		str1 += strlen("Function name: ");
		str2 = strstr(str1, "\r");
		if (NULL == str2 || str2 <= str1)
			str2 = strstr(str1, "\n");
		gfc.fname = DuplicateASubstring(str1, str2);
	}
	/* Extract description. */
	str1 = strstr((const char *)arrbuf.pdata, "Description:   ");
	if (NULL != str1)
	{
		str1 += strlen("Description:   ");
		str2 = strstr(str1, ".");
		gfc.fdesc = strtolower(DuplicateASubstring(str1, str2));
	}
	gfc.record = strdup((char *)arrbuf.pdata);
}

static int cbfcmpstr(const void * px, const void * py)
{
	return strcmp(((P_FunRecord)px)->fname, ((P_FunRecord)py)->fname);
}

%}

%%

^{L}+({S}|\*)+{I}+\(.*$ {
	if (false == bIsInComment)
	{
		if (';' != yytext[strlen(yytext) - 2])
		{
			DissectAComment();
			gfc.prototype = strdup(yytext);
			treInsertRBT(pdb, &gfc, sizeof(gfc), cbfcmpstr);
		}
	}
	if (true == bIsFunc)
		bIsFunc = false;
}
\/\* {
	if (false == bIsInComment)
		bIsInComment = true;
	pbuf = arrbuf.pdata;
	len = sprintf((char *)pbuf, "%s", yytext);
	CheckBufferSize(len);
}
\*\/ {
	if (true == bIsInComment)
		bIsInComment = false;
	len = sprintf((char *)pbuf, "%s", yytext);
	CheckBufferSize(len);
}
"Function name: " {
	if (false == bIsFunc)
		bIsFunc = true;
	len = sprintf((char *)pbuf, "%s", yytext);
	CheckBufferSize(len);
}
"Description" {
	if (false == bIsDesc)
		bIsDesc = true;
	len = sprintf((char *)pbuf, "%s", yytext);
	CheckBufferSize(len);
}
"Parameter" {
	if (true == bIsDesc)
		bIsDesc = false;
	len = sprintf((char *)pbuf, "%s", yytext);
	CheckBufferSize(len);
}
. {
	if (true == bIsInComment)
	{
		len = sprintf((char *)pbuf, "%s", yytext);
		CheckBufferSize(len);
	}
}
\n|\r {
	if (true == bIsInComment)
	{
		len = sprintf((char *)pbuf, "%s", yytext);
		CheckBufferSize(len);
	}
}

%%

extern int _grpCBFCompareInteger(const void * px, const void * py);

/* StoneValley source file name array. */
const char * SZCFILE[] =
{
	"svatom.c",
	"svarray.c",
	"svlist.c",
	"svmatrix.c",
	"svmisc.c",
	"svstack.c",
	"svqueue.c",
	"svbytree.c",
	"svhtree.c",
	"svctree.c",
	"svgtree.c",
	"svstree.c",
	"svhash.c",
	"svset.c",
	"svgraph.c"
};

static int cbftvsCleanNode(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = (P_FunRecord)pnode->pdata;
	size_t i;
	
	for (i = 0; i < 4; ++i)
		free(i[(char **)pfc]);
	
	DWC4100(param);
	return CBF_CONTINUE;
}

static int cbftvsListAll(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = (P_FunRecord)pnode->pdata;
	
	printf("%zd\t%s\n", ++0[(size_t *)param], pfc->fname);
	
	return CBF_CONTINUE;
}

static int cbftvsListAllRelevant(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = (P_FunRecord)pnode->pdata;
	
	if (0 == memcmp(pfc->fname, (void *)1[(size_t *)param], 2[(size_t *)param]))
		printf("%zd\t%s\n", ++0[(size_t *)param], pfc->fname);
	
	return CBF_CONTINUE;
}

static void PrintHelp(void)
{
	printf("list: List relevant functions by a name.\n\tUsage: list [all|*|name|name*|number name]\n\tExamples:\n\tlist all\n\tlist *\n\tlist str*\n\tlist strCreateArrayZ\n\tlist 5 str\n\tNotice:\n\tNumber equals to an editorial distance to the following name.\n");
	printf("apropos: List relevant functions by keywords.\n\tUsage: apropos [key1 key2 ... keyn]\n\tExample:\n\tapropos create arrayz\n");
	printf("version: Show StoneValley library version.\n");
	printf("help: Show this content.\n");
	printf("exit: Exit program.\n");
	printf("quit: Ditto.\n");
}

#define _MIN(a, b) ((a) > (b) ? (b) : (a))

static size_t WagnerFischer(P_MATRIX pdp, const char * str1, const char * str2)
{
	size_t i = 0, j;
	size_t l1 = strlen(str1);
	size_t l2 = strlen(str2);

	if (0 == l1)
		return l2;
	if (0 == l2)
		return l1;

	for (i = 0; i <= l1; ++i)
		strSetValueMatrix(pdp, i, 0, &i, sizeof(size_t));

	for (j = 0; j <= l2; ++j)
		strSetValueMatrix(pdp, 0, j, &j, sizeof(size_t));

	for (i = 1; i <= l1; ++i)
	{
		char ch1 = str1[i - 1];
		for (j = 1; j <= l2; ++j)
		{
			char ch2 = str2[j - 1];

			size_t k = ch1 == ch2 ? 0 : 1;

			size_t m, n;

			strGetValueMatrix(&m, pdp, i - 1, j, sizeof(size_t));
			++m;
			strGetValueMatrix(&n, pdp, i, j - 1, sizeof(size_t));
			++n;
			m = _MIN(m, n);
			strGetValueMatrix(&n, pdp, i - 1, j - 1, sizeof(size_t));
			n += k;
			m = _MIN(m, n);

			strSetValueMatrix(pdp, i, j, &m, sizeof(size_t));
		}
	}
	
	strGetValueMatrix(&i, pdp, l1, l2, sizeof(size_t));

	return i;
}

static int cbftvsFuzzyQuery(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = (P_FunRecord)pnode->pdata;
	
	if (0 != 3[(size_t *)param])
	{
		if (WagnerFischer((P_MATRIX)2[(size_t *)param], pfc->fname, (const char *)1[(size_t *)param]) <= 3[(size_t *)param])
		{
			printf("%zd\t%s\n", ++0[(size_t *)param], pfc->fname);
		}
	}
	else
	{
		if (0 == strcmp((const char *)1[(size_t *)param], pfc->fname))
		{
			printf("NAME\n%s\nSYNOPSIS\n%c%s.\nDESCRIPTION\n%s\nPROTOTYPE\n%s\n", pfc->fname, toupper(pfc->fdesc[0]), pfc->fdesc + 1, pfc->record, pfc->prototype);
			return CBF_CONTINUE;
		}
	}
	return CBF_CONTINUE;
}

static bool IsStrNumeric(char * str)
{
	char * p = str;
	
	while(*p)
	{
		if (! isdigit(*p))
			return false;
		++p;
	}
	return true;
}

static void RewindArrStr(P_ARRAY_Z parr)
{
	size_t i;
	
	for (i = 0; i < strLevelArrayZ(parr); ++i)
	{
		char * pstr = *(char **)strLocateItemArrayZ(parr, sizeof(char *), i);
		if (NULL == pstr)
			break;
		free(pstr);
	}
	*(char **)strLocateItemArrayZ(parr, sizeof(char *), 0) = NULL;
}

static int cbftvsListAproposItems(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = (P_FunRecord)pnode->pdata;
	P_SET_T pseta = (P_SET_T)0[(size_t *)param];
	P_SET_T psetb = (P_SET_T)1[(size_t *)param];
	P_ARRAY_Z parr = (P_ARRAY_Z)2[(size_t *)param];
	char namebuf[MATSIZ];
	size_t i;
	
	for (i = 0; i < strLevelArrayZ(parr); ++i)
	{
		char * pstr = *(char **)strLocateItemArrayZ(parr, sizeof(char *), i);
		if (NULL != pstr)
		{
			strcpy(namebuf, pfc->fname);
			strtolower(namebuf);
			
			if (NULL != strstr(namebuf, pstr))
				setInsertT(pseta, &pfc, sizeof(P_FunRecord), _grpCBFCompareInteger);
			else
				break;
		}
		else
			break;
	}
	
	for (i = 0; i < strLevelArrayZ(parr); ++i)
	{
		char * pstr = *(char **)strLocateItemArrayZ(parr, sizeof(char *), i);
		if (NULL != pstr)
		{
			if (NULL != strstr(pfc->fdesc, pstr))
				setInsertT(psetb, &pfc, sizeof(P_FunRecord), _grpCBFCompareInteger);
		}
		else
			break;
	}
	
	return CBF_CONTINUE;
}

static int cbftvsListPrintSet(void * pitem, size_t param)
{
	P_TNODE_BY pnode = P2P_TNODE_BY(pitem);
	P_FunRecord pfc = *(P_FunRecord *)pnode->pdata;
	
	printf("%zd\t%s\n", ++*(size_t *)param, pfc->fname);
	
	return CBF_CONTINUE;
}

static void CommandParser(FILE * fpin)
{
	size_t i, j = 0, k = 0;
	MATRIX mat;
	bool bpstr;
	ARRAY_Z arrstr;
	char fnmebuf[BUFSIZ];
	enum en_function { EF_NONE, EF_LIST, EF_APROPOS } f = EF_NONE;
	
	strInitMatrix(&mat, MATSIZ, MATSIZ, sizeof(size_t));
	strInitArrayZ(&arrstr, BUFSIZ, sizeof(char *));
	
	for ( ;; )
	{
		char * p;
		
		i = 0;
		
		if (NULL != fpin)
		{
			printf("> ");
			fgets(cmdbuf, BUFSIZ, fpin);
		}
		
		p = strtok(cmdbuf, " \t\v\r\n");
		bpstr = false;
		
		while (NULL != p)
		{
			if (0 == i) /* p is the first token. */
			{
				if (0 == strcmp(p, "list"))
				{
					f = EF_LIST;
				}
				else if (0 == strcmp(p, "apropos"))
				{
					f = EF_APROPOS;
				}
				else if (0 == strcmp(p, "version"))
				{
					svPrintVersion();
					break;
				}
				else if (0 == strcmp(p, "help"))
				{
					PrintHelp();
					break;
				}
				else if (0 == strcmp(p, "exit") || 0 == strcmp(p, "quit"))
				{
					goto Lbl_Exit;
				}
				else
				{
					printf("Bad command: '%s'. Type help to show helpful contents.\n", p);
					break;
				}
			}
			else
			{
				if (EF_LIST == f)
				{
					if (0 == strcmp(p, "all") || '*' == p[0])
					{
						size_t sum = 0;
						if (i > 1) printf("\n");
						treMorrisTraverseBYIn(P2P_TNODE_BY(*pdb), cbftvsListAll, (size_t)&sum);
					}
					else if ('*' == p[strlen(p) - 1])
					{
						size_t a[3];
						
						a[0] = 0;
						a[1] = (size_t)p;
						a[2] = strlen(p) - 1;
						
						if (i > 1) printf("\n");
						treMorrisTraverseBYIn(P2P_TNODE_BY(*pdb), cbftvsListAllRelevant, (size_t)a);
					}
					else
					{
						if (IsStrNumeric(p))
						{
							j = (size_t) atoll(p);
						}
						else
						{
							size_t a[4];
							
							fnmebuf[0] = 0;
							strncat(fnmebuf, p, MATSIZ);
							
							a[0] = 0;
							a[1] = (size_t)fnmebuf;
							a[2] = (size_t)&mat;
							a[3] = j;
							
							if (bpstr) printf("\n");
							
							treTraverseBYIn(P2P_TNODE_BY(*pdb), cbftvsFuzzyQuery, (size_t)a);
							j = 0;
							bpstr = true;
						}
					}
				}
				else if (EF_APROPOS == f)
				{
					*(char **)strLocateItemArrayZ(&arrstr, sizeof(char *), k) = strtolower(strdup(p));
					*(char **)strLocateItemArrayZ(&arrstr, sizeof(char *), k + 1) = NULL;
					
					++k;
					if (k >= strLevelArrayZ(&arrstr))
						if (NULL == strResizeBufferedArrayZ(&arrstr, sizeof(char *), +BUFSIZ))
							goto Lbl_Exit;
				}
			}
			p = strtok(NULL, " \t\v\r\n");
			++i;
		}
		
		if (EF_APROPOS == f)
		{
			size_t a[3];
			size_t n = 0;
			P_SET_T pseta, psetb, psetc;
			
			pseta = setCreateT();
			psetb = setCreateT();
			
			a[0] = (size_t)pseta;
			a[1] = (size_t)psetb;
			a[2] = (size_t)&arrstr;
			
			treMorrisTraverseBYIn(P2P_TNODE_BY(*pdb), cbftvsListAproposItems, (size_t)a);
			
			psetc = setCreateIntersectionT(pseta, psetb, sizeof(P_FunRecord), _grpCBFCompareInteger);
			
			if (NULL != psetc) setTraverseTDispatch(psetc, cbftvsListPrintSet, (size_t)&n, treMorrisTraverseBYIn);
			
			if (NULL != pseta) setDeleteT(pseta);
			if (NULL != psetb) setDeleteT(psetb);
			if (NULL != psetc) setDeleteT(psetc);
		}
		
		RewindArrStr(&arrstr);
		f = EF_NONE;
		k = 0;
		
		if (NULL == fpin)
			break;
	}
Lbl_Exit:
	strFreeMatrix(&mat);
	strFreeArrayZ(&arrstr);
	return;
}

int main(int argc, char * argv[])
{
	size_t i;
	FILE * fpin;
	
	pdb = treCreateRBT();
	strInitArrayZ(&arrbuf, BUFSIZ, sizeof(char));
	pbuf = arrbuf.pdata;
	
	for (i = 0; i < sizeof(SZCFILE) / sizeof(SZCFILE[0]); ++i)
	{
		fpin = fopen(SZCFILE[i], "r");
		
		if (NULL != fpin)
		{
			yyrestart(fpin);
			yylex();
			fclose(fpin);
			fpin = NULL;
		}
		else
			printf("WARNING: Cannot open file \"%s\"!\n", SZCFILE[i]);
	}
	
	strFreeArrayZ(&arrbuf);
	
	if (argc <= 1)
	{
		printf("################################\n");
		printf("# StoneValley API Query System #\n");
		printf("#             SVAQS            #\n");
		printf("################################\n");
		svPrintVersion();
		CommandParser(stdin);
	}
	else
	{
		cmdbuf[0] = 0;
		for (i = 1; i < (const size_t)argc; ++i)
		{
			if (strlen(cmdbuf) + strlen(argv[i]) + 2 < BUFSIZ)
			{
				strncat(cmdbuf, argv[i], strlen(argv[i]) + 1);
				strncat(cmdbuf, " ", 2);
			}
		}
		CommandParser(NULL);
	}
	
	treMorrisTraverseBYPre(P2P_TNODE_BY(*pdb), cbftvsCleanNode, 0);
	treDeleteRBT(pdb);
	return 0;
}

