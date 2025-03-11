/*
 * Name:        svatom.c
 * Description: Atomic structures.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948A00201251300L00305
 * License:     LGPLv3
 * Copyright (C) 2017-2025 John Cage
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
 *            If size equaled to 0, function would return a NULL.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitArrayZ(P_ARRAY_Z parrz, size_t num, size_t size)
{
	if (NULL == (parrz->pdata = (PUCHAR) malloc(num * size)))
	{
		parrz->num = 0;
		parrz->pdata = NULL;
	}
	else
		parrz->num = num;
	return parrz->pdata;
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
	REGISTER size_t i = strLevelArrayZ(parrz), j = 0;
	for ( ; i >= 1; --i, j += size)
		memcpy(parrz->pdata + j, pval, size);
}

/* Function name: strResizeArrayZ
 * Description:   Resize an array.
 * Parameters:
 *      parrz Pointer to the sized array you want to allocate.
 *        num Number of elements.
 *       size Size of each element.
 *            If size equaled to 0, function would return a NULL after deallocating the array.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strResizeArrayZ(P_ARRAY_Z parrz, size_t num, size_t size)
{
	PUCHAR pnew = (PUCHAR) realloc(parrz->pdata, num * size);
	if (NULL == pnew)
	{	/* Reallocation failure. */
		parrz->num = 0;
		parrz->pdata = NULL;
	}
	else
	{
		parrz->num = num;
		parrz->pdata = pnew;
	}
	return parrz->pdata;
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
 * Tip:           // strResizeBufferedArrayZ(parr, sizeof(int), +BUFSIZ);
 *                // strResizeBufferedArrayZ(parr, sizeof(int), -BUFSIZ);
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
		if (strLevelArrayZ(parrz) <= incl)
			return NULL;
		return strResizeArrayZ(parrz, strLevelArrayZ(parrz) - (size_t)incl, size);
	}
}

/* Function name: strFreeArrayZ_O
 * Description:   Deallocate an array of which is allocated by function strInitArrayZ.
 * Parameter:
 *     parrz Pointer to the sized array you want to delete.
 * Return value:  N/A.
 * Caution:       Address of parrz Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeArrayZ_M is available.
 */
void strFreeArrayZ_O(P_ARRAY_Z parrz)
{
	free(parrz->pdata);
	parrz->pdata = NULL;
	parrz->num = 0;
}

/* Function name: strDeleteArrayZ_O
 * Description:   Delete an array of which is allocated by function strCreateArrayZ.
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
 *       size Size of that element.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitNodeS(P_NODE_S pnode, const void * pval, size_t size)
{
	if (NULL == (pnode->pdata = (PUCHAR) malloc(size)))
		return NULL; /* Buffer creation failed. */
	if (NULL != pval)
		memcpy(pnode->pdata, pval, size);
	pnode->pnode = NULL;
	return pnode->pdata;
}

/* Function name: strCreateNodeS
 * Description:   Dynamically allocate a node which is with a single pointer.
 * Parameters:
 *       pval Address of the initial value.
 *            If size equaled to 0, function would return a NULL.
 *       size Size of the data of a node.
 * Return value:  Pointer to the new allocated node.
 */
P_NODE_S strCreateNodeS(const void * pval, size_t size)
{
	P_NODE_S pnode;
	if (NULL == (pnode = (P_NODE_S) malloc(sizeof(NODE_S))))
		return NULL;
	if (NULL == strInitNodeS(pnode, pval, size)) /* Buffer creation failed. */
	{
		free(pnode);
		return NULL;
	}
	return pnode;
}

/* Function name: strFreeNodeS_O
 * Description:   Deallocate a NODE_S of which is allocated by function strInitNodeS.
 * Parameter:
 *     pnode Pointer to that node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeNodeS_M is available.
 */
void strFreeNodeS_O(P_NODE_S pnode)
{
	free(pnode->pdata);
}

/* Function name: strDeleteNodeS_O
 * Description:   Delete a NODE_S of which is allocated by function strCreateNodeS.
 * Parameter:
 *     pnode Pointer to that node you want to deallocate.
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
 * Description:   Initiate a node which has has two pointers.
 * Parameters:
 *      pnode Pointer to the node you want to initiate.
 *       pval Pointer to the value you want to set into the new node.
 *       size Size of that element.
 * Return value:  Pointer to new allocated buffer.
 * Caution:       Address of parrz Must Be Allocated first.
 */
void * strInitNodeD(P_NODE_D pnode, const void * pval, size_t size)
{
	if (NULL == (pnode->pdata = (PUCHAR) malloc(size)))
		return NULL; /* Buffer creation failed. */
	if (NULL != pval)
		memcpy(pnode->pdata, pval, size);
	pnode->ppnode[PREV] = pnode->ppnode[NEXT] = NULL;
	return pnode->pdata;
}

/* Function name: strCreateNodeD
 * Description:   Dynamically allocate a node which has two pointers.
 * Parameters:
 *       pval Pointer to the value you want to set into the new node.
 *       size Size of the data of that node.
 *            If size equaled to 0, function would return a NULL.
 * Return value:  Pointer to the new allocated node.
 */
P_NODE_D strCreateNodeD(const void * pval, size_t size)
{
	P_NODE_D pnode;
	if (NULL == (pnode = (P_NODE_D) malloc(sizeof(NODE_D))))
		return NULL;
	if (NULL == strInitNodeD(pnode, pval, size))
	{
		free(pnode);
		return NULL;
	}
	return pnode;
}

/* Function name: strFreeNodeD_O
 * Description:   Deallocate a NODE_D of which is allocated by function strCreateNodeD.
 * Parameter:
 *     pnode Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeNodeD_M is available.
 */
void strFreeNodeD_O(P_NODE_D pnode)
{
	free(pnode->pdata);
}

/* Function name: strDeleteNodeD_O
 * Description:   Delete a NODE_D of which is allocated by function strCreateNodeD.
 * Parameter:
 *     pnode Pointer to that node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteNodeD_M is available.
 */
void strDeleteNodeD_O(P_NODE_D pnode)
{
	strFreeNodeD(pnode);
	free(pnode);
}

