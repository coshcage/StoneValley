//
//  exp_2017-12-26_1.c
//  This file demonstrates how to use Huffman coding tree.
//  Created by cosh.cage#hotmail.com on 12/24/17.
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
#include <stdio.h>
#include "svstring.h"
#include "svtree.h"

/* Define a character string. */
#define SZ_STR "I am the architect of StoneValley. I love this library."

/* This function illustrates how to use Huffman algorithm to compress data. */
int main(void)
{
	P_ARRAY_Z ptbl = NULL, pout; /* Used to store a symbol table. */
	P_BITSTREAM pbsin; /* Bit streams that used to contain data. */
	/* Encode string. Caution that table is important for decoding. */
	if (NULL == (ptbl = treCreateHuffmanTable(SZ_STR, sizeof(SZ_STR))))
	{
		fprintf(stderr, "Error! Can not create symbol table.\n");
		return __LINE__;
	}
	if (NULL == (pbsin = treHuffmanEncoding(ptbl, SZ_STR, sizeof(SZ_STR))))
	{
		fprintf(stderr, "Error! Can not encode.\n");
		return __LINE__;
	}
	printf("%zd/%zd bits.\n", BITSTREAM_BLOCK_BIT * (strLevelArrayZ(&pbsin->arrz) - 1) + pbsin->nbil, CHAR_BIT * sizeof(SZ_STR));
	if (NULL == (pout = treHuffmanDecoding(ptbl, pbsin)))
	{
		strDeleteBitStream(pbsin);
		strDeleteArrayZ(ptbl);
		fprintf(stderr, "Error! Can not decode.\n");
		return __LINE__;
	}
	/* Print the original string. */
	printf("%s\n", (char *)pout->pdata);
	/* Don't forget to free the bit stream and symbol table after use. */
	strDeleteBitStream(pbsin);
	strDeleteArrayZ(ptbl);
	strDeleteArrayZ(pout);
	return 0;
}

