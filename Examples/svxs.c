/*
 * Name:        svxs.c
 * Description: EXternal sort.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0415251642A0709251630L00238
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
#include <stdio.h>  /* Using type FILE, function fclose and remove. */
#include <stdlib.h> /* Using function free. */
#include <string.h> /* Using function strdup. */
#include "svstring.h"
#include "svxs.h"

/* Internal sorting function switch. */
#define FN_SORT svMergeSort

/* File level function declaration. */
void _svxsDestroyMFileArrayZ(P_ARRAY_Z parrChunkFile, size_t uChunkCount);

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
 *   uChunkCount How many elements there are in the array.
 * Return value:  N/A.
 */
void _svxsDestroyMFileArrayZ(P_ARRAY_Z parrChunkFile, size_t uChunkCount)
{
	size_t i;
	for (i = 0; i < uChunkCount; ++i)
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
 *      fpout Pointer to a file to store result after sorting for writing.
 *       fpin Pointer to a file that you want to sort for reading.
 *        len How many data elements in fpin.
 *        num How many numbers there are in a chunk.
 *       size Size of each data element in the chunk buffer and fpin.
 *     cbfcmp Pointer to compare function.
 *            Please refer to svdef.h for more detail.
 * Return value:  Please refer to svxs.h XSortError enumeration.
 * Tip:           After sorting, you should close fpin and fpout.
 *                Parameters fpout and fpin can be equivalent.
 * Usage:         Please refer to the tail of svxs.h for more details.
 */
XSortError svXSort(FILE * fpout, FILE * fpin, size_t len, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	if ((!len) || (!num) || (!size))
		return XSE_BAD_ARGUMENT;
	else
	{
		size_t i;
		fpos_t pos;
		P_ARRAY_Z parrChunk     = strCreateArrayZ(num, size);
		P_ARRAY_Z parrChunkFile = strCreateArrayZ(BUFSIZ, sizeof(MFILE));
		P_ARRAY_Z parrBuffer;
		P_BITMAT  pbmValid;
		
		size_t    uChunkCount = 0;
		size_t    uCount      = 0;
		ptrdiff_t uValidCount = 0;
		
		if (!fpin)
		{
			strDeleteArrayZ(parrChunkFile);
			strDeleteArrayZ(parrChunk);
			return XSE_OPEN_INPUT_FILE;
		}
		
		if (fpin == fpout)
			fgetpos(fpout, &pos);

		while (!feof(fpin) && len--)
		{
			fread(strLocateItemArrayZ(parrChunk, size, uCount++), size, 1, fpin);

			if (uCount == num)
			{
				FILE * fpChunkFile;
				char szChunkFilename[L_tmpnam];

				fpChunkFile = fopen(tmpnam(szChunkFilename), "wb+");
				((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), uChunkCount))->fp = fpChunkFile;
				((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), uChunkCount))->fname = strdup(szChunkFilename);
				++uChunkCount;
				
				if (uChunkCount >= strLevelArrayZ(parrChunkFile))
					strResizeBufferedArrayZ(parrChunkFile, sizeof(MFILE), +BUFSIZ);
				
				if (!fpChunkFile)
				{
					_svxsDestroyMFileArrayZ(parrChunkFile, uChunkCount);
					strDeleteArrayZ(parrChunk);
					return XSE_OPEN_CHUNK_FILE;
				}

				FN_SORT(parrChunk->pdata, uCount, size, cbfcmp);

				fwrite(parrChunk->pdata, size, uCount, fpChunkFile);
				
				uCount = 0;
			}
		}

		if (uCount > 0)
		{
			FILE * fpChunkFile;
			char szChunkFilename[L_tmpnam];

			fpChunkFile = fopen(tmpnam(szChunkFilename), "wb+");
			((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), uChunkCount))->fp = fpChunkFile;
			((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), uChunkCount))->fname = strdup(szChunkFilename);
			++uChunkCount;
			
			if (!fpChunkFile)
			{
				_svxsDestroyMFileArrayZ(parrChunkFile, uChunkCount);
				strDeleteArrayZ(parrChunk);
				return XSE_OPEN_CHUNK_FILE;
			}

			FN_SORT(parrChunk->pdata, uCount, size, cbfcmp);
			
			fwrite(parrChunk->pdata, size, uCount, fpChunkFile);
		}

		strDeleteArrayZ(parrChunk);
		
		if (!fpout)
		{
			_svxsDestroyMFileArrayZ(parrChunkFile, uChunkCount);
			return XSE_OPEN_OUTPUT_FILE;
		}
		
		if (fpin == fpout)
			fsetpos(fpout, &pos);

		for (i = 0; i < uChunkCount; ++i)
		{
			rewind(((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), i))->fp);
		}
		
		/* Merge chunk files. */
		parrBuffer = strCreateArrayZ(uChunkCount, size);
		pbmValid   = strCreateBMap(1, uChunkCount, FALSE);

		for (i = 0; i < uChunkCount; ++i)
		{
			FILE * fp = ((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), i))->fp;

			if (!feof(fp))
			{
				fread(strLocateItemArrayZ(parrBuffer, size, i), size, 1, fp);
				strSetBitBMap(pbmValid, 0, i, TRUE);
				++uValidCount;
			}
			else
			{
				strSetBitBMap(pbmValid, 0, i, FALSE);
			}
		}

		while (uValidCount > 0)
		{
			FILE * fp;
			ptrdiff_t iMinIndex = -1;
			for (i = 0; i < uChunkCount; ++i)
			{
				if
				(
					strGetBitBMap(pbmValid, 0, i) &&
					(
						iMinIndex == -1 ||
						cbfcmp(strLocateItemArrayZ(parrBuffer, size, i), strLocateItemArrayZ(parrBuffer, size, iMinIndex)) < 0
					)
				)
				{
					iMinIndex = (ptrdiff_t)i;
				}
			}

			fwrite(strLocateItemArrayZ(parrBuffer, size, iMinIndex), size, 1, fpout);
			
			fp = ((P_MFILE)strLocateItemArrayZ(parrChunkFile, sizeof(MFILE), iMinIndex))->fp;
			if (feof(fp))
			{
				strSetBitBMap(pbmValid, 0, iMinIndex, FALSE);
				--uValidCount;
			}
			else
			{
				fread(strLocateItemArrayZ(parrBuffer, size, iMinIndex), size, 1, fp);
			}
		}

		strDeleteArrayZ(parrBuffer);
		strDeleteBMap(pbmValid);

		_svxsDestroyMFileArrayZ(parrChunkFile, uChunkCount);

		return XSE_NONE;
	}
}

