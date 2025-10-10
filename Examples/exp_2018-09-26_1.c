//
//  exp_2018-09-26_1.c
//  Sorting for linked-lists.
//  Created by cosh.cage#hotmail.com on 09/26/18.
//  License:  LGPLv3
//  Platform: Cross Platform.
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
#include <stdio.h>
#include <stdlib.h>
#include "svstring.h"

#define LIST1_LEN (10) // Length of single pointer linked list.
#define LIST2_LEN (20) // Length of double pointer linked list.

// Function: cbftvs_print
// Desc:     Print list contents.
// Param:    pitem: pointer to each node in a list. param: value of enumeration NodeType.
// Return:   Either CBF_CONTINUE or CBF_TERMINATE.
int cbftvs_print(void * pitem, size_t param)
{
	if (ENT_SINGLE == param)
		printf("%c ", (char)(size_t)((P_NODE_S)pitem)->pdata);
	else if (ENT_DOUBLE == param)
		printf("%2d ", *(int *)((P_NODE_D)pitem)->pdata);
	else
		return CBF_TERMINATE;
	return CBF_CONTINUE;
}

// Function: cbftvs_copy_1
// Desc:     Copy data between a single linked-list and an array.
// Param:    pitem: pointer to each node in a list. param: a size_t[2] array.
// Return:   CBF_CONTINUE only.
int cbftvs_copy_1(void * pitem, size_t param)
{
	if (0[(size_t *)param]) /* Linked-list -> array. */
		*((*(char **)1[(size_t *)param])++) = (char)(size_t)((P_NODE_S)pitem)->pdata;
	else /* Array -> linked-list. */
		((P_NODE_S)pitem)->pdata = (PUCHAR)(size_t)*((*(char **)1[(size_t *)param])++);
	return CBF_CONTINUE;
}

// Function: cbftvs_copy_2
// Desc:     Copy data between a doubly linked-list and an array.
// Param:    pitem: pointer to each node in a list. param: a size_t[2] array.
// Return:   CBF_CONTINUE only.
int cbftvs_copy_2(void * pitem, size_t param)
{
	if (0[(size_t *)param]) /* Linked-list -> array. */
		*((*(int **)1[(size_t *)param])++) = *(int *)((P_NODE_D)pitem)->pdata;
	else /* Array -> linked-list. */
		*(int *)((P_NODE_D)pitem)->pdata = *((*(int **)1[(size_t *)param])++);
	return CBF_CONTINUE;
}

// Function: cbfcmp_1
// Desc:     Compare values for characters.
// Param:    px: a pointer to a char value. py: another pointer to a char value.
// Return:   Either 1, -1 or 0 depends on comparison result.
int cbfcmp_1(const void * px, const void * py)
{
	if (*(char *)px > *(char *)py) return  1;
	if (*(char *)px < *(char *)py) return -1;
	return 0;
}

// Function: cbfcmp_2
// Desc:     Compare values for ints.
// Param:    px: a pointer to an int value. py: another pointer to an int value.
// Return:   Either 1, -1 or 0 depends on comparison result.
int cbfcmp_2(const void * px, const void * py)
{
	if (*(int *)px > *(int *)py) return  1;
	if (*(int *)px < *(int *)py) return -1;
	return 0;
}

// Function: SortLinkedListSC
// Desc:     Sort data in a single pointer linked list. This function takes pdata as value directly.
// Param:    plist: pointer to a linked list. cbfcmp: pointer to a comparison function.
// Return:   TRUE: sorting succeeded; FALSE: sorting failed.
BOOL SortLinkedListSC(P_NODE_S plist, CBF_COMPARE cbfcmp)
{
	size_t a[2];
	char * pstr;
	P_ARRAY_Z parr;
	// Move list 1's content to an array.
	if (NULL == (parr = strCreateArrayZ(strLevelLinkedListSC(plist), sizeof(char))))
		return FALSE; // Allocation failure.
	pstr = (char *)parr->pdata;
	a[0] = TRUE;
	a[1] = (size_t)&pstr;
	strTraverseLinkedListSC_A(plist, NULL, cbftvs_copy_1, (size_t)a);
	strSortArrayZ(parr, sizeof(char), cbfcmp);
	// Copy data back to list.
	pstr = (char *)parr->pdata;
	a[0] = FALSE;
	a[1] = (size_t)&pstr;
	strTraverseLinkedListSC_A(plist, NULL, cbftvs_copy_1, (size_t)a);
	// Deallocate array.
	strDeleteArrayZ(parr);
	return TRUE;
}

// Function: SortLinkedListDC
// Desc:     Sort data in a single pointer linked list.
// Param:    plist:  pointer to a linked list.
//           cbfcmp: pointer to a comparison function.
//           brev:   TRUE, search list downward; FALSE, search list upward.
// Return:   TRUE: sorting succeeded; FALSE: sorting failed.
BOOL SortLinkedListDC(P_NODE_D plist, CBF_COMPARE cbfcmp, BOOL brev)
{
	size_t a[2];
	int * pint;
	P_ARRAY_Z parr;
	// Move list 2's content to an array.
	if (NULL == (parr = strCreateArrayZ(strLevelLinkedListDC(plist, brev), sizeof(int))))
		return FALSE; // Allocation failure.
	pint = (int *)parr->pdata;
	a[0] = TRUE;
	a[1] = (size_t)&pint;
	strTraverseLinkedListDC_A(plist, NULL, cbftvs_copy_2, (size_t)a, brev);
	strSortArrayZ(parr, sizeof(int), cbfcmp);
	// Copy data back to list.
	pint = (int *)parr->pdata;
	a[0] = FALSE;
	a[1] = (size_t)&pint;
	strTraverseLinkedListDC_A(plist, NULL, cbftvs_copy_2, (size_t)a, brev);
	// Deallocate array.
	strDeleteArrayZ(parr);
	return TRUE;
}

// Function: main
// Desc:     Program entry.
// Param:    N/A.
// Return:   0: no error; 1, 2: allocation failure.
int main(void)
{
	int i, n;
	NODE_S list1[LIST1_LEN];
	LIST_D list2, pnoded;
	char * pstr = "helloworld";
	strInitLinkedListDC(&list2);
	// Assemble single-linked list 1.
	list1[0].pdata = (PUCHAR)(size_t)(*pstr++);
	for (i = 1; i < LIST1_LEN; ++i)
	{
		/* To convert a size_t value to PUCHAR after converting char value to size_t
		 * to cheat compilers from warning me. Warning codes are shocking.
		 * I wish for compilers' forgiveness for my false conduct. :)
		 */
		list1[i].pdata = (PUCHAR)(size_t)(*pstr++);
		list1[i - 1].pnode = &list1[i];
	}
	list1[9].pnode = NULL; // Close tail.
	// Print list 1.
	puts("Linked lists sorting\n\nList 1:"); strTraverseLinkedListSC_A(list1, NULL, cbftvs_print, ENT_SINGLE); printf("\n");
	if (SortLinkedListSC(list1, cbfcmp_1))
		puts("List 1 after sorting:"), strTraverseLinkedListSC_A(list1, NULL, cbftvs_print, ENT_SINGLE);
	else
		puts("Error occurred while sorting list 1.");
	// Assemble doubly-pointer-linked list 2.
	srand((unsigned int)&n);
	n = rand() % 99 + 1;
	list2 = strCreateNodeD(&n, sizeof(int));
	for (i = 0, pnoded = list2; n = rand() % 99 + 1, i < (LIST2_LEN - 1); ++i)
	{
		if (NULL == (pnoded->ppnode[NEXT] = strCreateNodeD(&n, sizeof(int))))
		{
			strFreeLinkedListDC(&list2, FALSE);
			puts("Error occurred while creating list 2.");
			return 2; /* Allocation failure while creating doubly linked list. */
		}
		pnoded = pnoded->ppnode[NEXT];
	}
	// Print list 2.
	puts("\n\nList 2:"); strTraverseLinkedListDC_A(list2, NULL, cbftvs_print, ENT_DOUBLE, FALSE); printf("\n");
	if (SortLinkedListDC(list2, cbfcmp_2, FALSE))
		puts("List 2 after sorting:"), strTraverseLinkedListDC_A(list2, NULL, cbftvs_print, ENT_DOUBLE, FALSE);
	else
		puts("Error occurred while sorting list 2.");
	printf("\n");
	strFreeLinkedListDC(&list2, FALSE);
	return 0;
}

