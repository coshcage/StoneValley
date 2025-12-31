/*
 * Name:        svgtree.c
 * Description: Generic trees.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0809171737H0615252050L00562
 * License:     LGPLv3
 * Copyright (C) 2017-2026 John Cage
 * Copyright (C) 2026      Sarah Silva @github.com/sah524
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
#include <string.h> /* Using function memcpy, memcpy. */
#include "svtree.h"
#include "svqueue.h"

/* A structure for generic tree nodes' copy. */
typedef struct _st_GTreeCopy {
	void *    pnroot; /* Root of new tree.    */
	P_QUEUE_L pquel;  /* Children node queue. */
	size_t    size;   /* Node data size.      */
} _GTreeCopy, * _P_GTreeCopy;

/* Callback function for generic trees. */
extern int _strCBFNodesCounter (void * pitem, size_t param);
int _treCBFGNodeEnqueue        (void * pitem, size_t param);
int _treCBFGTreeLocateChild    (void * pitem, size_t param);
int _treCBFCompareTNodeDataG   (void * pitem, size_t param);
int _treCBFCopyTreeNodeG       (void * pitem, size_t param);

/* File level function declarations. */
P_TNODE_BY _treG2BYConvertPuppet(P_BYTREE ppnil, P_TNODE_G pnode, size_t size, P_QUEUE_L pql);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFGNodeEnqueue
 * Description:   This function enqueues child nodes for a generic tree.
 * Parameters:
 *      pitem Pointer to each node in the parent.
 *      param Pointer to a linked-list queue.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFGNodeEnqueue(void * pitem, size_t param)
{
	P_QUEUE_L pq = (P_QUEUE_L)param;
	if (NULL != pitem)
		queInsertL(pq, pitem, sizeof(P_TNODE_G));
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFFreeTNodeG
 * Description:   This function is used to free nodes in a generic tree.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFFreeTNodeG(void * pitem, size_t param)
{
	treDeleteTNodeG((P_TNODE_G)pitem);
	DWC4100(param);
	/* Add this above sentence to prevent function from producing a
	 * unreferenced formal parameter warning for param.
	 */
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFGTreeLocateChild
 * Description:   This function is used to locate child node for parent in a generic tree.
 * Parameters:
 *      pitem Pointer to each node in the generic tree.
 *      param Pointer to a FindingInfo structure.
 * Return value:  If parent node had been found, function would return value CBF_TERMINATE.
 *                If parent node could not be found in a tree, function would return value CBF_CONTINUE.
 */
int _treCBFGTreeLocateChild(void * pitem, size_t param)
{
	P_FindingInfo pfi = (P_FindingInfo)param;
	if (0 != strLinearSearchArrayZ(&((P_TNODE_G)pitem)->children, &pfi->pitem, sizeof(P_TNODE_G), false))
	{
		pfi->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFCompareTNodeDataG
 * Description:   This function is compare data for nodes in a generic tree.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param Pointer to a FindingInfo structure.
 * Return value:  If data were matched, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _treCBFCompareTNodeDataG(void * pitem, size_t param)
{
	/* The type of param is P_FindingInfo. */
	if (0 == memcmp(((P_TNODE_G)pitem)->pdata,
			((P_FindingInfo)param)->pitem,
			((P_FindingInfo)param)->size)
		)
	{
		((P_FindingInfo)param)->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFCopyTreeNodeG
 * Description:   This function is used to copy nodes in generic tree.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param Pointer to a _GTreeCopy structure.
 * Return value:  If any error occurred while copying, function would return value CBF_TERMINATE.
 *                If there were no errors produced while copying, function would return value CBF_CONTINUE.
 */
int _treCBFCopyTreeNodeG(void * pitem, size_t param)
{
	size_t tptr;
	REGISTER size_t i;
	_P_GTreeCopy ptc = (_P_GTreeCopy)param;
	P_TNODE_G pcur = (P_TNODE_G)pitem;
	P_TNODE_G pnew = treCreateTNodeG(pcur->pdata, ptc->size);
	if (NULL == pnew)
		goto Lbl_Allocation_Failure;
	/* Set new root. */
	if (NULL == ptc->pnroot)
		ptc->pnroot = pnew;
	if (! queIsEmptyL(ptc->pquel))
	{	/* Solve each parents' child node for the previous level. */
		P_TNODE_G * ppnode;
		queRemoveL(&ppnode, sizeof(P_TNODE_BY), ptc->pquel);
		*ppnode = pnew;
	}
	/* Record the needed child for the next tree level. */
	if (NULL == strInitArrayZ(&pnew->children, strLevelArrayZ(&pcur->children), sizeof(P_TNODE_G)))
		goto Lbl_Allocation_Failure;
	for (i = 0; i < strLevelArrayZ(&pnew->children); ++i)
	{
		tptr = (size_t)&(i[(P_TNODE_G *)pnew->children.pdata]);
		queInsertL(ptc->pquel, &tptr, sizeof(P_TNODE_G));
	}
	return CBF_CONTINUE;
Lbl_Allocation_Failure:
	treFreeG((P_GTREE)&ptc->pnroot);
	ptc->pnroot = NULL;
	return CBF_TERMINATE;
}

/* Function name: treTraverseGLevel
 * Description:   Traverse a generic tree by level-order.
 * Parameters:
 *      pnode Pointer to the node you want to start traversal in a tree.
 *     cbftvs Pointer to a callback function.
 *      param Parameter which can be transfer into callback function.
 * Return value:  The same value as callback function returns.
 */
int treTraverseGLevel(P_TNODE_G pnode, CBF_TRAVERSE cbftvs, size_t param)
{
	QUEUE_L q;
	int r = CBF_CONTINUE;
	if (NULL == pnode)
		return CBF_CONTINUE;
	queInitL(&q);
	queInsertL(&q, &pnode, sizeof(P_TNODE_G));
	while (! queIsEmptyL(&q))
	{
		queRemoveL(&pnode, sizeof(P_TNODE_G), &q);
		/* Enqueue all the children of the node. */
		if (strLevelArrayZ(&pnode->children) > 0)
			r = strTraverseArrayZ(&pnode->children, sizeof(P_TNODE_G), _treCBFGNodeEnqueue, (size_t)&q, false);
		/* Visit the current node. */
		if (CBF_CONTINUE != cbftvs(pnode, param))
		{	/* Do not forget to free queue here. */
			queFreeL(&q);
			return CBF_TERMINATE;
		}
	}
	queFreeL(&q);
	return r;
}

/* Function name: treArityG
 * Description:   Returns how many nodes in a generic tree.
 * Parameter:
 *     pnode Pointer to a node of a generic tree. It is usually the root node of a tree.
 * Return value:  Nodes number in a generic tree.
 */
size_t treArityG(P_TNODE_G pnode)
{
	size_t s = 0;
	treTraverseGLevel(pnode, _strCBFNodesCounter, (size_t)&s);
	return s;
}

/* Function name: treHeightG
 * Description:   Test the height of a node in a generic tree.
 * Parameter:
 *     pnode Pointer to a node of a binary tree.
 * Return value:  The height of a node.
 * Tip:           If pnode were the root node of a generic tree,
 *                the height of tree would return.
 */
size_t treHeightG(P_TNODE_G pnode)
{
	REGISTER size_t i, j, k;
	if (NULL == pnode)
		return 0;
	/* pnode is a leaf. */
	if (0 == strLevelArrayZ(&pnode->children))
		return 1;
	/* Solve each height of descendants recursively. */
	for (i = 0, j = 0; i < strLevelArrayZ(&pnode->children); ++i)
	{
		k = treHeightG(i[(P_TNODE_G *)pnode->children.pdata]);
		if (k > j)
			j = k;
	}
	return j + 1;
}

/* Function name: treInitTNodeG
 * Description:   Initialize a node of generic tree.
 * Parameters:
 *      pnode Pointer to the node you want to allocate.
 *      pitem Pointer to node data.
 *       size Size of node data.
 * Return value:  Pointer to the buffer of node data.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void * treInitTNodeG(P_TNODE_G pnode, const void * pitem, size_t size)
{
	pnode->pdata = (PUCHAR) malloc(size);
	if (NULL != pitem) /* Copy data content by size. */
		if (NULL != pnode->pdata)
			memcpy(pnode->pdata, pitem, size);
	return pnode->pdata;
}

/* Function name: treFreeTNodeG
 * Description:   Deallocate a node of which is allocated by function treInitTNodeG.
 * Parameter:
 *     ptreb Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void treFreeTNodeG(P_TNODE_G pnode)
{
	free(pnode->pdata);
	strFreeArrayZ(&pnode->children);
}

/* Function name: treCreateTNodeG
 * Description:   Allocate a new node for generic tree dynamically.
 * Parameters:
 *      pitem Pointer to the data in node.
 *       size Size of data.
 * Return value:  Pointer to the new allocated node.
 */
P_TNODE_G treCreateTNodeG(const void * pitem, size_t size)
{
	P_TNODE_G pnew = (P_TNODE_G) malloc(sizeof(TNODE_G));
	if (NULL == pnew || NULL == treInitTNodeG(pnew, pitem, size))
	{	/* Node allocation failed. */
		free(pnew);
		return NULL;
	}
	/* Build child array. */
	strInitArrayZ(&pnew->children, 0, sizeof(P_TNODE_G));
	return pnew;
}

/* Function name: treDeleteTNodeG
 * Description:   Delete a node of which is allocated by function treCreateTNodeG.
 * Parameter:
 *     pnode Pointer to the node you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void treDeleteTNodeG(P_TNODE_G pnode)
{
	treFreeTNodeG(pnode);
	free(pnode);
}

/* Function name: treInitG_O
 * Description:   Initialize a generic tree.
 * Parameter:
 *     ptreb Pointer to the generic tree you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of ptreb Must Be Allocated first.
 * Tip:           A macro version of this function named treInitG_M is available.
 */
void treInitG_O(P_GTREE ptreg)
{
	*ptreg = NULL;
}

/* Function name: treFreeG
 * Description:   Deallocate a generic tree of which is allocated by function treInitG_O.
 * Parameter:
 *     ptreb Pointer to the generic tree you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of ptreb Must Be Allocated first.
 */
void treFreeG(P_GTREE ptreg)
{
	treTraverseGLevel(*ptreg, _treCBFFreeTNodeG, 0);
	*ptreg = NULL;
}

/* Function name: treCreateG
 * Description:   Allocate a new generic tree dynamically.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated tree.
 */
P_GTREE treCreateG(void)
{
	P_GTREE ptreg = (P_GTREE) malloc(sizeof(GTREE));
	/* Sarah Silva found this issue that we must detect NULL pointer before alter it. */
	if (NULL != ptreg)
		treInitG(ptreg);
	return ptreg;
}

/* Function name: treDeleteG
 * Description:   Delete a generic tree of which is allocated by function treCreateG.
 * Parameter:
 *     ptreb Pointer to a generic tree you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of ptreb Must Be Allocated first.
 */
void treDeleteG(P_GTREE ptreg)
{
	treFreeG(ptreg);
	free(ptreg);
}

/* Function name: treInsertG
 * Description:   Insert a node into generic tree.
 * Parameters:
 *      pnode Pointer to the parent node.
 *      pitem Pointer to the data you want to insert into the tree.
 *       size Size of data.
 * Return value:  If insertion succeeded, function would return a pointer of the new inserted node.
 *                Otherwise function would return a NULL.
 * Caution:       If pnode equaled NULL, function would create a new node and return its pointer.
 */
P_TNODE_G treInsertG(P_TNODE_G pnode, const void * pitem, size_t size)
{
	if (NULL == pnode)
		return treCreateTNodeG(pitem, size);
	else
	{
		P_TNODE_G pnew;
		if (NULL == (pnew = treCreateTNodeG(pitem, size)))
			return NULL; /* Init node allocation failed. */
		if (NULL == strResizeArrayZ(&pnode->children, strLevelArrayZ(&pnode->children) + 1, sizeof(P_TNODE_G)))
			return NULL; /* Reallocation for parent node failed. */
		/* Put the new node into parent's children array. */
		return (strLevelArrayZ(&pnode->children) - 1)[(P_TNODE_G *)pnode->children.pdata] = pnew;
	}
}

/* Function name: treRemoveSubtreeG
 * Description:   Remove the sub tree of a generic tree.
 * Parameters:
 *     parent Pointer to the parent node which contains the pchild pointer.
 *     pchild Pointer to the child node that you want to remove.
 *     bclear Withdraw child pointer array from parent node.
 * Return value:  Pointer to the child node.
 *                If function returned a NULL, it should mean that pchild you input is not a child node of parent.
 * Caution:       The whole sub tree that the child node holds will be deallocated! Do not panic. It is a feature.
 */
P_TNODE_G treRemoveSubtreeG(P_TNODE_G parent, P_TNODE_G pchild, bool bclear)
{
	size_t i = strLinearSearchArrayZ(&parent->children, &pchild, sizeof(P_TNODE_G), false);
	if (0 == i) /* Can not find pchild in parent. */
		return NULL;
	if (bclear)
	{
		treFreeG(&pchild);
		pchild = NULL;
	}
	strRemoveItemArrayZ(&parent->children, sizeof(P_TNODE_G), i - 1, true);
	return pchild;
}

/* Function name: treGetParentNodeG
 * Description:   Locate parent node of pchild in a generic tree.
 * Parameters:
 *      proot Pointer to the root node of a generic tree.
 *     pchild Pointer to the child node.
 * Return value:  Pointer to parent node of pchild.
 */
P_TNODE_G treGetParentNodeG(P_TNODE_G proot, P_TNODE_G pchild)
{
	FindingInfo fi;
	if (proot == pchild)
		return NULL; /* Parent of root node is NULL. */
	fi.pitem  = pchild;
	fi.result = NULL;
	treTraverseGLevel(proot, _treCBFGTreeLocateChild, (size_t)&fi);
	return (P_TNODE_G)fi.result;
}

/* Function name: treSearchDataG
 * Description:   Search for the specific data in a generic tree.
 * Parameters:
 *      proot Pointer to the root node that you want to start your searching procedure in a generic tree.
 *      pitem Pointer to the data you want to search.
 *       size Size of that data.
 * Return value:  Pointer to a node in the binary tree that contains the same data as pitem referred.
 *                If the specific data could not be found in the tree, function would return a NULL.
 * Caution:       This function will only use the level order traversal.
 */
P_TNODE_G treSearchDataG(P_TNODE_G proot, const void * pitem, size_t size)
{
	FindingInfo fi;
	fi.result = NULL;
	fi.pitem  = pitem;
	fi.size   = size;
	treTraverseGLevel(proot, _treCBFCompareTNodeDataG, (size_t)&fi);
	return (P_TNODE_G)fi.result;
}

/* Function name: treSwapNodesG
 * Description:   Swap two nodes in a generic tree.
 * Parameters:
 *     prootx Pointer to the root node that has a certain path to pnodex.
 *     pnodex Pointer to a node.
 *     prooty Pointer to the root node that has a certain path to pnodey.
 *     pnodey Pointer to another node.
 * Return value:  If swapping succeeded, function would return pnodey,
 *                otherwise function would return a NULL.
 * Caution:       prootx may equal to pnodey.
 *                Both pnodex and pnodey shall not in the same sub-tree.
 */
P_TNODE_G treSwapNodesG(P_TNODE_G prootx, P_TNODE_G pnodex, P_TNODE_G prooty, P_TNODE_G pnodey)
{
	REGISTER P_TNODE_G prtx, prty;
	if (NULL == prootx || NULL == prooty ||
		NULL == pnodex || NULL == pnodey ||
		NULL != (prtx = treGetParentNodeG(pnodex, pnodey)) ||
		NULL != (prty = treGetParentNodeG(pnodey, pnodex))
		) return NULL; /* T1 contains T2, or T2 contains T1. */
	else
	{
		REGISTER size_t i, j;
		P_TNODE_G ptmp;
		/* Locate pnodex and y in their parents. */
		i = strLinearSearchArrayZ(&prtx->children, &pnodex, sizeof(P_TNODE_G), false) - 1;
		j = strLinearSearchArrayZ(&prty->children, &pnodey, sizeof(P_TNODE_G), false) - 1;
		svSwap(&i[(P_TNODE_G *)prtx->children.pdata], &j[(P_TNODE_G *)prty->children.pdata], &ptmp, sizeof(P_TNODE_G));
		return pnodey;
	}
}

/* Function name: treCopyG
 * Description:   Copy a generic tree entirely.
 * Parameters:
 *      proot Pointer to the root node of the original generic tree.
 *       size Size of each data in every node of a generic tree.
 * Return value:  A copy of the original generic tree.
 * Caution:       Size of data of every nodes shall be in the same value.
 */
P_TNODE_G treCopyG(P_TNODE_G proot, size_t size)
{
	_GTreeCopy tp;
	QUEUE_L q;
	queInitL(&q);
	tp.pnroot = NULL;
	tp.pquel = &q;
	tp.size = size;
	treTraverseGLevel(proot, _treCBFCopyTreeNodeG, (size_t)&tp);
	/* Do not forget to delete the queue. */
	queFreeL(&q);
	return (P_TNODE_G)tp.pnroot;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treG2BYConvertPuppet
 * Description:   This function is used to convert a generic tree into a binary tree.
 * Parameters:
 *      ppnil Pointer to pnode of the caller function.
 *      pnode Pointer to a node of the original generic tree.
 *       size Size of data in node.
 *        pql Pointer to a queue.
 * Return value:  Pointer to a TNODE_BY.
 */
P_TNODE_BY _treG2BYConvertPuppet(P_BYTREE ppnil, P_TNODE_G pnode, size_t size, P_QUEUE_L pql)
{
	REGISTER size_t i;
	P_TNODE_BY pn;
	P_TNODE_BY * po;
	if (NULL == pnode)
		return 0;
	/* pnode is an external node. */
	if (0 == strLevelArrayZ(&pnode->children))
	{
		if (NULL == (pn = strCreateNodeD(pnode->pdata, size)))
			return (*ppnil = NULL);
		queInsertL(pql, &pn, sizeof(P_TNODE_BY));
		return pn;
	}
	/* Create a new node for binary tree. */
	if (NULL == (pn = strCreateNodeD(pnode->pdata, size)))
		return (*ppnil = NULL);
	queInsertL(pql, &pn, sizeof(P_TNODE_BY));
	/* Set root of new binary tree. */
	if (NULL == *ppnil)
		*ppnil = pn;
	po = &(pn->ppnode[LEFT]);
	/* Solve each height of descendants recursively. */
	for (i = 0; i < strLevelArrayZ(&pnode->children); ++i)
	{
		*po = _treG2BYConvertPuppet(ppnil, i[(P_TNODE_G *)pnode->children.pdata], size, pql);
		po = &((*po)->ppnode[RIGHT]);
	}
	return pn;
}

/* Function name: treG2BYConvert
 * Description:   Convert a generic tree into a corresponding binary tree.
 * Parameters:
 *      pnode Pointer to the root node of the original generic tree.
 *       size Size of each data in every node of a generic tree.
 * Return value:  Pointer to the new generated binary tree.
 * Caution:       Size of data of every nodes shall be in the same value.
 */
P_TNODE_BY treG2BYConvert(P_TNODE_G pnode, size_t size)
{
	QUEUE_L q;
	/* This queue is used to maintain new bnodes.
	 * When allocation failed, dequeue and free each element in the queue.
	 */
	P_TNODE_BY pb = NULL;
	queInitL(&q);
	_treG2BYConvertPuppet(&pb, pnode, size, &q);
	if (NULL == pb) /* Allocation error in dummy function. */
		while (! queIsEmptyL(&q)) /* Clear bnodes one by one. */
			queRemoveL(&pb, sizeof(P_TNODE_BY), &q);
	queFreeL(&q);
	return pb;
}

