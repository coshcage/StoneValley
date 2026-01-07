/*
 * Name:        svarray.c
 * Description: Sized array.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948B1025252335L00860
 * License:     LGPLv3
 * Copyright (C) 2017-2026 John Cage
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

#include <stdio.h>  /* Using macro BUFSIZ. */
#include <stdlib.h> /* Using function malloc, calloc, realloc, free, srand, rand. */
#include <string.h> /* Using function memcpy, memmove, memcmp. */
#include "svstring.h"

/* File level function declarations here. */
void _strGetZArray(size_t z[], P_ARRAY_Z parrz, size_t size);

/* Function name: strInitCharacterStringArrayZ
 * Description:   Initialize a sized array through a string.
 * Parameters:
 *      parrz Pointer to the sized array you want to allocate.
 *       pstr Pointer to the string you want to copy.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
char * strInitCharacterStringArrayZ(P_ARRAY_Z parrz, const char * pstr)
{
	parrz->num = 0;
	while (pstr[parrz->num])
		++(parrz->num);
	/* While this is not an efficient implementation. */
	/* parrz->num = strlen(pstr). */
	parrz->pdata = (PUCHAR) malloc(strLevelArrayZ(parrz));
	if (NULL == parrz->pdata)
		parrz->num = 0;
	else
		memcpy(parrz->pdata, pstr, strLevelArrayZ(parrz));
	return (char *)parrz->pdata;
}

/* Function name: strCreateCharacterStringArrayZ
 * Description:   Create a sized array from a string.
 * Parameters:
 *       pstr Pointer to the string you want to copy.
 * Return value:  Pointer to new allocated structure.
 *                If function returned NULL, it would indicate an allocation failure.
 * Caution:       N/A.
 */
P_ARRAY_Z strCreateCharacterStringArrayZ(const char * pstr)
{
	P_ARRAY_Z parrz = (P_ARRAY_Z) malloc(sizeof(ARRAY_Z));
	if (NULL != parrz)
	{
		if (NULL == strInitCharacterStringArrayZ(parrz, pstr))
		{	/* Allocation failure. */
			free(parrz);
			return NULL;
		}
	}
	return parrz;
}

/* Function name: strLevelArrayZ
 * Description:   Return how many items there are stored in a sized array.
 * Parameter:
 *     parrz Pointer to a sized array.
 * Return value:  Number of nodes of a sized array.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strLevelArrayZ_M is available.
 */
size_t strLevelArrayZ_O(P_ARRAY_Z parrz)
{
	return parrz->num;
}

/* Function name: strTraverseArrayZ
 * Description:   Traverse each elements in a sized array.
 * Parameters:
 *      parrz Pointer to the array you want to traverse.
 *       size Element size of an array.
 *     cbftvs Pointer to the callback function.
 *      param Parameter which can be transferred into callback function.
 *       brev Input true to traverse array in order.
 *            Input false to traverse array in reverse.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of parrz Must Be Allocated first.
 */
int strTraverseArrayZ(P_ARRAY_Z parrz, size_t size, CBF_TRAVERSE cbftvs, size_t param, bool brev)
{
	if (strLevelArrayZ(parrz) > 0)
	{
		REGISTER size_t i = 0, j = strLevelArrayZ(parrz);
		REGISTER PUCHAR p;
		if (brev)
		{
			for (p = parrz->pdata + j * size - size; i < j; ++i, p -= size)
				if (CBF_CONTINUE != cbftvs(p, param))
					return CBF_TERMINATE;
		}
		else
		{
			for (p = parrz->pdata; i < j; ++i, p += size)
				if (CBF_CONTINUE != cbftvs(p, param))
					return CBF_TERMINATE;
		}
	}
	return CBF_CONTINUE;
}

/* Function name: strCopyArrayZ_O
 * Description:   Copy data in sized array psrc to sized array pdest.
 * Parameters:
 *      pdest Pointer to the destination array where the content is to be copied.
 *       psrc Pointer to the source of array to be copied.
 *       size Size of element in both two arrays and they must have the same value.
 * Return value:  pdest->pdata
 * Caution:       Size of element in both two arrays must have the same value.
 *                Number of two arrays shall equal to each other.
 *                Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 * Tip:           A macro version of this function named strCopyArrayZ_M is available.
 */
void * strCopyArrayZ_O(P_ARRAY_Z pdest, P_ARRAY_Z psrc, size_t size)
{
	return memcpy(pdest->pdata, psrc->pdata, strLevelArrayZ(psrc) * size);
}

/* Function name: strMoveArrayZ_O
 * Description:   Copy data in sized array psrc to sized array pdest.
 * Parameters:
 *      pdest Pointer to the destination array where the content is to be copied.
 *       psrc Pointer to the source of array to be copied.
 *       size Size of element in both two arrays must have the same value.
 * Return value:  pdest->pdata
 * Caution:       Size of element in both two arrays must have the same value.
 *                Number of two arrays shall equal to each other.
 *                Address of pdest and psrc Must Be Allocated first.
 * Tip:           Function allows the destination and source to overlap.
 *                A macro version of this function named strMoveArrayZ_M is available.
 */
void * strMoveArrayZ_O(P_ARRAY_Z pdest, P_ARRAY_Z psrc, size_t size)
{
	return memmove(pdest->pdata, psrc->pdata, strLevelArrayZ(pdest) * size);
}

/* Function name: strLocateItemArrayZ_O
 * Description:   Locate an item in a sized array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *      index Index of the item that you want to locate in the array. Index starts from 0.
 * Return value:  Pointer to the item in the array.
 *                If the index that you inputted were out of range, function would return value NULL.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strLocateItemArrayZ_M is available.
 */
void * strLocateItemArrayZ_O(P_ARRAY_Z parrz, size_t size, size_t index)
{
	return (index < strLevelArrayZ(parrz) ? parrz->pdata + index * size : NULL);
}

/* Function name: strLinearSearchArrayZ
 * Description:   Linear search items in a sized array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      pitem Pointer to an element as the searching target.
 *       size Size of an element.
 *       brev Input true  to search array in reverse.
 *            Input false to search array in order. That means to search the element from index 0 to the end of array.
 * Return value:  (*) Index of element + 1. If function returned 0, it should mean pitem could not found.
 * Caution:       Address of parrz Must Be Allocated first.
 */
size_t strLinearSearchArrayZ(P_ARRAY_Z parrz, const void * pitem, size_t size, bool brev)
{
	if (strLevelArrayZ(parrz) > 0)
	{
		REGISTER size_t i;
		REGISTER PUCHAR p;
		if (brev)
		{
			for (i = strLevelArrayZ(parrz), p = parrz->pdata + (strLevelArrayZ(parrz) - 1) * size; i >= 1; --i, p -= size)
				if (0 == memcmp(p, pitem, size))
					return i;
		}
		else
		{
			for (i = 1, p = parrz->pdata; i <= strLevelArrayZ(parrz); ++i, p += size)
				if (0 == memcmp(p, pitem, size))
					return i;
		}
	}
	return 0;
}

/* Function name: strInsertItemArrayZ
 * Description:   Insert an item into an array by index.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      pitem Pointer to an element that to be inserting.
 *       size Size of each element in the array. Size equaled the size of the item that pitem points.
 *      index Index of the item you want to insert into the array.
 *            Index starts from 0 and ends with parrz->num.
 * Return value:  The address of new inserted item in the array.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           Users may call function strResizeArrayZ first to increase the size of the parrz pointed array.
 *                This following text shows how this function works:
 *                Before insertion, the array could be: 1 2 3 4 5
 *                After inserting value 9 onto index 2: 1 2 9 3 4 5
 */
void * strInsertItemArrayZ(P_ARRAY_Z parrz, const void * pitem, size_t size, size_t index)
{
	return memcpy /* Give new item spaces and insert new item into array. */
	(
		(PUCHAR)memmove
		(
			parrz->pdata + (index + 1) * size,
			parrz->pdata + index * size,
			(strLevelArrayZ(parrz) - 1 - index) * size
		) - size,
		pitem,
		size
	);
}

/* Function name: strRemoveItemArrayZ
 * Description:   Remove item in an array by the index of this item.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *      index Index of the item you want to remove. Index starts from 0.
 *    bshrink Input true to shrink array, otherwise input false.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void strRemoveItemArrayZ(P_ARRAY_Z parrz, size_t size, size_t index, bool bshrink)
{
	if (index >= strLevelArrayZ(parrz))
		return; /* Index out of range. */
	else
	{
		if (0 != --parrz->num)
			memmove(parrz->pdata + index * size,
					parrz->pdata + (index + 1) * size,
					(strLevelArrayZ(parrz) - index) * size);
		if (bshrink)
			strResizeArrayZ(parrz, strLevelArrayZ(parrz), size);
	}
}

/* Function name: strSortArrayZ_O
 * Description:   Sort an array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strSortArrayZ_M is available.
 */
void strSortArrayZ_O(P_ARRAY_Z parrz, size_t size, CBF_COMPARE cbfcmp)
{
	svQuickSort(parrz->pdata, strLevelArrayZ(parrz), size, cbfcmp);
}

/* Function name: strMergeSortedArrayZ
 * Description:   Merge two sorted arrays.
 * Parameters:
 *      pdest Pointer to the destination array that you want to merge.
 *       psrc Pointer to the source array.
 *       size Size of each element in two arrays.
 *            Each item in these two sorted arrays shall be in the same size.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition of CBF_COMPARE in svdef.h.
 * Return value:  If merging succeeded, function would return (pdest->pdata),
 *                otherwise function would return value NULL.
 * Caution:       Address of pdest and psrc Must Be Allocated first dynamically.
 *                (*) Two arrays that pdest and psrc pointed shall be sorted in either
 *                increasing or decreasing order before invoking this function.
 */
void * strMergeSortedArrayZ(P_ARRAY_Z pdest, P_ARRAY_Z psrc, size_t size, CBF_COMPARE cbfcmp)
{
#define _P_ARRAY_Z_ITEM(parrz, index) ((parrz)->pdata + (index) * (size))

	REGISTER size_t di = 0, si = 0, sj = 0, t;
	REGISTER int r0, r1;
	REGISTER bool binc =
	(
		cbfcmp(pdest->pdata, _P_ARRAY_Z_ITEM(pdest, strLevelArrayZ(pdest) - 1)) <= 0 &&
		cbfcmp(psrc->pdata,  _P_ARRAY_Z_ITEM(psrc,  strLevelArrayZ(psrc)  - 1)) <= 0
	) ?
	true : /* Both destination and source are in increasing order. */
	false; /* Both destination and source are in decreasing order. */

	PUCHAR pnew = (PUCHAR) realloc(pdest->pdata, (strLevelArrayZ(pdest) + strLevelArrayZ(psrc)) * size);
	if (NULL == pnew)
	{
		pdest->num = 0;
		pdest->pdata = NULL;
	}
	else
	{
		pdest->pdata = pnew;

		/* Assume we have two arrays to merge,
		 *   di (di + 1)
		 *    \   /
		 *    56|78  : Destination.
		 * 234  7  9 : Source.
		 *     /
		 *    si; sj == 1;
		 * Before merging, the destination array had 4 items and the source had 5 items.
		 * This procedure uses a variable 'di' as a separator and two variables 'si' and 'sj' as collectors.
		 * When merging, the separator scan through the destination to find a place to hold a collection that
		 * numbers in this collection of source array can be inserted into destination.
		 */

		while
		(
			(r0 = cbfcmp(_P_ARRAY_Z_ITEM(psrc, sj), pdest->pdata)),
			(binc ? r0 <= 0 : r0 >= 0)
		)
		{
			if (sj < psrc->num)
				++sj;
			else
				break;
		}
		if (sj)
		{
			memmove(_P_ARRAY_Z_ITEM(pdest, sj), pdest->pdata, size * pdest->num);
			memmove(pdest->pdata, psrc->pdata, size * sj);
			pdest->num += sj;
			t = pdest->num - 1;
			si = sj < t ? sj : t;
			di += sj;
			sj = 0;
		}
		if (si < psrc->num - 1)
		{
			for ( ;; )
			{
				t = si + sj;
				r0 = cbfcmp(_P_ARRAY_Z_ITEM(psrc, t), _P_ARRAY_Z_ITEM(pdest, di + 1));
				r1 = cbfcmp(_P_ARRAY_Z_ITEM(psrc, t), _P_ARRAY_Z_ITEM(pdest, di));
				if (binc ? r0 <= 0 && r1 >= 0 : r0 >= 0 && r1 <= 0)
					++sj;
				else
				{
					if (sj)
					{
						t = di + 1;
						memmove(_P_ARRAY_Z_ITEM(pdest, t + sj), _P_ARRAY_Z_ITEM(pdest, t), (pdest->num - t) * size);
						memmove(_P_ARRAY_Z_ITEM(pdest, t), _P_ARRAY_Z_ITEM(psrc, si), sj * size);
						pdest->num += sj;
						si += sj;
						di += sj;
						sj =  0;
					}
					if (di + 1 <= pdest->num - 1)
						++di;
					else
						break;
				}
			}
			if
			(
				(r0 = cbfcmp(_P_ARRAY_Z_ITEM(pdest, di), _P_ARRAY_Z_ITEM(psrc, si))),
				(binc ? r0 <= 0 : r0 >= 0)
			)
			{
				t = psrc->num - si;
				memmove(_P_ARRAY_Z_ITEM(pdest, di + 1), _P_ARRAY_Z_ITEM(psrc, si), t * size);
				pdest->num += t;
			}
		}
	}
	return pdest->pdata;
#undef _P_ARRAY_Z_ITEM
}

/* Function name: strBinarySearchArrayZ_O
 * Description:   Search array using the binary search algorithm.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       pkey Pointer to the object that serves as key for searching.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition of CBF_COMPARE in svdef.h.
 * Return value:  A pointer to an entry in the array that matches the search key.
 *                Please refer to the return value of bsearch function in stdlib.h.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strBinarySearchArrayZ_M is available.
 */
void * strBinarySearchArrayZ_O(P_ARRAY_Z parrz, const void * pkey, size_t size, CBF_COMPARE cbfcmp)
{
	return svBinarySearch(pkey, parrz->pdata, strLevelArrayZ(parrz), size, cbfcmp);
}

/* Function name: strReverseArrayZ
 * Description:   Reverse a fixed-size array.
 * Parameters:
 *      parrz Pointer to a fixed-size array.
 *      ptemp Pointer to a buffer whose size equals to each size of the element in the array.
 *       size Size of each element in the array.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp shall equal to parameter size.
 */
void strReverseArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size)
{
	if (strLevelArrayZ(parrz) > 1 && size > 0)
	{
		REGISTER PUCHAR phead = parrz->pdata;
		REGISTER PUCHAR ptail = parrz->pdata + (strLevelArrayZ(parrz) - 1) * size;
		while (phead < ptail)
		{	/* Swap two elements. */
			svSwap(phead, ptail, ptemp, size);
			/* Alter two pointers. */
			phead += size;
			ptail -= size;
		}
	}
}

/* Function name: strGetLimitationArrayZ
 * Description:   Get the maximum or minimum value in an array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      ptemp Pointer to a buffer whose size equals to each size of the element in the array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares any two elements in the array.
 *       bmax If bmax were not zero, function would return the maximum value of an array,
 *            otherwise, function would return the minimum value of an array.
 * Return value:  Pointer to the limit in array and cast the pointer to (void *).
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 * Tip:           After invoking, the buffer that ptemp pointed stored the limit value.
 *                To get the index of maximum value in fixed-size array arr, use following sentences:
 *                size_t index; PUCHAR ptr = strGetLimitationArrayZ(&arr, ptemp, size, cbfcmp, true);
 *                if (NULL != ptr) index = (ptr == parrz->pdata ? 0 : (ptr - arr.pdata) / size - 1);
 *
 */
void * strGetLimitationArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, bool bmax)
{
	REGISTER void * prtn = NULL;
	if (strLevelArrayZ(parrz) > 0)
	{
		REGISTER size_t i;
		REGISTER PUCHAR p;
		REGISTER int r;
		/* If there were only one element in array, the limit would be the first one. */
		prtn = parrz->pdata;
		/* Copy the first element in array to the temporary buffer. */
		memcpy(ptemp, parrz->pdata, size);
		for (i = 0, p = parrz->pdata; i < strLevelArrayZ(parrz); ++i, p += size)
		{
			r = cbfcmp(p, ptemp);
			if (bmax ? r > 0 : r < 0)
				memcpy(ptemp, prtn = p, size);
		}
	}
	return prtn;
}

/* Function name: strUniqueArrayZ
 * Description:   Delete repeated elements in an array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      ptemp Pointer to a buffer whose size equals to each size of the element in the array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares any two elements in array.
 *    bshrink Input true to shrink array, otherwise input false.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 */
void strUniqueArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, bool bshrink)
{
	REGISTER size_t i, j;
	REGISTER PUCHAR px, py;
	for (i = 0, px = parrz->pdata; i < strLevelArrayZ(parrz); ++i, px += size)
	{
		memcpy(ptemp, px, size);
		for (j = i + 1, py = parrz->pdata + j * size; j < strLevelArrayZ(parrz); ++j, py += size)
			if (0 == cbfcmp(ptemp, py))
				strRemoveItemArrayZ(parrz, size, j, bshrink), --j, py -= size;
	}
}

/* Function name: strPermuteArrayZ
 * Description:   Permute a fixed size array in dictionary order.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      ptemp Pointer to a buffer whose size equals to each size of the element in the array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares any two elements in array.
 *      bnext Input true to permute an array next; Input false to permute an array previously.
 * Return value:  true indicates permutation continued; false indicates permutation ended.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                (*) The size of the buffer of ptemp pointed shall equal to parameter size.
 *                (*) Each element in the array shall be unique.
 * Tip:           Users may call strUniqueArrayZ to generate a suitable array for permuting.
 *                This function references to two similar templates in STL of C Plus Plus.
 */
bool strPermuteArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, bool bnext)
{
	if (strLevelArrayZ(parrz) > 1 && size > 0) /* Worth permuting. */
	{	/* ptrl Always points the last element. */
		REGISTER PUCHAR ptrl = parrz->pdata + (strLevelArrayZ(parrz) - 1) * size;
		REGISTER PUCHAR ptri, ptrj;
		for (ptri = ptrl - size, ptrj = ptrl;; ptri -= size, ptrj -= size)
		{
			REGISTER int r = cbfcmp(ptri, ptrj);
			if (bnext ? r < 0 : r > 0)
			{
				REGISTER PUCHAR ptrk;
				for
				(
					ptrk = ptrl;
					(r = cbfcmp(ptrk, ptri)),
					!(bnext ? r > 0 : r < 0);
					ptrk -= size
				);
				/* Swap (*i) and (*k). */
				svSwap(ptri, ptrk, ptemp, size);
				{	/* Reverse array from j to last. */
					ARRAY_Z arrt; /* Auxiliary array header for reversing. */
					arrt.num   = (size_t)((ptrl - ptrj) / size + 1);
					arrt.pdata = ptrj;
					strReverseArrayZ(&arrt, ptemp, size);
				}
				return true;
			}
			if (ptri <= parrz->pdata)
			{	/* Reverse array from first to last. */
				strReverseArrayZ(parrz, ptemp, size);
				goto Lbl_End_Permuting;
			}
		}
	}
Lbl_End_Permuting:
	return false;
}

/* Function name: strCombineNextArrayZ
 * Description:   Generate the next combination of an array in dictionary order.
 *                If n equaled (parrzn->num) and r equaled (parrzr->num), this function would generate
 *                the subset r of parrzn aka (n C r) aka C(n, r) and finally copy the result into parrzr.
 * Parameters:
 *     parrzr Pointer to an initialized array that contains a result of a previous combination.
 *     parrzn Pointer to a sized array that is sorted in increasing order.
 *       size Size of each element in both two arrays.
 *     cbfcmp Pointer to a function that compares any two elements in two arrays.
 * Return value:  true indicates combination continued; false indicates combination ended.
 * Caution:       Address of Both parrzn and parrzr Must Be Allocated first.
 *                (*) Each element in parrzn shall be unique.
 *                (*) Elements in array that parrzn and parrzr pointed shall be sorted in increasing order.
 * Tip:           Users may call strUniqueArrayZ(parrzn, ptemp, size, cbfcmp, true);
 *                to generate a suitable array for combination.
 */
bool strCombineNextArrayZ(P_ARRAY_Z parrzr, P_ARRAY_Z parrzn, size_t size, CBF_COMPARE cbfcmp)
{	/* Assume that the array that parrzn contains has been assigned and sorted yet. */
	if (parrzr->num > 0 && parrzr->num < parrzn->num)
	{
		REGISTER size_t i, j = parrzr->num - 1;
		REGISTER PUCHAR pa = &parrzr->pdata[size * j];
		REGISTER PUCHAR pt = &parrzn->pdata[size * (parrzn->num - 1)];
		/* Compare back through parrzn with parrzr to find a position as pa. */
		for (i = 0; i < j; ++i, pt -= size, pa -= size)
			if (0 != cbfcmp(pt, pa))
				break;
		if (0 == cbfcmp(pt, pa))
			goto Lbl_End_Combination; /* Combination reaches at the end. */
		if (NULL == (pt = (PUCHAR) svBinarySearch(pa, parrzn->pdata, parrzn->num, size, cbfcmp)))
			goto Lbl_End_Combination; /* An element in parrzr doesn't match any element in parrzn. */
		/* Fill subset r with values in parrzn. */
		pt += size;
		i = parrzr->num - (pa - parrzr->pdata + size) / size;
		do
		{
			memcpy(pa, pt, size);
			pa += size;
			pt += size;
		}
		while (0 != i--);
		return true;
	}
Lbl_End_Combination:
	return false; /* No next combination. */
}

/* Function name: strShuffleArrayZ
 * Description:   Random shuffle elements in a fixed size array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      ptemp Pointer to a buffer whose size equals to each size of the element in the array.
 *       size Size of each element in the array.
 *       seed A seed for random number generation.
 *            Function strShuffleArrayZ calls srand from C Standard Library to initialize the random seed.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 * Tips:          This function uses Fisher-Yates algorithm.
 */
void strShuffleArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, unsigned int seed)
{
	REGISTER size_t i, j;
	srand(seed);
	switch (strLevelArrayZ(parrz))
	{
	case 0: case 1: return; /* Why should I shuffle nothing or one thing? */
	default:
		for (i = strLevelArrayZ(parrz) - 1; i >= 1; --i)
		{
			j = rand() * rand() % (i + 1);
			svSwap(parrz->pdata + size * i, parrz->pdata + size * j, ptemp, size);
		}
	}
}

/* Function name: strKMPSearchArrayZ
 * Description:   Match pattern in text by using Knuth-Morris-Pratt algorithm.
 * Parameters:
 *    parrtxt Pointer to the sized array you want to search.
 *    parrptn Pointer to the sized array which contains the pattern.
 *       size Size of each element in both parrtxt and parrptn.
 *     cbftvs Callback function. Every time cbftvs is called, pitem points to the occurrence of pattern in text.
 *      param Parameter that is used to transfer into callback function.
 * Return value:  The same value as callback function cbftvs returned.
 *                (*) If function returned CBF_TERMINATE,
 *                    it would either indicate allocation failure or callback function cbftvs returned.
 * Caution:       Address of parrtxt and parrptn Must Be Allocated first.
 *                Parameter size shall not equal to zero.
 * Tips:          Users may reference to the following codes:
 *                // int cbftvs(void * pitem, size_t param) {
 *                //     DWC4100(param);
 *                //     wprintf(L"%ls\n", pitem);
 *                //     return CBF_CONTINUE;
 *                // }
 *                // wchar_t txt[] = L"ABABDABACDABABCABAB";
 *                // wchar_t ptn[] = L"ABABCABAB";
 *                // ARRAY_Z parrtxt, parrptn;
 *                // parrtxt.pdata = (PUCHAR)txt;
 *                // parrtxt.num   = wcslen(txt);
 *                // parrptn.pdata = (PUCHAR)ptn;
 *                // parrptn.num   = wcslen(ptn);
 *                // strKMPSearchArrayZ(&parrtxt, &parrptn, sizeof(wchar_t), cbftvs, 0);
 */
int strKMPSearchArrayZ(P_ARRAY_Z parrtxt, P_ARRAY_Z parrptn, size_t size, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER size_t i = 0; /* Index for parrtxt. */
	REGISTER size_t j = 0; /* Index for parrptn. */
	size_t * lps = (size_t *)malloc(strLevelArrayZ(parrptn) * sizeof(size_t));
	
	if (NULL == lps)
		return CBF_TERMINATE;

	/* Process the pattern to generate the longest prefix suffix (lps) array. */
	{
		REGISTER size_t len = 0;
		REGISTER size_t k   = 1;
		
		lps[0] = 0; /* lps[0] is always 0. */
		
		while (k < parrptn->num)
		{
			if (memcmp(strLocateItemArrayZ(parrptn, size, k), strLocateItemArrayZ(parrptn, size, len), size) == 0)
			{
				lps[k] = ++len;
				++k;
			}
			else
			{
				if (0 != len)
				{
					len = lps[len - 1];
				}
				else
				{
					lps[k] = 0;
					++k;
				}
			}
		}
	}

	while (i < parrtxt->num)
	{
		if (memcmp(strLocateItemArrayZ(parrptn, size, j), strLocateItemArrayZ(parrtxt, size, i), size) == 0)
		{
			++j;
			++i;
		}
		
		if (j == parrptn->num)
		{
			if (CBF_CONTINUE != cbftvs(parrtxt->pdata + (i - j) * size, param))
			{
				free(lps);
				return CBF_TERMINATE;
			}
			
			lps[j - 1] = j;
		}
		
		else if (i < parrtxt->num && (memcmp(strLocateItemArrayZ(parrptn, size, j), strLocateItemArrayZ(parrtxt, size, i), size) != 0))
		{
			if (0 != j)
				j = lps[j - 1];
			else
				++i;
		}
	}
	
	free(lps);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strGetZArray
 * Description:   This function is used to fill Z array for given parrz.
 * Parameters:
 *          z Pointer to a size_t array named Z.
 *      paraz Pointer to a sized array that contains the pattern and text.
 *       size Size of each element in parrz.
 * Return value:  N/A.
 */
void _strGetZArray(size_t z[], P_ARRAY_Z parrz, size_t size)
{
	REGISTER size_t n = strLevelArrayZ(parrz);
	REGISTER size_t i, l, r, k;

	/* [l,r] Make a window which matches with prefix of parrz. */
	l = r = 0;
	for (i = 1; i < n; ++i)
	{
		/* If i > r, nothing matches so we will calculate.
		 * z[i] using naive way.
		 */
		if (i > r)
		{
			l = r = i;
			/* r - l == 0 in starting, so it will start checking from 0'th index.
			 * For example, for "ababab" and i == 1, the value of r remains 0 and z[i] becomes 0.
			 * For string "aaaaaa" and i = 1, z[i] and r become 5.
			 */
			while (r < n && 0 == memcmp(strLocateItemArrayZ(parrz, size, r - l), strLocateItemArrayZ(parrz, size, r), size))
				++r;
			z[i] = r - l;
			--r;
		}
		else
		{
			/* k = i - l, so k corresponds to number which matches in [l,r] interval. */
			k = i - l;
			/* If z[k] is less than remaining interval then z[i] will be equal to z[k].
			 * For example, str == "ababab", i == 3, r == 5 and l == 2.
			 */
			if (z[k] < r - i + 1)
				z[i] = z[k]; /* For example str == "aaaaaa" and i == 2, r is 5, l is 0. */
			else
			{
				/* Start from r and check manually. */
				l = i;
				while (r < n && 0 == memcmp(strLocateItemArrayZ(parrz, size, r - l), strLocateItemArrayZ(parrz, size, r), size))
					++r;
				z[i] = r - l;
				--r;
			}
		}
	}
}

/* Function name: strCreateZSearchArrayZ
 * Description:   Match pattern in text by using Z algorithm.
 * Parameters:
 *    parrtxt Pointer to the sized array you want to search.
 *    parrptn Pointer to the sized array which contains the pattern.
 *       size Size of each element in both parrtxt and parrptn.
 *     cbftvs Callback function. Every time cbftvs is called, pitem points to the occurrence of pattern in text.
 *      param Parameter that is used to transfer into callback function.
 * Return value:  The same value as callback function cbftvs returned.
 *                (*) If function returned CBF_TERMINATE,
 *                    it would either indicate allocation failure or callback function cbftvs returned.
 * Caution:       Address of parrtxt and parrptn Must Be Allocated first.
 *                Parameter size shall not equal to zero.
 * Tips:          Z algorithm references to geeksforgeeks.org.
 *                Users may reference to the following codes:
 *                // int cbftvs(void * pitem, size_t param) {
 *                //     size_t i;
 *                //     for (i = 0; i < param; ++i)
 *                //         putchar(i[(char *)pitem]);
 *                //     printf("\n");
 *                //     return CBF_CONTINUE;
 *                // }
 *                // P_ARRAY_Z pp = strCreateCharacterStringArrayZ("GEEK");
 *                // P_ARRAY_Z pt = strCreateCharacterStringArrayZ("GEEKS FOR GEEKS");
 *                // strZSearchArrayZ(pt, pp, sizeof(char), cbftvs, 0);
 *                // strDeleteArrayZ(pp); // Do not forget to delete array here.
 *                // strDeleteArrayZ(pt);
 */
int strZSearchArrayZ(P_ARRAY_Z parrtxt, P_ARRAY_Z parrptn, size_t size, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER size_t i, l = strLevelArrayZ(parrtxt) + 1 + strLevelArrayZ(parrptn);
	size_t * z = (size_t *) calloc(l, sizeof(size_t));
	ARRAY_Z arrstr = { 0 };
	int r = CBF_TERMINATE;

	if (NULL != z)
	{
		if (NULL != strInitArrayZ(&arrstr, l, size))
		{
			/* Create concatenated string "P\0T". */
			memcpy(arrstr.pdata, parrptn->pdata, size * strLevelArrayZ(parrptn));
			memset(arrstr.pdata + size * strLevelArrayZ(parrptn), 0, size);
			memcpy(arrstr.pdata + size * (strLevelArrayZ(parrptn) + 1), parrtxt->pdata, size * strLevelArrayZ(parrtxt));
			/* Use Z algorithm to find string matching. */
			_strGetZArray(z, &arrstr, size);
			strFreeArrayZ(&arrstr);

			/* Looping through Z array for matching condition. */
			for (i = 0; i < l; ++i)
			{
				/* If z[i](matched region) is equal to pattern length then we got the pattern. */
				if (strLevelArrayZ(parrptn) == z[i])
				{
					if (CBF_CONTINUE != cbftvs(parrtxt->pdata + (i - strLevelArrayZ(parrptn) - 1) * size, param))
					{
						free(z);
						return CBF_TERMINATE;
					}
				}
			}
			r = CBF_CONTINUE;
		}
		free(z);
	}
	return r;
}

