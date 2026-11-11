/*
 * Name:        svhtree.c
 * Description: Heap tree.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0809171737E1110261933L00720
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

#include <stdlib.h> /* Use function malloc, free. */
#include <string.h> /* Use function memcpy. */
#include "svtree.h"

/* Function name: treInitHeapA
 * Description:   Initialize an array implemented heap tree.
 * Parameters:
 *      pheap Pointer to a heap tree that you want to allocate.
 *        num Number of elements in the heap.
 *       size Size of each element in the heap.
 * Return value:  N/A.
 * Caution:       Address of pheap Must Be Allocated first.
 */
void treInitHeapA(P_HEAP_A pheap, size_t num, size_t size)
{
	pheap->irear = 0;
	strInitArrayZ(&pheap->hdarr, num, size);
}

/* Function name: treFreeHeapA
 * Description:   Retract a heap tree which is allocated by function treInitHeapA.
 * Parameter:
 *     pheap Pointer to a heap that you want to release.
 * Return value:  N/A.
 * Caution:       Address of heap Must Be Allocated first.
 */
void treFreeHeapA(P_HEAP_A pheap)
{
	strFreeArrayZ(&pheap->hdarr);
	pheap->irear = 0;
}

/* Function name: treCreateHeapA
 * Description:   Allocate a new array implemented heap dynamically.
 * Parameters:
 *        num Maximum number of elements that the heap can hold.
 *       size Size of each element.
 * Return value:  Pointer to a new allocated heap.
 */
P_HEAP_A treCreateHeapA(size_t num, size_t size)
{
	REGISTER P_HEAP_A pheap = (P_HEAP_A) malloc(sizeof(HEAP_A));
	if (NULL != pheap)
	{
		pheap->irear = 0;
		strInitArrayZ(&pheap->hdarr, num, size);
	}
	return pheap;
}

/* Function name: treDeleteHeapA
 * Description:   Delete a heap tree which is allocated by function treCreateHeapA.
 * Parameter:
 *     pheap Pointer to a heap you want to delete.
 * Return value:  N/A.
 * Caution:       Address of pheap Must Be Allocated first.
 */
void treDeleteHeapA(P_HEAP_A pheap)
{
	treFreeHeapA(pheap);
	free(pheap);
}

/* Function name: treIsEmptyHeapA_O
 * Description:   Make a judgment whether an array implemented heap is empty or not.
 * Parameter:
 *     pheap Pointer to the heap you want to check.
 * Return value:  true  Heap is empty.
 *                false Heap is NOT empty.
 * Tip:           A macro version of this function named treIsEmptyHeapA_M is available.
 */
bool treIsEmptyHeapA_O(P_HEAP_A pheap)
{
	return 0 == pheap->irear;
}

/* Function name: treIsFullHeapA_O
 * Description:   Make a judgment for whether an array implemented heap is full or not.
 * Parameter:
 *     pheap Pointer to the heap you want to check.
 * Return value:  true  Heap is full.
 *                false Heap is NOT full.
 * Tip:           A macro version of this function named treIsFullHeapA_M is available.
 */
bool treIsFullHeapA_O(P_HEAP_A pheap)
{
	return pheap->irear == strLevelArrayZ(&pheap->hdarr);
}

/* Function name: treMakeEmptyHeapA_O
 * Description:   Forcibly clear a heap.
 * Parameter:
 *     pheap Pointer to the heap you want to clear.
 * Return value:  N/A.
 * Caution:       This function would not free the buffer array of a heap.
 * Tip:           A macro version of this function named treMakeEmptyHeapA_M is available.
 */
void treMakeEmptyHeapA_O(P_HEAP_A pheap)
{
	pheap->irear = 0;
}

/* Function name: treInsertHeapA
 * Description:   Insert an element into an array implemented heap.
 * Parameters:
 *      pheap Pointer to the heap you want to operate with.
 *      pitem Pointer to an element you want to insert into the heap.
 *      ptemp Pointer to a buffer that is used to swap data.
 *            The length of this buffer shall equal to each heap element size.
 *       size Size of element.
 *     cbfcmp Pointer to a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the heap.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 *       bmax true  Insert data into a max heap.
 *            false Insert data into a min heap.
 *            (*) If a heap has been defined as a max heap in creation, do not assign false before deletion, vice versa.
 * Return value:  true  indicates insertion succeeded.
 *                false indicates insertion failed.
 * Caution:       pheap must be allocated first.
 *                ptemp cannot be written as NULL. This buffer is managed by caller function.
 */
bool treInsertHeapA(P_HEAP_A pheap, const void * pitem, void * ptemp, size_t size, CBF_COMPARE cbfcmp, bool bmax)
{
	if (SV_ASSERT(! treIsFullHeapA(pheap)))
	{
		REGISTER PUCHAR px, py;
		REGISTER size_t i, j;
		REGISTER int r;
		j = pheap->irear++;
		/* Put new element at the end of array. */
		memcpy(pheap->hdarr.pdata + j * size, pitem, size);
		while (0 != j)
		{
			/* Fetch parent node of new element. */
			i = (j - 1) >> 1;
			px = pheap->hdarr.pdata + i * size;
			py = pheap->hdarr.pdata + j * size;
			r = cbfcmp(py, px);
			if (bmax ? r > 0 : r < 0)
			{	/* Swap when j > i in a max heap. */
				svSwap(px, ptemp, py, size);
				j = i;
			}
			else /* j <= i in a max heap. */
				break;
		}
		return true;
	}
	return false;
}

/* Function name: treRemoveHeapA
 * Description:   Remove an element from an array implemented heap.
 * Parameters:
 *      pitem Pointer to an element you want to hold the removed element from a heap.
 *            If pitem equaled to NULL, this function would drop the polarized(maximum or minimum) item directly.
 *      ptemp Pointer to a buffer that is used to swap data.
 *            The length of this buffer shall equal to item size.
 *       size Size of element.
 *      pheap Pointer to the heap you want to operate with.
 *     cbfcmp Pointer to a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the heap.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 *       bmax true  Remove data from a max heap.
 *            false Remove data from a min heap.
 *            (*) If a heap has been defined as a min heap in creation, do not assign true before deletion, vice versa.
 * Return value:  true  indicates removal succeeded.
 *                false indicates removal failed.
 * Caution:       pheap must be allocated first.
 *                ptemp cannot be NULL. This buffer is managed by caller function.
 */
bool treRemoveHeapA(void * pitem, void * ptemp, size_t size, P_HEAP_A pheap, CBF_COMPARE cbfcmp, bool bmax)
{
	REGISTER size_t i = pheap->irear;
	if (i > 0)
	{
		REGISTER size_t t, l, m, n;
		REGISTER int r;
		/* Alter the rear index of pheap. */
		pheap->irear = --i;
		/* Remove the biggest one. */
		if (NULL != pitem)
			memcpy(pitem, pheap->hdarr.pdata, size);
		/* Put the last element onto the head of array and decrease the real size of the array. */
		memcpy(pheap->hdarr.pdata, pheap->hdarr.pdata + i * size, size);
		/* Heapify. */
		for (i = 0;; )
		{
			t = (i << 1);
			l = t + 1; /* Left  child. */
			n = t + 2; /* Right child. */
			t = pheap->irear;
			m =
			(
				l < t &&
				(
					r = cbfcmp(pheap->hdarr.pdata + l * size, pheap->hdarr.pdata + i * size),
					bmax ? r > 0 : r < 0
				)
			) ? l : i;
			if
			(
				n < t &&
				(
					r = cbfcmp(pheap->hdarr.pdata + n * size, pheap->hdarr.pdata + m * size),
					bmax ? r > 0 : r < 0
				)
			)
				m = n;
			if (m == i)
				break;
			else
			{
				svSwap(pheap->hdarr.pdata + i * size, ptemp, pheap->hdarr.pdata + m * size, size);
				i = m;
			}
		}
		return true;
	}
	return false;
}

/* Function name: trePeepHeapA
 * Description:   Have a peek at the first element of heap.
 * Parameters:
 *      pitem Pointer to a buffer to hold the first element of a heap.
 *            Set this parameter to NULL will cause this function to return false.
 *       size Size of the element in heap.
 *      pheap Pointer to the heap you want to operate with.
 * Return value:  true  Peep succeeded.
 *                false Peep failed.
 * Caution:       pheap must be allocated first.
 *                pitem should be a valid address that can hold an element.
 */
bool trePeepHeapA(void * pitem, size_t size, P_HEAP_A pheap)
{
	if (! treIsEmptyHeapA(pheap))
	{
		if (NULL != pitem)
		{
			memcpy(pitem, pheap->hdarr.pdata, size);
			return true;
		}
	}
	return false;
}

/* File-level function declarations for linked node heap trees. */
int _treCBFFreeHeapNodeD(void * pitem, size_t param);

/* Function name: treInitNodeHeapD
 * Description:   Initialize a linked heap tree node.
 * Parameters:
 *      pnode Pointer to the node you want to initialize.
 *       pval Pointer to an element.
 *       size Size of the element.
 *     parent Pointer to the parent node of the node that pnode pointed out.
 *     degree Signifies the number of nodes in the tree that rooted with pnode.
 *     height Height of the current node. The height of a leaf node is 0.
 *            The height of a root node is its number of longest path of nodes from the root's child to a leaf.
 * Return value:  knot.pdata pointer of a TNODE_HD structure.
 *                If function could not initialize a node, it would return NULL.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void * treInitNodeHeapD(P_TNODE_HD pnode, const void * pval, size_t size, P_TNODE_HD parent, size_t degree, size_t height)
{
	pnode->knot.ppnode[LEFT] = pnode->knot.ppnode[RIGHT] = NULL;
	pnode->knot.pdata = NULL;
	
	if (0 != size && NULL != pval)
	{
		pnode->knot.pdata = (PUCHAR) malloc(size);
		
		if (NULL != pnode->knot.pdata)
			memcpy(pnode->knot.pdata, pval, size);
	}
	
	pnode->degree = degree;
	pnode->height = height;
	pnode->parent = parent;
	
	return pnode->knot.pdata;
}

/* Function name: treFreeNodeHeapD_O
 * Description:   Retract a node which is allocated by function treInitNodeHeapD.
 * Parameter:
 *     pnode Pointer to the node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named treInitNodeHeapD_M is available.
 */
void treFreeNodeHeapD_O(P_TNODE_HD pnode)
{
	if (SV_ASSERT(NULL != pnode->knot.pdata)) /* Circumvent NULL pointer freedom. */
	{
		free(pnode->knot.pdata);
		pnode->knot.pdata = NULL;
	}
}

/* Function name: treCreateNodeHeapD
 * Description:   Dynamically allocate a node of a linked binary heap tree.
 * Parameters:
 *       pval Pointer to an element.
 *       size Size of the element.
 *     parent Pointer to the parent node of the node that pnode pointed out.
 *     degree Signifies the number of nodes in the tree that rooted with pnode.
 *     height Height of the current node. The height of a leaf node is 0.
 *            The height of a root node is its number of longest path of nodes from the root's child to a leaf.
 * Return value:  Pointer to a new allocated node.
 *                If function could not create a node, it would return NULL.
 */
P_TNODE_HD treCreateNodeHeapD(const void * pval, size_t size, P_TNODE_HD parent, size_t degree, size_t height)
{
	REGISTER P_TNODE_HD pnode = (P_TNODE_HD) malloc(sizeof(TNODE_HD));
	if (NULL != pnode)
	{
		if (NULL == treInitNodeHeapD(pnode, pval, size, parent, degree, height))
		{	/* Allocation failure. */
			free(pnode);
			pnode = NULL;
		}
	}
	return pnode;
}

/* Function name: treDeleteNodeHeapD_O
 * Description:   Retract a node which is allocated by function treCreateNodeHeapD.
 * Parameter:
 *     pnode Pointer to the node you want to release.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named treDeleteNodeHeapD_M is available.
 */
void treDeleteNodeHeapD_O(P_TNODE_HD pnode)
{
	treFreeNodeHeapD(pnode);
	free(pnode);
}

/* Function name: treInitHeapD_O
 * Description:   Initialize a linked heap tree.
 * Parameter:
 *     pheap Pointer to a heap tree that you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of pheap Must Be Allocated first.
 * Tip:           A macro version of this function named treInitHeapD_M is available.
 */
void treInitHeapD_O(P_HEAP_D pheap)
{
	*pheap = NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFFreeHeapNodeD
 * Description:   Linked heap tree node freeing actuator.
 * Parameters:
 *      pitem Pointer to each node in a heap tree.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFFreeHeapNodeD(void * pitem, size_t param)
{
	DISUSE(param);
	treDeleteNodeHeapD(P2P_TNODE_HD(pitem));
	return CBF_CONTINUE;
}

/* Function name: treFreeHeapD
 * Description:   Retract a linked heap tree which is allocated by function treInitHeapD.
 * Parameter:
 *     pheap Pointer to a heap that you want to release.
 * Return value:  N/A.
 * Caution:       Address of heap Must Be Allocated first.
 */
void treFreeHeapD(P_HEAP_D pheap)
{
	treTraverseBYPost(P2P_TNODE_BY(*pheap), _treCBFFreeHeapNodeD, 0);
	*pheap = NULL;
}

/* Function name: treCreateHeapD
 * Description:   Allocate a new linked heap dynamically.
 * Parameters:    N/A.
 * Return value:  Pointer to a new allocated heap.
 */
P_HEAP_D treCreateHeapD(void)
{
	REGISTER P_HEAP_D pheap= (P_HEAP_D) malloc(sizeof(HEAP_D));
	if (NULL != pheap)
		treInitHeapD(pheap);
	return pheap;
}

/* Function name: treDeleteHeapD_O
 * Description:   Delete a heap tree which is allocated by function treCreateHeapD.
 * Parameter:
 *     pheap Pointer to a heap you want to delete.
 * Return value:  N/A.
 * Caution:       Address of pheap Must Be Allocated first.
 * Tip:           A macro version of this function named treDeleteHeapD_M is available.
 */
void treDeleteHeapD_O(P_HEAP_D pheap)
{
	treFreeHeapD(pheap);
	free(pheap);
}

/* Function name: treIsEmptyHeapD_O
 * Description:   Test whether a linked heap is empty or not.
 * Parameter:
 *     pheap Pointer to the heap you want to check.
 * Return value:  true  Heap is empty.
 *                false Heap is NOT empty.
 * Caution:       pheap must be allocated first.
 * Tip:           A macro version of this function named treIsEmptyHeapD_M is available.
 */
bool treIsEmptyHeapD_O(P_HEAP_D pheap)
{
	return NULL != *pheap ? true : false;
}

/* Function name: treIsFullHeapD_O
 * Description:   Test whether a linked heap is a full binary tree or not.
 *                Note that, each internal node in a FULL binary tree must have two children.
 *                And each leaf node in a full binary tree must not have any child.
 * Parameter:
 *     pheap Pointer to the heap you want to check.
 * Return value:  true  Heap is a full binary tree.
 *                false Heap is NOT a full binary tree.
 * Caution:       pheap must be allocated first.
 * Tip:           A macro version of this function named treIsFullHeapD_M is available.
 */
bool treIsFullHeapD_O(P_TNODE_HD pnode)
{
	return (2 << pnode->height) - 1U == pnode->degree ? true : false;
}

/* Function name: treInsertHeapD
 * Description:   Insert an element into a linked heap.
 * Parameters:
 *      pheap Pointer to the linked heap you want to operate on.
 *      pitem Pointer to an element you want to insert into the heap.
 *       size Size of element.
 *     cbfcmp Pointer to a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the heap.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 *       bmax true  Insert data into a max heap.
 *            false Insert data into a min heap.
 *            (*) If a heap has been defined as a max heap in creation, do not assign false before deletion, vice versa.
 * Return value:  A pointer to a node in heap, the node contains the exact value to pitem.
 * Caution:       pheap must be allocated first.
 */
P_TNODE_HD treInsertHeapD(P_HEAP_D pheap, const void * pitem, size_t size, CBF_COMPARE cbfcmp, bool bmax)
{
	if (NULL == *pheap)
	{	/* Create a node for one node heap. */
		*pheap = treCreateNodeHeapD(pitem, size, NULL, 1, 0);
		return *pheap;
	}
	else
	{
		REGISTER P_TNODE_HD pn = *pheap, pp = pn, pt, pb = NULL;
		REGISTER bool bcmp;
		
		/* Find heap tail's parent to pp. */
		while (NULL != pn)
		{
			REGISTER P_TNODE_HD pl, pr;
			
			if (treIsFullHeapD(pn))
			{
				while (NULL != pn)
				{
					pp = pn;
					pn = P2P_TNODE_HD(pn->knot.ppnode[LEFT]);
				}
				break;
			}
			else
			{
				pp = pn;
				pl = P2P_TNODE_HD(pn->knot.ppnode[LEFT]);
				pr = P2P_TNODE_HD(pn->knot.ppnode[RIGHT]);
				
				if (pl == NULL && NULL != pr)
					return NULL; /* Bad heap tree. */
				else if (NULL != pl)
					pn = treIsFullHeapD(pl) ? pr : pl;
				else break; /* Program reaches at a leaf. */
			}
		}
		
		/* Create a new node for data insertion. */
		pn = treCreateNodeHeapD(pitem, size, pp, 1, 0);
		
		/* Find an exact position to insert new created node. */
		pt = pb = NULL == pp->knot.ppnode[LEFT] ?
			P2P_TNODE_HD(pp->knot.ppnode[LEFT] = P2P_TNODE_BY(pn)) :
			P2P_TNODE_HD(pp->knot.ppnode[RIGHT] = P2P_TNODE_BY(pn));
		
		/* Backtrack to increase degree and height for parents. */
		for (bcmp = true, pn = pp; NULL != pn; pn = pn->parent)
		{
			if (bcmp)
			{
				REGISTER int r = cbfcmp(pt->knot.pdata, pn->knot.pdata);
				if ((const bool)bmax ? r > 0 : r < 0)
				{
					PUCHAR t;
					svSwap(&pt->knot.pdata, &t, &pn->knot.pdata, sizeof(PUCHAR));
					pt = pn;
				}
				else bcmp = false; /* Can not bubble up anymore. */
			}
			
			/* If pb is its parent's left child, increase its parent's height. */
			if (pn->knot.ppnode[LEFT] == P2P_TNODE_BY(pb))
			{
				++pn->height;
				pb = pn;
			}
			
			++pn->degree;
		}
		return pt;
	}
}

/* Function name: treRemoveHeapD
 * Description:   Remove an element from a linked heap.
 * Parameters:
 *      pitem Pointer to an element you want to hold the removed element from a heap.
 *            If pitem equaled to NULL, this function would drop the polarized(maximum or minimum) item directly.
 *       size Size of element.
 *      pheap Pointer to the heap you want to operate on.
 *     cbfcmp Pointer to a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the heap.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 *       bmax true  Remove data from a max heap.
 *            false Remove data from a min heap.
 *            (*) If a heap has been defined as a min heap in creation, do not assign true before deletion, vice versa.
 * Return value:  true  indicates removal succeeded.
 *                false indicates removal failed.
 * Caution:       pheap must be allocated first.
 */
bool treRemoveHeapD(void * pitem, size_t size, P_HEAP_D pheap, CBF_COMPARE cbfcmp, bool bmax)
{
	REGISTER P_TNODE_HD pn = *pheap, pp = NULL;
	REGISTER P_TNODE_HD pl, pr;
	
	/* Find heap tail's parent to pp. */
	while (NULL != pn)
	{
		if (treIsFullHeapD(pn))
		{
			while (NULL != pn)
			{
				pp = pn;
				pn = P2P_TNODE_HD(pn->knot.ppnode[RIGHT]);
			}
			break;
		}
		else
		{
			pp = pn;
			pl = P2P_TNODE_HD(pn->knot.ppnode[LEFT]);
			pr = P2P_TNODE_HD(pn->knot.ppnode[RIGHT]);
			
			/* Adhere to the principle of binary heap to find the tail. */
			if (pl == NULL && NULL != pr)
				return false; /* Bad heap tree. */
			else if (NULL != pl)
			{
				if (treIsFullHeapD(pl))
					pn = pr;
				if (NULL != pr && treIsFullHeapD(pr))
					pn = pl;
			}
			else break; /* Program reaches at a leaf. */
		}
	}
	
	if (NULL != pp)
	{
		REGISTER P_TNODE_HD pt = P2P_TNODE_HD(*pheap);
		REGISTER P_TNODE_HD pb = pp, px;
		PUCHAR   t;
		
		if (NULL != pitem) /* Output data if possible. */
			memcpy(pitem, pt->knot.pdata, size);

		if (pb == pt && NULL == pt->knot.ppnode[LEFT]) /* Meet an one node heap. */
			*pheap = NULL;
		else
		{
			if (NULL != pp->knot.ppnode[LEFT])
				pb = P2P_TNODE_HD(pp->knot.ppnode[LEFT]);
			if (NULL != pp->knot.ppnode[RIGHT])
				pb = P2P_TNODE_HD(pp->knot.ppnode[RIGHT]);
			
			/* Set root to base. */
			memcpy(pt->knot.pdata, pb->knot.pdata, size);
			
			/* Downgrade to heapify. */
			pn = pt;
			while (NULL != pn)
			{
				px = pn;
				pl = P2P_TNODE_HD(pn->knot.ppnode[LEFT]);
				pr = P2P_TNODE_HD(pn->knot.ppnode[RIGHT]);

				if (NULL == pl && NULL != pr)
					return false; /* Heap corruption. */
				else
				{
					REGISTER int r;

					if (NULL != pl)
					{
						r = cbfcmp(pn->knot.pdata, pl->knot.pdata);
						if ((const bool)bmax ? r < 0 : r > 0)
							px = pl;
					}

					if (NULL != pr)
					{
						r = cbfcmp(px->knot.pdata, pr->knot.pdata);
						if ((const bool)bmax ? r < 0 : r > 0)
							px = pr;
					}
				}
				/* In this situation, root equals its children, we escape from loop immediately.
				 *   3
				 *  / \
				 * 3   3
				 */
				if (px == pn)
					break;
				else
				{
					svSwap(&pn->knot.pdata, &t, &px->knot.pdata, sizeof(PUCHAR));
					pn = px;
				}
			}
			
			/* Up search to decrease degree and height from the bottom. */
			for (pn = pb->parent, pt = pb; NULL != pn; pn = pn->parent)
			{
				if (pn->knot.ppnode[LEFT] == P2P_TNODE_BY(pt))
				{
					--pn->height;
					pt = pn;
				}
				--pn->degree;
			}

			/* Retract pb from its parent. */
			if (pb->parent->knot.ppnode[LEFT] == P2P_TNODE_BY(pb))
				pb->parent->knot.ppnode[LEFT] = NULL;
			else
				pb->parent->knot.ppnode[RIGHT] = NULL;
		}
		
		treDeleteNodeHeapD(pb);

		return true;
	}
	return false;
}

/* Function name: trePeepHeapD
 * Description:   Have a peek at the first element of a heap.
 * Parameters:
 *      pitem Pointer to a buffer to hold the first element of a heap.
 *            Set this parameter to NULL will cause this function to return false.
 *       size Size of the element in heap.
 *      pheap Pointer to the heap you want to operate with.
 * Return value:  true  Peep succeeded.
 *                false Peep failed.
 * Caution:       pheap must be allocated first.
 *                pitem should be a valid address that can hold an element.
 */
bool trePeepHeapD(void * pitem, size_t size, P_HEAP_D pheap)
{
	if (! treIsEmptyHeapD(pheap))
	{
		if (NULL != pitem)
		{
			memcpy(pitem, (*pheap)->knot.pdata, size);
			return true;
		}
	}
	return false;
}

