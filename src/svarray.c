/*
 * Name:        svarray.c
 * Description: Sized array.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948B0721260727L00908
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
#include <stdlib.h> /* Using function malloc, calloc, realloc, free. */
#include <string.h> /* Using function memcpy, memmove, memcmp. */
#include "svstring.h"

/* File level macro definitions go here. */
/* Macro _P_ARRAY_Z_ITEM_M gives us a method to fetch the pointer of an item without checking index's bound.
 * It is the library programmer's duty to ensure index_M is in the range of an array.
 */
#define _P_ARRAY_Z_ITEM_M(parrz_M, size_M, index_M) ((parrz_M)->pdata + (size_M) * (index_M))

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
	/* While this is not an efficient implementation but a concise one to get string length. */
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
 * Parameter:
 *      pstr Pointer to the string you want to copy in to a new sized array.
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

/* Function name: strLevelArrayZ_O
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

/* Function name: strIndexOfArrayZ_O
 * Description:   Return the index of an object in a sized array by a given address.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      pitem Pointer to the address of the object in the sized array.
 *       size Size in bytes of each element in the array.
 * Return value:  The offset of the item in a sized array.
 * Caution:       If the object that pitem pointed were not in the array whose pointer was parrz, this function would return -1 as an index.
 * Tip:           A macro version of this function named strIndexOfArrayZ_M is available.
 */
ptrdiff_t strIndexOfArrayZ_O(P_ARRAY_Z parrz, const void * pitem, size_t size)
{
	return ((size_t)pitem >= (size_t)parrz->pdata && (size_t)pitem <= (size_t)&parrz->pdata[size * (strLevelArrayZ(parrz) - 1)]) ?
		svIndexOf(parrz->pdata, pitem, size) :
		-1;
}

/* Function name: strTraverseArrayZ
 * Description:   Traverse each element in a sized array.
 * Parameters:
 *      parrz Pointer to the array you want to traverse.
 *       size Element size of an array.
 *     cbftvs Pointer to a callback function.
 *      param Parameter which can be transferred into callback function.
 *       brev Input true to traverse array from the first item to the last item.
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

/* Function name: strCreateCopyArrayZ
 * Description:   Create a replica of a sized array.
 * Parameters:
 *       psrc Pointer to the source of array to be replicate.
 *       size Size of each element for the source array.
 * Return value:  A pointer to the new sized array which is exactly the replica of the source array.
 * Caution:       Size of each element in the replica has the same value as source array's.
 *                Item number of two arrays shall equal to each other.
 *                Address of psrc shall be Allocated first to ensure a valid buffer address.
 *                Destination and source will not overlap.
 */
P_ARRAY_Z strCreateCopyArrayZ(P_ARRAY_Z psrc, size_t size)
{
	REGISTER P_ARRAY_Z parrz = strCreateArrayZ(strLevelArrayZ(psrc), size);
	if (NULL != parrz)
		strCopyArrayZ(parrz, psrc, size);
	return parrz;
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
 *            Input false to search array in order. That means to search the element from index 0 to the last index of array.
 * Return value:  (*) Index of element + 1. If function returned 0, it should mean pitem could not be found.
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
			for (i = strLevelArrayZ(parrz), p = (PUCHAR)strLocateItemArrayZ(parrz, size, strLevelArrayZ(parrz) - 1); i >= 1; --i, p -= size)
				if (0 == memcmp(p, pitem, size))
					return i;
		}
		else
		{
			for (i = 0, p = parrz->pdata; i < strLevelArrayZ(parrz); ++i, p += size)
				if (0 == memcmp(p, pitem, size))
					return i + 1;
		}
	}
	return 0;
}

/* Function name: strInsertItemArrayZ
 * Description:   Insert an item into an array by a given index.
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
	return memcpy /* Give new item spaces and insert it into array. */
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
	if (index < strLevelArrayZ(parrz))
	{
		if (0 != --parrz->num)
			memmove
			(
				parrz->pdata + index * size,
				parrz->pdata + (index + 1) * size,
				(strLevelArrayZ(parrz) - index) * size
			);
		if (bshrink)
			strResizeArrayZ(parrz, strLevelArrayZ(parrz), size);
	}
}

/* Function name: strSortArrayZ
 * Description:   Sort an array as fast as possible.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 *    bstable Input [true]  to sort an array by using stable   sort algorithm.
 *            Input [false] to sort an array by using unstable sort algorithm.
 * Return value:  If sorting succeeded, this function would return a pointer to the first element to the array,
 *                otherwise, this function would return value NULL.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strSortArrayZ(P_ARRAY_Z parrz, size_t size, CBF_COMPARE cbfcmp, bool bstable)
{
	if (bstable)
		return svMergeSort(parrz->pdata, strLevelArrayZ(parrz), size, cbfcmp);
	else
		return svQuickSort(parrz->pdata, strLevelArrayZ(parrz), size, cbfcmp);
}

/* Function name: strMergeSortedArrayZ
 * Description:   Merge two sorted arrays.
 * Parameters:
 *      pdest Pointer to the destination array that you want to merge.
 *       psrc Pointer to the source array.
 *       size Size of each element in two arrays.
 *            (*) Each item in these two sorted arrays shall be in the same size.
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
	REGISTER size_t di = 0, si = 0, sj = 0, t;
	REGISTER int r0, r1;
	REGISTER bool binc =
	(
		cbfcmp(pdest->pdata, _P_ARRAY_Z_ITEM_M(pdest, size, strLevelArrayZ(pdest) - 1)) <= 0 &&
		cbfcmp(psrc->pdata,  _P_ARRAY_Z_ITEM_M(psrc,  size, strLevelArrayZ(psrc)  - 1)) <= 0
	) ?
	true : /* Both destination and source are in increasing order. */
	false; /* Both destination and source are in decreasing order. */
	
	t = strLevelArrayZ(pdest);
	if (NULL == strResizeBufferedArrayZ(pdest, size, +strLevelArrayZ(psrc)))
		return NULL;
	else
	{
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
		pdest->num = t;
		while
		(
			(r0 = cbfcmp(_P_ARRAY_Z_ITEM_M(psrc, size, sj), pdest->pdata)),
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
			memmove(_P_ARRAY_Z_ITEM_M(pdest, size, sj), pdest->pdata, size * pdest->num);
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
				r0 = cbfcmp(_P_ARRAY_Z_ITEM_M(psrc, size, t), _P_ARRAY_Z_ITEM_M(pdest, size, di + 1));
				r1 = cbfcmp(_P_ARRAY_Z_ITEM_M(psrc, size, t), _P_ARRAY_Z_ITEM_M(pdest, size, di));
				if (binc ? r0 <= 0 && r1 >= 0 : r0 >= 0 && r1 <= 0)
					++sj;
				else
				{
					if (sj)
					{
						t = di + 1;
						memmove(_P_ARRAY_Z_ITEM_M(pdest, size, t + sj), _P_ARRAY_Z_ITEM_M(pdest, size, t), (pdest->num - t) * size);
						memmove(_P_ARRAY_Z_ITEM_M(pdest, size, t), _P_ARRAY_Z_ITEM_M(psrc, size, si), sj * size);
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
				(r0 = cbfcmp(_P_ARRAY_Z_ITEM_M(pdest, size, di), _P_ARRAY_Z_ITEM_M(psrc, size, si))),
				(binc ? r0 <= 0 : r0 >= 0)
			)
			{
				t = psrc->num - si;
				memmove(_P_ARRAY_Z_ITEM_M(pdest, size, di + 1), _P_ARRAY_Z_ITEM_M(psrc, size, si), t * size);
				pdest->num += t;
			}
		}
	}
	return (void *)pdest->pdata;
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
 *                The size of the buffer of ptemp shall equal to each parameter size of the array.
 */
void strReverseArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size)
{
	if (strLevelArrayZ(parrz) > 1 && size > 0)
	{
		REGISTER PUCHAR phead = parrz->pdata;
		REGISTER PUCHAR ptail = parrz->pdata + (strLevelArrayZ(parrz) - 1) * size;
		while (phead < ptail)
		{	/* Swap two elements. */
			svSwap(phead, ptemp, ptail, size);
			/* Alter two pointers. */
			phead += size;
			ptail -= size;
		}
	}
}

/* Function name: strGetLimitationArrayZ
 * Description:   Get the first maximum or minimum value in an array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *     cbfcmp Pointer to a function that compares any two elements in the array.
 *       bmax If bmax were true, function would return the maximum value of an array,
 *            otherwise, function would return the minimum value of an array.
 *       brev If brev were true, function would search the limitation from array's tail to head,
 *            otherwise, function would search for the limitation from head to tail.
 * Return value:  Pointer to the limit in array and cast the pointer to (void *).
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 * Tip:           To get the index of maximum value in fixed-size array arr, use the following sentences:
 *                size_t index = 0; PUCHAR ptr = (PUCHAR)strGetLimitationArrayZ(&arr, size, cbfcmp, true);
 *                if (NULL != ptr) index = (ptr - arr.pdata) / size;
 *                Or simply use: i = strIndexOfArrayZ(&arr, ptr, size);
 */
void * strGetLimitationArrayZ(P_ARRAY_Z parrz, size_t size, CBF_COMPARE cbfcmp, bool bmax, bool brev)
{
	REGISTER void * prtn = NULL;
	if (strLevelArrayZ(parrz) > 0)
	{
		REGISTER size_t i;
		REGISTER PUCHAR p;
		REGISTER int r;
		
		if (brev)
		{
			prtn = p = (PUCHAR)strLocateItemArrayZ(parrz, size, strLevelArrayZ(parrz) - 1);
			if (strLevelArrayZ(parrz) > 1)
			{
				for (i = strLevelArrayZ(parrz) - 1; i >= 1; --i, p -= size)
				{
					r = cbfcmp(p, prtn);
					if (bmax ? r > 0 : r < 0)
						prtn = p;
				}
			}
		}
		else
		{
			prtn = parrz->pdata;
			for (i = 1, p = parrz->pdata + size; i < strLevelArrayZ(parrz); ++i, p += size)
			{
				r = cbfcmp(p, prtn);
				if (bmax ? r > 0 : r < 0)
					prtn = p;
			}
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
	REGISTER size_t i,  j;
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
	{	/* ptrl always points the last element. */
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
				svSwap(ptri, ptemp, ptrk, size);
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
 *     nxtrnd Pointer to a random number generator function to return a size_t random integer.
 *            Users shall implement this function by their own and manually control it's distribution.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 * Tip:           This function uses Fisher-Yates algorithm.
 */
void strShuffleArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, size_t (*nxtrnd)(void))
{
	if (strLevelArrayZ(parrz) >= 2) /* It is worth to shuffle the array. */
	{
		REGISTER size_t i, j;
		for (i = strLevelArrayZ(parrz) - 1; i >= 1; --i)
		{
			j = nxtrnd() % (i + 1);
			svSwap(parrz->pdata + size * i, ptemp, parrz->pdata + size * j, size);
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
 * Usage:         int cbftvs(void * pitem, size_t param) {
 *                    DWC4100(param);
 *                    wprintf(L"%ls\n", pitem);
 *                    return CBF_CONTINUE;
 *                }
 *                wchar_t txt[] = L"ABABDABACDABABCABAB";
 *                wchar_t ptn[] = L"ABABCABAB";
 *                ARRAY_Z parrtxt, parrptn;
 *                parrtxt.pdata = (PUCHAR)txt;
 *                parrtxt.num   = wcslen(txt);
 *                parrptn.pdata = (PUCHAR)ptn;
 *                parrptn.num   = wcslen(ptn);
 *                strKMPSearchArrayZ(&parrtxt, &parrptn, sizeof(wchar_t), cbftvs, 0);
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
			if (memcmp(_P_ARRAY_Z_ITEM_M(parrptn, size, k), _P_ARRAY_Z_ITEM_M(parrptn, size, len), size) == 0)
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
		if (0 == memcmp(_P_ARRAY_Z_ITEM_M(parrptn, size, j), _P_ARRAY_Z_ITEM_M(parrtxt, size, i), size))
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
		
		else if (i < parrtxt->num && 0 != memcmp(_P_ARRAY_Z_ITEM_M(parrptn, size, j), _P_ARRAY_Z_ITEM_M(parrtxt, size, i), size))
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
			while (r < n && 0 == memcmp(_P_ARRAY_Z_ITEM_M(parrz, size, r - l), _P_ARRAY_Z_ITEM_M(parrz, size, r), size))
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
				while (r < n && 0 == memcmp(_P_ARRAY_Z_ITEM_M(parrz, size, r - l), _P_ARRAY_Z_ITEM_M(parrz, size, r), size))
					++r;
				z[i] = r - l;
				--r;
			}
		}
	}
}

/* Function name: strZSearchArrayZ
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
 * Tip:           Z algorithm references to geeksforgeeks.org.
 * Usage:         int cbftvs(void * pitem, size_t param) {
 *                    size_t i;
 *                    for (i = 0; i < param; ++i)
 *                        putchar(i[(char *)pitem]);
 *                    printf("\n");
 *                    return CBF_CONTINUE;
 *                }
 *                P_ARRAY_Z pp = strCreateCharacterStringArrayZ("GEEK");
 *                P_ARRAY_Z pt = strCreateCharacterStringArrayZ("GEEKS FOR GEEKS");
 *                strZSearchArrayZ(pt, pp, sizeof(char), cbftvs, 0);
 *                strDeleteArrayZ(pp); // Do not forget to delete array here.
 *                strDeleteArrayZ(pt);
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

