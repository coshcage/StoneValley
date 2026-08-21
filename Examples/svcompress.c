/*
 * Name:        svcompress.c
 * Description: Compress files.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0120211637B0820262335L00250
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
 */
#include <stdlib.h>
#include <string.h>
#include "svcompress.h"
#include "svtree.h"

#define _GET_ABS(x) ((x) < 0 ? -(x) : (x))

static signed char _svcGetEndianness(void);

/* SVCF_File_structure:___________________________________
 * |Length:     |Name:                                   |
 * |------------|----------------------------------------|
 * |signed char |Platform integer length and endianness. |
 * |UCHART      |Symbol table length.                    |
 * |N/A         |Symbol table.                           |
 * |            |                                        |
 * |size_t      |Compressed data length.                 |
 * |UCHART      |The number of remaining bits.           |
 * |N/A         |Compressed data.                        |
 * |____________|________________________________________|
 */

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _svcGetEndianness
 * Description:   Get endianness during run time.
 * Parameters:    N/A
 * Return value:   1 Little endian.
 *                -1 Big endian.
 */
static signed char _svcGetEndianness(void)
{
	size_t t = 1;
	return BOOLIZE(*(char *)&t) ? 1 : -1;
}

/* Function name: svcCompressFile
 * Description:   Compress a file to a file.
 * Parameters:
 *      fpout Pointer to the output file.
 *       fpin Pointer to the input file.
 * Return value:  Error code.
 *                Please refer to the SVCERROR enumeration at file 'svcompress.h'.
 */
SVCERROR svcCompressFile(FILE * fpout, FILE * fpin)
{
	REGISTER int c;
	REGISTER size_t i = 0;
	P_BITSTREAM pbstm = NULL;
	ARRAY_Z arrInBuffer, * parrTable = NULL;
	
	if (NULL == fpin || NULL == fpout)
		return SVC_FILE_OPEN;
	
	if (NULL == strInitArrayZ(&arrInBuffer, BUFSIZ, sizeof(UCHART)))
		return SVC_ALLOCATION;
	
	/* Read fpin into buffer in the memory. */
	while (EOF != (c = fgetc(fpin)))
	{
		arrInBuffer.pdata[i] = (UCHART) c;
		if (++i >= strLevelArrayZ(&arrInBuffer) && NULL == strResizeBufferedArrayZ(&arrInBuffer, sizeof(UCHART), +BUFSIZ))
			goto Lbl_Compress_Error;
	}
	
	/* Create symbol table. */
	if (NULL == (parrTable = treCreateHuffmanTable((const char *)arrInBuffer.pdata, i)))
		goto Lbl_Compress_Error;
	
	/* Compress data. */
	if (NULL == (pbstm = treHuffmanEncoding(parrTable, (const char *)arrInBuffer.pdata, i)))
		goto Lbl_Compress_Error;
	
	/* Free in-buffer array. */
	strFreeArrayZ(&arrInBuffer);

	/* // In testing case, print decoded text out to show correctness.
	{
		P_ARRAY_Z pout;
		pout = treHuffmanDecoding(parrTable, pbstm);
		strDeleteArrayZ(pout);
	}
	*/
	
	/* Write file header which is a UCHART variable that indicates platform integer length and endianness. */
	if (EOF == fputc((signed char)sizeof(size_t) * _svcGetEndianness(), fpout))
		return SVC_FILE_IO;
	
	/* Write symbol table length. */
	if (EOF == fputc(parrTable->num, fpout))
		return SVC_FILE_IO;
	
	/* Write symbol table. */
	if (strLevelArrayZ(parrTable) != fwrite(parrTable->pdata, sizeof(HFM_SYMBOL), strLevelArrayZ(parrTable), fpout))
		return SVC_FILE_IO;
	
	/* Delete symbol table. */
	strDeleteArrayZ(parrTable);
	
	/* Write compressed data length. */
	if (1 != fwrite(&pbstm->arrz.num, sizeof(size_t), 1, fpout))
		return SVC_FILE_IO;
	
	/* Write the number of remaining bits. */
	if (EOF == fputc(pbstm->nbil, fpout))
		return SVC_FILE_IO;
	
	/* Write compressed data. */
	if (strLevelArrayZ(&pbstm->arrz) != fwrite(pbstm->arrz.pdata, sizeof(bitstream_block_t), strLevelArrayZ(&pbstm->arrz), fpout))
		return SVC_FILE_IO;
	
	/* Cleanup. */
	strDeleteBitStream(pbstm);
	
	return SVC_NONE;
	
Lbl_Compress_Error:
	/* Cleanup. */
	if (NULL != pbstm)
		strDeleteBitStream(pbstm);
	
	if (NULL != parrTable)
		strDeleteArrayZ(parrTable);
	
	if (NULL != arrInBuffer.pdata)
		strFreeArrayZ(&arrInBuffer);
	
	return SVC_COMPRESS;
}

/* Function name: svcDecompressFile
 * Description:   Decompress a file to a file.
 * Parameters:
 *      fpout Pointer to the output file.
 *       fpin Pointer to the input file.
 * Return value:  Error code.
 *                Please refer to the SVCERROR enumeration at file 'svcompress.h'.
 */
SVCERROR svcDecompressFile(FILE * fpout, FILE * fpin)
{
	size_t k;
	REGISTER int c;
	BITSTREAM bsin;
	REGISTER size_t i, j;
	P_ARRAY_Z parrTable, parro;

	if (NULL == fpin || NULL == fpout)
		return SVC_FILE_OPEN;
	
	/* Clear to decompress. */
	clearerr(fpin);
	
	/* Read platform length and endianness. */
	if (EOF == (c = (signed char)fgetc(fpin)))
		return SVC_FILE_TYPE;
	if (_svcGetEndianness() * c < 0 || sizeof(size_t) != (size_t)_GET_ABS(c))
		return SVC_PLATFORM;
	
	/* Read symbol table length. */
	if (EOF == (c = fgetc(fpin)))
		return SVC_FILE_TYPE;
	
	if (NULL == (parrTable = strCreateArrayZ(c, sizeof(HFM_SYMBOL))))
		return SVC_ALLOCATION;
	
	/* Read symbol table. */
	if ((size_t)c != fread(parrTable->pdata, sizeof(HFM_SYMBOL), c, fpin))
	{
		strDeleteArrayZ(parrTable);
		return SVC_FILE_IO;
	}
	
	/* Read compressed data length. */
	if (1 != fread(&k, sizeof(size_t), 1, fpin))
	{
		strDeleteArrayZ(parrTable);
		return SVC_FILE_IO;
	}
	
	/* Read the number of remaining bits. */
	if (EOF == (c = fgetc(fpin)))
	{
		strDeleteArrayZ(parrTable);
		return SVC_FILE_TYPE;
	}
	bsin.nbil = (size_t)c;
	
	/* Allot memory for compressed stream. */
	if (NULL == strInitArrayZ(&bsin.arrz, k, sizeof(bitstream_block_t)))
	{
		strDeleteArrayZ(parrTable);
		return SVC_ALLOCATION;
	}
	
	/* Read compressed data. */
	if (k != fread(bsin.arrz.pdata, sizeof(bitstream_block_t), k, fpin))
	{
		strFreeArrayZ(&bsin.arrz);
		strDeleteArrayZ(parrTable);
		return SVC_FILE_IO;
	}
	
	/* Decompress. */
	if (NULL == (parro = treHuffmanDecoding(parrTable, &bsin)))
	{
		strFreeArrayZ(&bsin.arrz);
		strDeleteArrayZ(parrTable);
		return SVC_DECOMPRESS;
	}
	
	/* Cleanup. */
	strFreeArrayZ(&bsin.arrz);
	strDeleteArrayZ(parrTable);
	
	/* Output result. */
	for (i = 0, j = strLevelArrayZ(parro); i < j; ++i)
	{
		if (EOF == fputc(parro->pdata[i], fpout))
		{
			strDeleteArrayZ(parro);
			return SVC_FILE_IO;
		}
	}
	
	/* Cleanup. */
	strDeleteArrayZ(parro);
	return SVC_NONE;
}

