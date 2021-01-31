//
//  exp_2017-12-26_1.c
//  This file demonstrates how to use Huffman coding tree.
//  Created by cosh.cage#hotmail.com on 12/24/17.
//  Licence:  Public domain.
//  Platform: Unix|GNU/Linux
//

#include <stdio.h>
#include "../src/svstring.h"
#include "../src/svtree.h"

/* This function illustrates how to use Huffman algorithm to compress data. */
int main(void)
{
	P_ARRAY_Z ptbl = NULL; /* Used to store a symbol table. */
	P_BITSTREAM pbsout, pbsin; /* Bit-streams that used to contain data. */
	/* Encode string. Caution that table is important for decoding. */
	if (NULL == (pbsin = treHuffmanEncoding(&ptbl, (const PUCHAR)"I am the architect of StoneValley. I love this library.", 56)))
	{
		fprintf(stderr, "Error! Can not Encode.\n");
		return __LINE__;
	}
	if (NULL == (pbsout = treHuffmanDecoding(ptbl, pbsin)))
	{
		fprintf(stderr, "Error! Can not Decode.\n");
		return __LINE__;
	}
	/* Print the original string. */
	printf("%s\n", (char *)pbsout->arrz.pdata);
	/* Don't forget to free bit-streams and symbol table after use. */
	strDeleteBitStream(pbsout);
	strDeleteBitStream(pbsin);
	strDeleteArrayZ(ptbl);
	return 0;
}
