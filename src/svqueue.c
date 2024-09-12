/*
 * Name:        svqueue.c
 * Description: Queues.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0417171257F0604231338L00510
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

#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function memcpy. */
#include "svqueue.h"

/* Functions for circular queues that use fixed-size arrays. */

/* Function name: queInitAC
 * Description:   Initialize a circular queue.
 * Parameters:
 *     pqueac Pointer to the circular queue you want to allocate.
 *        num Number of elements.
 *       size Size of each element in the queue.
 * Return value:  Pointer to the new allocated buffer.
 * Caution:       Address of pqueac Must Be Allocated first.
 */
void * queInitAC(P_QUEUE_A pqueac, size_t num, size_t size)
{
	strInitArrayZ(&pqueac->arr, num, size);
	pqueac->front = pqueac->rear = 0;
	return pqueac->arr.pdata;
}

/* Function name: queFreeAC
 * Description:   Deallocate the circular queue of which is allocated by function queInitAC.
 * Parameter:
 *    pqueac Pointer to the circular queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pqueac Must Be Allocated first.
 */
void queFreeAC(P_QUEUE_A pqueac)
{
	strFreeArrayZ(&pqueac->arr);
	pqueac->front = pqueac->rear = 0;
}

/* Function name: queCreateAC
 * Description:   Create a circular queue.
 * Parameters:
 *        num Number of elements.
 *       size Size of each elements.
 * Return value:  Pointer to the new allocated circular queue.
 * Caution:       Address of pqueac Must Be Allocated first.
 */
P_QUEUE_A queCreateAC(size_t num, size_t size)
{
	REGISTER P_QUEUE_A pquen = (P_QUEUE_A) malloc(sizeof(QUEUE_A));
	if (NULL != pquen)
	{
		if (NULL == queInitAC(pquen, num, size))
		{	/* Allocation failue. */
			free(pquen);
			return NULL;
		}
	}
	return pquen;
}

/* Function name: queDeleteAC
 * Description:   Delete the circular queue of which is allocated by function queCreateAC.
 * Parameter:
 *    pqueac Pointer to the circular queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pqueac Must Be Allocated first.
 */
void queDeleteAC(P_QUEUE_A pqueac)
{
	queFreeAC(pqueac);
	free(pqueac);
}

/* Function name: queIsInitialAC
 * Description:   Determin whether a queue is initial.
 * Parameter:
 *    pqueac Pointer to the circular queue you want to test.
 * Return value:  If the queue were initial, function would return a TRUE,
 *                otherwise it would return a FALSE.
 * Caution:       Address of pqueac Must Be Allocated first.
 * Tip:           A macro version of this function named queIsInitialA_M is available.
 */
BOOL queIsInitialAC_O(P_QUEUE_A pqueac)
{
	return pqueac->front == pqueac->rear;
}

/* Function name: queUsageAC_O
 * Description:   Determine how many items there are in a queue.
 * Parameter:
 *    pqueac Pointer to the circular queue you want to have a test on.
 * Return value:  Number of items in use.
 * Caution:       Address of pqueac Must Be Allocated first.
 *                If status of the queue were initial and the front index of the queue equaled 0,
 *                Function will return 0. That is in this situation,
 *                this function would return 0 when either the queue were full or empty.
 *                In other cases, the return value of this function would always be a number of real usage minus 1.
 * Tip:           A macro version of this function named queUsageAC_M is available.
 */
size_t queUsageAC_O(P_QUEUE_A pqueac)
{
	return (pqueac->front > pqueac->rear) ?
		(pqueac->arr.num - pqueac->front + pqueac->rear) :
		(pqueac->rear - pqueac->front);
}

/* Function name: queInsertAC_O
 * Description:   Insert an element into a circular queue.
 * Parameters:
 *     pqueac Pointer to the circular queue you want to operate with.
 *      pitem Pointer to the address of elements in queue.
 *       size Size of each element in the queue.
 * Return value:  N/A.
 * Caution:       Address of pqueac Must Be Allocated first.
 * Tip:           A macro version of this function named queInsertAC_M is available.
 */
void queInsertAC_O(P_QUEUE_A pqueac, const void * pitem, size_t size)
{
	memcpy(pqueac->arr.pdata + pqueac->rear * size, pitem, size);
	pqueac->rear = (pqueac->rear + 1) % pqueac->arr.num;
}

/* Function name: queRemoveAC_O
 * Description:   Dequeue an item from a circular queue.
 * Parameters:
 *      pitem Pointer to the address of element.
 *       size Size of each element in the queue.
 *     pqueac Pointer to the circular queue you want to operate with.
 * Return value:  N/A.
 * Caution:       Address of pqueac Must Be Allocated first.
 * Tip:           A macro version of this function named queRemoveAC_M is available.
 */
void queRemoveAC_O(void * pitem, size_t size, P_QUEUE_A pqueac)
{
	memcpy(pitem, pqueac->arr.pdata + pqueac->front * size, size);
	pqueac->front = (pqueac->front + 1) % pqueac->arr.num;
}

/* Functions for queues that implemented with single linked-lists. */

/* Function name: queInitL_O
 * Description:   Initialize a linked-list queue.
 * Parameter:
 *     pquel Pointer to the linked-list queue you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of pquel Must Be Allocated first.
 * Tip:           A macro version of this function named queInitL_M is available.
 */
void queInitL_O(P_QUEUE_L pquel)
{
	pquel->pfront = pquel->prear = NULL;
}

/* Function name: queFreeL
 * Description:   Deallocate a linked-list queue of which is allocated by function queInitL.
 * Parameter:
 *     pquel Pointer to the linked-list queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pquel Must Be Allocated first.
 */
void queFreeL(P_QUEUE_L pquel)
{
	strFreeLinkedListSC(&pquel->pfront);
	pquel->pfront = pquel->prear = NULL;
}

/* Function name: queCreateL
 * Description:   Create a linked-list queue.
 * Parameter:     N/A.
 * Return value:  Pointer to the new created linked-list queue.
 */
P_QUEUE_L queCreateL(void)
{
	REGISTER P_QUEUE_L pquen = (P_QUEUE_L) malloc(sizeof(QUEUE_L));
	if (NULL == pquen)
		return NULL; /* Allocation failure. */
	queInitL(pquen);
	return pquen;
}

/* Function name: queDeleteL
 * Description:   Delete a linked-list queue of which is allocated by function queCreateL.
 * Parameter:
 *     pquel Pointer to the linked-list queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pquel Must Be Allocated first.
 */
void queDeleteL(P_QUEUE_L pquel)
{
	queFreeL(pquel);
	free(pquel);
}

/* Function name: queIsEmptyL
 * Description:   Determine whether a linked-list queue is empty or not.
 * Parameter:
 *     pquel Pointer to the linked-list queue you want to test.
 * Return value:  If the queue were empty, function would return a TRUE,
 *                otherwise function would return a FALSE.
 * Caution:       Address of pquel Must Be Allocated first.
 * Tip:           A macro version of this function named queIsEmptyL_M is available.
 */
BOOL queIsEmptyL_O(P_QUEUE_L pquel)
{
	return !pquel->pfront;
}

/* Function name: queUsageL_O
 * Description:   Determine how many elements there are stored in a queue.
 * Parameter:
 *    pdeque Pointer to the single pointer linked-list queue you want to test.
 * Return value:  Number of elements in the queue.
 * Caution:       Address of pquel Must Be Allocated first.
 * Tip:           A macro version of this function named queUsageL_M is available.
 */
size_t queUsageL_O(P_QUEUE_L pquel)
{
	return strLevelLinkedListSC(pquel->pfront);
}

/* Function name: queInsertL
 * Description:   Insert an element into linked-list queue.
 * Parameters:
 *      pquel Pointer to the linked-list queue you want to operate with.
 *      pitem Pointer to the address of an element.
 *       size Size of each element in the queue.
 * Return value:  If function work succeeded, it would return a TRUE,
 *                otherwise function would return a FALSE.
 */
BOOL queInsertL(P_QUEUE_L pquel, const void * pitem, size_t size)
{
	REGISTER P_NODE_S ptail;
	if (NULL == (ptail = strCreateNodeS(pitem, size)))
		return FALSE;
	/* Adjust the tail of queue. */
	if (NULL != pquel->prear)
		pquel->prear->pnode = ptail;
	pquel->prear = ptail;
	/* Do while queue is empty. */
	if (NULL == pquel->pfront)
		pquel->pfront = pquel->prear;
	return TRUE;
}

/* Function name: queRemoveL
 * Description:   Dequeue an element from linked-list queue.
 * Parameters:
 *      pitem Pointer to the address of an element.
 *       size Size of each element in the queue.
 *      pquel Pointer to the linked-list queue you want to operate with.
 * Return value:  If function work succeeded, it would return a TRUE,
 *                otherwise function would return a FALSE.
 * Caution:       To check whether queue is empty or not is NOT necessary.
 */
BOOL queRemoveL(void * pitem, size_t size, P_QUEUE_L pquel)
{
	if (NULL != pquel->pfront)
	{
		/* Save new header first. */
		REGISTER P_NODE_S phead = pquel->pfront->pnode;
		/* Restore data of the old header. */
		memcpy(pitem, pquel->pfront->pdata, size);
		/* Free old header. */
		strDeleteNodeS(pquel->pfront);
		/* Assign new header. */
		if (pquel->pfront == pquel->prear)
			pquel->pfront = pquel->prear = NULL;
		else /* Queue is not empty. */
			pquel->pfront = phead;
		return TRUE;
	}
	return FALSE;
}

/* Functions for queues that implemented with doubly linked-lists. */

/* Function name: queInitDL_O
 * Description:   Initialize a doubly linked-list queue.
 * Parameter:
 *    pdeque Pointer to the doubly linked-list queue you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of pdeque Must Be Allocated first.
 * Tip:           A macro version of this function named queInitDL_M is available.
 */
void queInitDL_O(P_DEQUE_DL pdeque)
{
	pdeque->pfirst = pdeque->plast = NULL;
}

/* Function name: queFreeDL
 * Description:   Deallocate a doubly linked-list queue of which is allocated by function queInitDL.
 * Parameter:
 *    pdeque Pointer to the doubly linked-list queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pdeque Must Be Allocated first.
 */
void queFreeDL(P_DEQUE_DL pdeque)
{
	strFreeLinkedListDC(&pdeque->pfirst, FALSE);
	pdeque->pfirst = pdeque->plast = NULL;
}

/* Function name: queCreateDL
 * Description:   Create a doubly linked-list queue.
 * Parameter:     N/A.
 * Return value:  Pointer to the new created doubly linked-list queue.
 */
P_DEQUE_DL queCreateDL(void)
{
	P_DEQUE_DL pdequen = (P_DEQUE_DL) malloc(sizeof(DEQUE_DL));
	if (NULL == pdequen)
		return NULL; /* Allocation failure. */
	queInitDL(pdequen);
	return pdequen;
}

/* Function name: queDeleteDL
 * Description:   Delete a doubly linked-list queue of which is allocated by function queCreateDL.
 * Parameter:
 *    pdeque Pointer to the linked-list queue you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pdeque Must Be Allocated first.
 */
void queDeleteDL(P_DEQUE_DL pdeque)
{
	queFreeDL(pdeque);
	free(pdeque);
}

/* Function name: queIsEmptyDL_O
 * Description:   Determine whether a doubly linked-list queue is empty.
 * Parameter:
 *    pdeque Pointer to the doubly linked-list queue you want to test.
 * Return value:  If the queue were empty, function would return a TRUE,
 *                otherwise function would return a FALSE.
 * Caution:       Address of pdeque Must Be Allocated first.
 * Tip:           A macro version of this function named queIsEmptyDL_M is available.
 */
BOOL queIsEmptyDL_O(P_DEQUE_DL pdeque)
{
	return !pdeque->pfirst;
}

/* Function name: queUsageDL_O
 * Description:   Determine how many elements there are stored in a queue.
 * Parameter:
 *    pdeque Pointer to the doubly linked-list queue you want to test.
 * Return value:  Number of elements in the queue.
 * Caution:       Address of pdeque Must Be Allocated first.
 * Tip:           A macro version of this function named queUsageDL_M is available.
 */
size_t queUsageDL_O(P_DEQUE_DL pdeque)
{
	return strLevelLinkedListDC(pdeque->pfirst, FALSE);
}

/* Function name: queFirstDL_O
 * Description:   Have a peek at the first element in he queue.
 * Parameters:
 *      pitem Pointer to a buffer of an element.
 *       size Size of element.
 *    pdeque Pointer to the doubly linked-list queue you want to check.
 * Return value:  N/A.
 * Caution:       Address of pdeque Must Be Allocated first.
 * Tip:           A macro version of this function named queFirstDL_M is available.
 */
void queFirstDL_O(void * pitem, size_t size, P_DEQUE_DL pdeque)
{
	memcpy(pitem, pdeque->pfirst->pdata, size);
}

/* Function name: quePushDL
 * Description:   Push an element onto the top of a doubly linked-list queue.
 * Parameters:
 *     pdeque Pointer to the doubly linked-list queue you want to operate on.
 *      pitem Pointer to the address of element you want to insert.
 *       size Size of element.
 * Return value:  Pointer to the new inserted node.
 *                If insertion failed, function would return a NULL.
 */
P_NODE_D quePushDL(P_DEQUE_DL pdeque, const void * pitem, size_t size)
{
	REGISTER P_NODE_D pnew;
	if (NULL != (pnew = strCreateNodeD(pitem, size)))
	{
		if (NULL != pdeque->pfirst)
		{
			pdeque->pfirst->ppnode[PREV] = pnew;
			pnew->ppnode[NEXT] = pdeque->pfirst;
		}
		else /* Empty queue. */
			pdeque->plast = pnew;
		pdeque->pfirst = pnew;
	}
	return pnew;
}

/* Function name: quePopDL
 * Description:   Pop an element out of a doubly linked-list queue from the top position.
 * Parameters:
 *      pitem Pointer to the address of element you want to insert.
 *       size Size of element.
 *     pdeque Pointer to the doubly linked-list queue you want to operate on.
 * Return value:  Pointer to the item which is lain after the deleted one.
 *                If deletion failed, function returns NULL.
 */
P_NODE_D quePopDL(void * pitem, size_t size, P_DEQUE_DL pdeque)
{
	REGISTER P_NODE_D ptmp = pdeque->pfirst;
	if (NULL != ptmp)
	{
		if (NULL != pitem)
			memcpy(pitem, ptmp->pdata, size);
		ptmp = pdeque->pfirst->ppnode[NEXT];
		strDeleteNodeD(pdeque->pfirst);
		if (NULL != (pdeque->pfirst = ptmp))
			ptmp->ppnode[PREV] = NULL;
		else
			pdeque->plast = NULL;
	}
	return ptmp;
}

/* Function name: queLastDL_O
 * Description:   Have a peek at the last element in he queue.
 * Parameters:
 *      pitem Pointer to a buffer of an element.
 *       size Size of element.
 *     pdeque Pointer to the doubly linked-list queue you want to check.
 * Return value:  N/A.
 * Caution:       Address of pdeque Must Be Allocated first.
 * Tip:           A macro version of this function named queLastDL_M is available.
 */
void queLastDL_O(void * pitem, size_t size, P_DEQUE_DL pdeque)
{
	memcpy(pitem, pdeque->plast->pdata, size);
}

/* Function name: queInjectDL
 * Description:   Insert an element at the bottom of a doubly linked-list queue.
 * Parameters:
 *     pdeque Pointer to the doubly linked-list queue you want to operate on.
 *      pitem Pointer to the address of element you want to insert.
 *       size Size of element.
 * Return value:  Pointer to the new inserted node.
 *                If insertion failed, function returns NULL.
 */
P_NODE_D queInjectDL(P_DEQUE_DL pdeque, const void * pitem, size_t size)
{
	REGISTER P_NODE_D ptmp;
	if (NULL != (ptmp = strCreateNodeD(pitem, size)))
	{
		if (NULL != pdeque->plast)
		{
			ptmp->ppnode[PREV] = pdeque->plast;
			pdeque->plast->ppnode[NEXT] = ptmp;
		}
		else /* Empty queue. */
			pdeque->pfirst = ptmp;
		pdeque->plast = ptmp;
	}
	return ptmp;
}

/* Function name: queEjectDL
 * Description:   Remove an element out of a doubly linked-list queue from the bottom.
 * Parameters:
 *      pitem Pointer to the address of element you want to insert.
 *       size Size of element.
 *     pdeque Pointer to the doubly linked-list queue you want to operate on.
 * Return value:  Pointer to the item which is lain before the deleted one.
 *                If deletion failed, function would return a NULL.
 */
P_NODE_D queEjectDL(void * pitem, size_t size, P_DEQUE_DL pdeque)
{
	REGISTER P_NODE_D ptmp = pdeque->plast;
	if (NULL != ptmp)
	{
		if (NULL != pitem)
			memcpy(pitem, ptmp->pdata, size);
		ptmp = pdeque->plast->ppnode[PREV];
		strDeleteNodeD(pdeque->plast);
		if (NULL != (pdeque->plast = ptmp))
			ptmp->ppnode[NEXT] = NULL;
		else
			pdeque->pfirst = NULL;
	}
	return ptmp;
}
