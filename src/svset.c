/*
 * Name:        svset.c
 * Description: Sets.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171620L0512251300L00914
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

#include "svset.h"

/* These following two macros are used to switch between BSTs. */
#define _setInsertBST treBSTInsertAA
#define _setRemoveBST treBSTRemoveAA

/* Callback function declarations for sets using hash table. */
int _setCBFIsSubsetHPuppet     (void * pitem, size_t param);
int _setCBFUnionHPuppet        (void * pitem, size_t param);
int _setCBFIntersectionHPuppet (void * pitem, size_t param);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFIsSubsetHPuppet
 * Description:   This function is used to check the sub-set property of a set.
 * Parameters:
 *      pitem Pointer to each NODE_S in hash table.
 *      param Pointer to a size_t[3] array.
 *            size_t[0] stores psetb of the caller function.
 *            size_t[1] stores a pointer to hash function.
 *            size_t[2] stores the size of each element.
 * Return value:  CBF_CONTINUE  Element cannot be found in psetb.
 *                CBF_TERMINATE Element has been found in psetb.
 */
int _setCBFIsSubsetHPuppet(void * pitem, size_t param)
{
	if
	(
		NULL == hshSearchC
		(
			(P_HSHTBL_C) 0[(size_t *)param],
			(CBF_HASH)1[(size_t *)param],
			pitem,
			2[(size_t *)param]
		)
	)
		return CBF_TERMINATE;
	return CBF_CONTINUE;
}

/* Function name: setInitH_O
 * Description:   Initialize a set.
 * Parameters:
 *       pset Pointer to the set you want to allocate.
 *    buckets Number of buckets for the hash table in the set.
 *            This value shall be a prime number.
 * Return value:  true  Succeeded.
 *                false Failed.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setInitH_M is available.
 */
bool setInitH_O(P_SET_H pset, size_t buckets)
{
	return hshInitC(pset, buckets);
}

/* Function name: setFreeH_O
 * Description:   Deallocate the set of which is allocated by function setInitH_O.
 * Parameter:
 *      pset Pointer to the set you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setFreeH_M is available.
 */
void setFreeH_O(P_SET_H pset)
{
	hshFreeC(pset);
}

/* Function name: setCreateH_O
 * Description:   Create a set.
 * Parameter:
 *   buckets Number of buckets for the hash table in the set.
 *           This value shall be a prime number.
 * Return value:  Pointer to the new allocated set.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setCreateH_M is available.
 */
P_SET_H setCreateH_O(size_t buckets)
{
	return hshCreateC(buckets);
}

/* Function name: setDeleteH_O
 * Description:   Delete the set of which is allocated by function setCreateH_O.
 * Parameter:
 *      pset Pointer to the set you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setDeleteH_M is available.
 */
void setDeleteH_O(P_SET_H pset)
{
	if (NULL != pset)
		hshDeleteC(pset);
}

/* Function name: setSizeH_O
 * Description:   Determine how many elements there are in the set.
 * Parameter:
 *      pset Pointer to the set you want to test.
 * Return value:  Number of elements.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setSizeH_M is available.
 */
size_t setSizeH_O(P_SET_H pset)
{
	return NULL == pset ? 0 : hshSizeC(pset);
}

/* Function name: setIsEmptyH_O
 * Description:   Make a judgment whether a set is empty or not.
 * Parameter:
 *      pset Pointer to the stack you want to check.
 * Return value:
 *         true Set is empty.
 *        false Set is not empty.
 * Tip:           A macro version of this function named setIsEmptyH_M is available.
 */
bool setIsEmptyH_O(P_SET_H pset)
{
	return !setSizeH(pset);
}

/* Function name: setIsMemberH_O
 * Description:   Check whether an element belongs to a set or not.
 * Parameters:
 *       pset Pointer to the set you want to check.
 *     cbfhsh Pointer to a hash function.
 *            The same set should use the same hash function.
 *      pitem Pointer to an element to want to check.
 *       size Size of that element.
 * Return value:
 *         true Element belongs to the set.
 *        false Element does not belong to the set.
 * Tip:           A macro version of this function named setIsMemberH_M is available.
 */
bool setIsMemberH_O(P_SET_H pset, CBF_HASH cbfhsh, const void * pitem, size_t size)
{
	return NULL == pset ? false : (NULL != hshSearchC(pset, cbfhsh, pitem, size) ? true : false);
}

/* Function name: setIsSubsetH
 * Description:   Check whether a set is a sub set of another.
 * Parameters:
 *      pseta Pointer to the set you want to check.
 *      psetb Pointer to the other set you want to check.
 *     cbfhsh Pointer to a hash function for psetb.
 *            Two sets should use the same hash function.
 *       size Size of that element.
 * Return value:
 *         true seta belongs to setb.
 *        false seta does not belong setb.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
bool setIsSubsetH(P_SET_H pseta, P_SET_H psetb, CBF_HASH cbfhsh, size_t size)
{
	if (NULL != pseta)
	{
		size_t a[3];
		a[0] = (size_t)psetb;
		a[1] = (size_t)cbfhsh;
		a[2] = size;
		return CBF_CONTINUE != hshTraverseC(pseta, _setCBFIsSubsetHPuppet, (size_t)a) ? false : true;
	}
	return true; /* An empty set is a subset of any set. */
}

/* Function name: setIsEqualH
 * Description:   Check whether a set equal to another.
 * Parameters:
 *      pseta Pointer to the set you want to check.
 *      psetb Pointer to the other set you want to check.
 *     cbfhsh Pointer to a hash function for both pseta and psetb.
 *            Two sets should use the same hash function.
 *       size Size of element.
 * Return value:
 *         true Two sets are equal.
 *        false Two sets are NOT equal.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
bool setIsEqualH(P_SET_H pseta, P_SET_H psetb, CBF_HASH cbfhsh, size_t size)
{
	if (pseta == psetb || (setIsSubsetH(pseta, psetb, cbfhsh, size) && setIsSubsetH(psetb, pseta, cbfhsh, size)))
		return true;
	return false;
}

/* Function name: setInsertH
 * Description:   Insert an element into a set.
 * Parameters:
 *       pset Pointer to the set you want to operate.
 *     cbfhsh Pointer to a hash function.
 *            The same set should use the same hash function.
 *      pitem Pointer to an element to want to insert.
 *       size Size of that element.
 * Return value:
 *         true Insertion succeeded.
 *        false Insertion failed.
 * Caution:  pset cannot be NULL. Please check the value of pset before invoking.
 */
bool setInsertH(P_SET_H pset, CBF_HASH cbfhsh, const void * pitem, size_t size)
{
	if (NULL != hshSearchC(pset, cbfhsh, pitem, size))
		return false; /* Item has already existed. */
	return hshInsertC(pset, cbfhsh, pitem, size);
}

/* Function name: setRemoveH
 * Description:   Remove an element from a set.
 * Parameters:
 *       pset Pointer to the set you want to operate.
 *     cbfhsh Pointer to a hash function.
 *            The same set should use the same hash function.
 *      pitem Pointer to an element you want to remove.
 *       size Size of that element.
 * Return value:
 *         true Removal succeeded.
 *        false Removal failed.
 * Caution:  pset can not be NULL. Please check the value of pset before invoking.
 */
bool setRemoveH(P_SET_H pset, CBF_HASH cbfhsh, const void * pitem, size_t size)
{
	return hshRemoveC(pset, cbfhsh, pitem, size);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFUnionHPuppet
 * Description:   This function is used to generate a union set.
 * Parameters:
 *      pitem Pointer to each NODE_S in hash table.
 *      param Pointer to a size_t[3] array.
 *            size_t[0] stores psetr of the caller function.
 *            size_t[1] stores a pointer to hash function.
 *            size_t[2] stores the size of each element.
 * Return value:  CBF_CONTINUE only.
 */
int _setCBFUnionHPuppet(void * pitem, size_t param)
{
	setInsertH
	(
		(P_SET_H)0[(size_t *)param],
		(CBF_HASH)1[(size_t *)param],
		pitem,
		2[(size_t *)param]
	);
	return CBF_CONTINUE;
}

/* Function name: setCreateUnionH
 * Description:   Generate a union for two sets.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfhsh Pointer to a hash function for both pseta and psetb.
 *            Two sets should use the same hash function.
 * Return value:  Pointer to a new set that contains the result of the union of two sets.
 *                NULL would be returned if the result were an empty set.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_H setCreateUnionH(P_SET_H pseta, P_SET_H psetb, CBF_HASH cbfhsh, size_t size)
{
	if (NULL != pseta || NULL != psetb)
	{
		REGISTER P_SET_H psetr;
		size_t a[3];
		if (NULL == (psetr = setCreateH(NULL == psetb ? strLevelArrayZ(pseta) : strLevelArrayZ(psetb))))
			return NULL;
		a[0] = (size_t)psetr;
		a[1] = (size_t)cbfhsh;
		a[2] = size;
		if (NULL != pseta)
			hshTraverseC(pseta, _setCBFUnionHPuppet, (size_t)a);
		if (NULL != psetb)
			hshTraverseC(psetb, _setCBFUnionHPuppet, (size_t)a);
		if (setIsEmptyH(psetr))
		{
			setDeleteH(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFIntersectionHPuppet
 * Description:   This function is used to generate an intersection set.
 * Parameters:
 *      pitem Pointer to each NODE_S in hash table.
 *      param Pointer to a size_t[5] array.
 *            size_t[0] stores psetr of the caller function.
 *            size_t[1] stores a pointer to hash function.
 *            size_t[2] stores the size of each element.
 *            size_t[3] stores either pseta or psetb of the caller function. (Flexible)
 *            size_t[4] stores a boolean value.
 *                      If size_t[4] == false, Do insertion only when element in set
 *                      size_t[3] did not match elements in set size_t[0] at all.
 *                      So that we can generate a difference set of set A and set B.
 * Return value:  CBF_CONTINUE only.
 */
int _setCBFIntersectionHPuppet(void * pitem, size_t param)
{
	if
	(
		setIsMemberH
		(
			(P_SET_H)3[(size_t *)param],
			(CBF_HASH)1[(size_t *)param],
			pitem,
			2[(size_t *)param]
		) == (bool)4[(size_t *)param]
	)
	{
		setInsertH
		(
			(P_SET_H)0[(size_t *)param],
			(CBF_HASH)1[(size_t *)param],
			pitem,
			2[(size_t *)param]
		);
	}
	return CBF_CONTINUE;
}

/* Function name: setCreateIntersectionH
 * Description:   Generate an intersection between two sets.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfhsh Pointer to a hash function for both pseta and psetb.
 *            Two sets should use the same hash function.
 * Return value:  Pointer to a new set that contains the result of the intersection between two sets.
 *                NULL would be returned if the result were an empty set.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_H setCreateIntersectionH(P_SET_H pseta, P_SET_H psetb, CBF_HASH cbfhsh, size_t size)
{
	if (NULL != pseta || NULL != psetb)
	{
		REGISTER P_SET_H psetr;
		size_t a[5];
		if (NULL == (psetr = setCreateH(NULL == psetb ? strLevelArrayZ(pseta) : strLevelArrayZ(psetb))))
			return NULL;
		a[0] = (size_t)psetr;
		a[1] = (size_t)cbfhsh;
		a[2] = size;
		a[4] = true;
		if (NULL != psetb)
		{
			a[3] = (size_t)psetb;
			hshTraverseC(pseta, _setCBFIntersectionHPuppet, (size_t)a);
		}
		if (NULL != pseta && pseta != psetb)
		{
			a[3] = (size_t)pseta;
			hshTraverseC(psetb, _setCBFIntersectionHPuppet, (size_t)a);
		}
		if (setIsEmptyH(psetr))
		{
			setDeleteH(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Function name: setCreateDifferenceH
 * Description:   Generate a difference set of two sets.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfhsh Pointer to a hash function for both pseta and psetb.
 *            Two sets should use the same hash function.
 * Return value:  Pointer to a new set that contains the result of a difference set of two sets.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_H setCreateDifferenceH(P_SET_H pseta, P_SET_H psetb, CBF_HASH cbfhsh, size_t size)
{
	if (NULL != pseta)
	{
		REGISTER P_SET_H psetr;
		size_t a[5];
		if (NULL == (psetr = setCreateH(NULL == psetb ? strLevelArrayZ(pseta) : strLevelArrayZ(psetb))))
			return NULL;
		a[0] = (size_t)psetr;
		a[1] = (size_t)cbfhsh;
		a[2] = size;
		a[3] = (size_t)psetb;
		a[4] = false;
		hshTraverseC(pseta, _setCBFIntersectionHPuppet, (size_t)a);
		if (setIsEmptyH(psetr))
		{
			setDeleteH(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Functions for BST style sets. */

/* Callback function for sets using BSTs. */
int _setCBFIsSubsetTPuppet     (void * pitem, size_t param);
int _setCBFInsertItemTPuppet   (void * pitem, size_t param);
int _setCBFIntersectionTPuppet (void * pitem, size_t param);

/* Function name: setInitT_O
 * Description:   Initialize a BST style set.
 * Parameter:
 *      pset Pointer to the set you want to allocate.
 * Return value:  N/A.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setInitT_M is available.
 */
void setInitT_O(P_SET_T pset)
{
	treInitBST(pset);
}

/* Function name: setFreeT_O
 * Description:   Deallocate the set of which is allocated by function setInitT_O.
 * Parameter:
 *      pset Pointer to the set you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setFreeT_M is available.
 */
void setFreeT_O(P_SET_T pset)
{
	treFreeBST(pset);
}

/* Function name: setCreateT_O
 * Description:   Create a set.
 * Parameter:     N/A.
 * Return value:  Pointer to the new allocated set.
 * Tip:           A macro version of this function named setCreateT_M is available.
 */
P_SET_T setCreateT_O(void)
{
	return treCreateBST();
}

/* Function name: setDeleteT_O
 * Description:   Delete the set of which is allocated by function setCreateT_O.
 * Parameter:
 *      pset Pointer to the set you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setDeleteT_M is available.
 */
void setDeleteT_O(P_SET_T pset)
{
	treDeleteBST(pset);
}

/* Function name: setCopyT
 * Description:   Make a duplication of a BST set.
 * Parameters:
 *       pset Pointer to the set you want to copy.
 *       size Size of each element in the original set.
 * Return value:  Pointer to the copy set.
 * Caution:       Address of pset Must Be Allocated first.
 */
P_SET_T setCopyT(P_SET_T pset, size_t size)
{
	P_SET_T prtn = setCreateT();
	if (NULL != prtn)
		*prtn = treCopyBST(*pset, size);
	return prtn;
}

/* Function name: setSizeT_O
 * Description:   Determine how many elements there are in the set.
 * Parameter:
 *      pset Pointer to the set you want to test.
 * Return value:  Number of elements.
 * Caution:       Address of pset Must Be Allocated first.
 * Tip:           A macro version of this function named setSizeT_M is available.
 */
size_t setSizeT_O(P_SET_T pset)
{
	return NULL == pset ? 0 : treArityBY(P2P_TNODE_BY(*pset));
}

/* Function name: setIsEmptyT_O
 * Description:   Make a judgment whether a set is empty or not.
 * Parameter:
 *      pset Pointer to the set you want to check.
 * Return value:
 *         true Set is empty.
 *        false Set is not empty.
 * Tip:           A macro version of this function named setIsEmptyT_M is available.
 */
bool setIsEmptyT_O(P_SET_T pset)
{
	return NULL == pset ? true: !(*pset);
}

/* Function name: setIsMemberT_O
 * Description:   Check whether an element belongs to the set or not.
 * Parameters:
 *       pset Pointer to the set you want to check.
 *      pitem Pointer to an element to want to check.
 *     cbfcmp Pointer to a comparison function for pset.
 * Return value:
 *         true Element belongs to the set.
 *        false Element does not belong to the set.
 * Tip:           A macro version of this function named setIsMemberT_M is available.
 */
bool setIsMemberT_O(P_SET_T pset, const void * pitem, CBF_COMPARE cbfcmp)
{
	return NULL == treBSTFindData_X(*pset, pitem, cbfcmp) ? false : true;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFIsSubsetTPuppet
 * Description:   This function is used to check the sub-set property of a set.
 * Parameters:
 *      pitem Pointer to each NODE_S in the tree.
 *      param Pointer to a size_t[2] array.
 *            size_t[0] stores psetb of the caller function.
 *            size_t[1] stores a pointer to comparison function.
 * Return value:  CBF_CONTINUE  Element can not be found in psetb.
 *                CBF_TERMINATE Element has been found in psetb.
 */
int _setCBFIsSubsetTPuppet(void * pitem, size_t param)
{
	if
	(
		NULL == treBSTFindData_X
		(
			(P_BSTNODE)  0[(size_t *)param],
			((P_BSTNODE)pitem)->knot.pdata,
			(CBF_COMPARE)1[(size_t *)param]
		)
	)
		return CBF_TERMINATE;
	return CBF_CONTINUE;
}

/* Function name: setIsSubsetT
 * Description:   Check whether a set is a sub set of another.
 * Parameters:
 *      pseta Pointer to the set you want to check.
 *      psetb Pointer to the other set you want to check.
 *     cbfcmp Pointer to a comparison function for psetb.
 *            Two sets should use the same comparison function.
 * Return value:
 *         true seta belongs to setb.
 *        false seta does not belong to setb.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
bool setIsSubsetT(P_SET_T pseta, P_SET_T psetb, CBF_COMPARE cbfcmp)
{
	if (NULL != pseta)
	{
		size_t a[2];
		a[0] = NULL != psetb ? (size_t)*psetb : (size_t)NULL;
		a[1] = (size_t)cbfcmp;
		if (setIsEmptyT(pseta))
			return true; /* The empty set is the subset of any sets. */
		if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*pseta), _setCBFIsSubsetTPuppet, (size_t)a))
			return false;
	}
	return true; /* An empty set is a subset of any set. */
}

/* Function name: setIsEqualT
 * Description:   Check whether a set is equal to another.
 * Parameters:
 *      pseta Pointer to the set you want to check.
 *      psetb Pointer to the other set you want to check.
 *     cbfcmp Pointer to a comparison function for both pseta and psetb.
 *            Two sets should use the same comparison function.
 *       size Size of element.
 * Return value:
 *         true Two sets are equal.
 *        false Two sets are NOT equal.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
bool setIsEqualT(P_SET_T pseta, P_SET_T psetb, CBF_COMPARE cbfcmp)
{
	if (pseta == psetb || (setIsSubsetT(pseta, psetb, cbfcmp) && setIsSubsetT(psetb, pseta, cbfcmp)))
		return true;
	return false;
}

/* Function name: setInsertT
 * Description:   Insert an element into a set.
 * Parameters:
 *       pset Pointer to the set you want to operate.
 *      pitem Pointer to an element that you want to insert.
 *       size Size of the element.
 *     cbfcmp Pointer to a comparison function.
 * Return value:  true:  Insertion succeeded.
 *                false: Insertion failed.
 * Tip:           You may use function setIsMemberT to check whether insertion succeeded or not after invoking.
 */
bool setInsertT(P_SET_T pset, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	if (setIsMemberT(pset, pitem, cbfcmp))
		return false; /* Item has already existed. */
	*pset = _setInsertBST(*pset, pitem, size, cbfcmp);
	return true;
}

/* Function name: setRemoveT
 * Description:   Remove an element from a set.
 * Parameters:
 *       pset Pointer to the set you want to operate.
 *      pitem Pointer to an element that you want to remove.
 *       size Size of the element.
 *     cbfcmp Pointer to a comparison function.
 * Return value:  true:  Deletion succeeded.
 *                false: Deletion failed.
 * Tip:           You may use function setIsMemberT to check whether removal succeeded or not after calling.
 */
bool setRemoveT(P_SET_T pset, const void * pitem, size_t size, CBF_COMPARE cbfcmp)
{
	if (setIsMemberT(pset, pitem, cbfcmp))
	{
		*pset = _setRemoveBST(*pset, pitem, size, cbfcmp);
		return true;
	}
	return false;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFInsertItemTPuppet
 * Description:   This function is used to generate a union set.
 * Parameters:
 *      pitem Pointer to each NODE_S in the tree.
 *      param Pointer to a size_t[3] array.
 *            size_t[0] stores the pointer to psetr of the caller function.
 *            size_t[1] stores a pointer of comparison function.
 *            size_t[2] stores the size of each element.
 * Return value:  CBF_CONTINUE  Insertion succeeded.
 *                CBF_TERMINATE Insertion failed.
 */
int _setCBFInsertItemTPuppet(void * pitem, size_t param)
{
	if
	(! setIsMemberT
		(
			*(P_SET_T *)0[(size_t *)param],
			((P_BSTNODE)pitem)->knot.pdata,
			(CBF_COMPARE)1[(size_t *)param]
		)
	)
	{
		P_BSTNODE pnode =
			_setInsertBST
			(
				**(P_SET_T *)0[(size_t *)param],
				((P_BSTNODE)pitem)->knot.pdata,
				2[(size_t *)param],
				(CBF_COMPARE)1[(size_t *)param]
			);
		if (NULL == pnode)
			return CBF_TERMINATE; /* Allocation failure. */
		**(P_SET_T *)0[(size_t *)param] = pnode;  /* Alter the root of tree. */
	}
	return CBF_CONTINUE;
}

/* Function name: setCreateUnionT
 * Description:   Generate a union for two sets.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfcmp Pointer to a comparison function for both pseta and psetb.
 *            Two sets should use the same comparison function.
 * Return value:  Pointer to a new set that contains the result of the union of two sets.
 *                NULL would be returned if the result were an empty set.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_T setCreateUnionT(P_SET_T pseta, P_SET_T psetb, size_t size, CBF_COMPARE cbfcmp)
{
	if (NULL != pseta || NULL != psetb)
	{
		P_SET_T psetr = setCreateT();
		size_t a[3];
		a[0] = (size_t)&psetr;
		a[1] = (size_t)cbfcmp;
		a[2] = size;
		if (! setIsEmptyT(pseta))
			if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*pseta), _setCBFInsertItemTPuppet, (size_t)a))
				goto Lbl_Erase_Bad_Set; /* Allocation failure. */
		if (! setIsEmptyT(psetb))
			if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*psetb), _setCBFInsertItemTPuppet, (size_t)a))
				goto Lbl_Erase_Bad_Set; /* Allocation failure. */
		if (setIsEmptyT(psetr))
		{
Lbl_Erase_Bad_Set: /* Erase the rest of the bad set. */
			setDeleteT(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _setCBFIntersectionTPuppet
 * Description:   This function is used to generate an intersection set.
 * Parameters:
 *      pitem Pointer to each NODE_S in the table.
 *      param Pointer to a size_t[5] array.
 *            size_t[0] stores the pointer to psetr of the caller function.
 *            size_t[1] stores a pointer to comparison function.
 *            size_t[2] stores the size of each element.
 *            size_t[3] stores the pointer to either pseta or psetb of the caller function. (Flexible)
 *            size_t[4] stores a boolean value.
 *                      If size_t[4] == false, do insertion only when an element settled-
 *                      in set size_t[3] did not match elements in psetr at all.
 *                      So that we can generate a difference set of set A and set B.
 * Return value:  CBF_CONTINUE  Insertion succeeded.
 *                CBF_TERMINATE Insertion failed.
 */
int _setCBFIntersectionTPuppet(void * pitem, size_t param)
{
	bool r = (NULL != *(P_SET_T *)3[(size_t *)param] && NULL ==
		treBSTFindData_X
		(
			**(P_SET_T *)3[(size_t *)param],
			((P_BSTNODE)pitem)->knot.pdata,
			(CBF_COMPARE)1[(size_t *)param]
		)
		? false : true);
	if (r == (bool)4[(size_t *)param])
	{
		if
		(! setIsMemberT
			(
				*(P_SET_T *)0[(size_t *)param],
				((P_BSTNODE)pitem)->knot.pdata,
				(CBF_COMPARE)1[(size_t *)param]
			)
		)
		{
			P_BSTNODE pnode =
			_setInsertBST
			(
				**(P_SET_T *)0[(size_t *)param],
				((P_BSTNODE)pitem)->knot.pdata,
				2[(size_t *)param],
				(CBF_COMPARE)1[(size_t *)param]
			);
			if (NULL == pnode)
				return CBF_TERMINATE; /* Allocation failure. */
			**(P_SET_T *)0[(size_t *)param] = pnode; /* Alter the root of tree. */
		}
	}
	return CBF_CONTINUE;
}

/* Function name: setCreateIntersectionT
 * Description:   Generate an intersection between two sets.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfcmp Pointer to a comparison function for both pseta and psetb.
 *            Two sets should use the same comparison function.
 * Return value:  Pointer to a new set that contains the result of the intersection between two sets.
 *                NULL would be returned if result were an empty set.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_T setCreateIntersectionT(P_SET_T pseta, P_SET_T psetb, size_t size, CBF_COMPARE cbfcmp)
{
	if (NULL != pseta || NULL != psetb)
	{
		P_SET_T psetr = setCreateT();
		size_t a[5];
		a[0] = (size_t)&psetr;
		a[1] = (size_t)cbfcmp;
		a[2] = size;
		a[4] = true;
		if (! setIsEmptyT(pseta))
		{
			a[3] = (size_t)&psetb;
			if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*pseta), _setCBFIntersectionTPuppet, (size_t)a))
				goto Lbl_Erase_Bad_Set; /* Allocation failure. */
		}
		if (! setIsEmptyT(psetb) && pseta != psetb)
		{
			a[3] = (size_t)&pseta;
			if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*psetb), _setCBFIntersectionTPuppet, (size_t)a))
				goto Lbl_Erase_Bad_Set; /* Allocation failure. */
		}
		if (setIsEmptyT(psetr))
		{
Lbl_Erase_Bad_Set: /* Erase the rest of the bad set. */
			setDeleteT(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Function name: setCreateDifferenceT
 * Description:   Generate a difference set of two sets. Get the result of A - B.
 * Parameters:
 *      pseta Pointer to a set.
 *      psetb Pointer to the other set.
 *       size Size of element.
 *     cbfcmp Pointer to a comparison function for both pseta and psetb.
 *            Two sets should use the same comparison function.
 * Return value:  Pointer to a new set that contains the result of a difference set of two sets.
 *                NULL would be returned if the result were an empty set.
 * Caution:       Elements in two sets that pseta and psetb pointed should be in the same size.
 */
P_SET_T setCreateDifferenceT(P_SET_T pseta, P_SET_T psetb, size_t size, CBF_COMPARE cbfcmp)
{
	if (NULL != pseta)
	{
		P_SET_T psetr = setCreateT();
		size_t a[5];
		a[0] = (size_t)&psetr;
		a[1] = (size_t)cbfcmp;
		a[2] = size;
		a[4] = false;
		a[3] = (size_t)&psetb;
		if (! setIsEmptyT(pseta))
		{
			if (setIsEmptyT(psetb)) /* R = A - 0. */
			{
				if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*pseta), _setCBFInsertItemTPuppet, (size_t)a))
					goto Lbl_Erase_Bad_Set; /* Allocation failure. */
			}
			else /* R = A - B. */
			{
				if (CBF_CONTINUE != treTraverseBYPre(P2P_TNODE_BY(*pseta), _setCBFIntersectionTPuppet, (size_t)a))
					goto Lbl_Erase_Bad_Set; /* Allocation failure. */
			}
		}
		if (setIsEmptyT(psetr))
		{
Lbl_Erase_Bad_Set: /* Erase the rest of the bad set. */
			setDeleteT(psetr);
			goto Lbl_Empty_Set;
		}
		return psetr;
	}
Lbl_Empty_Set:
	return NULL;
}

/* Function name: setTraverseT
 * Description:   Traverse elements in a BST-style set. This function is a wrapper or dispatcher.
 * Parameters:
 *       pset Pointer to a set.
 *            (*) Especially, if pset equaled to NULL, function would return value CBF_CONTINUE.
 *     cbftvs Pointer to a callback function.
 *      param Parameter which can be transfer into the callback function.
 *         tm Method of traversal. This parameter can be any value in enumeration TvsMtd.
 *            (*) Especially, if tm does not equaled to any value in enumeration TvsMtd, function would return value CBF_TERMINATE.
 * Return value:  The same value as callback function cbftvs returns.
 * Caution:       Address of pset Must Be Allocated first.
 */
int setTraverseT(P_SET_T pset, CBF_TRAVERSE cbftvs, size_t param, TvsMtd tm)
{
	if (setIsEmptyT(pset)) /* There is no need to traverse elements in an empty set. */
		return CBF_CONTINUE;
	else
	{
		int r = CBF_TERMINATE;
		switch (tm)
		{
		case ETM_PREORDER:   r = treTraverseBYPre  (P2P_TNODE_BY(*pset), cbftvs, param); break;
		case ETM_INORDER:    r = treTraverseBYIn   (P2P_TNODE_BY(*pset), cbftvs, param); break;
		case ETM_POSTORDER:  r = treTraverseBYPost (P2P_TNODE_BY(*pset), cbftvs, param); break;
		case ETM_LEVELORDER: r = treTraverseBYLevel(P2P_TNODE_BY(*pset), cbftvs, param); break;
		}
		return r;
	}
}

