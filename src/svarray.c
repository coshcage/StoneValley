/*
 * Name:        svarray.c
 * Description: Sized array.
 * Author:      cyycoish#hotmail.com
 * File ID:     0306170948B0720200120L00613
 *
 * The following text is copied from the source code of SQLite and padded
 * with a little bit addition to fit the goals for StoneValley project:
 *
 * The author disclaims copyright to this source code. In place of
 * a legal notice, here is a blessing:
 *   May you do good and not evil.
 *   May you find forgiveness for yourself and forgive others.
 *   May you share freely, never taking more than you give.
 *   Hope you never need to push yourself or other people too hard.
 */

#include <stdlib.h> /* Using function srand, rand. */
#include <string.h> /* Using function memcpy, memmove, memcmp. */
#include "svstring.h"

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
 *      param Parameter which can be transfer into callback function.
 *       brev Input TRUE to traverse array in order.
 *            Input FALSE to traverse array in reverse.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of parrz Must Be Allocated first.
 */
int strTraverseArrayZ(P_ARRAY_Z parrz, size_t size, CBF_TRAVERSE cbftvs, size_t param, BOOL brev)
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
 *       brev Input TRUE  to search array in reverse.
 *            Input FALSE to search array in order. That means to search the element from index 0 to the end of array.
 * Return value:  (*) Index of element + 1. If function returned 0, it should mean pitem could not found.
 * Caution:       Address of parrz Must Be Allocated first.
 */
size_t strLinearSearchArrayZ(P_ARRAY_Z parrz, const void * pitem, size_t size, BOOL brev)
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

/* Function name: strBinaryLocateArrayZ
 * Description:   Binary locate an item in a sized-array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *      pitem Pointer to an element as a searching target.
 *       size Size of an element.
 *     cbfcmp Pointer to a CBF_COMPARE function.
 *            The left parameter of cbfcmp may point to any element in the array,
 *            whereas the right parameter of cbfcmp always holds the same pointer that pitem stores.
 *            Please refer to the definition of callback function type CBF_COMPARE at file svdef.h for more details.
 * Return value:  Index of an element.
 * Caution:       Address of parrz Must Be Allocated first.
 *                (*) The array that parrz pointed must be sorted in increasing order.
 * Tip:           For instence, to locate number 5 in array (1,2,3,4,4,6) will get an index as 5 pointes to element 6.
 *                Returned index always pointes to the smallest item in the array that is great than the value that pitem pointed.
 *                If the value you retrieved exceeded the maximum value in the array, the returned index would be (parrz->num - 1).
 */
size_t strBinaryLocateArrayZ(P_ARRAY_Z parrz, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	if (strLevelArrayZ(parrz) > 0)
	{
		REGISTER size_t l = 0, m = 0, r = strLevelArrayZ(parrz) - 1;
		while (l < r)
		{
			m = l + ((r - l) >> 1);
			if (cbfcmp(parrz->pdata + m * size, pitem) < 0)
				l = m + 1;
			else
				r = m;
		}
		if (cbfcmp(parrz->pdata + r * size, pitem) <= 0)
			return r + 1;
		return r;
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
 *    bshrink Input TRUE to shrink array, otherwise input FASLE.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void strRemoveItemArrayZ(P_ARRAY_Z parrz, size_t size, size_t index, BOOL bshrink)
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
	if (NULL == strResizeArrayZ(pdest, strLevelArrayZ(pdest) + strLevelArrayZ(psrc), size))
		return NULL;
	else
	{
		REGISTER size_t di = 0, si = 0, sj = 0, t;
		REGISTER int r0, r1;
		REGISTER BOOL binc =
		(
			cbfcmp(pdest->pdata, _P_ARRAY_Z_ITEM(pdest, strLevelArrayZ(pdest) - 1)) <= 0 &&
			cbfcmp(psrc->pdata,  _P_ARRAY_Z_ITEM(psrc,  strLevelArrayZ(psrc) - 1))  <= 0
		) ?
		TRUE : /* Both destination and source are in increasing order. */
		FALSE; /* Both destination and source are in decreasing order. */
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
 * Return value:  Pointer to the limit in array and casted the pointer to (void *).
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 * Tip:           After invoking, the buffer that ptemp pointed stored the limit value.
 *                To get the index of maximum value in fixed-size array arr, use following sentences:
 *                size_t index; PUCHAR ptr = strGetLimitationArrayZ(&arr, ptemp, size, cbfcmp, TRUE);
 *                if (NULL != ptr) index = (ptr == parrz->pdata ? 0 : (ptr - arr.pdata) / size - 1);
 *
 */
void * strGetLimitationArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, BOOL bmax)
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
 *    bshrink Input TRUE to shrink array, otherwise input FASLE.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                The size of the buffer of ptemp pointed shall equal to parameter size.
 */
void strUniqueArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, BOOL bshrink)
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
 *      bnext Input TRUE to permute an array next; Input FALSE to permute an array previously.
 * Return value:  TRUE indicates permutation continued; FALSE indicates permutation ended.
 * Caution:       Address of parrz Must Be Allocated first.
 *                Users shall manage the buffer that ptemp points at.
 *                (*) The size of the buffer of ptemp pointed shall equal to parameter size.
 *                (*) Each element in the array shall be unique.
 * Tip:           Users may call strUniqueArrayZ to generate a suitable array for permuting.
 *                This function references to two similar templates in STL of C Plus Plus.
 */
BOOL strPermuteArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, CBF_COMPARE cbfcmp, BOOL bnext)
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
				return TRUE;
			}
			if (ptri <= parrz->pdata)
			{	/* Reverse array from first to last. */
				strReverseArrayZ(parrz, ptemp, size);
				goto Lbl_End_Permuting;
			}
		}
	}
Lbl_End_Permuting:
	return FALSE;
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
 * Return value:  TRUE indicates combination continued; FALSE indicates combination ended.
 * Caution:       Address of Both parrzn and parrzr Must Be Allocated first.
 *                (*) Each element in parrzn shall be unique.
 *                (*) Elements in array that parrzn and parrzr pointed shall be sorted in increasing order.
 * Tip:           Users may call strUniqueArrayZ(parrzn, ptemp, size, cbfcmp, TRUE);
 *                to generate a suitable array for combination.
 */
BOOL strCombineNextArrayZ(P_ARRAY_Z parrzr, P_ARRAY_Z parrzn, size_t size, CBF_COMPARE cbfcmp)
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
		return TRUE;
	}
Lbl_End_Combination:
	return FALSE; /* No next combination. */
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
 */
void strShuffleArrayZ(P_ARRAY_Z parrz, void * ptemp, size_t size, unsigned int seed)
{
	srand(seed);
	switch (strLevelArrayZ(parrz))
	{
	case 0: case 1: return; /* Why should I shuffle nothing or one thing? */
	case 2: svSwap(parrz->pdata, parrz->pdata + size * (rand() & 1), ptemp, size); return;
	default:
		{
			REGISTER PUCHAR p;
			REGISTER size_t i, j, rnd;
			for (i = 0, j = strLevelArrayZ(parrz) - 1, p = parrz->pdata; rnd = rand() * rand(), i <= j; ++i, p += size)
				svSwap(p, parrz->pdata + size * (rnd % j), ptemp, size);
		}
	}
}
