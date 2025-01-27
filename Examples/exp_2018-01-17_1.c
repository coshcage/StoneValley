//
//  exp_2018-01-17_1.c
//  This file demonstrates how to use a chaining hash table represented set.
//  Created by cosh.cage#hotmail.com on 01/17/18.
//  License:  LGPLv3
//  Platform: Unix|GNU/Linux
//  Copyright (C) 2018-2025 John Cage
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

// Prelude: Run and see what appears. I think you'll get the idea behind this program.
// Have a nice day and keep fun!

#include <stdio.h>
#include <string.h>
#include "../src/svset.h"
#include "../src/svstack.h"

static const char * pstr = "thequickbrownfoxjumpsoverthelazydog";

// This is a simple hash function. It converts char to size_t directly.
size_t hash(const void * param)
{
	return (size_t)*(char *)param;
}

int cbftvs(void * pitem, size_t param)
{
	stkPushA((P_STACK_A)param, pitem, sizeof(char));
	// Remember to return a CBF_CONTINUE in traverse functions.
	// Or traversal procedure will not go.
	return CBF_CONTINUE;
}

int cbfcmp(const void * x, const void * y)
{
	/* Attention here please:
	 * As usual, compare function will return +1 when x > y.
	 * And if x < y, function will return -1.
	 * But we don't want to print an alphabet in reverse.
	 * So we swap circumstances that in compare function branch to return -1 and +1 here.
	 */
	if (*(char *)x > *(char *)y) return -1;
	if (*(char *)x < *(char *)y) return  1;
	return 0;
}

int main(void)
{
	int i;
	// Notice that 113 is a prime number.
	// A suitable prime number is required while creating a chaining hash table.
	// The technic of choosing a prime number is simple:
	// Not too big, Not too small than the number of data you want to insert.
	P_SET_H pseta = setCreateH(113);
	if (NULL == pseta)
	{
		printf("%s\n", "Error! Can not create a set.");
		return 1;
	}
	// Print the source string.
	printf("     %s\n", pstr);
	for (i = 0; i < 35; ++i)
		setInsertH(pseta, hash, pstr + i, sizeof(char));
	// Put the string into set.
	// Set will absorb similar characters.
	// So that we can calculate the frequency of each letter in string.
	{
		int j = 0;
		char c = '\0';
		static const char * sign = "12345";
		char * psign = (char *)sign;
		// Create a stack. And the size of stack is the number of element in set.
		P_STACK_A pstk = stkCreateA(setSizeH(pseta), sizeof(char));
		if (NULL == pstk)
		{
			printf("%s\n", "Error! Can not create a stack.");
			return 2;
		}
		// Traverse the whole set. And push each element in set onto stack.
		hshTraverseC(pseta, cbftvs, (size_t)pstk);
		// Sort the array in the stack.
		strSortArrayZ(&pstk->arr, sizeof(char), cbfcmp);
		// Print the stack.
		while (!stkIsEmptyA(pstk))
		{
			stkPopA(&c, sizeof(char), pstk);
			printf("%2d %c ", ++j, c);
			// This following procedure is used to locate the first occurrence place of a letter in the string.
			for (i = 0; i < 35; ++i)
			{
				if (pstr[i] == c)
				{
					// Prevent from index over size.
					if ('\0' == *psign)
						psign = (char *)sign;
					putc((int)*(psign++), stdout); // Place a mark.
				}
				else
					putc(32, stdout); // Print a space.
			}
			psign = (char *)sign; // Put mark index back to the beginning.
			putc(124, stdout); // Print a pipe sign.
			putc(10, stdout); // Print the return character.
		}
		// Revoke space of stack.
		stkDeleteA(pstk);
	}
	printf("Through this map, we can not only know how many characters in a string, \n\
but also the frequency of each character and which place they dwelled in.\nA charming fancy clarity!\n");
	// Don't forget to revoke set spaces.
	setDeleteH(pseta);
	return 0;
}

