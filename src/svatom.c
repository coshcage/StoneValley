/*
 * Name:        svatom.c
 * Description: Atomic structures.
 * Author:      cyycoish#hotmail.com
 * File ID:     0306170948A1025171110L00259
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
	if (NULL == parrz || NULL == strInitArrayZ(parrz, num, size))
	{
		free(parrz);
		return NULL;
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
 *                If function returned NULL, it would indicate an allocation failure.
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
	parrz->num = (NULL == (parrz->pdata = (PUCHAR) realloc(parrz->pdata, num * size)) ? 0 : num);
	return parrz->pdata;
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
 *       size Size of the data of a node.
 *       pval Address of the initial value.
 *            If size equaled to 0, function would return a NULL.
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
