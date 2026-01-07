//
//  exp_2026-08-16_1.c
//  Use Trie and array to calculate the frequency of words in an article.
//  Created by cosh.cage#hotmail.com on 08/16/25.
//  License:  LGPLv3
//  Platform: Unix|GNU/Linux
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
// Tips of use:
// Put this C source file with StoneValley/src/*.* together and run:
// $ cc *.c
// Use -s to sort words by their frequencies. Use -c to measure case sensitively.
// $ echo "bbb aaa" > ./a.out -s
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "svtree.h"
#include "svstring.h"
 
typedef struct st_VOCABULARY
{
	long   freq;  /* Word frequency. */
	char * pword; /* Allocated string. */
} VOCABULARY, * P_VOCABULARY;

/* File level function declarations. */
static P_VOCABULARY CreateWord(void);
static void DeleteWord(P_VOCABULARY pvol);
static void DestoryArray(P_ARRAY_Z parrz);
static int CbfPrintResult(void * pitem, size_t param);
static int cbfcmpchar(const void * px, const void * py);
static int cbfcmpfreq(const void * px, const void * py);

static P_VOCABULARY CreateWord(void)
{
	P_VOCABULARY pvol = malloc(sizeof(VOCABULARY));
	if (NULL != pvol)
	{
		pvol->freq  = 1;
		pvol->pword = NULL;
	}
	return pvol;
}

static void DeleteWord(P_VOCABULARY pvol)
{
	free(pvol->pword);
	free(pvol);
}

static void DestoryArray(P_ARRAY_Z parrz)
{
	size_t i = 0;
	for (; i < strLevelArrayZ(parrz); ++i)
	{
		P_VOCABULARY pvol = *(((P_VOCABULARY *)parrz->pdata) + i);
		if (NULL != pvol)
			DeleteWord(pvol);
		else
			break;
	}
	strFreeArrayZ(parrz);
}

static int CbfPrintResult(void * pitem, size_t param)
{
	printf("%s %ld\n", (*((P_VOCABULARY *)pitem))->pword, (*((P_VOCABULARY *)pitem))->freq);
	DWC4100(param);
	return CBF_CONTINUE;
}

static int cbfcmpchar(const void * px, const void * py)
{
	return *(char *)px - *(char *)py;
}

static int cbfcmpfreq(const void * px, const void * py)
{
	return (*(P_VOCABULARY *)py)->freq - (*(P_VOCABULARY *)px)->freq;
}

int main(int argc, char * argv[])
{
	int c;
	long i, j = 0, k = 0;
	bool bCase = false, bSort = false;
	ARRAY_Z arrbuf = { 0 };
	ARRAY_Z arrz   = { 0 };
	P_TRIE_A ptrie;
	
	if (NULL == strInitArrayZ(&arrbuf, BUFSIZ, sizeof(char)))
		return 1;
	arrbuf.pdata[0] = '\0';
	if (NULL == (ptrie = treCreateTrieA()))
	{
		strFreeArrayZ(&arrbuf);
		return 2;
	}
	if (NULL == strInitArrayZ(&arrz, BUFSIZ, sizeof(P_VOCABULARY)))
	{
		treDeleteTrieA(ptrie, sizeof(char));
		strFreeArrayZ(&arrbuf);
		return 3;
	}
 
	for (i = 1; i < argc; ++i)
	{
		if (0 == strcmp("-s", argv[i])) /* Sorting switch. */
			bSort = true;
		else if (0 == strcmp("-c", argv[i])) /* Case switch. */
			bCase = true;
		else
		{
			DestoryArray(&arrz);
			treDeleteTrieA(ptrie, sizeof(char));
			strFreeArrayZ(&arrbuf);
			return 4;
		}
	}
	while (EOF != (c = fgetc(stdin)))
	{
		if (j >= strLevelArrayZ(&arrbuf))
		{
			if (NULL == strResizeBufferedArrayZ(&arrbuf, sizeof(char), +BUFSIZ))
			{
				DestoryArray(&arrz);
				treDeleteTrieA(ptrie, sizeof(char));
				strFreeArrayZ(&arrbuf);
				return 5;
			}
		}
		if (c >= 'A' && c <= 'Z')
		{
			arrbuf.pdata[j++] = bCase ? (char)c : (char)tolower(c);
		}
		else if (c >= 'a' && c <= 'z')
		{
			arrbuf.pdata[j++] = (char)c;
		}
		else
		{
			arrbuf.pdata[j] = '\0';
			if (0 != j)
			{
				size_t * presult;
				P_VOCABULARY pvol;
				size_t bufferLen = strlen(arrbuf.pdata);
				/* Search word in the buffer at the Trie. */
				if (NULL == (presult = treSearchTrieA(ptrie, arrbuf.pdata, bufferLen, sizeof(char), cbfcmpchar)))
				{
					if (k >= strLevelArrayZ(&arrz) - 1)
					{
						if (NULL == strResizeBufferedArrayZ(&arrz, sizeof(P_VOCABULARY), +BUFSIZ))
						{
							DestoryArray(&arrz);
							treDeleteTrieA(ptrie, sizeof(char));
							strFreeArrayZ(&arrbuf);
							return 6;
						}
						*(P_VOCABULARY *)strLocateItemArrayZ(&arrz, sizeof(P_VOCABULARY), k) = NULL;
					}
					/* If can not find the word, then create a new word. */
					pvol = CreateWord();
					/* Insert the new word into the Trie. */
					treInsertTrieA(ptrie, arrbuf.pdata, bufferLen, sizeof(char), (size_t)pvol, cbfcmpchar);
					if (NULL == (pvol->pword = malloc(bufferLen + 1)))
						return 7;
					memcpy(pvol->pword, arrbuf.pdata, bufferLen + 1);
					/* Store the new word in the array. */
					*(P_VOCABULARY *)strLocateItemArrayZ(&arrz, sizeof(P_VOCABULARY), k++) = pvol;
				}
				else
				{	/* Found word. Fetch the appendix in the Trie and convert it to P_VOCABULARY, and increase the frequency. */
					pvol = (P_VOCABULARY)*presult;
					++pvol->freq;
				}
			}
			j = 0;
 
		}
	}
	strResizeArrayZ(&arrz, k, sizeof(P_VOCABULARY));
	if (bSort)
		strSortArrayZ(&arrz, sizeof(P_VOCABULARY), cbfcmpfreq);
	strTraverseArrayZ(&arrz, sizeof(P_VOCABULARY), CbfPrintResult, 0, false);
	DestoryArray(&arrz);
	treDeleteTrieA(ptrie, sizeof(char));
	strFreeArrayZ(&arrbuf);
	return 0;
}

