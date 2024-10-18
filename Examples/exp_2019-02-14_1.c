//
//  exp_2019-02-14_1.c
//  An example of matrices.
//  Created by cosh.cage#hotmail.com on 02/14/19.
//  License:  LGPLv3
//  Platform: Unix|GNU/Linux
//  Copyright (C) 2019-2024 John Cage
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
#include <string.h>
#include "../src/svstring.h"

// Function: Plus
// Desc:     Addition.
//           Please refer to the definition of callback function CBF_ALGEBRA.
// Return:   CBF_CONTINUE only.
int Plus(const void * pa, const void * pb)
{
	*(char *)pa += *(char *)pb;
	return CBF_CONTINUE;
}

// Function: Minus
// Desc:     Subtraction.
//           Please refer to the definition of callback function CBF_ALGEBRA.
// Return:   CBF_CONTINUE only.
int Minus(const void * pa, const void * pb)
{
	*(char *)pa -= *(char *)pb;
	return CBF_CONTINUE;
}

// Function: Times
// Desc:     Multiplication.
//           Please refer to the definition of callback function CBF_ALGEBRA.
// Return:   CBF_CONTINUE only.
int Times(const void * pa, const void * pb)
{
	*(char *)pa *= *(char *)pb;
	return CBF_CONTINUE;
}

// Function: PrintMatrix
// Desc:     Print a matrix.
// Param:    pmat: pointer to a matrix you want to print. width: Print matrix in a width * width area.
// Return:   N/A.
void PrintMatrix(P_MATRIX pmat, const char width)
{
	char i, j, k, b, m, n;
	for (i = 0; i < width; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			for (k = 0, b = FALSE; k < (const char) pmat->col; ++k)
			{
				if (NULL != strGetValueMatrix(&m, pmat, 0, k, sizeof(char)) && m == i &&
					NULL != strGetValueMatrix(&n, pmat, 1, k, sizeof(char)) && n == j
					) b = TRUE;
			}
			if (b)
				printf("E");
			else
				printf(" ");
		}
		printf("\n");
	}
}

// Function: main
// Desc:     Program entry.
// Param:    N/A.
// Return:   0: no error. NOT 0: allocation failure.
int main(void)
{
	const char data[] = { // Stores a big letter E.
	//    1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
	/*1*/ 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 3, 4, 4, 4, 4, 4, // Y.
	/*2*/ 0, 1, 2, 3, 4, 0, 0, 1, 2, 3, 0, 0, 1, 2, 3, 4  // X.
	};
	const char conv[] = { -1, 0, 0, 1 };
	char c = 5;
	MATRIX mat; // A matrix header.
	P_MATRIX pmword, pmimage, pmtemp, pmconv, ppmat[3]; // Matrix pointers.
	CBF_ALGEBRA cbfalg[2];
	if (NULL == (pmword  = strCreateMatrix(2, 16, sizeof(char)))) {
		c = 4; goto Lbl_Clr;
	}
	if (NULL == (pmimage = strCreateMatrix(2, 16, sizeof(char)))) {
		c = 3; goto Lbl_Clr;
	}
	if (NULL == (pmtemp  = strCreateMatrix(2, 16, sizeof(char)))) {
		c = 2; goto Lbl_Clr;
	}
	if (NULL == (pmconv  = strCreateMatrix(2,  2, sizeof(char)))) {
		c = 1; goto Lbl_Clr;
	}
	// Assemble cbfalg array for strM3Matrix.
	cbfalg[0] = Plus;
	cbfalg[1] = Times;
	// Assemble ppmat for strM3Matrix.
	ppmat[0] = pmtemp; // Matrix C.
	ppmat[1] = pmconv; // Matrix A.
	ppmat[2] = pmword; // Matrix B.
	// Copy original data.
	memcpy(pmword->arrz.pdata, data, sizeof(data));
	memcpy(pmconv->arrz.pdata, conv, sizeof(conv));
	// Fill pmimage with x=5, y=5.
	strSetMatrix(pmimage, &c, sizeof(char));
	// Print big letter 'E'.
	PrintMatrix(pmword, 5);
	printf("\nAfter converting:\n\n");
	strM3Matrix(ppmat, &c, sizeof(char), cbfalg); // Flip.
	strM2Matrix(pmimage, pmtemp, sizeof(char), Minus); // Move right down.
	// Assemble matrix header. Subtract every y by 5.
	mat.arrz.pdata = pmimage->arrz.pdata;
	mat.arrz.num = pmimage->arrz.num;
	mat.ln = 1;
	mat.col = pmimage->col;
	c = 5; strM1Matrix(&mat, &c, sizeof(char), Minus); // Move up.
	PrintMatrix(pmimage, 6); // Print converted 'E'mage.
	c = 0;
Lbl_Clr:
	switch(c)
	{
	case 0: strDeleteMatrix(pmconv);
	case 1: strDeleteMatrix(pmtemp);
	case 2: strDeleteMatrix(pmimage);
	case 3: strDeleteMatrix(pmword);
	default: break;
	}
	return (int)c;
}
