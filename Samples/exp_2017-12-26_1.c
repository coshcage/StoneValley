//
//  exp_2017-12-26_1.c
//  This file demonstrates how to use Huffman coding tree.
//  Created by cyycoish#hotmail.com on 12/24/17.
//  Licence:  Public domain.
//  Platform: Unix|GNU/Linux
//

#include <stdio.h>
#include <string.h>
#include "../src/svdef.h"
#include "../src/svstring.h"
#include "../src/svtree.h"

static const unsigned char sdata[] = { /* Prepared and compressed data. */
	0xE4, 0x4E, 0x0F, 0x56, 0x27, 0x71, 0x5A, 0xF7,
	0x1F, 0x2F, 0x59, 0xBF, 0xEE, 0x0F, 0x6A, 0x63,
	0x0F, 0x66, 0xE3, 0x93, 0x17, 0xA5, 0x9E, 0xB5,
	0x92, 0x5A, 0x3B, 0x6C, 0xAC, 0x88, 0x4D, 0x77,
	0x39, 0xAB, 0x09, 0x16, 0x7D, 0x8B, 0x53, 0xB3,
	0x37, 0xC3, 0xF5, 0xA8, 0x4C, 0x56, 0x61, 0xD7,
	0xCA, 0xFE, 0x92, 0x90
};

static const HFM_SYMBOL stabl[] = { /* Prepared symbol table. */
	0040, 0002, 0000, 0145, 0003, 0003, 0164, 0004,
	0017, 0157, 0004, 0013, 0151, 0004, 0012, 0150,
	0004, 0005, 0141, 0004, 0004, 0163, 0005, 0031,
	0154, 0005, 0030, 0156, 0005, 0020, 0143, 0005,
	0021, 0000, 0005, 0022, 0144, 0006, 0047, 0162,
	0006, 0073, 0147, 0006, 0046, 0111, 0006, 0071,
	0155, 0006, 0070, 0123, 0007, 0157, 0077, 0007,
	0164, 0126, 0007, 0155, 0056, 0007, 0156, 0160,
	0007, 0152, 0146, 0007, 0153, 0124, 0007, 0150,
	0047, 0007, 0165, 0166, 0007, 0151, 0171, 0007,
	0154
};

/* Strings. */
static const char * pstr[2] = {"Hello, ", "library."};

/* This's a tricky function. */
int foo(void)
{
	P_ARRAY_Z ptbl;
	BITSTREAM bsin;
	P_BITSTREAM pbsout;
	if (NULL == strInitBitStream(&bsin))
		return __LINE__; /* Can not initialize bit-stream. */
	if (NULL == (ptbl = strCreateArrayZ(sizeof(stabl), sizeof(HFM_SYMBOL))))
		return __LINE__; /* Can not initialize array. */
	/* Set bsin and symbol table up. */
	bsin.bilc = 5; /* This number is recorded after encoding. */
	strResizeArrayZ(&bsin.arrz, sizeof(sdata), sizeof(UCHART));
	memcpy(bsin.arrz.pdata, sdata, sizeof(sdata));
	memcpy(ptbl->pdata, stabl, sizeof(stabl));
	/* Decode and display. */
	pbsout = treHuffmanDecoding(ptbl, &bsin);
	printf("%s%s\n", (char *)pbsout->arrz.pdata, pstr[1]);
	/* Cleanup. */
	strDeleteBitStream(pbsout);
	/* Don't delete bsin here. Free bsin. */
	strFreeBitStream(&bsin);
	strDeleteArrayZ(ptbl);
	return 0;
}

/* This function illustrates how to use Huffman algorithm to compress data. */
int main(void)
{
	P_ARRAY_Z ptbl = NULL; /* Used to store a symbol table. */
	P_BITSTREAM pbsout, pbsin; /* Bit-streams that used to contain data. */
	/* Encode string 'hello'. Caution that table is important for decoding. */
	if (NULL == (pbsin = treHuffmanEncoding(&ptbl, (const PUCHAR)pstr[0], 8)))
	{
		fprintf(stderr, "Error! Can not Encode.\n");
		return __LINE__;
	}
	if (NULL == (pbsout = treHuffmanDecoding(ptbl, pbsin)))
	{
		fprintf(stderr, "Error! Can not Decode.\n");
		return __LINE__;
	}
	/* Print hello. */
	printf("%s", (char *)pbsout->arrz.pdata);
	/* Don't forget to free bit-streams and symbol table after use. */
	strDeleteBitStream(pbsout);
	strDeleteBitStream(pbsin);
	strDeleteArrayZ(ptbl);
	return foo();
}
