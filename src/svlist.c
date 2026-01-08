/*
 * Name:        svlist.c
 * Description: Linked lists.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170948C0107261000L01409
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
#include <string.h> /* Using function memcmp, memcpy, memmove. */
#include "svstring.h"

/* File level function declarations here. */
int _strCBFDeleteNode       (void * pitem, size_t param);
int _strCBFNodesCounter     (void * pitem, size_t param);
int _strCBFCompareNodeDataS (void * pitem, size_t param);
int _strCBFCompareNodeDataD (void * pitem, size_t param);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strCBFDeleteNode
 * Description:   Node freeing dispatcher.
 * Parameters:
 *      pitem Pointer to each item in the array.
 *      param A value that is in NodeType enumeration.
 *            If size equaled to ENT_SINGLE, function would free a NODE_S.
 *            If size equaled to ENT_DOUBLE, function would free a NODE_D.
 * Return value:  CBF_CONTINUE only.
 */
int _strCBFDeleteNode(void * pitem, size_t param)
{
	switch (param)
	{
	case ENT_SINGLE: strDeleteNodeS((P_NODE_S)pitem); break;
	case ENT_DOUBLE: strDeleteNodeD((P_NODE_D)pitem);
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strCBFNodesCounter
 * Description:   Accumulator for the number of nodes.
 * Parameters:
 *      pitem N/A.
 *      param A pointer to the address of accumulator.
 * Return value:  CBF_CONTINUE only.
 */
int _strCBFNodesCounter(void * pitem, size_t param)
{
	++*(size_t *)param;
	DWC4100(pitem);
	/* Add this above sentence to prevent function from producing a
	 * unreferenced formal parameter warning for pitem.
	 */
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strCBFCompareNodeDataS
 * Description:   Used to compare data of NODE_S.
 * Parameters:
 *      pitem Pointer to a NODE_S.
 *      param Pointer to FindingInfo.
 * Return value:  If data matched, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _strCBFCompareNodeDataS(void * pitem, size_t param)
{
	/* The type of param is P_FindingInfo. */
	if (0 == memcmp(((P_NODE_S)pitem)->pdata,
			( (P_FindingInfo) param)->pitem,
			( (P_FindingInfo) param)->size)
		)
	{
		((P_FindingInfo)param)->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strCBFCompareNodeDataD
 * Description:   Used to compare data of NODE_D.
 * Parameters:
 *      pitem Pointer to a NODE_S.
 *      param Pointer to FindingInfo.
 * Return value:  If data matched, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _strCBFCompareNodeDataD(void * pitem, size_t param)
{
	/* The type of param is P_FindingInfo. */
	if (0 == memcmp(((P_NODE_D)pitem)->pdata,
			( (P_FindingInfo) param)->pitem,
			( (P_FindingInfo) param)->size)
		)
	{
		((P_FindingInfo)param)->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Function name: strTraverseLinkedListSC_R
 * Description:   Recursively traverse a single-pointer-node linked-list.
 * Parameters:
 *       list Pointer to the first NODE_S element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to a callback function.
 *      param Additional information for each node.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return.
 * Tip:           An element of a circular linked-lists is suitable to be a parameter of this function.
 */
int strTraverseLinkedListSC_R(LIST_S list, P_NODE_S pnil, CBF_TRAVERSE cbftvs, size_t param)
{
	int r = CBF_CONTINUE;
	if (NULL != list)
	{
		if (list != pnil)
		{
			if (NULL == pnil)
				pnil = list;
			r = strTraverseLinkedListSC_R(list->pnode, pnil, cbftvs, param);
			if (CBF_CONTINUE != cbftvs(list, param))
				return CBF_TERMINATE;
		}
	}
	return r;
}

/* Function name: strTraverseLinkedListSC_A
 * Description:   Recursively traverse a single-pointer-node linked-list.
 *                The order of traversal is opposite of strTraverseLinkedListSC_R.
 * Parameters:
 *       list Pointer to the first NODE_S element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to a callback function.
 *      param Additional information for each node.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will be returned.
 * Tip:           An element of a circular linked-lists is suitable to be a parameter of this function.
 */
int strTraverseLinkedListSC_A(LIST_S list, P_NODE_S pnil, CBF_TRAVERSE cbftvs, size_t param)
{
	if (NULL != list)
	{
		if (list != pnil)
		{
			if (NULL == pnil)
				pnil = list;
			if (CBF_CONTINUE != cbftvs(list, param))
				return CBF_TERMINATE;
			return strTraverseLinkedListSC_A(list->pnode, pnil, cbftvs, param);
		}
	}
	return CBF_CONTINUE;
}

/* Function name: strTraverseLinkedListSC_N
 * Description:   Traverse a single-pointer-node linked-list using a loop.
 *                The order of traversal is as the same as strTraverseLinkedListSC_A.
 * Parameters:
 *       list Pointer to the first element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to the callback function.
 *      param Additional information for each node.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will be returned.
 * Tip:           An element of a circular linked-lists is suitable to be a parameter of this function.
 */
int strTraverseLinkedListSC_N(LIST_S list, P_NODE_S pnil, CBF_TRAVERSE cbftvs, size_t param)
{
	REGISTER P_NODE_S pcur = list;
	for ( ; NULL != pcur; pcur = pcur->pnode)
	{
		/* Break at the header of a circular list. */
		if (NULL != pnil && pcur == list)
			break;
		/* Dead loop appears only if A->B and B->B.
		 * We have to prevent it from occurring.
		 */
		if (pnil == pcur)
			break;
		if (CBF_CONTINUE != cbftvs(pcur, param))
			return CBF_TERMINATE;
		pnil = pcur;
	}
	return CBF_CONTINUE;
}

/* Function name: strInitLinkedListSC_O
 * Description:   Initialize a single-pointer-node linked-list.
 * Parameter:
 *     plist Pointer to the list you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of plist Must Be Allocated first.
 * Tip:           A macro version of this function named strInitLinkedListSC_M is available.
 */
void strInitLinkedListSC_O(P_LIST_S plist)
{
	*plist = NULL;
}

/* Function name: strFreeLinkedListSC
 * Description:   Deallocate a single-pointer-node linked-list
 *                of which is allocated by function strInitLinkedListSC.
 * Parameter:
 *     plist Pointer to the plist you want to free.
 * Return value:  N/A.
 * Caution:       Address of plist Must Be Allocated first.
 */
void strFreeLinkedListSC(P_LIST_S plist)
{
	strTraverseLinkedListSC_R(*plist, NULL, _strCBFDeleteNode, ENT_SINGLE);
	strInitLinkedListSC(plist);
}

/* Function name: strCreateLinkedListSC
 * Description:   Create a new single-pointer-node linked-list.
 * Parameter:     N/A.
 * Return value:  Pointer to the new list.
 */
P_LIST_S strCreateLinkedListSC(void)
{
	P_LIST_S plist = (P_LIST_S) malloc(sizeof(LIST_S));
	if (NULL == plist)
		return NULL; /* Allocation failure. */
	strInitLinkedListSC(plist);
	return plist;
}

/* Function name: strDeleteLinkedListSC_O
 * Description:   Delete a single-pointer-node linked-list.
 * Parameter:
 *     plist Pointer to the list you want to delete.
 * Return value:  N/A.
 * Caution:       Parameter plist Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteLinkedListSC_M is available.
 */
void strDeleteLinkedListSC_O(P_LIST_S plist)
{
	strFreeLinkedListSC(plist);
	free(plist);
}

/* Function name: strLevelLinkedListSC
 * Description:   Return how many nodes there are stored in a single-pointer linked-list.
 * Parameter:
 *      list Pointer to the first NODE_S element while traversal.
 * Return value:  The number of nodes in a single-pointer linked-list.
 * Tip:           An element of a circular linked-list is suitable to be a parameter of this function.
 */
size_t strLevelLinkedListSC(LIST_S list)
{
	size_t l = 0;
	strTraverseLinkedListSC_X(list, NULL, _strCBFNodesCounter, (size_t)&l);
	return l;
}

/* Function name: strCopyLinkedListSC
 * Description:   Copy the entire single linked-list.
 * Parameters:
 *       psrc Pointer to the header of a linked-list.
 *       size Size of data in each node.
 * Return value:  The first element of new linked-list.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S strCopyLinkedListSC(LIST_S psrc, size_t size)
{
	REGISTER bool bClr = false;
	REGISTER P_NODE_S pcur = psrc;
	REGISTER P_NODE_S ptmp = NULL;
	REGISTER P_NODE_S pnew = NULL;
	REGISTER P_NODE_S pdst = NULL;
	P_NODE_S prtn = NULL;
	while (NULL != pcur)
	{
		if (NULL == (pnew = strCreateNodeS(pcur->pdata, size)))
		{
			strFreeLinkedListSC(&prtn);
			return NULL;
		}
		if (NULL == prtn)
			prtn = pnew;
		if (NULL != pdst)
			pdst->pnode = pnew;
		pdst = pnew;
		pcur = pcur->pnode;
		if (psrc == pcur) /* Circular list. */
		{
			bClr = true;
			break;
		}
		else
		{	/* Dead loop appears only if ->A<- and A<-B->A.
			 * We have to prevent it from occurring.
			 */
			if (ptmp == pcur)
				break;
			ptmp = pcur;
		}
	}
	if (bClr) /* If list is a circular list, then restore it in the copy. */
		pnew->pnode = prtn;
	return prtn;
}

/* Function name: strCompareLinkedListSC
 * Description:   Compares linked-list X to linked-list Y.
 * Parameters:
 *      listx A linked-list to be compared.
 *      listy Another linked-list to be compared.
 *     cbfcmp Pointer to a function that compares two elements in NODE_S structures.
 *            Comparison takes two pointers as arguments, the first one is always pdata of listx
 *            and the second one points to pdata of listy. Both of them are type-cast to (const void *).
 * Return value:   1: listx is greater than listy.
 *                 0: listx equal to listy.
 *                -1: listx is less than listy.
 *                The following table shows characteristics about this function:
 *                listx  listy result
 *                abcde  abdef     -1
 *                bcdef  abcde      1
 *                abcde  abcde      0
 *                abc    abcd      -1
 *                abcd   abc        1
 * Caution:       Data in each node of two linked-lists must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
int strCompareLinkedListSC(LIST_S listx, LIST_S listy, CBF_COMPARE cbfcmp)
{
	REGISTER int rtn = 0;
	REGISTER P_NODE_S pstatx = listx;
	REGISTER P_NODE_S pstaty = listy;
	while ((NULL != listx) && (NULL != listy))
	{
		if (0 != (rtn = cbfcmp(listx->pdata, listy->pdata)))
			break;
		listx = listx->pnode;
		listy = listy->pnode;
		if ((pstatx == listx) || (pstaty == listy)) /* Circular list. */
			break;
	}
	if (0 == rtn) /* Determine results when two lists have a same overlapped part. */
	{
		size_t rx = strLevelLinkedListSC(listx);
		size_t ry = strLevelLinkedListSC(listy);
		if (rx > ry)
			return 1;
		if (rx < ry)
			return -1;
		else
			return 0;
	}
	return rtn;
}

/* Function name: strSearchLinkedListSC
 * Description:   Find pitem in a single linked-list list.
 * Parameters:
 *       list Pointer to the first NODE_S element you want to search in the linked-list.
 *      pitem Pointer to the item you want to search.
 *       size Size of data of pitem.
 * Return value:  Pointer to the NODE_S which contains the same data as pitem.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S strSearchLinkedListSC(LIST_S list, const void * pitem, size_t size)
{
	FindingInfo fi;
	fi.result = NULL;
	fi.pitem  = pitem;
	fi.size   = size;
	strTraverseLinkedListSC_X(list, NULL, _strCBFCompareNodeDataS, (size_t)&fi);
	return (P_NODE_S)fi.result;
}

/* Function name: strLocatePreviousItemSC
 * Description:   Locate an item before pitem in linked-list plist.
 * Parameters:
 *       list Pointer to the first NODE_S element of linked-list.
 *      pnode Pointer to the current node.
 * Return value:  Pointer to the item before pitem.
 * Caution:       If pnode equals to list, function would return value NULL;
 * Tip:           No dead cycles for circular linked-lists.
 *                Node list must locate before the node you want to find.
 */
P_NODE_S strLocatePreviousItemSC(LIST_S list, P_NODE_S pnode)
{
	if (list != pnode)
	{
		REGISTER P_NODE_S pcur = list;
		REGISTER P_NODE_S ptmp = NULL;
		while (NULL != pcur)
		{
			if (pcur->pnode == pnode)
				return pcur;
			pcur = pcur->pnode;
			if (pnode == pcur) /* Circular list. */
				break;
			else
			{	/* Dead loop appears only if B->B and A->B.
				 * We have to prevent it from occurring.
				 */
				if (ptmp == pcur)
					break;
				ptmp = pcur;
			}
		}
	}
	return NULL;
}

/* Function name: strLocateLastItemSC
 * Description:   Locate the last item in linked-list.
 * Parameter:
 *      list Pointer to the first NODE_S element of linked-list.
 * Return value:  Pointer to the last node.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S strLocateLastItemSC(LIST_S list)
{
	REGISTER P_NODE_S plast = list;
	REGISTER P_NODE_S ptmp  = NULL;
	if (NULL != list)
	{
		while ((NULL != plast->pnode) && (list != plast->pnode))
		{
			plast = plast->pnode;
			/* Dead loop appears only if B->B and A->B.
			 * We have to prevent it from occurring.
			 */
			if (ptmp == plast)
				break;
			ptmp = plast;
		}
	}
	return plast;
}

/* Function name: strLocateItemSC_R
 * Description:   Locate a node in a single linked-list recursively.
 * Parameters:
 *      pnode Pointer to the current node.
 *     incmtl Incremental of location.
 * Return value:  Pointer to the NODE_S which forwarding incmtl numbers of nodes from the start.
 * Tip:           A counterpart named strLocateItemSC_N could be use.
 *                Compilers that installed tail recursive optimization would facilitate this procedure.
 */
P_NODE_S strLocateItemSC_R(P_NODE_S pnode, size_t incmtl)
{
	if (NULL != pnode && incmtl != 0)
		return strLocateItemSC_R(pnode->pnode, --incmtl);
	return pnode;
}

/* Function name: strLocateItemSC_N
 * Description:   Locate a node in a single linked-list by using a loop.
 * Parameters:
 *      pnode Pointer to the current node.
 *     incmtl Incremental of location.
 * Return value:  Pointer to the NODE_S which forwarding incmtl numbers of nodes from the start.
 * Tip:           A counterpart named strLocateItemSC_R could be use.
 */
P_NODE_S strLocateItemSC_N(P_NODE_S pnode, size_t incmtl)
{
	while (NULL != pnode)
	{
		if (0 == incmtl)
			return pnode;
		else
		{
			--incmtl;
			pnode = pnode->pnode;
		}
	}
	return NULL;
}

/* Function name: strInsertItemLinkedListSC
 * Description:   Insert an item into a linked-list.
 * Parameters:
 *       list Pointer to the first NODE_S element of linked-list.
 *      pdest Pointer to the node you want to operate with.
 *      pnode Pointer to a single link node that you want to insert.
 *     bafter Input true to insert pnode in front of pdest.
 *            Input false to insert pnode after pdest.
 * Return value:  The same pointer as pnode. If function returned value NULL, it should indicate an insertion failure.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S strInsertItemLinkedListSC(LIST_S list, P_NODE_S pdest, P_NODE_S pnode, bool bafter)
{
	if (NULL == pdest || NULL == pnode)
		return NULL;
	if (bafter)
	{	/* Locate tail in pnode headed linked-list. */
		P_NODE_S plast = strLocateLastItemSC(pnode);
		plast->pnode = pdest->pnode;
		return pdest->pnode = pnode;
	}
	else /* Previous. */
	{
		P_NODE_S pr = strLocatePreviousItemSC(list, pdest);
		if (NULL != pr)
			return strInsertItemLinkedListSC(list, pr, pnode, true);
		else
			return strInsertItemLinkedListSC(NULL, strLocateLastItemSC(pnode), pdest, true);
	}
}

/* Function name: strRemoveItemLinkedListSC
 * Description:   Remove an item from a linked-list.
 * Parameters:
 *       list Pointer to the first NODE_S element of linked-list.
 *      pnode Pointer to a single-pointer node you want to remove.
 * Return value:  The same pointer as pnode.
 * Caution:       You may need to alter the header node when list == pnode.
 * Tip:           No dead cycles for circular linked-lists.
 *                Function will NOT deallocate the address of pnode.
 *                Use strFreeNodeS(strRemoveItemLinkedListSC(list, pnode)); to free a removed node.
 */
P_NODE_S strRemoveItemLinkedListSC(LIST_S list, P_NODE_S pnode)
{
	P_NODE_S prev = strLocatePreviousItemSC(list, pnode);
	P_NODE_S pnxt = pnode->pnode;
	if (NULL != prev)
		prev->pnode = pnxt;
	pnode->pnode = NULL;
	return pnode;
}

/* Function name: strReverseLinkedListSC
 * Description:   Reverse a circular single pointer linked-list.
 * Parameter:
 *     phead Pointer to the first node of the circular single linked-list.
 * Return value:  Pointer to the new header of the circular single linked-list.
 * Tip:           This function can not reverse a single linked-list that:
 *                A->B->C->B.
 */
P_NODE_S strReverseLinkedListSC(LIST_S phead)
{
	REGISTER P_NODE_S prev = NULL;
	REGISTER P_NODE_S pcur = phead;
	REGISTER P_NODE_S pnxt = NULL;
	REGISTER P_NODE_S psta = NULL;

	while (NULL != pcur && psta != pcur)
	{
		if (NULL == psta)
			psta = phead;

		/* Store the next node. */
		pnxt = pcur->pnode;

		/* Reverse the pointer of current node. */
		pcur->pnode = prev;

		/* Move pointers one node forward. */
		prev = pcur;
		pcur = pnxt;
	}

	return prev;
}

/* Function name: strSwapItemLinkedListS
 * Description:   Swap data pointer between two nodes.
 * Parameters:
 *     pnodex Pointer to a node with a single link.
 *     pnodey Pointer to another node with a single link.
 * Return value:  N/A.
 * Tip:           This function only swap data pointer between two nodes.
 *                If you need to swap data that stored in the address of two pointers,
 *                please use function strSwapContentLinkedListSD.
 */
void strSwapItemLinkedListS(P_NODE_S pnodex, P_NODE_S pnodey)
{
	if (pnodex->pdata != pnodey->pdata)
	{	/* Worth swapping. */
		PUCHAR pbuf;
		svSwap(&pnodex->pdata, &pnodey->pdata, &pbuf, sizeof(PUCHAR));
	}
}

/* Function name: strQuickSortLinkedListS
 * Description:   Quickly sort a single pointer linked-list.
 * Parameters:
 *      phead Pointer to the first node of the single linked-list to be sorted.
 *     cbfcmp Pointer to a function that compares two elements in nodes.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  This function would return a new header for sorted single linked-list. 
 * Tip:           This function can not sort circular single linked-list.
 *                Quick sort algorithm is used for this function.
 */
LIST_S strQuickSortLinkedListS(LIST_S phead, CBF_COMPARE cbfcmp)
{
	if (NULL == phead || NULL == phead->pnode)
		return phead;
	else
	{
		REGISTER P_NODE_S pivot = phead;
		REGISTER P_NODE_S pleft = NULL;
		REGISTER P_NODE_S pright = NULL;
		REGISTER P_NODE_S pnode = phead->pnode;
		REGISTER P_NODE_S ptemp;

		while (NULL != pnode)
		{
			P_NODE_S pcur = pnode;
			pnode = pnode->pnode;

			if (cbfcmp(pcur->pdata, pivot->pdata) < 0)
			{
				pcur->pnode = pleft;
				pleft = pcur;
			}
			else
			{
				pcur->pnode = pright;
				pright = pcur;
			}
		}

		pleft = strQuickSortLinkedListS(pleft, cbfcmp);
		pright = strQuickSortLinkedListS(pright, cbfcmp);

		pivot->pnode = pright;
		if (NULL == pleft)
			return pivot;

		ptemp = pleft;
		while (NULL != ptemp->pnode)
		{
			ptemp = ptemp->pnode;
		}
		ptemp->pnode = pivot;

		return pleft;
	}
}

/* Section for doubly-pointer-node linked-list started bellow.  */

/* Function name: strTraverseLinkedListDC_R
 * Description:   Recursively traverse a doubly-pointer-node linked-list.
 * Parameters:
 *       list Pointer to the first NODE_D element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to the callback function.
 *      param Additional information for each node.
 *       brev If brev equaled true, doubly linked-list would be traversed reversely.
 *             That is the order of deallocation which were from the current one to the previous one.
 *             If brev equaled false, the order of deallocation would run from the current one to the next one.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will be returned.
 * Tip:           No dead cycles for circular linked-lists.
 */
int strTraverseLinkedListDC_R(LIST_D list, P_NODE_D pnil, CBF_TRAVERSE cbftvs, size_t param, bool brev)
{
	int r = CBF_CONTINUE;
	if (NULL != list)
	{
		if (list != pnil)
		{
			if (NULL == pnil)
				pnil = list;
			if (brev)
			{
				r = strTraverseLinkedListDC_R(list->ppnode[PREV], pnil, cbftvs, param, brev);
				if (CBF_CONTINUE != cbftvs(list, param))
					return CBF_TERMINATE;
			}
			else
			{
				r = strTraverseLinkedListDC_R(list->ppnode[NEXT], pnil, cbftvs, param, brev);
				if (CBF_CONTINUE != cbftvs(list, param))
					return CBF_TERMINATE;
			}
		}
	}
	return r;
}

/* Function name: strTraverseLinkedListDC_A
 * Description:   Recursively traverse a doubly-pointer-node linked-list.
 *                The order of traversal is opposite of strTraverseLinkedListDC_R.
 * Parameters:
 *       list Pointer to the first NODE_D element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to the callback function.
 *      param Additional information for each node.
 *       brev If brev equaled true, doubly linked-list would be traversed reversely.
 *            That is the order of deallocation which were from the current one to the previous one.
 *            If brev equaled false, the order of deallocation would run from the current one to the next one.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will be returned.
 * Tip:           No dead cycles for circular linked-lists.
 */
int strTraverseLinkedListDC_A(LIST_D list, P_NODE_D pnil, CBF_TRAVERSE cbftvs, size_t param, bool brev)
{
	if (NULL != list)
	{
		if (list != pnil)
		{
			if (NULL == pnil)
				pnil = list;
			if (brev)
			{
				if (CBF_CONTINUE != cbftvs(list, param))
					return CBF_TERMINATE;
				return strTraverseLinkedListDC_A(list->ppnode[PREV], pnil, cbftvs, param, brev);
			}
			else
			{
				if (CBF_CONTINUE != cbftvs(list, param))
					return CBF_TERMINATE;
				return strTraverseLinkedListDC_A(list->ppnode[NEXT], pnil, cbftvs, param, brev);
			}
		}
	}
	return CBF_CONTINUE;
}

/* Function name: strTraverseLinkedListDC_N
 * Description:   Traverse a single-pointer-node linked-list using a loop.
 *                The order of traversal is as the same as strTraverseLinkedListDC_A.
 * Parameters:
 *       list Pointer to the first NODE_D element while traversal.
 *       pnil Please Leave It As NULL.
 *     cbftvs Pointer to the callback function.
 *      param Additional information for each node.
 *       brev If brev equaled true, doubly linked-list would be traversed reversely.
 *            That is the order of deallocation which were from the current one to the previous one.
 *            If brev equaled false, the order of deallocation would run from the current one to the next one.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will be returned.
 * Tip:           No dead cycles for circular linked-lists.
 */
int strTraverseLinkedListDC_N(LIST_D list, P_NODE_D pnil, CBF_TRAVERSE cbftvs, size_t param, bool brev)
{
	REGISTER P_NODE_D pcur = list;
	for ( ; NULL != pcur; pcur = (brev ? pcur->ppnode[PREV] : pcur->ppnode[NEXT]))
	{
		/* Break at the header of a circular list. */
		if (NULL != pnil && pcur == list)
			break;
		/* Dead loop appears only if A->B and B->B.
		 * We have to prevent it from occurring.
		 */
		if (pnil == pcur)
			break;
		if (CBF_CONTINUE != cbftvs(pcur, param))
			return CBF_TERMINATE;
		pnil = pcur;
	}
	return CBF_CONTINUE;
}

/* Function name: strInitLinkedListDC_O
 * Description:   Initialize a double-pointer-node linked-list.
 * Parameter:
 *     plist Pointer to the list you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of plist Must Be Allocated first.
 * Tip:           A macro version of this function named strInitLinkedListDC_M is available.
 */
void strInitLinkedListDC_O(P_LIST_D plist)
{
	*plist = NULL;
}

/* Function name: strFreeLinkedListDC
 * Description:   Deallocate a single-pointer-node linked-list.
 * Parameter:
 *     plist Pointer to the plist you want to free.
 *      brev If brev equaled true, doubly linked-list would be traversed reversely.
 *            That is the order of deallocation which were from the current one to the previous one.
 *            If brev equaled false, the order of deallocation would run from the current one to the next one.
 * Return value:  N/A.
 * Caution:       Address of plist Must Be Allocated first.
 */
void strFreeLinkedListDC(P_LIST_D plist, bool brev)
{
	strTraverseLinkedListDC_R(*plist, NULL, _strCBFDeleteNode, ENT_DOUBLE, brev);
	strInitLinkedListDC(plist);
}

/* Function name: strCreateLinkedListDC
 * Description:   Create a new doubly-pointer-node linked-list.
 * Parameter:     N/A.
 * Return value:  Pointer to the new list.
 */
P_LIST_D strCreateLinkedListDC(void)
{
	P_LIST_D plist = (P_LIST_D) malloc(sizeof(LIST_D));
	if (NULL == plist)
		return NULL; /* Allocation failure. */
	strInitLinkedListDC(plist);
	return plist;
}

/* Function name: strDeleteLinkedListDC_O
 * Description:   Delete a doubly-pointer-node linked-list.
 * Parameter:
 *     plist Pointer to the list you want to delete.
 * Return value:  N/A.
 * Caution:       Parameter plist Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteLinkedListDC_M is available.
 */
void strDeleteLinkedListDC_O(P_LIST_D plist, bool brev)
{
	strFreeLinkedListDC(plist, brev);
	free(plist);
}

/* Function name: strLevelLinkedListDC
 * Description:   Return how many nodes there are stored in a doubly linked-list.
 * Parameters:
 *       list Pointer to the first NODE_D element while traversal.
 *       brev If brev equaled true, doubly linked-list would be traversed reversely.
 *            That is the order of deallocation which were from the current one to the previous one.
 *            If brev equaled false, the order of deallocation would go from the current one to the next one.
 * Return value:  The number of nodes in a doubly linked-list.
 * Tip:           No dead cycles to circular linked-lists.
 */
size_t strLevelLinkedListDC(LIST_D list, bool brev)
{
	size_t l = 0;
	strTraverseLinkedListDC_X(list, NULL, _strCBFNodesCounter, (size_t)&l, brev);
	return l;
}

/* Function name: strCopyLinkedListDC
 * Description:   Copy the entire doubly linked-list.
 * Parameters:
 *       psrc Pointer to the header of a linked-list.
 *       size Size of data in each node.
 *       brev If brev equaled true, copying procedure would start at the current node and search for the NEXT node.
 *            Otherwise copying procedure will start at the current node and search for the PREVIOUS node.
 *            If the doubly linked-list had only one direction in clockwise or counterclockwise,
 *            to adjust this parameter could be useful to redirect copying.
 * Return value:  The first element of new linked-list.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_D strCopyLinkedListDC(LIST_D psrc, size_t size, bool brev)
{
	REGISTER bool bClr = false;
	REGISTER P_NODE_D pcur = psrc;
	REGISTER P_NODE_D ptmp = NULL;
	REGISTER P_NODE_D pnew = NULL;
	REGISTER P_NODE_D pdst = NULL;
	P_NODE_D prtn = NULL;
	while (NULL != pcur)
	{
		if (NULL == (pnew = strCreateNodeD(pcur->pdata, size)))
		{
			strFreeLinkedListDC(&prtn, brev);
			return NULL;
		}
		if (NULL == prtn)
			prtn = pnew;
		if (NULL != pdst)
		{
			if (brev)
			{
				pdst->ppnode[PREV] = pnew;
				pnew->ppnode[NEXT] = pdst;
			}
			else
			{
				pdst->ppnode[NEXT] = pnew;
				pnew->ppnode[PREV] = pdst;
			}
		}
		pdst = pnew;
		if (brev)
			pcur = pcur->ppnode[PREV];
		else
			pcur = pcur->ppnode[NEXT];
		if (psrc == pcur) /* Circular list. */
		{
			bClr = true;
			break;
		}
		else
		{	/* Dead loop appears only if ->A<- and A<-B->A.
			 * We have to prevent it from occurring.
			 */
			if (ptmp == pcur)
				break;
			ptmp = pcur;
		}
	}
	if (bClr) /* If the list is a circular list, restore it in the copy. */
	{
		if (brev)
		{
			pnew->ppnode[PREV] = prtn;
			prtn->ppnode[NEXT] = pnew;
		}
		else
		{
			pnew->ppnode[NEXT] = prtn;
			prtn->ppnode[PREV] = pnew;
		}
	}
	return prtn;
}

/* Function name: strCompareLinkedListDC
 * Description:   Compare linked-list list X to linked-list list Y.
 * Parameters:
 *     listx The first linked-list to be compared.
 *     listy The second linked-list to be compared.
 *     cbfcmp Pointer to a function that compares two elements in a NODE_D.
 *            Comparison takes two pointers as arguments, the first one is always the pdata of listx
 *            and the second one points to pdata of listy. Both of them are type-cast to const void pointer.
 *       brev If brev equaled true, function would do comparison from current node to the next node.
 *            If brev equaled false, function would perform comparison reversely.
 * Return value:   1: listx is greater than listy.
 *                 0: listx equal to listy.
 *                -1: listx is less than listy.
 *                Please refer to function strCompareLinkedListSC for more details.
 * Caution:       Data in each node of two linked-lists must be in the same size.
 *                If the following situation occurred, dead cycle would happen:
 *                (->A<-) and (A<-B->A).
 *                (->A<-)   means A.ppnode[PREV] == &A && A.ppnode[NEXT] == &A;
 *                (A<-B->A) means B.ppnode[PREV] == &A && B.ppnode[NEXT] == &A;
 * Tip:           No dead cycles for other circular linked-lists.
 */
int strCompareLinkedListDC(LIST_D listx, LIST_D listy, CBF_COMPARE cbfcmp, bool brev)
{
	int rtn = 0;
	REGISTER P_NODE_D pstatx = listx;
	REGISTER P_NODE_D pstaty = listy;
	while ((NULL != listx) && (NULL != listy))
	{
		if (0 != (rtn = cbfcmp(listx->pdata, listy->pdata)))
			break;
		if (brev)
		{
			listx = listx->ppnode[PREV];
			listy = listy->ppnode[PREV];
		}
		else
		{
			listx = listx->ppnode[NEXT];
			listy = listy->ppnode[NEXT];
		}
		if ((pstatx == listx) || (pstaty == listy)) /* Circular list. */
			break;
	}
	if (0 == rtn) /* Determine whether two lists are in the same size. */
	{
		size_t rx = strLevelLinkedListDC(listx, brev);
		size_t ry = strLevelLinkedListDC(listy, brev);
		if (rx > ry)
			return  1;
		if (rx < ry)
			return -1;
		else
			return  0;
	}
	return rtn;
}

/* Function name: strSearchLinkedListDC
 * Description:   Find pitem in a doubly linked-list list.
 * Parameters:
 *       list Pointer to the first NODE_D element when traversal.
 *      pitem Pointer to the item you want to search in the doubly linked-list.
 *       size Size of data of pitem.
 *       brev If brev equaled true, function would search linked-list from current node to the next node.
 *            If brev equaled false, function would search linked-list reversely.
 * Return value:  Pointer to the NODE_S which contains the same data as pitem.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_D strSearchLinkedListDC(LIST_D list, const void * pitem, size_t size, bool brev)
{
	FindingInfo fi;
	fi.result = NULL;
	fi.pitem  = pitem;
	fi.size   = size;
	strTraverseLinkedListDC_X(list, NULL, _strCBFCompareNodeDataD, (size_t)&fi, brev);
	return (P_NODE_D)fi.result;
}

/* Function name: strLocateItemDC_R
 * Description:   Locate a node in a doubly linked-list recursively.
 * Parameters:
 *      pnode Pointer to the current node.
 *     incmtl Incremental of location.
 *            If incmtl  > 0, locate nodes forward.
 *            If incmtl  < 0, locate nodes backward.
 *            If incmtl == 0, function would return the pointer to the current node, that was the same value as pnode stored.
 * Return value:  Pointer to the NODE_D which is located forward or backward for incmtl numbers of nodes.
 * Caution:       (*) Addressing capability of this function limited in the range of which a ptrdiff_t integer could perform.
 * Tip:           This function worked on acyclic doubly pointer linked lists as well as cyclic lists.
 *                You may use this function conveniently like these ways:
 *                pnode = strLocateItemDC_R(pnode, +5); // Returns the pointer of 5 nodes next if possible.
 *                pnode = strLocateItemDC_R(pnode, -3); // Returns the pointer of 3 nodes previously.
 *                pnode = strLocateItemDC_R(pnode,  0); // Returns the same value as pnode.
 */
P_NODE_D strLocateItemDC_R(P_NODE_D pnode, ptrdiff_t incmtl)
{
	if (NULL != pnode && incmtl != 0)
	{
		if (incmtl > 0)
			return strLocateItemDC_R(pnode->ppnode[NEXT], --incmtl);
		else
			return strLocateItemDC_R(pnode->ppnode[PREV], ++incmtl);
	}
	return pnode;
}

/* Function name: strLocateItemDC_N
 * Description:   Locate a node in a doubly linked-list by using a loop.
 * Parameters:
 *      pnode Pointer to the current node.
 *     incmtl Incremental of location.
 *            If incmtl  > 0, locate nodes forward.
 *            If incmtl  < 0, locate nodes backward.
 *            If incmtl == 0, function would return the pointer to the current node, that was the same value as pnode stored.
 * Return value:  Pointer to the NODE_D which is located forward or backward for incmtl numbers of nodes.
 * Caution:       (*) Addressing capability of this function limited in the range which a ptrdiff_t integer could perform.
 * Tip:           This function worked on acyclic doubly pointer linked lists as well as cyclic lists.
 *                (*) Compare this function with its recursive version, either two functions might be optimized by compilers.
 *                If your compiler supported tail recursive optimization, strLocateItemDC_R would be optimized, and its size
 *                would be reduced; Its execution speed would be accelerated, unless to use strLocateItemDC_N is a good way to
 *                fit both goals of a high speed and a small size for your programs.
 */
P_NODE_D strLocateItemDC_N(P_NODE_D pnode, ptrdiff_t incmtl)
{
	while (NULL != pnode)
	{
		if (0 == incmtl)
			return pnode;
		else if (incmtl > 0)
		{
			--incmtl;
			pnode = pnode->ppnode[NEXT];
		}
		else
		{
			++incmtl;
			pnode = pnode->ppnode[PREV];
		}
	}
	return NULL;
}

/* Function name: strInsertItemLinkedListDC
 * Description:   Insert an item into linked-list.
 * Parameters:
 *      pdest Pointer to the first NODE_D element of linked-list.
 *      pnode Pointer to a double link node you want to insert.
 *     bafter Input true to insert pnode in front of pdest.
 *            Input false to insert pnode after pdest.
 * Return value:  The new header of the current linked-list.
 * Caution:       If the following situation occurred, dead cycle would happen:
 *                (->A<-) and (A<-B->A).
 *                (->A<-)   means A.ppnode[PREV] == &A && A.ppnode[NEXT] == &A;
 *                (A<-B->A) means B.ppnode[PREV] == &A && B.ppnode[NEXT] == &A;
 * Tip:           No dead cycles for other circular linked-lists.
 */
P_NODE_D strInsertItemLinkedListDC(P_NODE_D pdest, P_NODE_D pnode, bool bafter)
{
	REGISTER P_NODE_D ptmp = NULL;
	REGISTER P_NODE_D plast = pnode;
	if (NULL == pdest || NULL == pnode)
		return NULL;
	/* Locate tail in pitem. */
	while ((NULL != plast->ppnode[NEXT]) && (pnode != plast->ppnode[NEXT]))
	{
		plast = plast->ppnode[NEXT];
		/* Dead loop appears only if ->A<- and A<-B->A.
		 * We have to prevent it from occurring.
		 */
		if (ptmp == plast)
			break;
		ptmp = plast;
	}
	if (bafter)
	{
		plast->ppnode[NEXT] = pdest->ppnode[NEXT];
		if (NULL != pdest->ppnode[NEXT])
			pdest->ppnode[NEXT]->ppnode[PREV] = plast;
		pdest->ppnode[NEXT] = pnode;
		pnode->ppnode[PREV] = pdest;
		return pdest;
	}
	else /* Before. */
	{
		plast->ppnode[NEXT] = pdest;
		if (NULL != (pnode->ppnode[PREV] = pdest->ppnode[PREV]))
		{
			pdest->ppnode[PREV]->ppnode[NEXT] = pnode;
			pdest->ppnode[PREV] = plast;
		}
		return pnode;
	}
}

/* Function name: strRemoveItemLinkedListDC
 * Description:   Remove an item from a doubly pointer linked-list.
 * Parameter:
 *     pnode Pointer to a double link node you want to remove.
 * Return value:  The same pointer as pnode.
 * Tip:           No dead cycles for circular linked-lists.
 *                Function will NOT deallocate the address of pnode rather than put two pointers in pnode into value NULL.
 *                Use strFreeNodeD(strRemoveItemLinkedListDC(pnode)); to free a removed node.
 *                Use strDeleteNodeD(strRemoveItemLinkedListDC(pnode)); to delete an allocated node from list.
 */
P_NODE_D strRemoveItemLinkedListDC(P_NODE_D pnode)
{
	if (NULL != pnode->ppnode[PREV])
		pnode->ppnode[PREV]->ppnode[NEXT] = pnode->ppnode[NEXT];
	if (NULL != pnode->ppnode[NEXT])
		pnode->ppnode[NEXT]->ppnode[PREV] = pnode->ppnode[PREV];
	pnode->ppnode[PREV] = pnode->ppnode[NEXT] = NULL;
	return pnode;
}

/* Function name: strSwapItemLinkedListD
 * Description:   Swap data pointer between two nodes.
 * Parameters:
 *     pnodex Pointer to a node with double pointers.
 *     pnodey Pointer to another node with double pointers.
 * Return value:  N/A.
 * Tip:           This function only swap data pointer between two nodes.
 *                If you wish to swap data stored in two pointers, please use strSwapContentLinkedListSD.
 */
void strSwapItemLinkedListD(P_NODE_D pnodex, P_NODE_D pnodey)
{
	if (pnodex->pdata != pnodey->pdata)
	{	/* Worth swapping. */
		PUCHAR pbuf;
		svSwap(&pnodex->pdata, &pnodey->pdata, &pbuf, sizeof(PUCHAR));
	}
}

/* Function name: strSwapContentLinkedListSD
 * Description:   Swap data between two nodes.
 * Parameters:
 *     pnodex Pointer to a node with a single link.
 *      sizex Size of data in pnodex.
 *     pnodey Pointer to a node with a single link.
 *      sizey Size of data in pnodey.
 *        ntp Node type.
 *            Input ENT_DOUBLE for doubly linked-list nodes. Cast P_NODE_D to (void *) as parameters.
 *            Input ENT_SINGLE for single linked-list nodes. Cast P_NODE_S to (void *) as parameters.
 * Return value:  N/A.
 * Tip:           This function will swap content between two pointers in their nodes.
 */
void strSwapContentLinkedListSD(void * pnodex, size_t sizex, void * pnodey, size_t sizey, NodeType ntp)
{
	REGISTER PUCHAR pdatax, pdatay;
	REGISTER PUCHAR pbuf,   pnew;
	switch (ntp)
	{
	case ENT_SINGLE:
		pdatax = (((P_NODE_S)pnodex)->pdata);
		pdatay = (((P_NODE_S)pnodey)->pdata);
		break;
	case ENT_DOUBLE:
		pdatax = (((P_NODE_D)pnodex)->pdata);
		pdatay = (((P_NODE_D)pnodey)->pdata);
		break;
	default:
		return;
	}
	if (NULL == (pbuf = (PUCHAR) malloc(sizex)))
		return;
	memcpy(pbuf, pdatax, sizex);        /* t = a; */
	if (NULL != (pnew = (PUCHAR) realloc(pdatax, sizey)))
	{
		pdatax = pnew;
		memmove(pdatax, pdatay, sizey); /* a = b; */
	}
	if (NULL != (pnew = (PUCHAR) realloc(pdatay, sizex)))
	{
		pdatay = pnew;
		memcpy(pdatay, pbuf, sizex);    /* b = t; */
	}
	free(pbuf);
}

/* Function name: strIsCircularLinkedListSD
 * Description:   Make a decision whether a linked-list is circular or not.
 * Parameters:
 *     pfirst Pointer to the first node element while traversal.
 *        ntp Node type.
 *            Input ENT_DOUBLE for doubly linked-list nodes. Cast P_NODE_D to (void *) as parameters.
 *            Input ENT_SINGLE for single linked-list nodes. Cast P_NODE_S to (void *) as parameters.
 *       brev Input true to search a linked-list reversely.
 *            Input false to search a linked-list in order.
 * Return value:  Pointer of the last item of the linked-list will be cast into (void *) and returned.
 */
void * strIsCircularLinkedListSD(void * pfirst, NodeType ntp, bool brev)
{	/* Unfortunately, the pursuing of interchangeability made this function difficult to read. */
#define _P2P_NODE_S(pnode) ((P_NODE_S)(pnode)) /* Cast a pointer to P_NODE_S. */
#define _P2P_NODE_D(pnode) ((P_NODE_D)(pnode)) /* Cast a pointer to P_NODE_D. */
	REGISTER void * plast = pfirst;
	REGISTER void * ptmp = NULL;
	if (NULL != pfirst && (ENT_SINGLE == ntp || ENT_DOUBLE == ntp))
	{
		while
		(
			(ENT_SINGLE == ntp) ?
			((NULL != _P2P_NODE_S(plast)->pnode) && (pfirst != _P2P_NODE_S(plast)->pnode)) :
			(
				brev ?
				((NULL != _P2P_NODE_D(plast)->ppnode[PREV]) && (pfirst != _P2P_NODE_D(plast)->ppnode[PREV])) :
				((NULL != _P2P_NODE_D(plast)->ppnode[NEXT]) && (pfirst != _P2P_NODE_D(plast)->ppnode[NEXT]))
			)
		)
		{
			plast = (ENT_SINGLE == ntp ?
						(void *) _P2P_NODE_S(plast)->pnode :
						(void *)
						(brev ?
							_P2P_NODE_D(plast)->ppnode[PREV] :
							_P2P_NODE_D(plast)->ppnode[NEXT]
						)
					);
			if (ptmp == plast) /* Prevent function from a dead loop. */
				break;
			ptmp = plast;
		}
	}
	else
		return NULL;
	if
	(
		(ptmp =
			((ENT_SINGLE == ntp) ?
			(void *)(_P2P_NODE_S(plast)->pnode) :
			(void *)
				(brev ?
					(void *)(_P2P_NODE_D(plast)->ppnode[PREV]) :
					(void *)(_P2P_NODE_D(plast)->ppnode[NEXT])
				)
			)
		)
		== pfirst
	)
		return plast;
	else
		return NULL;
#undef _P2P_NODE_S
#undef _P2P_NODE_D
}

/* Function name: strMergeSortLinkedListSDC
 * Description:   Merge sort a linked-list.
 * Parameters:
 *       list Pointer to the first node of the linked-list to be sorted, cast into void *.
 *  bCircular true for circular linked-list, false for NON-circular linked-list.
 *        ntp Determine whether this is a doubly-linked-list or a single-linked-list.
 *            Please refer to NodeType enumeration at svstring.h.
 *     cbfcmp Pointer to a function that compares two elements in nodes.
 *            Please refer to the type definition CBF_COMPARE in svdef.h.
 * Return value:  This function would return a new header for sorted linked-list. 
 * Tip:           This function CAN sort circular single/double linked-list after parameter bCircular was set.
 *                Merge sort algorithm is used for this function.
 *                This function refers to web: https://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.html
 *                Thank him for his idea and code!
 */
void * strMergeSortLinkedListSDC(void * list, bool bCircular, NodeType ntp, CBF_COMPARE cbfcmp)
{
	REGISTER void * p, * q, * e, * tail, * oldhead;
	REGISTER size_t insize, nmerges, psize, qsize, i;
	REGISTER P_NODE_S pa, pb;
	REGISTER P_NODE_D px, py;
	REGISTER void * p1, * p2;

	if (NULL == list)
		return NULL;

	insize = 1;

	for ( ;; )
	{
		p = list;
		oldhead = list; /* Used for circular linkage. */
		list = NULL;
		tail = NULL;

		/* Counter of merges we do in this pass. */
		nmerges = 0;

		while (NULL != p)
		{
			/* There exists a merge to be done. */
			++nmerges;
			/* Step 'insize' places along from p. */
			q = p;
			psize = 0;
			for (i = 0; i < insize; ++i)
			{
				++psize;
				if (bCircular)
					q = (*(void **)q == oldhead ? NULL : *(void **)q);
				else
					q = *(void **)q;
				if (NULL == q)
					break;
			}

			/* If q hasn't fallen off end, we have two lists to merge. */
			qsize = insize;

			/* Now we have two lists; Merge them. */
			while (psize > 0 || (qsize > 0 && NULL != q))
			{
				/* Decide whether the next element of merge comes from p or q. */
				if (0 == psize)
				{
					/* p is empty; e must come from q. */
					e = q;
					q = *(void **)q;
					--qsize;
					if (bCircular && q == oldhead)
						q = NULL;
				}
				else if (qsize == 0 || NULL == q)
				{
					/* q is empty; e must come from p. */
					e = p;
					p = *(void **)p;
					--psize;
					if (bCircular && p == oldhead)
						p = NULL;
				}
				else if
				(
					(pa = (P_NODE_S)p),
					(pb = (P_NODE_S)q),
					(px = (P_NODE_D)p),
					(py = (P_NODE_D)q),
					(p1 = (void *)(ENT_DOUBLE != ntp ? pa->pdata : px->pdata)),
					(p2 = (void *)(ENT_DOUBLE != ntp ? pb->pdata : py->pdata)),
					cbfcmp(p1, p2) <= 0
				)
				{
					/* First element of p is lower (or same); e must come from p. */
					e = p;
					p = *(void **)p;
					--psize;
					if (bCircular && p == oldhead)
						p = NULL;
				}
				else
				{
					/* First element of q is lower; e must come from q. */
					e = q;
					q = *(void **)q;
					--qsize;
					if (bCircular && q == oldhead)
						q = NULL;
				}

				/* Add the next element to the merged list. */
				if (NULL != tail)
					*(void **)tail = e;
				else
					list = e;
				
				/* Maintain reverse pointers in a doubly linked list. */
				if (ENT_DOUBLE == ntp)
					*((void **)e + PREV) = tail;
				
				tail = e;
			}

			/* Now p has stepped 'insize' places along, and q has too. */
			p = q;
		}

		if (bCircular)
		{
			if (NULL != tail)
				*(void **)tail = list;
			if (ENT_DOUBLE == ntp && NULL != list)
				*((void **)list + PREV) = tail;
		}
		else if (NULL != tail)
			*(void **)tail = NULL;

		/* If we have done only one merge, we're finished. */
		if (nmerges <= 1) /* Allow for nmerges == 0, the empty list case. */
			return list;

		/* Otherwise repeat, merging lists twice the size. */
		insize <<= 1;
	}
}

