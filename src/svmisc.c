/*
 * Name:        svmisc.c
 * Description: Miscellaneous data structures.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948D0907241900L00640
 * License:     LGPLv3
 * Copyright (C) 2017-2024 John Cage
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

#include <stdio.h>  /* Using macro BUFSIZ, function printf. */
#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function memcpy, memset. */
#include <limits.h> /* Using macro CHAR_BIT, UCHAR_MAX. */
#include "svstring.h"

/* Function name: svPrintVersion
 * Description:   Print library version.
 * Parameter:     N/A.
 * Return value:  N/A.
 */
void svPrintVersion(void)
{
	printf("StoneValley %s", SV_LIB_VER);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: stdiv
 * Description:   Compute a quotient that truncates toward 0 and the corresponding remainder.
 * Parameters:
 *  numerator Numerator.
 *denominator Denominator.
 * Return value:  A stdiv_t structure which contains the result.
 */
stdiv_t stdiv(size_t numerator, size_t denominator)
{
	stdiv_t result;
	/* May your compiler optimize division of these two following codes into one instruction. */
	result.quot = numerator / denominator;
	result.rem  = numerator % denominator;
	return result;
}

/* Function name: svSwapMemory
 * Description:   Swap contents of two memory blocks.
 * Parameters:
 *      pleft Pointer to the left block you want to swap with the block of pright pointed.
 *     pright Pointer to the right block you want to swap with the block of pleft pointed.
 *      ptemp Pointer to a temporary buffer of which has a size that equals to parameter size.
 *            (*) Users shall manage this temporary buffer manually in the caller function.
 *       size Size of memory that pleft, pright and ptemp pointed.
 * Return value:  N/A.
 * Caution:       Length of each memory that pleft, pright and ptemp pointed shall in the same size.
 *                (*) Memory blocks that pleft, pright and ptemp pointed shall not overlap.
 */
void svSwap(void * pleft, void * pright, void * ptemp, size_t size)
{	/* Swap two elements. */
	memcpy(ptemp,  pleft, size);
	memcpy(pleft, pright, size);
	memcpy(pright, ptemp, size);
}

/* Functions for bit-streams. */

/* Function name: strInitBitStream
 * Description:   Initialize a bit-stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate with.
 * Return value:  Pointer to the buffer of bit-stream.
 *                NULL indicates that initialization failed.
 * Caution:       Address of pbstm Must Be Allocated first.
 */
void * strInitBitStream(P_BITSTREAM pbstm)
{
	strInitArrayZ(&pbstm->arrz, 1, sizeof(UCHART));
	pbstm->bilc = 0;
	return pbstm->arrz.pdata;
}

/* Function name: strFreeBitStream
 * Description:   Deallocate the buffer of bit-stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  N/A.
 * Caution:       Address of pbstm Must Be Allocated first.
 */
void strFreeBitStream(P_BITSTREAM pbstm)
{
	strFreeArrayZ(&pbstm->arrz);
	pbstm->bilc = 0;
}

/* Function name: strCreateBitStream
 * Description:   Create a bit-stream.
 * Parameter:     N/A.
 * Return value:  Pointer to the new created bit-stream.
 *                NULL indicates that bit-stream creation failed.
 */
P_BITSTREAM strCreateBitStream(void)
{
	P_BITSTREAM pbstm = (P_BITSTREAM) malloc(sizeof(BITSTREAM));
	if (NULL != pbstm)
	{
		if (NULL == strInitBitStream(pbstm))
		{	/* Allocation failure. */
			free(pbstm);
			pbstm = NULL;
		}
	}
	return pbstm;
}

/* Function name: strDeleteBitStream
 * Description:   Deallocate a bit-stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  N/A.
 * Caution:       Address of pbstm Must Be Allocated by function strCreateBitStream first.
 */
void strDeleteBitStream(P_BITSTREAM pbstm)
{
	strFreeBitStream(pbstm);
	free(pbstm);
}

/* Function name: strCopyBitStream
 * Description:   Copy a bit-stream from source to destination.
 * Parameters:
 *      pdest Pointer to the destination stream whose content is to be copied.
 *       psrc Pointer to the source of stream to be copied.
 * Return value:  pdest->arrz.pdata
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       After calling, the size of pdest->arrz equaled to the size of psrc->arrz.
 *                Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 */
void * strCopyBitStream(P_BITSTREAM pdest, P_BITSTREAM psrc)
{
	if (strResizeArrayZ(&pdest->arrz, strLevelArrayZ(&psrc->arrz), sizeof(UCHART)))
	{
		pdest->bilc = (NULL == strCopyArrayZ(&pdest->arrz, &psrc->arrz, sizeof(UCHART)) ? 0 : psrc->bilc);
		return pdest->arrz.pdata;
	}
	pdest->bilc = 0;
	return NULL;
}

/* Function name: strBitStreamIsEmpty_O
 * Description:   Test a bit-stream. Check Whether the bit-stream is empty or not.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  TRUE indicates that the bit-stream is empty.
 *                FALSE indicates that the bit-stream is NOT empty.
 * Caution:       Address of pbstm Must Be Allocated first.
 * Tip:           A macro version of this function named strBitStreamIsEmpty_M is available.
 */
BOOL strBitStreamIsEmpty_O(P_BITSTREAM pbstm)
{
	return (strLevelArrayZ(&pbstm->arrz) <= 1 && 0 == pbstm->bilc);
}

/* Function name: strBitStreamPush
 * Description:   Push a bit onto the start of the stream.
 * Parameters:
 *      pbstm Pointer to the bit-stream you want to operate on.
 *      value A bit value you are about to push into.
 * Return value:  TRUE indicates that no error has been occurred.
 *                FALSE indicates that error occurred while pushing bit onto the stream.
 * Caution:       Address of pbstm Must Be Allocated first.
 */
BOOL strBitStreamPush(P_BITSTREAM pbstm, BOOL value)
{
	REGISTER size_t i;
	if (++pbstm->bilc > CHAR_BIT)
	{
		if (NULL == strResizeArrayZ(&pbstm->arrz, strLevelArrayZ(&pbstm->arrz) + 1, sizeof(UCHART)))
		{
			--pbstm->bilc; /* Decrease bit number. */
			return FALSE; /* Allocation failure. */
		}
		pbstm->bilc = 1;
	}
	for (i = strLevelArrayZ(&pbstm->arrz) - 1; i >= 1; --i)
	{
		/* Right shift 1. */
		pbstm->arrz.pdata[i] >>= 1;
		/* Promote LSBit in the previous block. */
		if (0x01 & pbstm->arrz.pdata[i - 1])
			pbstm->arrz.pdata[i] |= ((UCHART) 0x01 << (CHAR_BIT - 1));
	}
	*pbstm->arrz.pdata >>= 1;
	if (FALSE != value) /* Alter MSBit in the first block. */
		*pbstm->arrz.pdata |= ((UCHART) 0x01 << (CHAR_BIT - 1));
	return TRUE;
}

/* Function name: strBitStreamPop
 * Description:   Pop a bit from the start of the stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  Popped bit. Converted to BOOL. Values TRUE 1 or FALSE 0.
 * Caution:       Use this function on a bit stream which is not empty.
 *                Address of pbstm Must Be Allocated first.
 * Tip:           Use function strBitStreamIsEmpty first
 *                To check whether bit-stream is empty.
 */
BOOL strBitStreamPop(P_BITSTREAM pbstm)
{
	BOOL r = (0 != (((UCHART) 0x01 << (CHAR_BIT - 1)) & *pbstm->arrz.pdata));
	REGISTER size_t i, j = strLevelArrayZ(&pbstm->arrz) - 1;
	for (i = 0; i < j; ++i)
	{
		/* Left shift 1. */
		pbstm->arrz.pdata[i] <<= 1;
		/* Descend MSBit in the next block onto the current block. */
		if (((UCHART) 0x01 << (CHAR_BIT - 1)) & pbstm->arrz.pdata[i + 1])
			++pbstm->arrz.pdata[i];
	}
	pbstm->arrz.pdata[j] <<= 1;
	/* Decrease stream length. */
	if (0 == --pbstm->bilc)
	{
		if (strLevelArrayZ(&pbstm->arrz) > 1)
		{
			strResizeArrayZ(&pbstm->arrz, strLevelArrayZ(&pbstm->arrz) - 1, sizeof(UCHART));
			pbstm->bilc = CHAR_BIT;
		}
	}
	return FALSE != r;
}

/* Function name: strBitStreamAdd
 * Description:   Add a bit to the end of the stream.
 * Parameters:
 *      pbstm Pointer to the bit-stream you want to operate on.
 *      value A bit value you are about to add.
 * Return value:  TRUE indicates that no error has been occurred.
 *                FALSE indicates that error occurred while adding bit to the stream.
 * Caution:       Address of pbstm Must Be Allocated first.
 */
BOOL strBitStreamAdd(P_BITSTREAM pbstm, BOOL value)
{
	size_t j;
	PUCHAR pt;
	if (++pbstm->bilc > CHAR_BIT)
	{	/* Need to reallocate. */
		if (NULL == strResizeArrayZ(&pbstm->arrz, strLevelArrayZ(&pbstm->arrz) + 1, sizeof(UCHART)))
		{
			--pbstm->bilc; /* Decrease bit number. */
			return FALSE; /* Allocation failure. */
		}
		pbstm->bilc = 1; /* Reset bits in the last block of UCHART integer. */
	}
	j = CHAR_BIT - pbstm->bilc;
	pt = &(pbstm->arrz.pdata[strLevelArrayZ(&pbstm->arrz) - 1]);
	*pt = (UCHART)
	(
		FALSE != value ?
		(*pt | ((UCHART)  0x01  << j)) : /* Pad 1. */
		(*pt & ((UCHAR_MAX - 1) << j))   /* Pad 0. */
	);
	return TRUE;
}

/* Function name: strBitStreamExtract
 * Description:   Extract a bit from the end of the stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  Extracted bit. Converted to BOOL. Values TRUE 1 or FALSE 0.
 * Caution:       Use this function on a bit stream that is not empty.
 *                Address of pbstm Must Be Allocated first.
 * Tip:           Use function strBitStreamIsEmpty firstly
 *                to check whether the bit-stream is empty.
 */
BOOL strBitStreamExtract(P_BITSTREAM pbstm)
{
	UCHART r = (UCHART)(pbstm->arrz.pdata[strLevelArrayZ(&pbstm->arrz) - 1] & ((UCHART) 0x01 << (UCHART)(CHAR_BIT - pbstm->bilc)));
	if (--pbstm->bilc < 1)
	{	/* Need to reallocate. */
		if (strLevelArrayZ(&pbstm->arrz) > 1)
		{
			if (NULL != strResizeArrayZ(&pbstm->arrz, strLevelArrayZ(&pbstm->arrz) - 1, sizeof(UCHART)))
				pbstm->bilc = CHAR_BIT;
		}
	}
	return FALSE != r;
}

/* Function name: strBitStreamLocate
 * Description:   Locate a bit in the stream by index.
 * Parameters:
 *      pbstm Pointer to the bit-stream you want to operate on.
 *      index Index value to be evaluated. Starts from 0.
 * Return value:  TRUE or FALSE.
 * Caution:       Address of pbstm Must Be Allocated first.
 *                If index exceeded the range, function would return FALSE.
 */
BOOL strBitStreamLocate(P_BITSTREAM pbstm, size_t index)
{
	if (index < (pbstm->arrz.num - 1) * CHAR_BIT + pbstm->bilc)
	{
		REGISTER stdiv_t st = stdiv(index, CHAR_BIT);
		return FALSE != (pbstm->arrz.pdata[st.quot] & ((UCHART)0x01 << (CHAR_BIT - st.rem - 1)));
	}
	return FALSE;
}

/* Function name: strBitStreamReverse
 * Description:   Reverse bits in bit-stream.
 * Parameter:
 *     pbstm Pointer to the bit-stream you want to operate on.
 * Return value:  N/A.
 * Caution:       Used only if the bit stream is not empty.
 *                Address of pbstm Must Be Allocated first.
 * Tip:           Use function strBitStreamIsEmpty first
 *                to check whether bit-stream is empty.
 *                Use this function to calculate bitwise NOT pbstm.
 */
void strBitStreamReverse(P_BITSTREAM pbstm)
{
	REGISTER size_t i, j = (0 == pbstm->bilc ? strLevelArrayZ(&pbstm->arrz) - 1 : strLevelArrayZ(&pbstm->arrz));
	for (i = 0; i < j; ++i) pbstm->arrz.pdata[i] = (UCHART)(~pbstm->arrz.pdata[i]);
}

/* Due to some reasons that this library could
 * be used in a variety number of environments,
 * we need to implement our own Quick Sort and Binary Search algorithm.
 * These following functions originated from two books:
 * The Standard C Library and Data Structures and Algorithm Analysis in C.
 * (With ISBNs of "ISBN 978-0-201-49840-0" and "ISBN 978-0-131-31509-9")
 * Authors of these two books are P.J. Plauger and Mark Allen Weiss.
 * And of course thanks for their ideas and codes! :)
 */

/* Function name: svShellSort
 * Description:   Shell sort algorithm.
 * Parameters:
 *      pbase Pointer to the first object of the array to be sorted.
 *      ptemp Pointer to a temporary buffer whose size equals to each size of the element in the array that pbase pointed.
 *            Users shall manage this buffer manually while coping with its caller.
 *        num Number of elements in the array.
 *       size Size in bytes of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  N/A.
 */
void svShellSort(void * pbase, void * ptemp, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER size_t g, gap;
	/* Using Marcin Ciura's gap sequence. */
	size_t gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
	for (g = 0; g < sizeof(gaps) / sizeof(gaps[0]); ++g)
	{
		REGISTER size_t i, j, k;
		gap = gaps[g];
		for (i = gap; i < num; ++i)
		{
			memcpy(ptemp, (PUCHAR)pbase + i * size, size);
			for (j = i; (k = (j - gap) * size, j >= gap) && cbfcmp((PUCHAR)pbase + k, ptemp) > 0; j -= gap)
				memcpy((PUCHAR)pbase + j * size, (PUCHAR)pbase + k, size);
			memcpy((PUCHAR)pbase + j * size, ptemp, size);
		}
	}
}

#define _CUTOFF (6) /* This macro is used by function svQuickSort. */

/* Function name: svQuickSort
 * Description:   Quick sort algorithm.
 * Parameters:
 *      pbase Pointer to the first object of the array to be sorted.
 *        num Number of elements in the array.
 *       size Size in bytes of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  If sorting succeeded, this function would return a same pointer as pbase pointed,
 *                otherwise, this function would return value NULL.
 * Tip:           Alter macro strSortArrayZ_M at file svstring.h,
 *                and replace qsort with svQuickSort to make this function in charge.
 */
void * svQuickSort(void * pbase, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	PUCHAR ptemp = NULL;
	UCHART tmpbuf[BUFSIZ];
	ptemp = size <= BUFSIZ ? tmpbuf : (PUCHAR) malloc(size);
	if (NULL != ptemp)
	{
		struct { size_t l; size_t r; } rawstk[CHAR_BIT * sizeof(size_t)], * pstk = rawstk + 1;
		REGISTER size_t l, r;
		REGISTER PUCHAR pa;
		REGISTER size_t i;
		/* Initialize parameter stack. */
		rawstk[0].l = 0;
		rawstk[0].r = num - 1;
	Lbl_Recurse:
		/* Pop parameters from stack. */
		--pstk;
		l = pstk->l;
		r = pstk->r;
		/* Assign new index value. */
		i = r - l + 1;
		pa = (PUCHAR) pbase + size * l;
		if (l + _CUTOFF > r)
			svShellSort(pa, ptemp, i, size, cbfcmp);
		else
		{
			REGISTER PUCHAR pb = (PUCHAR) pbase + size * ((l + r) >> 1);
			REGISTER PUCHAR pc = (PUCHAR) pbase + size * r;
			REGISTER PUCHAR pd = pc - size;
			if (cbfcmp(pa, pb) > 0)
				svSwap(pa, pb, ptemp, size);
			if (cbfcmp(pa, pc) > 0)
				svSwap(pa, pc, ptemp, size);
			if (cbfcmp(pb, pc) > 0)
				svSwap(pb, pc, ptemp, size);
			svSwap(pb, pd, ptemp, size);
			for (i = l, pb = pd;; )
			{
				while (cbfcmp((++i, pa += size), pd) < 0);
				while (cbfcmp(pb -= size, pd) > 0);
				if (pa < pb)
					svSwap(pa, pb, ptemp, size);
				else
					break;
			}
			svSwap(pa, pd, ptemp, size);
			/* Push parameters onto stack. */
			pstk->l = i + 1;
			pstk->r = r;
			++pstk;
			pstk->l = l;
			pstk->r = i - 1;
			++pstk;
		}
		if (rawstk != pstk)
			goto Lbl_Recurse;
		if (tmpbuf != ptemp)
			free(ptemp);
		return pbase;
	}
	return NULL; /* Allocation failure. */
}

#undef _CUTOFF

/* Function name: svMergeSort
 * Description:   Merge sort algorithm.
 * Parameters:
 *      pbase Pointer to the first object of the array to be sorted.
 *        num Number of elements in the array.
 *       size Size in bytes of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  If sorting succeeded, this function would return a same pointer as pbase pointed,
 *                otherwise, this function would return value NULL.
 * Tip:           Alter macro strSortArrayZ_M at file svstring.h,
 *                and replace qsort with svMergeSort to make this function in charge.
 */
void * svMergeSort(void * pbase, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER size_t i, j;
	REGISTER PUCHAR pl = (PUCHAR)pbase;
	REGISTER PUCHAR pr = (PUCHAR)malloc(num * size);

	if (NULL == pr)
		return NULL; /* Allocation failure. */

	for (i = 1; i < num; i <<= 1)
	{
		for (j = 0; j < num; j += (i << 1))
		{
			REGISTER size_t t, k, a, b, x, y;

			a = k = j;
			t = j + i;
			b = x = t < num ? t : num;
			t = j + (i << 1);
			y = t < num ? t : num;

			while (a < x && b < y)
				memcpy(pr + k++ * size, cbfcmp(pl + a * size, pl + b * size) < 0 ? pl + a++ * size : pl + b++ * size, size);

			while (a < x)
				memcpy(pr + k++ * size, pl + a++ * size, size);

			while (b < y)
				memcpy(pr + k++ * size, pl + b++ * size, size);
		}
		/* Swap pl and pr. */
		{
			REGISTER PUCHAR pt;
			pt = pl;
			pl = pr;
			pr = pt;
		}
	}
	if (pl != pbase)
	{
		memcpy(pbase, pl, num * size);
		pr = pl;
	}
	free(pr);
	return pbase;
}

/* Function name: svHeapSort
 * Description:   Heap sort algorithm.
 * Parameters:
 *      pbase Pointer to the first object of the array to be sorted.
 *        num Number of elements in the array.
 *       size Size in bytes of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  If sorting succeeded, this function would return a same pointer as pbase pointed,
 *                otherwise, this function would return value NULL.
 * Tip:           Alter macro strSortArrayZ_M at file svstring.h,
 *                and replace qsort with svMergeSort to make this function in charge.
 *                This algorithm takes O(1) space and O(n log n) time complexity.
 *                Notice that heap sort is not stable.
 */
void * svHeapSort(void * pbase, size_t num, size_t size, CBF_COMPARE cbfcmp)
{	
	REGISTER size_t i, k, n = num, m = n >> 1;
	REGISTER PUCHAR px = (PUCHAR)pbase, py, p;
	REGISTER size_t l, r;

	PUCHAR ptemp = NULL;
	UCHART tmpbuf[BUFSIZ];
	ptemp = size <= BUFSIZ ? tmpbuf : (PUCHAR) malloc(size);

	if (NULL == ptemp)
		return NULL; /* Allocation failure. */

	/* Heapify. */
	for (i = m; i > 0; --i)
	{
		k = i - 1;
		while (k < m)
		{
			l = (k << 1) + 1;
			r = l + 1;

			p = px + size * l;
			if (r < n && cbfcmp(px + size * r, p) > 0)
			{
				l = r;
				p = px + size * l;
			}

			py = px + size * k;
			if (cbfcmp(py, p) >= 0)
				break;

			svSwap(py, p, ptemp, size);
			k = l;
		}
	}

	for (i = n - 1; i > 0; --i)
	{
		svSwap(px, px + size * i, ptemp, size);
		m = i >> 1;
		k = 0;
		while (k < m)
		{
			l = (k << 1) + 1;
			r = l + 1;

			p = px + size * l;
			if (r < i && cbfcmp(px + size * r, p) > 0)
			{
				l = r;
				p = px + size * l;
			}

			py = px + size * k;
			if (cbfcmp(py, p) >= 0)
				break;

			svSwap(py, p, ptemp, size);
			k = l;
		}
	}

	if (tmpbuf != ptemp)
		free(ptemp);
	return pbase;
}

/* Function name: svBinarySearch
 * Description:   Binary searching algorithm.
 * Parameters:
 *       pkey Pointer to the object that serves as a key for searching.
 *      pbase Pointer to the first object of the array to be sorted.
 *        num Number of elements in an array that pointed by base.
 *       size Size in bytes of each element in the array.
 *     cbfcmp Pointer to a function that compares two elements.
 *            The left pointer of function cbfcmp points to the key.
 *            The right pointer of function cbfcmp may point to any position in the array.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  A pointer to an entry in the array that matches the searching key.
 */
void * svBinarySearch(const void * pkey, const void * pbase, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	if (NULL != pkey && size && NULL != pbase)
	{	/* Worth searching. */
		REGISTER PUCHAR p = (PUCHAR) pbase;
		REGISTER size_t n = num;
		while (n > 0)
		{
			REGISTER size_t pivot = n >> 1;
			REGISTER PUCHAR q = p + size * pivot;
			REGISTER int r = cbfcmp(pkey, q);
			if (r < 0)
				n = pivot; /* Search below pivot. */
			else if (0 == r)
				return (void *)q; /* Found. */
			else /* Search above pivot. */
			{
				p = q + size;
				n -= pivot + 1;
			}
		}
	}
	return NULL; /* No match. */
}
