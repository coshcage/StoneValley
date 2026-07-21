/*
 * Name:        svatom.c
 * Description: Atomic structures.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948A0720261304L00377
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

#include <stdlib.h> /* Using function malloc, realloc, free. */
#include <string.h> /* Using function memcpy. */
#include "svstring.h"

/* Function name: strInitArrayZ
 * Description:   Allocate a sized array.
 * Parameters:
 *      parrz Pointer to the sized array you want to allocate.
 *        num Number of elements.
 *       size Size of each element.
 *            If size equaled to 0, function would return NULL.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitArrayZ(P_ARRAY_Z parrz, size_t num, size_t size)
{
	REGISTER size_t sum = num * size;
	if (0 == sum)
	{
		parrz->num = 0;
		return (void *)(parrz->pdata = NULL);
	}
	else
	{
		if (NULL == (parrz->pdata = (PUCHAR) malloc(sum)))
			parrz->num = 0;
		else
			parrz->num = num;
		return parrz->pdata;
	}
}

/* Function name: strCreateArrayZ
 * Description:   Create a sized array.
 * Parameters:
 *        num Number of elements.
 *       size Size of each element.
 * Return value:  Pointer to new allocated buffer.
 *                If function returned NULL, it would indicate an allocation failure.
 * Caution:       Address of parrz Must Be Allocated first.
 */
P_ARRAY_Z strCreateArrayZ(size_t num, size_t size)
{
	P_ARRAY_Z parrz = (P_ARRAY_Z) malloc(sizeof(ARRAY_Z));
	if (NULL != parrz)
	{
		if (NULL == strInitArrayZ(parrz, num, size))
		{	/* Allocation failure. */
			free(parrz);
			return NULL;
		}
	}
	return parrz;
}

/* Function name: strSetArrayZ
 * Description:   Fill up an array.
 * Parameters:
 *      parrz Pointer to the sized array you want to set.
 *       pval Pointer to the value you want to set.
 *       size Size of each element.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void strSetArrayZ(P_ARRAY_Z parrz, const void * pval, size_t size)
{
	if (strLevelArrayZ(parrz) > 0)
	{
		if (sizeof(UCHART) == size) /* size is one for byte, call memset directly. */
			memset(parrz->pdata, (int)*(PUCHAR)pval, size * sizeof(UCHART) * strLevelArrayZ(parrz));
		else
		{
			REGISTER size_t i, j;
			if (sizeof(size_t) == size)
			{	/* Optimized loop for size_t. */
				REGISTER size_t * ps = (size_t *)parrz->pdata;
				for (i = strLevelArrayZ(parrz), j = *(size_t *)pval; i >= 1; --i) *ps++ = j;
			}
			else
			{	/* Set the first element in the array from pval. */
				memcpy(parrz->pdata, pval, size);
				/* Repeat the previous item in the array but not the item which pval pointed to fit the memory locality principle. */
				for (i = strLevelArrayZ(parrz), j = size; i > 1; --i, j += size) memcpy(parrz->pdata + j, parrz->pdata + j - size, size);
			}
		}
	}
}

/* Function name: strResizeArrayZ
 * Description:   Resize an array.
 * Parameters:
 *      parrz Pointer to the sized array you want to allocate.
 *        num Number of elements.
 *       size Size of each element.
 *            If size equaled to 0, function would return NULL after retracting the array.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strResizeArrayZ(P_ARRAY_Z parrz, size_t num, size_t size)
{
	REGISTER size_t sum = num * size;
	if (0 == sum)
	{
		if (NULL != parrz->pdata)
		{
			free(parrz->pdata);
			parrz->num = 0;
		}
		return (void *)(parrz->pdata = NULL);
	}
	else
	{
		if (NULL != parrz->pdata)
		{
			REGISTER PUCHAR pnew = (PUCHAR) realloc(parrz->pdata, sum);
			if (NULL == pnew)
				return NULL; /* Reallocation failure. Refer to luc0x61@codeberg.net */
			else
			{
				parrz->num = num;
				return (void *)(parrz->pdata = pnew);
			}
		}
		else
			return strInitArrayZ(parrz, num, size);
	}
}

/* Function name: strResizeBufferedArrayZ
 * Description:   Increase or decrease the size of a sized array.
 * Parameters:
 *      parrz Pointer to a sized array.
 *       size Size of each element in the array.
 *       incl Incremental of size which you want to change to the array.
 *            Input a positive number to increase size.
 *            Input a negative number to decrease size.
 * Return value:  The address of the buffer of array.
 * Caution:       Address of parrz Must Be Allocated first.
 * Usage:         strResizeBufferedArrayZ(parr, sizeof(int), +BUFSIZ);
 *                strResizeBufferedArrayZ(parr, sizeof(int), -BUFSIZ);
 */
void * strResizeBufferedArrayZ(P_ARRAY_Z parrz, size_t size, ptrdiff_t incl)
{
	if (0 == incl)
		return parrz->pdata;
	if (incl > 0)
		return strResizeArrayZ(parrz, strLevelArrayZ(parrz) + (size_t)incl, size);
	else
	{
		incl = -incl;
		if (strLevelArrayZ(parrz) <= (size_t)incl)
			return NULL;
		return strResizeArrayZ(parrz, strLevelArrayZ(parrz) - (size_t)incl, size);
	}
}

/* Function name: strFreeArrayZ_O
 * Description:   Retract an array which is allocated by function strInitArrayZ.
 * Parameter:
 *     parrz Pointer to the sized array you want to delete.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeArrayZ_M is available.
 */
void strFreeArrayZ_O(P_ARRAY_Z parrz)
{
	if (NULL != parrz->pdata) /* Circumvent freeing a NULL pointer. */
	{
		free(parrz->pdata);
		parrz->pdata = NULL;
	}
	parrz->num = 0;
}

/* Function name: strDeleteArrayZ_O
 * Description:   Delete an array which is allocated by function strCreateArrayZ.
 * Parameter:
 *     parrz Pointer to the sized array you want to delete.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteArrayZ_M is available.
 */
void strDeleteArrayZ_O(P_ARRAY_Z parrz)
{
	strFreeArrayZ(parrz);
	free(parrz);
}

/* Function name: strInitNodeS
 * Description:   Initiate a node which has a single pointer.
 * Parameters:
 *      pnode Pointer to the node you want to initiate.
 *       pval Pointer to the value you want to set into a new node.
 *            Set pval into NULL to allot a blank buffer for data pointer pdata.
 *       size Size of data of pval.
 *            Notice that 0 size results a NULL data pointer.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitNodeS(P_NODE_S pnode, const void * pval, size_t size)
{
	pnode->pnode = NULL;
	if (0 == size)
		pnode->pdata = NULL;
	else
	{
		if (NULL == (pnode->pdata = (PUCHAR) malloc(size)))
			return NULL; /* Buffer creation failed. */
		if (NULL != pval)
			memcpy(pnode->pdata, pval, size);
	}
	return pnode->pdata;
}

/* Function name: strCreateNodeS
 * Description:   Dynamically allocate a node which is with a single pointer.
 * Parameters:
 *       pval Address of the initial value.
 *            Set pval into NULL to create a blank buffer for data pointer.
 *       size Size of the data of a node.
 *            Notice that 0 size results a NULL data pointer.
 * Return value:  Pointer to the new allocated node.
 */
P_NODE_S strCreateNodeS(const void * pval, size_t size)
{
	REGISTER P_NODE_S pnode = (P_NODE_S) malloc(sizeof(NODE_S));
	if (NULL == pnode)
		return NULL;
	else
	{
		REGISTER void * p = strInitNodeS(pnode, pval, size);
		if (0 != size && NULL == p)
		{
			free(pnode); /* Allocation failure. */
			return NULL;
		}
	}
	return pnode;
}

/* Function name: strFreeNodeS_O
 * Description:   Retract a NODE_S which is allocated by function strInitNodeS.
 * Parameter:
 *     pnode Pointer to that node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeNodeS_M is available.
 */
void strFreeNodeS_O(P_NODE_S pnode)
{
	if (NULL != pnode->pdata) /* Circumvent freeing a NULL pointer. */
	{
		free(pnode->pdata);
		pnode->pdata = NULL;
	}
}

/* Function name: strDeleteNodeS_O
 * Description:   Delete a NODE_S which is allocated by function strCreateNodeS.
 * Parameter:
 *     pnode Pointer to that node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteNodeS_M is available.
 */
void strDeleteNodeS_O(P_NODE_S pnode)
{
	strFreeNodeS(pnode);
	free(pnode);
}

/* Function name: strInitNodeD
 * Description:   Initiate a node which has has two directive pointers.
 * Parameters:
 *      pnode Pointer to the node you want to initiate.
 *       pval Pointer to the value you want to set into the new node.
 *            Set pval into NULL to allot a blank buffer for data pointer pdata.
 *       size Size of data of pval.
 *            Notice that 0 size results a NULL data pointer.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitNodeD(P_NODE_D pnode, const void * pval, size_t size)
{
	pnode->ppnode[PREV] = pnode->ppnode[NEXT] = NULL;
	if (0 == size)
		pnode->pdata = NULL;
	else
	{
		if (NULL == (pnode->pdata = (PUCHAR) malloc(size)))
			return NULL; /* Buffer creation failed. */
		if (NULL != pval)
			memcpy(pnode->pdata, pval, size);
	}
	return pnode->pdata;
}

/* Function name: strCreateNodeD
 * Description:   Dynamically allocate a node which has two directive pointers.
 * Parameters:
 *       pval Pointer to the value you want to copy into the new node.
 *            Set pval into NULL to create a blank buffer for data pointer.
 *       size Size of the data of a node.
 *            Notice that 0 size results a NULL data pointer.
 * Return value:  Pointer to the new allocated double pointer node.
 */
P_NODE_D strCreateNodeD(const void * pval, size_t size)
{
	REGISTER P_NODE_D pnode = (P_NODE_D) malloc(sizeof(NODE_D));
	if (NULL == pnode)
		return NULL;
	else
	{
		REGISTER void * p = strInitNodeD(pnode, pval, size);
		if (0 != size && NULL == p)
		{
			free(pnode); /* Allocation failure. */
			return NULL;
		}
	}
	return pnode;
}

/* Function name: strFreeNodeD_O
 * Description:   Retract a NODE_D which is allocated by function strCreateNodeD.
 * Parameter:
 *     pnode Pointer to the node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeNodeD_M is available.
 */
void strFreeNodeD_O(P_NODE_D pnode)
{
	if (NULL != pnode->pdata) /* Circumvent freeing a NULL pointer. */
	{
		free(pnode->pdata);
		pnode->pdata = NULL;
	}
}

/* Function name: strDeleteNodeD_O
 * Description:   Delete a NODE_D which is allocated by function strCreateNodeD.
 * Parameter:
 *     pnode Pointer to that node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteNodeD_M is available.
 */
void strDeleteNodeD_O(P_NODE_D pnode)
{
	strFreeNodeD(pnode);
	free(pnode);
}

