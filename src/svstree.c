/*
 * Name:        svstree.c
 * Description: Search trees.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0809171737I0503231635L02042
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

#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function memcpy, memmove. */
#include "svtree.h"

/* A macro describes children nodes pointers of a BST node. */
#define pbstchild(pnode) *(P_BSTNODE *)&(pnode)->knot.ppnode

/* A macro to the node parameter which represented the level of an AA-tree node
 * or the balance factor of an AVL-tree node is defined below
 * of use _NODE_PARAM(pnode, act_type) to set value;
 * Use _NODE_PARAM(pnode, const act_type) to get value or triggering a compiling error
 * of l-value specifies constant object while users are attempting to set value.
 */
#define _NODE_PARAM(pnode, act_type) (*(act_type *)&(pnode)->param)

/* File level function declarations here. */
int _treCBFFreeNodeBST(void * pitem, size_t param);

/* Function name: treInitBSTNode
 * Description:   Initialize a node of binary search tree.
 * Parameters:
 *      pnode Pointer to the node you want to initialize.
 *      pitem Pointer to the address of an element.
 *       size Size of the element.
 *      param Additional information of the current node.
 * Return value:  pdata pointer of NODE_D structure in the node.
 *                If function could not initialize a node, it would return a NULL.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void * treInitBSTNode(P_BSTNODE pnode, const void * pitem, size_t size, size_t param)
{
	if (NULL == strInitNodeD(&pnode->knot, pitem, size))
		return NULL;
	pnode->param = param;
	return pnode->knot.pdata;
}

/* Function name: treFreeBSTNode_O
 * Description:   Deallocate a node of which is allocated by function treInitBSTNode.
 * Parameter:
 *     pnode Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named treFreeBSTNode_M is available.
 */
void treFreeBSTNode_O(P_BSTNODE pnode)
{
	strFreeNodeD(&pnode->knot);
	pnode->knot.pdata = NULL;
}

/* Function name: treCreateBSTNode
 * Description:   Dynamically allocate a node of binary search tree.
 * Parameters:
 *      pitem Pointer to the address of an element.
 *       size Size of the element.
 *      param Additional information of the current node.
 * Return value:  Pointer to the new allocated node.
 *                If function could not create a node, it would return a NULL.
 */
P_BSTNODE treCreateBSTNode(const void * pitem, size_t size, size_t param)
{
	P_BSTNODE pnew = P2P_BSTNODE(malloc(sizeof(BSTNODE)));
	if (NULL != pnew)
	{
		if (NULL == treInitBSTNode(pnew, pitem, size, param))
		{
			free(pnew);
			return NULL;
		}
	}
	return pnew;
}

/* Function name: treDeleteBYSTNode_O
 * Description:   Deallocate a node of which is allocated by function treCreateBSTNode.
 * Parameter:
 *     pnode Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named treDeleteBYSTNode_M is available.
 */
void treDeleteBSTNode_O(P_BSTNODE pnode)
{
	treFreeBSTNode(pnode);
	free(pnode);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFFreeNodeBST
 * Description:   This function is used to free nodes in a binary search tree.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFFreeNodeBST(void * pitem, size_t param)
{
	treDeleteBSTNode(P2P_BSTNODE(pitem));
	DWC4100(param);
	return CBF_CONTINUE;
}

/* Function name: treInitBST_O
 * Description:   Initialize a binary search tree.
 * Parameter:
 *      pbst Pointer to the binary search tree you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of pbst Must Be Allocated first.
 * Tip:           A macro version of this function named treInitBST_M is available.
 */
void treInitBST_O(P_BST pbst)
{
	*pbst = NULL;
}

/* Function name: treFreeBST
 * Description:   Deallocate a binary search tree of which is allocated by function treInitBST_O.
 * Parameter:
 *      pbst Pointer to the binary search tree you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbst Must Be Allocated first.
 */
void treFreeBST(P_BST pbst)
{
	/* A post-order traversal is needed here.
	 * Because we have to free nodes from crown to root.
	 */
	treTraverseBYPost(P2P_TNODE_BY(*pbst), _treCBFFreeNodeBST, 0);
	*pbst = NULL;
}

/* Function name: treCreateBST
 * Description:   Dynamically allocate a binary search tree.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated tree.
 *                If function could not create a pointer, it would return a NULL.
 */
P_BST treCreateBST(void)
{
	P_BST pbst = (P_BST) malloc(sizeof(BST));
	if (NULL == pbst)
		return NULL;
	treInitBST(pbst);
	return pbst;
}

/* Function name: treDeleteBYST_O
 * Description:   Deallocate a binary search tree of which is allocated by function treCreateBST.
 * Parameter:
 *      pbst Pointer to the tree you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbst Must Be Allocated first.
 * Tip:           A macro version of this function named treDeleteBYST_M is available.
 */
void treDeleteBST_O(P_BST pbst)
{
	treFreeBST(pbst);
	free(pbst);
}

/* Function name: treCopyBST
 * Description:   Copy a binary search tree.
 * Parameters:
 *      proot Pointer to the root node of the original BST.
 *       size Size of each element in the original BST.
 * Return value:  Pointer to the new root node.
 * Caution:       Address of proot Must Be Allocated first.
 */
P_BSTNODE treCopyBST(P_BSTNODE proot, size_t size)
{
	REGISTER P_BSTNODE pl, pr, pp;
	if (NULL == proot)
		return NULL;
	pl = treCopyBST(P2P_BSTNODE(proot->knot.ppnode[LEFT]), size);
	pr = treCopyBST(P2P_BSTNODE(proot->knot.ppnode[RIGHT]), size);
	if (NULL != (pp = treCreateBSTNode(proot->knot.pdata, size, proot->param)))
	{
		pp->knot.ppnode[LEFT] = P2P_TNODE_BY(pl);
		pp->knot.ppnode[RIGHT] = P2P_TNODE_BY(pr);
	}
	return pp;
}

/* Function name: treBSTFindData_R
 * Description:   Find data in a binary search tree recursively.
 * Parameters:
 *      proot Pointer to the root node.
 *      pitem Pointer to an element that contains the data you want to search.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the node that contains the data.
 * Caution:       Address of proot Must Be Allocated first.
 */
P_BSTNODE treBSTFindData_R(P_BSTNODE proot, const void * pitem, CBF_COMPARE cbfcmp)
{
	REGISTER int r;
	if (NULL == proot)
		return NULL;
	if (0 == (r = cbfcmp(pitem, proot->knot.pdata)))
		return proot;
	return treBSTFindData_R(pbstchild(proot)[r > 0], pitem, cbfcmp);
}

/* Function name: treBSTFindData_A
 * Description:   Find data in a binary search tree using a cycle.
 * Parameters:
 *      proot Pointer to the root node.
 *      pitem Pointer to an element that contains the data you want to search.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the node that contains the data.
 * Caution:       Address of proot Must Be Allocated first.
 */
P_BSTNODE treBSTFindData_A(P_BSTNODE proot, const void * pitem, CBF_COMPARE cbfcmp)
{
	REGISTER int r = 0;
	while (NULL != proot)
	{
		if (0 == (r = cbfcmp(pitem, proot->knot.pdata)))
			return proot;
		proot = pbstchild(proot)[r > 0];
	}
	return NULL;
}

/* AA-tree implementation achieved in the following section. */

/* Function declarations for AA-trees. */
P_BSTNODE _treBSTSkewAA (P_BSTNODE pnode);
P_BSTNODE _treBSTSplitAA(P_BSTNODE pnode);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTSkewAA
 * Description:   Skew a node in an AA-tree.
 * Parameter:
 *     pnode Pointer to the node that you want to skew.
 * Return value:  Pointer to the replaced node after skewing.
 */
P_BSTNODE _treBSTSkewAA(P_BSTNODE pnode)
{
	if (NULL != pnode && (NULL != pbstchild(pnode)[LEFT] && _NODE_PARAM(pbstchild(pnode)[LEFT], const size_t) == _NODE_PARAM(pnode, const size_t)))
	{	/* Rotate right. */
		REGISTER P_BSTNODE ptemp = pnode;
		pnode = pbstchild(pnode)[LEFT];
		pbstchild(ptemp)[LEFT]  = pbstchild(pnode)[RIGHT];
		pbstchild(pnode)[RIGHT] = ptemp;
	}
	return pnode;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTSplitAA
 * Description:   Split a node in an AA-tree.
 * Parameter:
 *     pnode Pointer to the node that you want to split on.
 * Return value:  Pointer to the replaced node after splitting.
 */
P_BSTNODE _treBSTSplitAA(P_BSTNODE pnode)
{
	if
	(
		NULL != pnode &&
		(
			NULL != pbstchild(pnode)[RIGHT] &&
			NULL != (pbstchild(pnode)[RIGHT])->knot.ppnode[RIGHT] &&
			_NODE_PARAM(P2P_BSTNODE((pbstchild(pnode)[RIGHT])->knot.ppnode[RIGHT]), const size_t) == _NODE_PARAM(pnode, const size_t)
		)
	)
	{	/* Rotate left. */
		REGISTER P_BSTNODE ptemp = pnode;
		pnode = pbstchild(pnode)[RIGHT];
		pbstchild(ptemp)[RIGHT] = pbstchild(pnode)[LEFT];
		pbstchild(pnode)[LEFT]  = ptemp;
		++_NODE_PARAM(pnode, size_t);
	}
	return pnode;
}

/* Function name: treBSTInsertAA
 * Description:   Insert data into an AA-tree.
 * Parameters:
 *      pnode Pointer to the root node of an AA-tree.
 *      pitem Pointer to an element that contains the data you want to insert.
 *       size Size of the element.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the new root node of an AA-tree.
 * Caution:       Function would not insert pitem successful, if it met an allocation error.
 * Tip:           Usage:
 *                P_BST pbst = treCreateBST(); // Create a new AA-binary-search tree.
 *                *pbst = treBSTInsertAA(*pbst, &a, sizeof(a), cbfcmp);
 *                treDeleteBST(pbst); // Destory AA-tree.
 */
P_BSTNODE treBSTInsertAA(P_BSTNODE pnode, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	if (NULL == pnode)
		pnode = treCreateBSTNode(pitem, size, 1);
	else
	{
		REGISTER int r = cbfcmp(pitem, pnode->knot.pdata);
		if (r < 0)
			pbstchild(pnode)[LEFT]  = treBSTInsertAA(pbstchild(pnode)[LEFT],  pitem, size, cbfcmp);
		else if (r > 0)
			pbstchild(pnode)[RIGHT] = treBSTInsertAA(pbstchild(pnode)[RIGHT], pitem, size, cbfcmp);
		pnode = _treBSTSkewAA(pnode);
		pnode = _treBSTSplitAA(pnode);
	}
	return pnode;
}

/* Function name: treBSTRemoveAA
 * Description:   Remove data from an AA-tree.
 * Parameters:
 *      pnode Pointer to the root node of an AA-tree.
 *      pitem Pointer to an element that contains the data you want to remove.
 *       size Size of the element.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the new root node of an AA-tree.
 * Caution:       Function would not insert pitem successful, if it met an allocation error.
 * Tip:           Usage:
 *                P_BST pbst = treCreateBST(); // Create a new AA-binary-search tree.
 *                *pbst = treBSTInsertAA(*pbst, &a, sizeof(a), cbfcmp); // Insertion.
 *                *pbst = treBSTRemoveAA(*pbst, &a, sizeof(a), cbfcmp); // Removal.
 *                treDeleteBST(pbst); // Destory AA-tree.
 */
P_BSTNODE treBSTRemoveAA(P_BSTNODE pnode, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER P_BSTNODE plast = NULL, pdelete = NULL;
	if (NULL != pnode)
	{
		plast = pnode;
		if (cbfcmp(pitem, pnode->knot.pdata) < 0)
			pbstchild(pnode)[LEFT]  = treBSTRemoveAA(pbstchild(pnode)[LEFT],  pitem, size, cbfcmp);
		else
		{
			pdelete = pnode;
			pbstchild(pnode)[RIGHT] = treBSTRemoveAA(pbstchild(pnode)[RIGHT], pitem, size, cbfcmp);
		}
		/* If pnode is a leaf node, remove it directly. */
		if (pnode == plast)
		{
			if (NULL != pdelete && 0 == cbfcmp(pitem, pdelete->knot.pdata))
			{
				memcpy(pdelete->knot.pdata, pnode->knot.pdata, size);
				pdelete = NULL;
				pnode = pbstchild(pnode)[RIGHT];
				treDeleteBSTNode(plast);
			}
		}
		else /* pnode is not a leaf. */
		{
			if ((NULL != pbstchild(pnode)[LEFT]  ? _NODE_PARAM(pbstchild(pnode)[LEFT],  const size_t) : 0) < _NODE_PARAM(pnode, const size_t) - 1 ||
				(NULL != pbstchild(pnode)[RIGHT] ? _NODE_PARAM(pbstchild(pnode)[RIGHT], const size_t) : 0) < _NODE_PARAM(pnode, const size_t) - 1)
			{
				if (NULL != pbstchild(pnode)[RIGHT] && _NODE_PARAM(pbstchild(pnode)[RIGHT], const size_t) > --_NODE_PARAM(pnode, size_t))
					_NODE_PARAM(pbstchild(pnode)[RIGHT], size_t) = _NODE_PARAM(pnode, const size_t);
				pnode = _treBSTSkewAA(pnode);
				pbstchild(pnode)[RIGHT] = _treBSTSkewAA(pbstchild(pnode)[RIGHT]);
				if (NULL != pbstchild(pnode)[RIGHT])
					(pbstchild(pnode)[RIGHT])->knot.ppnode[RIGHT] = P2P_TNODE_BY(_treBSTSkewAA(P2P_BSTNODE((pbstchild(pnode)[RIGHT])->knot.ppnode[RIGHT])));
				pnode = _treBSTSplitAA(pnode);
				pbstchild(pnode)[RIGHT] = _treBSTSplitAA(pbstchild(pnode)[RIGHT]);
			}
		}
	}
	return pnode;
}

/* An implementation for AVL-tree is listed below. */

/* An enumeration describes the balance factor for AVL-tree nodes. */
enum _en_AVLBalanceFactor {
	_ABF_HEAVY_RT = -1,
	_ABF_BALANCED = 00,
	_ABF_HEAVY_LT = 01
};
typedef ptrdiff_t _en_BalanceFactor;

/* Sub-section function declarations for AVL-tree. */
ptrdiff_t _treBSTGetBalanceFactorAVL  (P_BSTNODE pnode);
ptrdiff_t _treBSTMaxBalanceFactorAVL  (ptrdiff_t lbf,  ptrdiff_t rbf);
ptrdiff_t _treBSTReadBalanceFactorAVL (P_BSTNODE pnode);
P_BSTNODE _treBSTRightRotateAVL       (P_BSTNODE pnode);
P_BSTNODE _treBSTLeftRotateAVL        (P_BSTNODE pnode);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTGetBalanceFactorAVL
 * Description:   Get a node's balance factor.
 * Parameter:
 *     pnode Pointer to a node of an AVL-tree.
 * Return value:  Balance factor.
 */
ptrdiff_t _treBSTGetBalanceFactorAVL(P_BSTNODE pnode)
{
	return NULL == pnode ? _ABF_BALANCED : _NODE_PARAM(pnode, const ptrdiff_t);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTMaxBalanceFactorAVL
 * Description:   Get the maximize value from two balance factors.
 * Parameters:
 *        lbf A balance factor.
 *        rbf Another balance factor.
 * Return value:  The max value.
 */
ptrdiff_t _treBSTMaxBalanceFactorAVL(ptrdiff_t lbf, ptrdiff_t rbf)
{
	return lbf > rbf ? lbf : rbf;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTReadBalanceFactorAVL
 * Description:   Read the difference of a node's children's balance factor.
 * Parameter:
 *     pnode Pointer to a node of an AVL-tree.
 * Return value:  The difference between left child's and right child's balance factor.
 */
ptrdiff_t _treBSTReadBalanceFactorAVL(P_BSTNODE pnode)
{
	/* Check if node exists, if so then it applies the difference between it's children's heights. */
	return NULL == pnode ? _ABF_BALANCED :
		_treBSTGetBalanceFactorAVL(pbstchild(pnode)[LEFT]) -
		_treBSTGetBalanceFactorAVL(pbstchild(pnode)[RIGHT]);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTRightRotateAVL
 * Description:   Right rotate pnode.
 * Parameters:
 *      pnode Pointer to a node of an AVL-tree.
 * Return value:  Pointer to the rotated node in an AVL-tree.
 */
P_BSTNODE _treBSTRightRotateAVL(P_BSTNODE pnode)
{
	P_BSTNODE pnodex = pbstchild(pnode)[LEFT];
	P_BSTNODE pnodey = pbstchild(pnodex)[RIGHT];

	/* Adjust pointers. */
	pbstchild(pbstchild(pnode)[LEFT])[RIGHT] = pnode;
	pbstchild(pnode)[LEFT] = pnodey;

	/* Recalculating Balance Factors. */
	_NODE_PARAM(pnode, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[RIGHT])
		);
	_NODE_PARAM(pnodex, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnodex)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnodex)[RIGHT])
		);

	return pnodex;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBSTLeftRotateAVL
 * Description:   Left rotate pnode.
 * Parameters:
 *      pnode Pointer to a node of an AVL-tree.
 * Return value:  Pointer to the rotated node in an AVL-tree.
 */
P_BSTNODE _treBSTLeftRotateAVL(P_BSTNODE pnode)
{
	P_BSTNODE pnodex = pbstchild(pnode)[RIGHT];
	P_BSTNODE pnodey = pbstchild(pnodex)[LEFT];

	/* Adjust pointers. */
	pbstchild(pbstchild(pnode)[RIGHT])[LEFT] = pnode;
	pbstchild(pnode)[RIGHT] = pnodey;

	/* Recalculating Balance Factors */
	_NODE_PARAM(pnode, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[RIGHT])
		);
	_NODE_PARAM(pnodex, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnodex)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnodex)[RIGHT])
		);

	return pnodex;
}

/* Function name: treBSTInsertAVL
 * Description:   Insert data into an AVL-tree.
 * Parameters:
 *      pnode Pointer to the root node of an AVL-tree.
 *      pitem Pointer to an element that contains the data you want to insert.
 *       size Size of the element.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the new root node of an AVL-tree.
 * Tip:           Usage:
 *                P_BST pbst = treCreateBST(); // Create a new AVL-binary-search tree.
 *                *pbst = treBSTInsertAVL(*pbst, &a, sizeof(a), cbfcmp); // Insertion.
 *                treDeleteBST(pbst); // Destory AVL-tree.
 */
P_BSTNODE treBSTInsertAVL(P_BSTNODE pnode, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER int r;
	REGISTER ptrdiff_t cb;

	if (NULL == pnode)
	{
		pnode = treCreateBSTNode(pitem, size, _ABF_BALANCED);
		return pnode;
	}

	r = cbfcmp(pitem, pnode->knot.pdata);
	if (r < 0)
		pbstchild(pnode)[LEFT] = treBSTInsertAVL(pbstchild(pnode)[LEFT], pitem, size, cbfcmp);
	else /* r >= 0. */
		pbstchild(pnode)[RIGHT] = treBSTInsertAVL(pbstchild(pnode)[RIGHT], pitem, size, cbfcmp);

	/* Recalculate current height. */
	_NODE_PARAM(pnode, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[RIGHT])
		);

	/* Test rotation cases. */
	cb = _treBSTReadBalanceFactorAVL(pnode);

	/* Left left case. */
	if (cb > _ABF_HEAVY_LT && cbfcmp(pitem, (pbstchild(pnode)[LEFT])->knot.pdata) <= 0)
		return _treBSTRightRotateAVL(pnode);

	/* Right right case. */
	if (cb < _ABF_HEAVY_RT && cbfcmp(pitem, (pbstchild(pnode)[RIGHT])->knot.pdata) >= 0)
		return _treBSTLeftRotateAVL(pnode);

	/* Right left case. */
	if (cb < _ABF_HEAVY_RT && cbfcmp(pitem, (pbstchild(pnode)[RIGHT])->knot.pdata) <= 0)
	{
		pbstchild(pnode)[RIGHT] = _treBSTRightRotateAVL(pbstchild(pnode)[RIGHT]);
		return _treBSTLeftRotateAVL(pnode);
	}

	/* Left right case. */
	if (cb > _ABF_HEAVY_LT && cbfcmp(pitem, (pbstchild(pnode)[LEFT])->knot.pdata) >= 0)
	{
		pbstchild(pnode)[LEFT] = _treBSTLeftRotateAVL(pbstchild(pnode)[LEFT]);
		return _treBSTRightRotateAVL(pnode);
	}

	return pnode;
}

/* Function name: treBSTRemoveAVL
 * Description:   Remove data from an AVL-tree.
 * Parameters:
 *      pnode Pointer to the root node of an AVL-tree.
 *      pitem Pointer to an element that contains the data you want to remove.
 *       size Size of the element.
 *     cbfcmp Pointer to a callback function.
 * Return value:  Pointer to the new root node of an AVL-tree.
 * Tip:           Usage:
 *                P_BST pbst = treCreateBST(); // Create a new AVL-binary-search tree.
 *                *pbst = treBSTInsertAVL(*pbst, &a, sizeof(a), cbfcmp); // Insertion.
 *                *pbst = treBSTRemoveAVL(*pbst, &a, sizeof(a), cbfcmp); // Removal.
 *                treDeleteBST(pbst); // Destory AVL-tree.
 */
P_BSTNODE treBSTRemoveAVL(P_BSTNODE pnode, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER int r;
	REGISTER ptrdiff_t cb;

	/* Check if node exists. */
	if (NULL == pnode)
		return NULL;

	r = cbfcmp(pnode->knot.pdata, pitem);

	/* Check if we find the exact node. */
	if (0 == r)
	{
		REGISTER P_BSTNODE ptemp;
		if (NULL == pbstchild(pnode)[LEFT] && NULL == pbstchild(pnode)[RIGHT])
		{	/* No child. */
			treDeleteBSTNode(pnode);
			return NULL;
		}
		else if (NULL == pbstchild(pnode)[LEFT])
		{	/* One right child. */
			ptemp = pnode;
			pnode = pbstchild(pnode)[RIGHT];
			treDeleteBSTNode(ptemp);
		}
		else if (NULL == pbstchild(pnode)[RIGHT])
		{	/* One left child. */
			ptemp = pnode;
			pnode = pbstchild(pnode)[LEFT];
			treDeleteBSTNode(ptemp);
		}
		else /* Two children. */
		{
			ptemp = pbstchild(pnode)[RIGHT];
			if (NULL != ptemp) /* Walk down the tree to find the least one. */
				while (NULL != pbstchild(ptemp)[LEFT])
					ptemp = pbstchild(pnode)[LEFT];
			if (NULL != ptemp)
			{
				memcpy(pnode->knot.pdata, ptemp->knot.pdata, size);
				pbstchild(pnode)[RIGHT] = treBSTRemoveAVL(pbstchild(pnode)[RIGHT], ptemp->knot.pdata, size, cbfcmp);
			}
		}
	}
	else if (r > 0)
		pbstchild(pnode)[LEFT] = treBSTRemoveAVL(pbstchild(pnode)[LEFT], pitem, size, cbfcmp);
	else
		pbstchild(pnode)[RIGHT] = treBSTRemoveAVL(pbstchild(pnode)[RIGHT], pitem, size, cbfcmp);

	/* The tree has only one node. */
	if (NULL == pnode)
		return NULL;

	/* Recalculate current height. */
	_NODE_PARAM(pnode, ptrdiff_t) = _ABF_HEAVY_LT + _treBSTMaxBalanceFactorAVL
		(
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[LEFT]),
			_treBSTGetBalanceFactorAVL(pbstchild(pnode)[RIGHT])
		);

	/* Test rotation cases. */
	cb = _treBSTReadBalanceFactorAVL(pnode);

	/* Left left case. */
	if (cb > _ABF_HEAVY_LT && _treBSTReadBalanceFactorAVL(pbstchild(pnode)[LEFT]) >= _ABF_BALANCED)
		return _treBSTRightRotateAVL(pnode);

	/* Right right case. */
	if (cb < _ABF_HEAVY_RT && _treBSTReadBalanceFactorAVL(pbstchild(pnode)[RIGHT]) <= _ABF_BALANCED)
		return _treBSTLeftRotateAVL(pnode);

	/* Right left case. */
	if (cb < _ABF_HEAVY_RT && _treBSTReadBalanceFactorAVL(pbstchild(pnode)[RIGHT]) >= _ABF_BALANCED)
	{
		pbstchild(pnode)[RIGHT] = _treBSTRightRotateAVL(pbstchild(pnode)[RIGHT]);
		return _treBSTLeftRotateAVL(pnode);
	}

	/* Left right case. */
	if (cb > _ABF_HEAVY_LT && _treBSTReadBalanceFactorAVL(pbstchild(pnode)[LEFT]) <= _ABF_BALANCED)
	{
		pbstchild(pnode)[LEFT] = _treBSTLeftRotateAVL(pbstchild(pnode)[LEFT]);
		return _treBSTRightRotateAVL(pnode);
	}

	return pnode;
}

#undef _NODE_PARAM
#undef pbstchild
/* Undefine used macros for this section. */

/* Functions for B+ trees. */
#include "svqueue.h"
#define PARENTPTR (0)
#define NEXTPTR   (1)

/* Additional info for BPTNODE of B-plus trees. */
typedef struct _st_BPT_INFO {
	ARRAY_Z   keyarr;  /* Array of keys and child pointers. */
	P_BPTNODE headptr; /* The first child pointer of a node. */
} _BPT_INFO, * _P_BPT_INFO;

/* Keys and child pointer elements in array. */
typedef struct _st_BPT_KEY_INFO {
	PUCHAR    pkey;   /* Key pointer. */
	P_BPTNODE pchild; /* Child pointer. */
} _BPT_KEY_INFO, * _P_BPT_KEY_INFO;

/* File scope function declarations here. */
void             _treInitBPTInfo_O           (_P_BPT_INFO      pbi);
void             _treFreeBPTInfo             (_P_BPT_INFO      pbi);
_P_BPT_INFO      _treCreateBPTInfo           (void);
void             _treDeleteBPTInfo_O         (_P_BPT_INFO      pbi);
P_BPTNODE        _treGetParentBPTNode_O      (P_BPTNODE        pnode);
P_BPTNODE        _treGetNextBPTNode_O        (P_BPTNODE        pnode);
BOOL             _treIsLeafBPTNode_O         (P_BPTNODE        pnode);
void             _treFreeBPTPuppet           (P_QUEUE_L        pquelx,  P_QUEUE_L    pquely);
P_BPTNODE        _treLocateKeyChainHeaderBPT (P_BPT            pbpt);
P_BPTNODE        _treLocateKeyInLeafBPT      (P_BPT            pbpt,    const void * pkey,   CBF_COMPARE  cbfcmp);
_P_BPT_KEY_INFO  _treInsertKeyIntoArrayBPT   (_P_BPT_INFO      pbni,    const void * pkey,   P_BPTNODE    pchild,   CBF_COMPARE  cbfcmp);
size_t           _treRemoveKeyFromArrayBPT   (_P_BPT_INFO      pbni,    const void * pkey,   CBF_COMPARE  cbfcmp);
BOOL             _treSplitArrayInLeafBPT     (P_BPTNODE        pnew,    P_BPTNODE    pold,   const size_t degree);
BOOL             _treSplitArrayInNodeBPT     (_P_BPT_KEY_INFO  pbki,    P_BPTNODE    pnew,   P_BPTNODE    pold,     const size_t degree);
BOOL             _treMakeKeyChainBPT         (P_QUEUE_L        pquel,   P_BPTNODE *  pprev,  PUCHAR *     ppkeys[], const size_t num);
P_BPTNODE        _treGetSiblingNodeBPT       (P_BPTNODE        pnode,   BOOL         bright);
void             _treRedistributeNodesBPT    (P_BPTNODE        pnode,   P_BPTNODE    psib,   BOOL         bright);
void             _treMergeNodesBPT           (P_BPTNODE        pnode,   P_BPTNODE    psib,   P_BPTNODE    phead,    BOOL         bright);
BOOL             _treBPTRemovePuppet         (P_BPT            pbpt,    P_BPTNODE    pnode,  const size_t hdeg,     const void * pkey,  CBF_COMPARE cbfcmp);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treInitBPTInfo_O
 * Description:   Initialize a _BPT_INFO structure.
 * Parameter:
 *       pbi Pointer to the _BPT_INFO structure you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of pbi Must Be Allocated first.
 * Tip:           A macro version of this function named _treInitBPTInfo_M is available.
 */
void _treInitBPTInfo_O(_P_BPT_INFO pbi)
{
	pbi->headptr = NULL;
	pbi->keyarr.num = 0;
	pbi->keyarr.pdata = NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treFreeBPTInfo
 * Description:   Deallocate a _BPT_INFO structure of which is allocated by function _treInitBPTInfo_O.
 * Parameter:
 *       pbi Pointer to the _BPT_INFO structure you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbi Must Be Allocated first.
 */
void _treFreeBPTInfo(_P_BPT_INFO pbi)
{
	pbi->headptr = NULL;
	strFreeArrayZ(&pbi->keyarr);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCreateBPTInfo
 * Description:   Dynamically allocate a _BPT_INFO structure.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated structure.
 *                If function could not create a structure, it would return a NULL.
 */
_P_BPT_INFO _treCreateBPTInfo(void)
{
	_P_BPT_INFO pbi = (_P_BPT_INFO) malloc(sizeof(_BPT_INFO));
	if (NULL != pbi)
		_treInitBPTInfo(pbi);
	return pbi;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treDeleteBPTInfo_O
 * Description:   Deallocate a _BPT_INFO structure of which is allocated by function _treCreateBPTInfo.
 * Parameter:
 *       pbi Pointer to the _BPT_INFO structure you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbi Must Be Allocated first.
 * Tip:           A macro version of this function named _treDeleteBYPTInfo_M is available.
 */
void _treDeleteBPTInfo_O(_P_BPT_INFO pbi)
{
	_treFreeBPTInfo(pbi);
	free(pbi);
}

/* Function name: treInitBPTNode
 * Description:   Initialize a node of B-plus indexing tree.
 * Parameters:
 *      pnode Pointer to the node you want to initialize.
 *     parent Pointer to parent node.
 *      pnext Pointer to the next node in key chain.
 * Return value:  pdata pointer in the node.
 *                If function could not initialize a node, it would return a NULL.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void * treInitBPTNode(P_BPTNODE pnode, P_TNODE_BY parent, P_TNODE_BY pnext)
{
	pnode->ppnode[PARENTPTR] = parent;
	pnode->ppnode[NEXTPTR]   = pnext;
	return (pnode->pdata = (PUCHAR) _treCreateBPTInfo());
}

/* Function name: treFreeBPTNode
 * Description:   Deallocate a node of which is allocated by function treInitBPTNode.
 * Parameter:
 *     pnode Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void treFreeBPTNode(P_BPTNODE pnode)
{
	_treFreeBPTInfo((_P_BPT_INFO)pnode->pdata);
	pnode->pdata = NULL;
}

/* Function name: treCreateBPTNode
 * Description:   Dynamically allocate a node of B-plus indexing tree.
 * Parameters:
 *     parent Pointer to parent node.
 *      pnext Pointer to the next node in key chain.
 * Return value:  Pointer to the new allocated node.
 *                If function could not create a node, it would return a NULL.
 */
P_BPTNODE treCreateBPTNode(P_BPTNODE parent, P_BPTNODE pnext)
{
	P_BPTNODE pnew = strCreateNodeD(_treCreateBPTInfo(), sizeof(_BPT_INFO));
	if (NULL != pnew)
	{
		if (NULL == pnew->pdata)
		{
			strDeleteNodeD(pnew);
			return NULL;
		}
		pnew->ppnode[PARENTPTR] = parent;
		pnew->ppnode[NEXTPTR]   = pnext;
	}
	return pnew;
}

/* Function name: treDeleteBPTNode
 * Description:   Deallocate a node of which is allocated by function treCreateBPTNode.
 * Parameter:
 *     pnode Pointer to the node you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void treDeleteBPTNode(P_BPTNODE pnode)
{
	treFreeBPTNode(pnode);
	free(pnode);
}

/* Function name: _treGetParentBPTNode_O
 * Description:   Get parent node of a child node in a B-plus indexing tree.
 * Parameter:
 *     pnode Pointer to the child node.
 * Return value:  Pointer to parent node.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named _treGetParentBPTNode_M is available.
 */
P_BPTNODE _treGetParentBPTNode_O(P_BPTNODE pnode)
{
	return pnode->ppnode[PARENTPTR];
}

/* Function name: _treGetNextBPTNode_O
 * Description:   Get the next node in the key chain in a B-plus indexing tree.
 * Parameter:
 *     pnode Pointer to current node.
 * Return value:  Pointer to the next node.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named _treGetNextBPTNode_M is available.
 */
P_BPTNODE _treGetNextBPTNode_O(P_BPTNODE pnode)
{
	return pnode->ppnode[NEXTPTR];
}

/* Function name: _treIsLeafBPTNode_O
 * Description:   Check whether pnode is a leaf.
 * Parameter:
 *     pnode Pointer to a node you want to check.
 * Return value:  TRUE pnode is a leaf and pnode is a root node.
 *                FALSE pnode is not a leaf.
 * Caution:       Address of pnode Must Be Allocated first.
 * Tip:           A macro version of this function named _treIsLeafBPTNode_M is available.
 */
BOOL _treIsLeafBPTNode_O(P_BPTNODE pnode)
{
	return !((_P_BPT_INFO)pnode->pdata)->headptr;
}

/* Function name: treInitBPT_O
 * Description:   Initialize a B-plus tree.
 * Parameter:
 *      pbpt Pointer to the B-plus indexing tree you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of pbpt Must Be Allocated first.
 * Tip:           A macro version of this function named treInitBPT_M is available.
 */
void treInitBPT_O(P_BPT pbpt)
{
	*pbpt = NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treFreeBPTPuppet
 * Description:   Free BPT nodes level by level.
 * Parameters:
 *     pquelx Pointer to a queue that used to contain pointers of nodes to traverse a tree.
 *     pquelx Pointer to a queue that used to contain pointers of nodes to be deleted.
 * Return value:  N/A.
 * Caution:       Address of pquelx and pquely Must Be Allocated first.
 * Tip:           Callers of this function shall manage two queues that parameters pquelx and pquely pointed on its own.
 */
void _treFreeBPTPuppet(P_QUEUE_L pquelx, P_QUEUE_L pquely)
{
	REGISTER size_t i;
	P_BPTNODE pnode;
	_P_BPT_INFO pbni;
	while (! queIsEmptyL(pquelx))
	{
		queRemoveL(&pnode, sizeof(P_BPTNODE), pquelx);
		pbni = (_P_BPT_INFO) pnode->pdata;
		/* Enqueue all the children of the current node. */
		if (NULL != pbni->headptr)
		{	/* Do only if pnode is not a leaf node. */
			queInsertL(pquelx, &pbni->headptr, sizeof(P_BPTNODE));
			for (i = 0; i < pbni->keyarr.num; ++i)
			{
				_P_BPT_KEY_INFO pbki = ((_P_BPT_KEY_INFO)pbni->keyarr.pdata) + i;
				if (NULL != pbki->pchild)
					queInsertL(pquelx, &pbki->pchild, sizeof(P_BPTNODE));
			}
		}
		/* Enqueue current node for deletion later. */
		queInsertL(pquely, &pnode, sizeof(P_BPTNODE));
	}
	/* Delete each node in the queue. */
	while (! queIsEmptyL(pquely))
	{
		queRemoveL(&pnode, sizeof(P_BPTNODE), pquely);
		treDeleteBPTNode(pnode);
	}
}

/* Function name: treFreeBPT
 * Description:   Deallocate a B-plus indexing tree of which is allocated by function treInitBPT_O.
 * Parameter:
 *      pbpt Pointer to the binary search tree you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
void treFreeBPT(P_BPT pbpt)
{
	P_BPTNODE pnode = *pbpt;
	if (NULL != pnode)
	{	/* Tree is not empty. */
		QUEUE_L q1, q2;
		queInitL(&q1);
		queInitL(&q2);
		/* Insert the root of a tree into a queue. */
		queInsertL(&q1, &pnode, sizeof(P_BPTNODE));
		/* Free tree nodes repeatedly. */
		_treFreeBPTPuppet(&q1, &q2);
		queFreeL(&q1);
		queFreeL(&q2);
	}
}

/* Function name: treCreateBPT
 * Description:   Dynamically allocate a B-plus indexing tree.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated tree.
 *                If function could not create a structure, it would return a NULL.
 */
P_BPT treCreateBPT(void)
{
	P_BPT pbpt = (P_BPT) malloc(sizeof(BPT));
	if (NULL != pbpt)
		treInitBPT(pbpt);
	return pbpt;
}

/* Function name: treDeleteBPT
 * Description:   Deallocate a B-plus indexing tree of which is allocated by function treCreateBPT.
 * Parameter:
 *      pbpt Pointer to the tree you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
void treDeleteBPT(P_BPT pbpt)
{
	treFreeBPT(pbpt);
	free(pbpt);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treLocateKeyChainHeaderBPT
 * Description:   Locate the header of key chain in B-plus tree.
 * Parameter:
 *      pbpt Pointer to the B-plus tree.
 * Return value:  Pointer to the header node of key chain.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
P_BPTNODE _treLocateKeyChainHeaderBPT(P_BPT pbpt)
{
	P_BPTNODE pnode = *pbpt;
	while (NULL != pnode && (! _treIsLeafBPTNode(pnode)))
		pnode = ((_P_BPT_INFO)pnode->pdata)->headptr;
	return pnode;
}

/* Function name: treTraverseKeyBPT
 * Description:   Traverse each key in key chain.
 * Parameters:
 *       pbpt Pointer to the B-plus tree.
 *     cbftvs Pointer to a callback function.
 *      param A size_t value which can be transfer into callback function.
 * Return value:  The same value as callback function returns.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
int treTraverseKeyBPT(P_BPT pbpt, CBF_TRAVERSE cbftvs, size_t param)
{
	P_BPTNODE pnode = _treLocateKeyChainHeaderBPT(pbpt);
	while (NULL != pnode)
	{
		REGISTER size_t i;
		_P_BPT_INFO pbni = (_P_BPT_INFO)pnode->pdata;
		for (i = 0; i < pbni->keyarr.num; ++i)
			if (CBF_CONTINUE != cbftvs(i[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey, param))
				return CBF_TERMINATE;
		pnode = _treGetNextBPTNode(pnode);
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treLocateKeyInLeafBPT
 * Description:   Locate the specific key in a leaf node.
 * Parameters:
 *       pbpt Pointer to the B-plus tree.
 *       pkey A pointer value which stores the index key.
 *     cbfcmp Pointer to a callback function that compares the data in pkey and the data in the nodes of tree.
 * Return value:  A BPTNODE that contains the specific key.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
P_BPTNODE _treLocateKeyInLeafBPT(P_BPT pbpt, const void * pkey, CBF_COMPARE cbfcmp)
{
	P_BPTNODE pnode = *pbpt;
	while (NULL != pnode && (! _treIsLeafBPTNode(pnode)))
	{
		REGISTER int r;
		REGISTER size_t i;
		_P_BPT_INFO pbni = (_P_BPT_INFO)pnode->pdata;
		for (i = pbni->keyarr.num; i > 0; --i)
		{
			r = cbfcmp((i - 1)[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey, pkey);
			if (r <= 0)
			{
				pnode = (i - 1)[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pchild;
				break;
			}
			else if (1 == i)
				pnode = pbni->headptr;
		}
	}
	return pnode;
}

/* Function name: treSearchDataBPT
 * Description:   Search data in a B-plus tree.
 * Parameters:
 *       pbpt Pointer to B-plus indexing tree.
 *       pkey Pointer to an element in the memory.
 *     cbfcmp Pointer to a callback function that compares data size of pkey.
 * Return value:  pkey Found pkey in the B-plus tree.
 *                NULL Cannot find pkey.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
void * treSearchDataBPT(P_BPT pbpt, const void * pkey, CBF_COMPARE cbfcmp)
{
	P_BPTNODE pnode = _treLocateKeyInLeafBPT(pbpt, pkey, cbfcmp);
	if (NULL != pnode)
	{
		REGISTER size_t i;
		REGISTER _P_BPT_INFO pbni = (_P_BPT_INFO)pnode->pdata;
		for (i = pbni->keyarr.num; i > 0; --i)
			if (pkey == (i - 1)[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey)
				return (void *)pkey;
	}
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treInsertKeyIntoArrayBPT
 * Description:   Insert a key into array in a node.
 * Parameters:
 *       pbni Pointer to a _BPT_INFO structure in a node.
 *       pkey A pointer value which stores the index key.
 *     pchild Pointer to the child node of current node.
 *     cbfcmp Pointer to a callback function that compares the data in pkey and the data in the nodes of a tree.
 * Return value:  Pointer to _BPT_KEY_INFO structure.
 * Caution:       Address of pbni Must Be Allocated first.
 */
_P_BPT_KEY_INFO _treInsertKeyIntoArrayBPT(_P_BPT_INFO pbni, const void * pkey, P_BPTNODE pchild, CBF_COMPARE cbfcmp)
{
	REGISTER size_t i;
	/* Locate the suitable index. */
	for (i = 0; i < pbni->keyarr.num; ++i)
		if (cbfcmp(i[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey, pkey) > 0)
			break;
	if (NULL == strResizeArrayZ(&pbni->keyarr, pbni->keyarr.num + 1, sizeof(_BPT_KEY_INFO)))
		return NULL; /* Reallocation failure. */
	/* Split content in the array. */
	if (pbni->keyarr.num > (i + 1))
		memmove(i + 1 + ((_P_BPT_KEY_INFO)pbni->keyarr.pdata),
				i + ((_P_BPT_KEY_INFO)pbni->keyarr.pdata),
				sizeof(_BPT_KEY_INFO) * (pbni->keyarr.num - i - 1));
	i[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey   = (PUCHAR) pkey;
	i[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pchild = pchild;
	return i + ((_P_BPT_KEY_INFO)pbni->keyarr.pdata);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treRemoveKeyFromArrayBPT
 * Description:   Remove a key from the array in a node.
 * Parameters:
 *       pbni Pointer to a _BPT_INFO structure in a node.
 *       pkey A pointer value which stores the index key.
 *     cbfcmp Pointer to a callback function that compares the data in pkey and the data in the nodes of a tree.
 * Return value:  Value of index + 1 of element which has been removed.
 *                If function returned 0, that meant nothing had been deleted from array.
 * Caution:       Address of pbni Must Be Allocated first.
 */
size_t _treRemoveKeyFromArrayBPT(_P_BPT_INFO pbni, const void * pkey, CBF_COMPARE cbfcmp)
{
	REGISTER size_t i;
	for (i = 0; i < pbni->keyarr.num; ++i)
	{
		if (cbfcmp(i[(_P_BPT_KEY_INFO)pbni->keyarr.pdata].pkey, pkey) == 0)
		{
			strRemoveItemArrayZ(&pbni->keyarr, sizeof(_BPT_KEY_INFO), i, TRUE);
			return i + 1;
		}
	}
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treSplitArrayInLeafBPT
 * Description:   Split array in a leaf node by degree.
 * Parameters:
 *       pnew Pointer to a new node after pnode in key chain.
 *       pold Pointer to the node that you want to split.
 *     degree Degree of B-plus tree.
 *            Value of degree shall be greater than or equal to 3.
 * Return value:  TRUE  Splitting succeeded.
 *                FALSE Splitting failed.
 * Caution:       Address of pold and pnew Must Be Allocated first.
 */
BOOL _treSplitArrayInLeafBPT(P_BPTNODE pnew, P_BPTNODE pold, const size_t degree)
{
	_P_BPT_INFO pbni0 = (_P_BPT_INFO)pold->pdata;
	_P_BPT_INFO pbni1 = (_P_BPT_INFO)pnew->pdata;
	size_t j = degree >> 1;
	if (NULL == strResizeArrayZ(&pbni1->keyarr, pbni0->keyarr.num - j, sizeof(_BPT_KEY_INFO)))
		return FALSE; /* Reallocation failure. */
	/* Fill up new allocated array. */
	memcpy(pbni1->keyarr.pdata, ((_P_BPT_KEY_INFO)pbni0->keyarr.pdata) + j, sizeof(_BPT_KEY_INFO) * (pbni0->keyarr.num - j));
	/* Shrink array of the previous node. */
	if (NULL == strResizeArrayZ(&pbni0->keyarr, j, sizeof(_BPT_KEY_INFO)))
		return FALSE; /* Reallocation failure. */
	return TRUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treSplitArrayInNodeBPT
 * Description:   Split array in an internal node by degree.
 * Parameters:
 *       pbki Pointer to a valid _BPT_KEY_INFO structure.
 *            The address of pbki must be allocated first.
 *       pnew Pointer to a new node after pnode in key chain.
 *       pold Pointer to the node that you want to split.
 *     degree Degree of B-plus tree.
 *            Value of degree shall be greater than or equal to 3.
 * Return value:  TRUE  Splitting succeeded.
 *                FALSE Splitting failed.
 * Caution:       Address of pold and pnew Must Be Allocated first.
 */
BOOL _treSplitArrayInNodeBPT(_P_BPT_KEY_INFO pbki, P_BPTNODE pnew, P_BPTNODE pold, const size_t degree)
{
	_P_BPT_INFO pbni0 = (_P_BPT_INFO)pold->pdata;
	_P_BPT_INFO pbni1 = (_P_BPT_INFO)pnew->pdata;
	size_t j = degree >> 1;
	if (NULL == strResizeArrayZ(&pbni1->keyarr, pbni0->keyarr.num - j - 1, sizeof(_BPT_KEY_INFO)))
		return FALSE; /* Reallocation failure. */
	/* Extract the middle key from pold. */
	pbki->pkey   = (((_P_BPT_KEY_INFO)pbni0->keyarr.pdata) + j)->pkey;
	pbki->pchild = (((_P_BPT_KEY_INFO)pbni0->keyarr.pdata) + j)->pchild;
	/* Fill up new allocated array. */
	memcpy(pbni1->keyarr.pdata, ((_P_BPT_KEY_INFO)pbni0->keyarr.pdata) + j + 1, sizeof(_BPT_KEY_INFO) * (pbni0->keyarr.num - j - 1));
	/* Shrink array of the previous node. */
	if (NULL == strResizeArrayZ(&pbni0->keyarr, j, sizeof(_BPT_KEY_INFO)))
		return FALSE; /* Reallocation failure. */
	return TRUE;
}

/* Function name: treInsertBPT
 * Description:   Insert a key into a B-plus indexing tree.
 * Parameters:
 *       pbpt Pointer to B-plus indexing tree.
 *     degree Degree of nodes in B-plus indexing tree.
 *            Value of degree shall be greater than or equal to 3.
 *       pkey Pointer to an element in the memory.
 *            This key will be stored in B-plus tree directly.
 *     cbfcmp Pointer to a callback function that compares data size of pkey.
 * Return value:  TRUE  Insertion succeeded.
 *                FALSE Insertion failed.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
BOOL treInsertBPT(P_BPT pbpt, const size_t degree, const void * pkey, CBF_COMPARE cbfcmp)
{
	P_BPTNODE pnode = *pbpt;
	if (NULL == pnode)
	{	/* Tree is empty. */
		*pbpt = treCreateBPTNode(NULL, NULL);
		if (NULL == *pbpt)
			return FALSE;
		if (NULL == _treInsertKeyIntoArrayBPT((_P_BPT_INFO)((*pbpt)->pdata), pkey, NULL, cbfcmp))
			return FALSE;
	}
	else
	{
		BOOL bleaf = TRUE; /* This variable is used to restrict operations on leaf nodes. */
		pnode = _treLocateKeyInLeafBPT(pbpt, pkey, cbfcmp);
		if (NULL == _treInsertKeyIntoArrayBPT((_P_BPT_INFO)pnode->pdata, pkey, NULL, cbfcmp))
			return FALSE; /* Insertion failure. */
		while (NULL != pnode)
		{
			if (((_P_BPT_INFO)pnode->pdata)->keyarr.num >= degree)
			{	/* Need to split. */
				PUCHAR pk;
				P_ARRAY_Z parrz;
				P_BPTNODE pnew, parent = _treGetParentBPTNode(pnode);
				/* Create a new node. */
				if (NULL == (pnew = treCreateBPTNode(parent, NULL)))
					return FALSE; /* Allocation failure. */
				if (bleaf) /* Split a leaf node. */
				{
					if (FALSE == _treSplitArrayInLeafBPT(pnew, pnode, degree))
					{
						treDeleteBPTNode(pnew);
						return FALSE; /* Can not split array. */
					}
					/* Set next pointer of pnode. */
					pnode->ppnode[NEXTPTR] = pnew;
					/* Get key value form the new split node. */
					parrz = &((_P_BPT_INFO)pnew->pdata)->keyarr;
					pk = ((_P_BPT_KEY_INFO)parrz->pdata)->pkey;
				}
				else /* Split an internal node. */
				{
					_BPT_KEY_INFO bki;
					if (FALSE == _treSplitArrayInNodeBPT(&bki, pnew, pnode, degree))
					{
						treDeleteBPTNode(pnew);
						return FALSE; /* Can not split array. */
					}
					pk = bki.pkey;
					((_P_BPT_INFO)pnew->pdata)->headptr = bki.pchild;
				}
				/* Create a new parent. */
				if (NULL == parent)
				{	/* Root is a leaf. */
					if (NULL == (parent = treCreateBPTNode(parent, NULL)))
						return FALSE;
					/* Alter the root node of tree. */
					*pbpt = parent;
					/* Alter children's parent node. */
					pnew->ppnode[PARENTPTR] = pnode->ppnode[PARENTPTR] = parent;
					/* Alter parent's header child node. */
					((_P_BPT_INFO)parent->pdata)->headptr = pnode;
				}
				/* Insert key into parent. */
				if (NULL == _treInsertKeyIntoArrayBPT((_P_BPT_INFO)parent->pdata, pk, pnew, cbfcmp))
					return FALSE; /* Can not insert key into parent node. */
				pnode = parent;
				bleaf = FALSE;
			}
			else
				break;
		}
	}
	return TRUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treMakeKeyChainBPT
 * Description:   Make a leaf key chain for B-plus trees.
 *                This function is used to cooperate with function treBulkLoadBPT.
 * Parameters:
 *      pquel Pointer to a linked-list queue that used to save leaf nodes.
 *      pprev Pointer to a pointer that pointed the previously created node.
 *     ppkeys Pointer to a pointer to an array of keys.
 *        num Number of keys in new leaf node's array.
 * Return value:  TRUE  Allocation failure.
 *                FALSE Making succeeded.
 *                (*) Especially, if parameter num equaled to zero, function would return value FALSE.
 */
BOOL _treMakeKeyChainBPT(P_QUEUE_L pquel, P_BPTNODE * pprev, PUCHAR * ppkeys[], const size_t num)
{
	if (num > 0)
	{
		REGISTER size_t i;
		_P_BPT_INFO     pti;
		_P_BPT_KEY_INFO pki;
		P_BPTNODE pnew = treCreateBPTNode(NULL, NULL);
		if (NULL == pnew)
			return TRUE; /* Allocation failure. */
		if /* Allocate a new BPT info structure on new created tree node. */
		(  /* Then allocate an array in BPT info structure with num elements. */
			NULL == (pnew->pdata = (PUCHAR) (pti = _treCreateBPTInfo())) ||
			NULL == (pki = (_P_BPT_KEY_INFO) strInitArrayZ(&pti->keyarr, num, sizeof(_BPT_KEY_INFO)))
		)
		{
			free(pti);
			free(pnew);
			return TRUE; /* Allocation failure. */
		}
		for (i = 0; i < num; ++i)
		{
			pki->pchild = NULL;
			pki->pkey = **ppkeys;
			++(*ppkeys);
			++pki;
		}
		if (NULL != *pprev)
			(*pprev)->ppnode[NEXTPTR] = pnew;
		*pprev = pnew;
		queInsertL(pquel, &pnew, sizeof(P_BPTNODE));
	}
	return FALSE;
}

/* Function name: treBulkLoadBPT
 * Description:   Bulk load key pointers into an empty B-plus tree.
 * Parameters:
 *       pbpt Pointer to an empty B-plus tree.
 *     degree Degree of node in B-plus indexing tree.
 *            Value of degree shall be greater than or equal to 3.
 *      pkeys Pointer to an array that contains key pointers in sorted order.
 *            Each key pointer in this array should be converted into (PUCHAR).
 *        num Length of the key pointer array.
 * Return value:  TRUE  Loading succeeded.
 *                FALSE Loading failed.
 * Caution:       (*) The address of pbpt pointed must be allocated first and (*pbpt) shall equal to value NULL.
 *                (*) Users need to sort pointers in the array that pkeys pointed and let the 1st pointer points to the smallest value,
 *                and the last pointer in this array shall point to the biggest value.
 * Tip:           Please use bulk loading function like this way:
 *                int a[5] = { 2,1,3,4,5 }; PUCHAR keys[5]; P_BPT pbpt = treCreateBPT();
 *                // Key array HAS to be sorted, while data array does not need to be sorted, so that we assign (&a[0]) to (keys[1]) and (&a[1]) to (keys[0]).
 *                keys[0] = (PUCHAR)&a[1]; keys[1] = (PUCHAR)&a[0]; keys[2] = (PUCHAR)&a[2]; keys[3] = (PUCHAR)&a[3]; keys[4] = (PUCHAR)&a[4];
 *                treBulkLoadBPT(pbpt, 3, keys, 5); treDeleteBPT(pbpt);
 */
BOOL treBulkLoadBPT(P_BPT pbpt, const size_t degree, PUCHAR pkeys[], size_t num)
{
	if (num > 0 && degree > 2)
	{
		QUEUE_L q1, q2;
		P_BPTNODE pnew, pnode;
		P_QUEUE_L pqa = &q1, pqb = &q2, pqt = NULL;
		REGISTER size_t i;
		P_BPTNODE prev = NULL;
		stdiv_t dr = stdiv(num, degree - 1);
		/* Initialize queues. */
		queInitL(&q1);
		queInitL(&q2);
		/* Make a key chain to link leaf nodes together. Grab keys as many as possible.  */
		for (i = 0; i < dr.quot; ++i)
			if (_treMakeKeyChainBPT(&q1, &prev, &pkeys, degree - 1))
				goto Lbl_Allocation_Failure;
		/* Assemble the rest of keys. */
		if (_treMakeKeyChainBPT(&q1, &prev, &pkeys, dr.rem))
			goto Lbl_Allocation_Failure;
		/* All keys had been enqueued. No leaf node is left behind.
		 * Assemble internal nodes and propagate upward to root.
		 */
		while (! queIsEmptyL(pqa))
		{
			_P_BPT_INFO     pti;
			_P_BPT_KEY_INFO pki;
			/* Get the number of nodes in the queue. */
			i = strLevelLinkedListSC(pqa->pfront);
			if (1 == i)
			{
				queRemoveL(&pnode, sizeof(P_BPTNODE), pqa);
				*pbpt = pnode; /* Assign new root for the tree. */
				break;
			}
			/* Create a new node. */
			if (NULL == (pnew = treCreateBPTNode(NULL, NULL)))
				goto Lbl_Allocation_Failure;
			if /* Allocate a new BPT info structure on new created tree node. */
			(  /* Then allocate an array in BPT info structure with num elements. */
				NULL == (pnew->pdata = (PUCHAR) (pti = _treCreateBPTInfo())) ||
				NULL == (pki = (_P_BPT_KEY_INFO) strInitArrayZ(&pti->keyarr, (i <= degree ? i - 1 : 1), sizeof(_BPT_KEY_INFO)))
			)
			{
				free(pti);
				free(pnew);
				goto Lbl_Allocation_Failure;
			}
			/* Extract the first element in the queue. */
			queRemoveL(&pnode, sizeof(P_BPTNODE), pqa);
			pti->headptr = pnode;
			pnode->ppnode[PARENTPTR] = pnew;
			if (i <= degree)
			{
				while (! queIsEmptyL(pqa))
				{	/* Insert nodes pointers into parent's array. */
					queRemoveL(&pnode, sizeof(P_BPTNODE), pqa);
					pki->pchild = pnode;
					pki->pkey = ((_P_BPT_KEY_INFO)((_P_BPT_INFO)pnode->pdata)->keyarr.pdata)->pkey;
					pnode->ppnode[PARENTPTR] = pnew;
					++pki;
				}
			}
			else
			{	/* Insert nodes pointers into parent's array. */
				queRemoveL(&pnode, sizeof(P_BPTNODE), pqa);
				pki->pchild = pnode;
				pki->pkey = ((_P_BPT_KEY_INFO)((_P_BPT_INFO)pnode->pdata)->keyarr.pdata)->pkey;
				pnode->ppnode[PARENTPTR] = pnew;
			}
			/* Insert new created node into the 2nd. queue. */
			queInsertL(pqb, &pnew, sizeof(P_BPTNODE));
			if (queIsEmptyL(pqa))
				svSwap(&pqa, &pqb, &pqt, sizeof(P_BPTNODE));
		}
		queFreeL(&q1);
		queFreeL(&q2);
		return TRUE;
Lbl_Allocation_Failure:
		treDeleteBPT(pbpt);
		/* Move contents of the 2nd. queue to the 1st. queue. */
		while (!queIsEmptyL(pqb))
		{
			queRemoveL(&pnode, sizeof(P_BPTNODE), pqb);
			queInsertL(pqa, &pnode, sizeof(P_BPTNODE));
		}
		/* Free tree nodes repeatedly. */
		_treFreeBPTPuppet(pqa, pqb);
		queFreeL(&q1);
		queFreeL(&q2);
	}
	return FALSE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treGetSiblingNodeBPT
 * Description:   Get the sibling of a node.
 * Parameters:
 *      pnode Pointer to a node in B-plus tree.
 *     bright TRUE  Get the right sibling of pnode.
 *            FALSE Get the left sibling of pnode.
 * Return value:  Pointer to the sibling of pnode.
 * Caution:       Address of pnode Must Be Allocated first.
 */
P_BPTNODE _treGetSiblingNodeBPT(P_BPTNODE pnode, BOOL bright)
{
	P_BPTNODE parent = _treGetParentBPTNode(pnode);
	if (NULL != parent)
	{
		P_ARRAY_Z parrz = &((_P_BPT_INFO)parent->pdata)->keyarr;
		/* Retrieve entry in parent. */
		if (((_P_BPT_INFO)parent->pdata)->headptr == pnode)
		{	/* Entry pointer of pnode is the header pointer. */
			if (! bright)
				return NULL; /* Header pointer has no left sibling. */
			/* Other wise, return the first pointer in the array. */
			return ((_P_BPT_KEY_INFO)parrz->pdata)->pchild;
		}
		else
		{	/* Locate entry in the pointer array. */
			REGISTER size_t i;
			for (i = 0; i < strLevelArrayZ(parrz); ++i)
			{
				if (i[(_P_BPT_KEY_INFO)parrz->pdata].pchild == pnode)
				{	/* Index of the pointer has been found in the parent node. */
					if (! bright)
					{
						if (0 == i) /* The first element in the array. */
							return ((_P_BPT_INFO)parent->pdata)->headptr;
						return (i - 1)[(_P_BPT_KEY_INFO)parrz->pdata].pchild;
					}
					if (strLevelArrayZ(parrz) - 1 == i)
						return NULL; /* The last pointer in the array has no right sibling. */
					return (i + 1)[(_P_BPT_KEY_INFO)parrz->pdata].pchild;
				}
			}
		}
	}
	return NULL; /* Can not find entry point in the array. */
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treRedistributeNodesBPT
 * Description:   Redistribute node.
 *                This function is used to borrow nodes from pnode's sibling.
 * Parameters:
 *      pnode Pointer to a node in B-plus tree.
 *       psib Pointer to the sibling of pnode.
 *     bright TRUE  Get the right sibling of pnode.
 *            FALSE Get the left sibling of pnode.
 * Return value:  N/A.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void _treRedistributeNodesBPT(P_BPTNODE pnode, P_BPTNODE psib, BOOL bright)
{
	REGISTER size_t i;
	P_BPTNODE parent = _treGetParentBPTNode(pnode);
	P_ARRAY_Z parrz0 = &((_P_BPT_INFO)parent->pdata)->keyarr;
	P_ARRAY_Z parrz1 = &((_P_BPT_INFO)pnode->pdata)->keyarr;
	P_ARRAY_Z parrz2 = &((_P_BPT_INFO)psib->pdata)->keyarr;
	/* Reallocate mem-space for array in pnode. */
	strResizeArrayZ(parrz1, 1, sizeof(_BPT_KEY_INFO));
	/* Locate entry in parent. */
	for (i = 0; i < strLevelArrayZ(parrz0); ++i)
		if (i[(_P_BPT_KEY_INFO)parrz0->pdata].pchild == psib)
			break;
	if (bright) /* Left toward sharing. */
	{
		if (_treIsLeafBPTNode(pnode)) /* Operations on leaf nodes. */
		{
			/* Transfer data into parrz1. */
			((_P_BPT_KEY_INFO)parrz1->pdata)->pkey = i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey;
			/* Move data in sibling node. */
			memcpy(parrz2->pdata, ((_P_BPT_KEY_INFO)parrz2->pdata) + 1, sizeof(_BPT_KEY_INFO) * (strLevelArrayZ(parrz2) - 1));
			strResizeArrayZ(parrz2, strLevelArrayZ(parrz2) - 1, sizeof(_BPT_KEY_INFO));
			/* Alter the key in parent. */
			i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey = ((_P_BPT_KEY_INFO)parrz2->pdata)->pkey;
		}
		else /* Operations on internal nodes. */
		{
			/* Transfer data into parrz1. */
			((_P_BPT_KEY_INFO)parrz1->pdata)->pkey = i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey;
			((_P_BPT_KEY_INFO)parrz1->pdata)->pchild = ((_P_BPT_INFO)psib->pdata)->headptr;
			/* Alter the key in parent. */
			i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey = ((_P_BPT_KEY_INFO)parrz2->pdata)->pkey;
			/* Alter header pointer for sibling node. */
			((_P_BPT_INFO)psib->pdata)->headptr = ((_P_BPT_KEY_INFO)parrz2->pdata)->pchild;
			/* Move data in sibling node. */
			memcpy(parrz2->pdata, ((_P_BPT_KEY_INFO)parrz2->pdata) + 1, sizeof(_BPT_KEY_INFO) * (strLevelArrayZ(parrz2) - 1));
			strResizeArrayZ(parrz2, strLevelArrayZ(parrz2) - 1, sizeof(_BPT_KEY_INFO));
		}
	}
	else /* Right toward sharing. */
	{
		if (_treIsLeafBPTNode(pnode)) /* Operations on leaf nodes. */
		{
			/* Alter key in parent. */
			i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey = (strLevelArrayZ(parrz2) - 1)[(_P_BPT_KEY_INFO)parrz2->pdata].pkey;
			/* Alter key in pnode. */
			((_P_BPT_KEY_INFO)parrz1->pdata)->pkey = i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey;
			/* Alter sibling node. */
			strResizeArrayZ(parrz2, strLevelArrayZ(parrz2) - 1, sizeof(_BPT_KEY_INFO));
		}
		else /* Operations on internal nodes. */
		{
			/* Alter key in parent. */
			i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey = (strLevelArrayZ(parrz2) - 1)[(_P_BPT_KEY_INFO)parrz2->pdata].pkey;
			/* Alter key in pnode. */
			((_P_BPT_KEY_INFO)parrz1->pdata)->pkey = i[(_P_BPT_KEY_INFO)parrz0->pdata].pkey;
			((_P_BPT_KEY_INFO)parrz1->pdata)->pchild = ((_P_BPT_INFO)pnode->pdata)->headptr;
			/* Alter header pointer in pnode. */
			((_P_BPT_INFO)pnode->pdata)->headptr = (strLevelArrayZ(parrz2) - 1)[(_P_BPT_KEY_INFO)parrz2->pdata].pchild;
			/* Alter sibling node. */
			strResizeArrayZ(parrz2, strLevelArrayZ(parrz2) - 1, sizeof(_BPT_KEY_INFO));
		}
	}
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treMergeNodesBPT
 * Description:   Merge two leaf nodes in a B-plus indexing tree.
 *                This function is used to merge pnode and it's sibling.
 * Parameters:
 *      pnode Pointer to a node in B-plus tree.
 *       psib Pointer to the sibling node of pnode.
 *      phead Pointer to the header of key chain in B-plus indexing tree.
 *     bright TRUE  Get the right sibling of pnode.
 *            FALSE Get the left sibling of pnode.
 * Return value:  Pointer to the new merged node.
 * Caution:       Address of pnode Must Be Allocated first.
 */
void _treMergeNodesBPT(P_BPTNODE pnode, P_BPTNODE psib, P_BPTNODE phead, BOOL bright)
{
	P_BPTNODE parent = _treGetParentBPTNode(pnode);
	P_ARRAY_Z parrz0 = &((_P_BPT_INFO)parent->pdata)->keyarr;
	P_ARRAY_Z parrz1 = &((_P_BPT_INFO)pnode->pdata)->keyarr;
	P_ARRAY_Z parrz2 = &((_P_BPT_INFO)psib->pdata)->keyarr;
	if (bright) /* Left toward fusing. */
	{
		BOOL b = FALSE;
		if (_treIsLeafBPTNode(pnode))
		{
			/* Locate the previous item. */
			if (phead != pnode)
			{
				while (NULL != phead)
				{
					if (phead->ppnode[NEXTPTR] == pnode)
						break;
					phead = phead->ppnode[NEXTPTR];
				}
				if (NULL != phead)
					phead->ppnode[NEXTPTR] = psib;
			}
			else
				b = TRUE;
			/* Alter header pointer in parent node. */
			((_P_BPT_INFO)parent->pdata)->headptr = ((_P_BPT_KEY_INFO)parrz0->pdata)->pchild;
			/* Left shift key value in parent. */
			memcpy
			(
				parrz0->pdata,
				((_P_BPT_KEY_INFO)parrz0->pdata) + 1,
				sizeof(_BPT_KEY_INFO) * (strLevelArrayZ(parrz0) - 1)
			);
		}
		else
		{	/* Reallocate pnode. */
			strResizeArrayZ(parrz1, strLevelArrayZ(parrz1) + 1, sizeof(_BPT_KEY_INFO));
			/* Upload the first key in parent. */
			((_P_BPT_KEY_INFO)parrz1->pdata)->pkey = ((_P_BPT_KEY_INFO)parrz0->pdata)->pkey;
			((_P_BPT_KEY_INFO)parrz1->pdata)->pchild = ((_P_BPT_INFO)psib->pdata)->headptr;
			/* Copy data in sibling node into pnode. */
			memcpy
			(
				((_P_BPT_KEY_INFO)parrz1->pdata) + 1,
				(_P_BPT_KEY_INFO)parrz2->pdata,
				sizeof(_BPT_KEY_INFO)
			);
			/* Move data in parent. */
			if (strLevelArrayZ(parrz0) >= 2)
				memcpy(parrz0->pdata, ((_P_BPT_KEY_INFO)parrz0->pdata) + 1, sizeof(_BPT_KEY_INFO) * (strLevelArrayZ(parrz0) - 1));
		}
		/* Shrink parent. */
		strResizeArrayZ(parrz0, strLevelArrayZ(parrz0) - 1, sizeof(_BPT_KEY_INFO));
		if (b) return; /* If the tree had only one root node, we could not delete the same node twice. */
		pnode = psib;
	}
	else /* Right toward fusing. */
	{
		if (_treIsLeafBPTNode(pnode))
		{
			/* Detach pnode in key chain. */
			psib->ppnode[NEXTPTR] = pnode->ppnode[NEXTPTR];
			/* Left shift key value in parent. */
			if (((_P_BPT_KEY_INFO)parrz0->pdata)->pchild == pnode)
				memcpy(parrz0->pdata, ((_P_BPT_KEY_INFO)parrz0->pdata) + 1, sizeof(_BPT_KEY_INFO) * (strLevelArrayZ(parrz0) - 1));
		}
		else
		{
			/* Reallocate sibling node. */
			strResizeArrayZ(parrz2, strLevelArrayZ(parrz2) + 1, sizeof(_BPT_KEY_INFO));
			/* Upload key in parent. */
			(strLevelArrayZ(parrz2) - 1)[(_P_BPT_KEY_INFO)parrz2->pdata].pkey = ((_P_BPT_KEY_INFO)parrz0->pdata)->pkey;
			(strLevelArrayZ(parrz2) - 1)[(_P_BPT_KEY_INFO)parrz2->pdata].pchild = ((_P_BPT_INFO)pnode->pdata)->headptr;
			/* Move keys in parent. */
			memcpy(parrz0->pdata, ((_P_BPT_KEY_INFO)parrz0->pdata) + 1, sizeof(_BPT_KEY_INFO));
		}
		/* Shrink the array in parent. */
		strResizeArrayZ(parrz0, strLevelArrayZ(parrz0) - 1, sizeof(_BPT_KEY_INFO));
	}
	/* Delete node. */
	treDeleteBPTNode(pnode);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treBPTRemovePuppet
 * Description:   This function is a callee for function treRemoveBPT.
 * Parameters:
 *       pbpt Pointer to B-plus indexing tree.
 *      pnode Pointer to the left-most leaf in a B-plus tree.
 *       hdeg The value of degree of node in B-plus indexing tree divide by 2.
 *       pkey Pointer to an element which is stored in the B-plus tree.
 *     cbfcmp Pointer to a callback function that compares data size of pkey.
 * Return value:  TRUE  Removal succeed.
 *                FALSE Removal failed.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
BOOL _treBPTRemovePuppet(P_BPT pbpt, P_BPTNODE pnode, const size_t hdeg, const void * pkey, CBF_COMPARE cbfcmp)
{
	size_t i;
	P_BPTNODE phead  = _treLocateKeyChainHeaderBPT(pbpt);
	P_ARRAY_Z parrz = &((_P_BPT_INFO)pnode->pdata)->keyarr;
	/* Remove the entry. */
	if (0 == (i = _treRemoveKeyFromArrayBPT((_P_BPT_INFO)pnode->pdata, pkey, cbfcmp)))
		return FALSE; /* Removal failure. */
	else if (1 == i && pnode == *pbpt)
	{	/* Current node is the root node and it is empty.*/
		treDeleteBPTNode(pnode); /* Delete parent. */
		*pbpt = NULL;
	}
	else if (strLevelArrayZ(parrz) < hdeg)
	{
		BOOL d;
		/* Fetch sibling node. */
		P_BPTNODE psib = _treGetSiblingNodeBPT(pnode, (d = RIGHT));
		if (NULL == psib)
		{
			psib = _treGetSiblingNodeBPT(pnode, (d = LEFT));
			if (NULL == psib)
				return FALSE; /* Error occurred while pnode has no sibling. */
		}
		parrz = &((_P_BPT_INFO)psib->pdata)->keyarr;
		if (strLevelArrayZ(parrz) > hdeg) /* Borrow a key from sibling node. */
			_treRedistributeNodesBPT(pnode, psib, d);
		else /* Merge node and it's sibling. */
		{
			_treMergeNodesBPT(pnode, psib, phead, d);
			return _treBPTRemovePuppet(pbpt, _treGetParentBPTNode(pnode), hdeg, pkey, cbfcmp);
		}
	}
	return TRUE;
}

/* Function name: treRemoveBPT
 * Description:   Remove a key from a B-plus indexing tree.
 * Parameters:
 *       pbpt Pointer to B-plus indexing tree.
 *     degree Degree of nodes in B-plus indexing tree.
 *            Value of degree shall be greater than or equal to 3.
 *       pkey Pointer to an element which is stored in the B-plus tree.
 *     cbfcmp Pointer to a callback function that compares data size of pkey.
 * Return value:  TRUE  Removal succeeded.
 *                FALSE Removal failed.
 * Caution:       Address of pbpt Must Be Allocated first.
 */
BOOL treRemoveBPT(P_BPT pbpt, const size_t degree, const void * pkey, CBF_COMPARE cbfcmp)
{
	P_BPTNODE pnode = *pbpt;
	if (NULL != pnode)
	{	/* Remove key when tree is not empty. */
		pnode = _treLocateKeyInLeafBPT(pbpt, pkey, cbfcmp);
		return _treBPTRemovePuppet(pbpt, pnode, degree >> 1, pkey, cbfcmp);
	}
	return FALSE;
}

#undef PARENTPTR
#undef NEXTPTR /* Undefine two macros here, because B-plus tree section ends at this place. */

/* Functions that implemented tries are listed here. */
#include "svstack.h"

/* A macro used to calculate the length of an element in a trie's array. */
#define _ELESIZ(size) ((size) + sizeof(TRIE_A) + sizeof(size_t) + sizeof(size_t) + sizeof(UCHART))
/* Element consists of: data-----next_trie----reference_counter---appendix---------flag. */

/* File level function declarations. */
void _treFreeTrieNode(P_TRIE_A ptrie, size_t size);

/* Function name: treInitTrieA_O
 * Description:   Initialize an array implemented trie.
 * Parameters:
 *      ptrie Pointer to a trie you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of ptrie Must Be Allocated first.
 * Tip:           A macro version of this function named treInitTrieA_M is available.
 */
void treInitTrieA_O(P_TRIE_A ptrie)
{
	*ptrie = NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treFreeTrieNode
 * Description:   This function is used to free array implemented nodes in a trie.
 * Parameters:
 *      ptrie Pointer to a trie you want to deallocate.
 *       size Size of an element in the string which had been inserted into trie before.
 * Return value:  N/A.
 * Caution:       Address of ptrie Must Be Allocated first.
 *                Size shall not equal to 0.
 */
void _treFreeTrieNode(P_TRIE_A ptrie, size_t size)
{
	if (NULL != *ptrie)
	{
		REGISTER size_t i;
		REGISTER PUCHAR pdat = (*ptrie)->pdata;
		for (i = 0; i < strLevelArrayZ(*ptrie); ++i, pdat += _ELESIZ(size))
		{
			P_TRIE_A pt = (P_TRIE_A)(pdat + size);
			if (NULL != *pt)
				_treFreeTrieNode(pt, size);
		}
		strDeleteArrayZ(*ptrie);
	}
}

/* Function name: treFreeTrieA_O
 * Description:   Deallocate a trie which is allocated by function treInitTrieA_O.
 * Parameters:
 *      ptrie Pointer to a trie you want to deallocate.
 *       size Size of an element in the string which had been inserted into trie before.
 * Return value:  N/A.
 * Caution:       Address of ptrie Must Be Allocated first.
 *                Size shall not equal to 0.
 * Tip:           A macro version of this function named treFreeTrieA_M is available.
 */
void treFreeTrieA_O(P_TRIE_A ptrie, size_t size)
{
	_treFreeTrieNode(ptrie, size);
	*ptrie = NULL;
}

/* Function name: treCreateTrieA
 * Description:   Allocate a new array implemented trie dynamically.
 * Parameters:    N/A.
 * Return value:  Pointer to the new allocated trie.
 */
P_TRIE_A treCreateTrieA(void)
{
	P_TRIE_A ptrie = (P_TRIE_A) malloc(sizeof(TRIE_A));
	if (NULL != ptrie)
		treInitTrieA(ptrie);
	return ptrie;
}

/* Function name: treDeleteTrieA_O
 * Description:   Delete a trie of which is allocated by function treCreateTrieA.
 * Parameters:
 *      ptrie Pointer to a trie you want to allocate.
 *       size Size of an element in the string which had been inserted into trie before.
 * Return value:  N/A.
 * Caution:       Address of ptrie Must Be Allocated first.
 *                Size shall not equal to 0.
 * Tip:           A macro version of this function named treDeleteTrieA_M is available.
 */
void treDeleteTrieA_O(P_TRIE_A ptrie, size_t size)
{
	_treFreeTrieNode(ptrie, size);
	free(ptrie);
}

/* Function name: treSearchTrieA
 * Description:   Search a string in a trie.
 * Parameters:
 *      ptrie Pointer to a trie that you wanna operate on.
 *       pstr Pointer to the first element to a string and casted the pointer into (const void *).
 *        num Number of elements in the string.
 *       size Size of each element in the string.
 *     cbfcmp Pointer a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the string.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 * Return value:  Pointer to the value that contains vapdx stored in a trie.
 *                If this function returned value NULL, it should indicate that the string pstr pointed cannot find.
 * Caution:       ptrie must be allocated first.
 * Tip:           The following codes may give you an example for the usage of tries:
 *                const char ptip[] = "lynx"; size_t apdx = (size_t)ptip; P_TRIE_A pt = treCreateTrieA();
 *                treInsertTrieA(pt, "cat", strlen("cat"), sizeof(char), apdx, cbfcmp);
 *                apdx = *treSearchTrieA(pt, "cat", strlen("cat"), sizeof(char), cbfcmp);
 *                printf("%s\n", (const char *)apdx); treDeleteTrieA(pt, sizeof(char));
 */
size_t * treSearchTrieA(P_TRIE_A ptrie, const void * pstr, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	REGISTER PUCHAR pbase = (PUCHAR) pstr;
	if (0 == num || 0 == size)
		return NULL;
	while (NULL != *ptrie && 0 != num)
	{
		REGISTER PUCHAR pdat;
		if (NULL == (pdat = (PUCHAR) strBinarySearchArrayZ(*ptrie, pbase, _ELESIZ(size), cbfcmp)))
			return NULL;
		ptrie = (P_TRIE_A) &pdat[size];
		--num;
		++pbase;
	}
	if (0 == num) /* Searching reaches at the end of string. */
		if (FALSE != *((PUCHAR) &(sizeof(TRIE_A) + sizeof(size_t) + sizeof(size_t))[(PUCHAR) ptrie]))
			return ((size_t *) &(sizeof(TRIE_A) + sizeof(size_t))[(PUCHAR) ptrie]);
	return NULL;
}

/* Function name: treInsertTrieA
 * Description:   Insert a string into a trie.
 * Parameters:
 *      ptrie Pointer to a trie that you wanna operate on.
 *       pstr Pointer to the first element to a string and casted the pointer into (const void *).
 *        num Number of elements in the string.
 *       size Size of each element in the string.
 *      vapdx An integer in size_t as an appendix that can store a pointer.
 *            You may get a pointer to the value that contains vapdx stored in a trie after calling function treSearchTrieA.
 *     cbfcmp Pointer a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the string.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 * Return value:  TRUE  Insertion succeeded.
 *                FALSE Insertion failed.
 * Caution:       ptrie must be allocated first.
 * Tip:           Inserting the same string in multiple times is allowed for tries;
 *                Then deletion of the same string needs to be duplicated in the same times.
 *                Value vapdx that stored with a string in trie will be replaced if users insert the same string repeatedly.
 */
BOOL treInsertTrieA(P_TRIE_A ptrie, const void * pstr, size_t num, size_t size, size_t vapdx, CBF_COMPARE cbfcmp)
{
	REGISTER size_t j;
	REGISTER PUCHAR pdat;
	REGISTER PUCHAR pbase = (PUCHAR) pstr;
	for (j = 0; j < num; ++j, ++pbase, ptrie = (P_TRIE_A) &pdat[size])
	{
		if (NULL != *ptrie)
		{
			if (NULL != (pdat = (PUCHAR) strBinarySearchArrayZ(*ptrie, pbase, _ELESIZ(size), cbfcmp)))
			{
				++ *(size_t *) &pdat[size + sizeof(TRIE_A)]; /* Increase reference counter. */
				continue; /* Found, then search the next element in string. */
			}
			else
			{	/* Find the maximal value that is less than (*pbase) to index i.
				 * This procedure would reduce the worst case of searching complexity to approximately O(lg N),
				 * but not restricts to the worst case of O(N) by using linear search.
				 */
				REGISTER size_t i = strBinaryLocateArrayZ(*ptrie, pbase, _ELESIZ(size), cbfcmp);
				if (NULL == strResizeArrayZ(*ptrie, strLevelArrayZ(*ptrie) + 1, _ELESIZ(size)))
					return FALSE; /* Allocation failure. */
				else
				{	/* Insert new item. */
					pdat = (PUCHAR)memcpy
					(	/* Leave spaces to the new element. */
						(PUCHAR)memmove
						(
							(*ptrie)->pdata + (i + 1) * _ELESIZ(size),
							(*ptrie)->pdata + i * _ELESIZ(size),
							(strLevelArrayZ(*ptrie) - 1 - i) * _ELESIZ(size)
						) - _ELESIZ(size),
						pbase,
						size
					);
				}
			}
		}
		else
		{
			if (NULL == (*ptrie = strCreateArrayZ(1, _ELESIZ(size))))
				return FALSE; /* Allocation failure. */
			else
				pdat = (PUCHAR) memcpy((*ptrie)->pdata, pbase, size);
		}
		/* Initialize new block. */
		*(P_TRIE_A)  &pdat[size] = NULL; /* pnext. */
		*(size_t *)  &pdat[size + sizeof(TRIE_A)] = 1; /* Reference counter. */
		*((size_t *) &pdat[size + sizeof(TRIE_A) + sizeof(size_t)]) = 0; /* Appendix. */
		*((PUCHAR)   &pdat[size + sizeof(TRIE_A) + sizeof(size_t) + sizeof(size_t)]) = FALSE; /* Flag. */
	}
	/* Bypass reference counter and assign appendix onto the right position. */
	*((size_t *) &(sizeof(TRIE_A) + sizeof(size_t))[(PUCHAR) ptrie]) = vapdx;
	/* Set the flag on the tail of string in trie into valid. */
	*((PUCHAR)  &(sizeof(TRIE_A) + sizeof(size_t) + sizeof(size_t))[(PUCHAR) ptrie]) = TRUE;
	return TRUE;
}

/* Function name: treRemoveTrieA
 * Description:   Remove a string from a trie.
 * Parameters:
 *      ptrie Pointer to a trie that you wanna operate on.
 *       pstr Pointer to the first element to a string and casted the pointer into (const void *).
 *        num Number of elements in the string.
 *       size Size of each element in the string.
 *     cbfcmp Pointer a CBF_COMPARE callback function.
 *            Two parameters of this callback function may point to any element in the string.
 *            Please refer to the prototype of callback function CBF_COMPARE in file svdef.h.
 * Return value:  TRUE  Removal succeeded.
 *                FALSE Removal failed.
 * Caution:       ptrie must be allocated first.
 */
BOOL treRemoveTrieA(P_TRIE_A ptrie, const void * pstr, size_t num, size_t size, CBF_COMPARE cbfcmp)
{
	BOOL r = FALSE;
	P_STACK_L pstk; /* Stores every data to be erased. */
	const P_TRIE_A pot = ptrie; /* Stores the original header of trie. */
	REGISTER PUCHAR pbase = (PUCHAR) pstr;
	/* In the following structure, pdat is the pointer that leads data in array (*trie). */
	struct { TRIE_A trie; PUCHAR pdat; } eleset;
	if ((0 == num || 0 == size) || NULL == (pstk = stkCreateL()))
		return FALSE; /* Allocation failure. */
	/* Traverse trie to pick elements that to be deleted into a stack. */
	while (NULL != *ptrie && 0 != num)
	{
		REGISTER PUCHAR pdat;
		if (NULL == (pdat = (PUCHAR) strBinarySearchArrayZ(*ptrie, pbase, _ELESIZ(size), cbfcmp)))
			goto Lbl_Clear;
		else
		{
			eleset.trie = *ptrie;
			eleset.pdat = pdat;
			if (NULL == stkPushL(pstk, &eleset, sizeof(eleset)))
				goto Lbl_Clear;
		}
		ptrie = (P_TRIE_A) &pdat[size];
		--num;
		++pbase;
	}
	if (0 == num)
	{
		PUCHAR pflag = ((PUCHAR) &(sizeof(TRIE_A) + sizeof(size_t) + sizeof(size_t))[(PUCHAR) ptrie]);
		if (FALSE == *pflag) /* Avoid to delete a found sub string. */
			goto Lbl_Clear;
		else
		{
			REGISTER TRIE_A tprev = NULL; /* Previous trie record. */
			if (*((size_t *) &(sizeof(TRIE_A))[(PUCHAR) ptrie]) < 2)
				*pflag = FALSE; /* Unflag. */
			while (! stkIsEmptyL(pstk))
			{	/* Pop elements to be deleted one by one that previously collected in the stack. */
				stkPopL(&eleset, sizeof(eleset), pstk);
				/* Erase the previous child pointer in parent node. */
				if (NULL != tprev)
				{
					*(P_TRIE_A)&eleset.pdat[size] = NULL;
					tprev = NULL;
				}
				/* Pick an element in array away by reducing reference counter. */
				if (0 == --*(size_t *)&eleset.pdat[size + sizeof(TRIE_A)])
				{
					strRemoveItemArrayZ
					(
						eleset.trie,
						_ELESIZ(size),
						(eleset.pdat - eleset.trie->pdata) / _ELESIZ(size),
						TRUE
					);
					/* If there are not exist elements in the set, then remove array entity either. */
					if (0 == strLevelArrayZ(eleset.trie))
					{
						tprev = eleset.trie; /* Save a deleted pointer and remove it in its parent later. */
						strDeleteArrayZ(eleset.trie);
						if (*pot == tprev)
							*pot = NULL; /* Removal reaches at the root of trie. */
					}
				}
			}
			r = TRUE;
		}
	}
Lbl_Clear:
	stkDeleteL(pstk);
	return r;
}

#undef _ELESIZ /* Undefine a used macro here to avert users mistakenly touching it. */
