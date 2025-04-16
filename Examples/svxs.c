/*
 * Name:        svxs.c
 * Description: EXternal sort.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0415251642A0415252317L00234
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svxs.h"

/* File level function declaration. */
void _svxsDestroyMFileArrayZ(P_ARRAY_Z parrChunkFile, size_t chunk_count);

/* A structure to store file name and pointer. */
typedef struct _st_MFILE
{
	char * fname; /* File name. */
	FILE * fp;    /* File pointer. */
} MFILE, * P_MFILE;

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _svxsDestroyMFileArrayZ
 * Description:   This function is used to clean and delete MFILE array.
 * Parameters:
 * parrChunkFile Pointer to array.
 *   chunk_count How many elements there are in the array.
 * Return value:  N/A.
 */
void _svxsDestroyMFileArrayZ(P_ARRAY_Z parrChunkFile, size_t chunk_count)
{
	size_t i;
	for (i = 0; i < chunk_count; ++i)
	{
		P_MFILE pmf = (P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), i);
		if (NULL != pmf->fp)
			fclose(pmf->fp);
		/* Expunge temporary chunk file. */
		remove(pmf->fname);
		free(pmf->fname);
	}
	strDeleteArrayZ(parrChunkFile);
}

/* Function name: svXSort
 * Description:   EXternal sort algorithm.
 * Parameters:
 *      szfin Pointer to the set you want to allocate.
 *     szfout Number of buckets for the hash table in the set.
 *        num How many numbers there are in a chunk.
 *       size Size of each element in the chunk buffer.
 *     cbfcmp Pointer to compare function.
 *            Please refer to svdef.h for more detail.
 * Return value:  Please refer to svxs.h XSortError enumeration.
 * Tip:           szfin and szfout could be equal.
 */
XSortError svXSort(const char * szfin, const char * szfout, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	size_t i;
	P_ARRAY_Z parrChunk = strCreateArrayZ(num, size);
	P_ARRAY_Z parrChunkFile = strCreateArrayZ(BUFSIZ, sizeof(MFILE));
	P_ARRAY_Z parrBuffer;
	P_ARRAY_Z parrValid;
	
	size_t chunk_count = 0;
	size_t count = 0;
	ptrdiff_t valid_count = 0;
	
	FILE * input = fopen(szfin, "rb");
	FILE * output;
	
	if (!input)
	{
		strDeleteArrayZ(parrChunkFile);
		strDeleteArrayZ(parrChunk);
		return XSE_OPEN_INPUT_FILE;
	}

	while (!feof(input))
	{
		fread(strLocateItemArrayZ(parrChunk, size, count++), size, 1, input);

		if (count == num)
		{
			FILE * chunk_file;
			char chunk_filename[L_tmpnam];

			chunk_file = fopen(tmpnam(chunk_filename), "wb+");
			((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), chunk_count))->fp = chunk_file;
			((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), chunk_count))->fname = strdup(chunk_filename);
			++chunk_count;
			
			if (chunk_count >= strLevelArrayZ(parrChunkFile))
				strResizeBufferedArrayZ(parrChunkFile, sizeof(MFILE), +BUFSIZ);
			
			if (!chunk_file)
			{
				_svxsDestroyMFileArrayZ(parrChunkFile, chunk_count);
				strDeleteArrayZ(parrChunk);
				fclose(input);
				return XSE_OPEN_CHUNK_FILE;
			}

			qsort(parrChunk->pdata, count, size, cbfcmp);

			fwrite(parrChunk->pdata, size, count, chunk_file);
			
			count = 0;
		}
	}

	if (count > 0)
	{
		FILE * chunk_file;
		char chunk_filename[L_tmpnam];

		chunk_file = fopen(tmpnam(chunk_filename), "wb+");
		((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), chunk_count))->fp = chunk_file;
		((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), chunk_count))->fname = strdup(chunk_filename);
		++chunk_count;
		
		if (!chunk_file)
		{
			_svxsDestroyMFileArrayZ(parrChunkFile, chunk_count);
			strDeleteArrayZ(parrChunk);
			fclose(input);
			return XSE_OPEN_CHUNK_FILE;
		}

		qsort(parrChunk->pdata, count, size, cbfcmp);
		
		fwrite(parrChunk->pdata, size, count, chunk_file);
	}

	strDeleteArrayZ(parrChunk);
	
	if (0 == strcmp(szfin, szfout))	/* They are the same. */
	{
		output = freopen(szfout, "wb", input);
	}
	else
	{
		fclose(input);
		output = fopen(szfout, "wb");
	}
	
	if (!output)
	{
		_svxsDestroyMFileArrayZ(parrChunkFile, chunk_count);
		return XSE_OPEN_OUTPUT_FILE;
	}

	for (i = 0; i < chunk_count; ++i)
	{
		rewind(((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), i))->fp);
	}
	
	/* Merge chunk files. */
	parrBuffer = strCreateArrayZ(chunk_count, size);
	parrValid  = strCreateArrayZ(chunk_count, sizeof(BOOL));

	for (i = 0; i < chunk_count; ++i)
	{
		FILE * fp = ((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), i))->fp;

		if (!feof(fp))
		{
			fread(strLocateItemArrayZ(parrBuffer, size, i), size, 1, fp);
			*(BOOL *)strLocateItemArrayZ(parrValid, sizeof(BOOL), i) = TRUE;
			valid_count++;
		}
		else
		{
			*(BOOL *)strLocateItemArrayZ(parrValid, sizeof(BOOL), i) = FALSE;
		}
	}

	while (valid_count > 0)
	{
		FILE * fp;
		ptrdiff_t min_index = -1;
		for (i = 0; i < chunk_count; ++i)
		{
			if
			(
				*(BOOL *)strLocateItemArrayZ(parrValid, sizeof(BOOL), i) &&
				(
					min_index == -1 ||
					cbfcmp(strLocateItemArrayZ(parrBuffer, size, i), strLocateItemArrayZ(parrBuffer, size, min_index)) < 0
				)
			)
			{
				min_index = (ptrdiff_t)i;
			}
		}

		fwrite(strLocateItemArrayZ(parrBuffer, size, min_index), size, 1, output);
		
		fp = ((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), min_index))->fp;
		if (feof(fp))
		{
			*(BOOL *)strLocateItemArrayZ(parrValid, sizeof(BOOL), min_index) = FALSE;
			--valid_count;
		}
		else
			fread(strLocateItemArrayZ(parrBuffer, size, min_index), size, 1, fp);
	}

	strDeleteArrayZ(parrBuffer);
	strDeleteArrayZ(parrValid);

	_svxsDestroyMFileArrayZ(parrChunkFile, chunk_count);

	fclose(output);

	return XSE_NONE;
}

