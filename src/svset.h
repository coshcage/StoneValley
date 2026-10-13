/*
 * Name:        svset.h
 * Description: Sets interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171620T1013261639L00266
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

#ifndef _SVSET_H_
#define _SVSET_H_

#include "svhash.h"
#include "svtree.h"

/* Definition for sets using arrays.
 * Data are being arranged in an array set in increasing order.
 */
typedef ARRAY_Z SET_A, * P_SET_A;

/* Definition of sets using chaining hash tables.
 * Also known as unordered set.
 */
typedef HSHTBL_C SET_H, * P_SET_H;

/* Definition of sets using binary search trees.
 * Also known as ordered set, which means elements in such a set are respectively comparable.
 */
typedef BST SET_T, * P_SET_T;

/* Define macros to switch set between trees. */
#define SET_TREE_AA     0x1
#define SET_TREE_AVL    0x2

/* Tree set method switch. Alter the following one macro to alter set implementation. */
#define SET_TREE_USING (SET_TREE_AA)

/* These following two macros are used to switch between two types of BSTs. */
#if   SET_TREE_USING == SET_TREE_AA
	#define _setInsertBST treBSTInsertAA
	#define _setRemoveBST treBSTRemoveAA
#elif SET_TREE_USING == SET_TREE_AVL
	#define _setInsertBST treBSTInsertAVL
	#define _setRemoveBST treBSTRemoveAVL
#endif

/* Functions for array style sets. */
void *   setInitA_O             (P_SET_A pset,   size_t       size);
void     setFreeA_O             (P_SET_A pset);
P_SET_A  setCreateA             (size_t size);
void     setDeleteA_O           (P_SET_A pset);
P_SET_A  setCreateCopyA_O       (P_SET_A pset,   size_t       size);
size_t   setSizeA_O             (P_SET_A pset);
bool     setIsEmptyA_O          (P_SET_A pset);
bool     setIsMemberA_O         (P_SET_A pset,   const void * pitem,  size_t       size,      CBF_COMPARE         cbfcmp);
bool     setIsSubsetA           (P_SET_A pseta,  P_SET_A      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
bool     setIsEqualA            (P_SET_A pseta,  P_SET_A      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
bool     setInsertA             (P_SET_A pset,   const void * pitem,  size_t       size,      CBF_COMPARE         cbfcmp);
bool     setRemoveA             (P_SET_A pset,   const void * pitem,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_A  setCreateUnionA        (P_SET_A pseta,  P_SET_A      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_A  setCreateIntersectionA (P_SET_A pseta,  P_SET_A      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_A  setCreateDifferenceA   (P_SET_A pseta,  P_SET_A      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
int      setTraverseA_O         (P_SET_A pset,   size_t       size,   CBF_TRAVERSE cbftvs,    size_t              param,   bool        brev);
/* Functions for hash table style sets. */
bool     setInitH_O             (P_SET_H pset,   size_t       buckets);
void     setFreeH_O             (P_SET_H pset);
P_SET_H  setCreateH_O           (size_t  buckets);
void     setDeleteH_O           (P_SET_H pset);
P_SET_H  setCreateCopyH         (P_SET_H pset,   size_t       size);
size_t   setSizeH_O             (P_SET_H pset);
bool     setIsEmptyH_O          (P_SET_H pset);
bool     setIsMemberH_O         (P_SET_H pset,   CBF_HASH     cbfhsh, const void * pitem,     CBF_COMPARE         cbfmch);
bool     setIsSubsetH           (P_SET_H pseta,  P_SET_H      psetb,  CBF_HASH     cbfhsh,    CBF_COMPARE         cbfmch);
bool     setIsEqualH            (P_SET_H pseta,  P_SET_H      psetb,  CBF_HASH     cbfhsh,    CBF_COMPARE         cbfmch);
bool     setInsertH             (P_SET_H pset,   CBF_HASH     cbfhsh, const void * pitem,     size_t              size,    CBF_COMPARE cbfmch);
bool     setRemoveH_O           (P_SET_H pset,   CBF_HASH     cbfhsh, const void * pitem,     CBF_COMPARE         cbfmch);
P_SET_H  setCreateUnionH        (P_SET_H pseta,  P_SET_H      psetb,  CBF_HASH     cbfhsh,    size_t              size,    CBF_COMPARE cbfmch);
P_SET_H  setCreateIntersectionH (P_SET_H pseta,  P_SET_H      psetb,  CBF_HASH     cbfhsh,    size_t              size,    CBF_COMPARE cbfmch);
P_SET_H  setCreateDifferenceH   (P_SET_H pseta,  P_SET_H      psetb,  CBF_HASH     cbfhsh,    size_t              size,    CBF_COMPARE cbfmch);
int      setTraverseItemH_O     (P_SET_H pset,   CBF_TRAVERSE cbftvs, size_t       param);
/* Functions for BST style sets. */
void     setInitT_O             (P_SET_T pset);
void     setFreeT_O             (P_SET_T pset);
P_SET_T  setCreateT_O           (void);
void     setDeleteT_O           (P_SET_T pset);
P_SET_T  setCreateCopyT         (P_SET_T pset,   size_t       size);
size_t   setSizeT_O             (P_SET_T pset);
bool     setIsEmptyT_O          (P_SET_T pset);
bool     setIsMemberT_O         (P_SET_T pset,   const void * pitem,  CBF_COMPARE  cbfcmp);
bool     setIsSubsetT           (P_SET_T pseta,  P_SET_T      psetb,  CBF_COMPARE  cbfcmp);
bool     setIsEqualT            (P_SET_T pseta,  P_SET_T      psetb,  CBF_COMPARE  cbfcmp);
bool     setInsertT             (P_SET_T pset,   const void * pitem,  size_t       size,      CBF_COMPARE         cbfcmp);
bool     setRemoveT             (P_SET_T pset,   const void * pitem,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_T  setCreateUnionT        (P_SET_T pseta,  P_SET_T      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_T  setCreateIntersectionT (P_SET_T pseta,  P_SET_T      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
P_SET_T  setCreateDifferenceT   (P_SET_T pseta,  P_SET_T      psetb,  size_t       size,      CBF_COMPARE         cbfcmp);
int      setTraverseT           (P_SET_T pset,   CBF_TRAVERSE cbftvs, size_t       param,     TvsMtd              tm);
int      setTraverseTDispatch   (P_SET_T pset,   CBF_TRAVERSE cbftvs, size_t       param,     CBF_TRAVERSE_BYTREE cbftvsbyt);
/* Function declarations for array set, hash set and tree set. */
P_SET_A  setCreateAFromT        (P_SET_T ptset,  size_t       size);
P_SET_T  setCreateTFromA        (P_SET_A paset,  size_t       size,   CBF_COMPARE  cbfcmp);
P_SET_H  setCreateHFromT        (P_SET_T ptset,  size_t       size,   size_t       buckets,   CBF_HASH            cbfhsh,  CBF_COMPARE cbfmch);
P_SET_T  setCreateTFromH        (P_SET_H phset,  size_t       size,   CBF_COMPARE  cbfcmp);

/* Macros for function inline to accelerate execution speed. */
/* Macros for array represented sets. */
#define setInitA_M(pset_M, size_M) \
	(((pset_M)->num = 0, 0 == (size_M)) ? (void *)((pset_M)->pdata = NULL) : (void *)((pset_M)->pdata = (PUCHAR) malloc(size_M)))
#define setFreeA_M(pset_M) do { \
	strFreeArrayZ(pset_M); \
} while (0)
#define setDeleteA_M(pset_M) do { \
	strDeleteArrayZ(pset_M); \
} while(0)
#define setCreateCopyA_M(pset_M, size_M) \
	(NULL == (pset_M) ? NULL : (! strLevelArrayZ(pset_M) ? NULL : strCreateCopyArrayZ((pset_M), (size_M))))
#define setSizeA_M(pset_M) (NULL == (pset_M) ? 0 : strLevelArrayZ(pset_M))
#define setIsEmptyA_M(pset_M) (NULL != (pset_M) ? 0 == strLevelArrayZ(pset_M) : true)
#define setIsMemberA_M(pset_M, pitem_M, size_M, cbfcmp_M) \
	(NULL != (pitem_M) ? (NULL != (pset_M) ? NULL != strBinarySearchArrayZ((pset_M), (pitem_M), (size_M), (cbfcmp_M)) : false) : true)
#define setTraverseA_M(pset_M, size_M, cbftvs_M, param_M, brev_M) \
	(NULL == (pset_M) ? CBF_CONTINUE : strTraverseArrayZ((pset_M), (size_M), (cbftvs_M), (param_M), (brev_M)))
/* Macros for hash table represented sets. */
#define setSizeH_M(pset_M) (NULL == (pset_M) ? 0 : hshSizeC(pset_M))
#define setIsEmptyH_M(pset_M) (0 == setSizeH_M(pset_M))
#define setIsMemberH_M(pset_M, cbfhsh_M, pitem_M, cbfmch_M) \
	(NULL == (pset_M) ? false : NULL != hshSearchC((pset_M), (cbfhsh_M), (pitem_M), (cbfmch_M)))
/* Macros for binary search tree represented sets. */
#define setSizeT_M(pset_M) (NULL == (pset_M) ? 0 : treArityBY(P2P_TNODE_BY(*(pset_M))))
#define setIsEmptyT_M(pset_M) (NULL == (pset_M) ? true : NULL == *(pset_M))
#define setIsMemberT_M(pset_M, pitem_M, cbfcmp_M) (NULL != treBSTFindData_X(*(pset_M), (pitem_M), (cbfcmp_M)))

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#include <stdlib.h> /* Use function malloc. */
	/* Macros for array represented sets. */
	#define setInitA         setInitA_M
	#define setFreeA         setFreeA_M
	#define setDeleteA       setDeleteA_M
	#define setCreateCopyA   setCreateCopyA_M
	#define setSizeA         setSizeA_M
	#define setIsEmptyA      setIsEmptyA_M
	#define setIsMemberA     setIsMemberA_M
	#define setTraverseA     setTraverseA_M
	/* Macros for hash table represented sets. */
	#define setInitH         hshInitC
	#define setFreeH         hshFreeC
	#define setCreateH       hshCreateC
	#define setDeleteH       hshDeleteC
	#define setSizeH         setSizeH_O
	#define setIsEmptyH      setIsEmptyH_O
	#define setIsMemberH     setIsMemberH_O
	#define setRemoveH       hshRemoveC
	#define setTraverseItemH hshTraverseC
	/* Macros for binary search tree represented sets. */
	#define setInitT         treInitBST
	#define setFreeT         treFreeBST
	#define setCreateT       treCreateBST
	#define setDeleteT       treDeleteBST
	#define setSizeT         setSizeT_O
	#define setIsEmptyT      setIsEmptyT_O
	#define setIsMemberT     setIsMemberT_O
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#include <stdlib.h> /* Use function malloc. */
	/* Macros for array represented sets. */
	#define setInitA         setInitA_M
	#define setFreeA         setFreeA_M
	#define setDeleteA       setDeleteA_M
	#define setCreateCopyA   setCreateCopyA_M
	#define setSizeA         setSizeA_M
	#define setIsEmptyA      setIsEmptyA_M
	#define setIsMemberA     setIsMemberA_M
	#define setTraverseA     setTraverseA_M
	/* Macros for hash table represented sets. */
	#define setInitH         hshInitC
	#define setFreeH         hshFreeC
	#define setCreateH       hshCreateC
	#define setDeleteH       hshDeleteC
	#define setSizeH         setSizeH_M
	#define setIsEmptyH      setIsEmptyH_M
	#define setIsMemberH     setIsMemberH_M
	#define setRemoveH       hshRemoveC
	#define setTraverseItemH hshTraverseC
	/* Macros for binary search tree represented sets. */
	#define setInitT         treInitBST
	#define setFreeT         treFreeBST
	#define setCreateT       treCreateBST
	#define setDeleteT       treDeleteBST
	#define setSizeT         setSizeT_M
	#define setIsEmptyT      setIsEmptyT_M
	#define setIsMemberT     setIsMemberT_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#include <stdlib.h> /* Use function malloc. */
	/* Macros for array represented sets. */
	#define setInitA         setInitA_M
	#define setFreeA         setFreeA_M
	#define setDeleteA       setDeleteA_M
	#define setCreateCopyA   setCreateCopyA_M
	#define setSizeA         setSizeA_M
	#define setIsEmptyA      setIsEmptyA_M
	#define setIsMemberA     setIsMemberA_M
	#define setTraverseA     setTraverseA_M
	/* Macros for hash table represented sets. */
	#define setInitH         hshInitC
	#define setFreeH         hshFreeC
	#define setCreateH       hshCreateC
	#define setDeleteH       hshDeleteC
	#define setSizeH         setSizeH_M
	#define setIsEmptyH      setIsEmptyH_M
	#define setIsMemberH     setIsMemberH_M
	#define setRemoveH       hshRemoveC
	#define setTraverseItemH hshTraverseC
	/* Macros for binary search tree represented sets. */
	#define setInitT         treInitBST
	#define setFreeT         treFreeBST
	#define setCreateT       treCreateBST
	#define setDeleteT       treDeleteBST
	#define setSizeT         setSizeT_M
	#define setIsEmptyT      setIsEmptyT_M
	#define setIsMemberT     setIsMemberT_M
#else /* Optimization has been disabled. */
	/* Macros for array represented sets. */
	#define setInitA         setInitA_O
	#define setFreeA         setFreeA_O
	#define setDeleteA       setDeleteA_O
	#define setCreateCopyA   setCreateCopyA_O
	#define setSizeA         setSizeA_O
	#define setIsEmptyA      setIsEmptyA_O
	#define setIsMemberA     setIsMemberA_O
	#define setTraverseA     setTraverseA_O
	/* Macros for hash table represented sets. */
	#define setInitH         setInitH_O
	#define setFreeH         setFreeH_O
	#define setCreateH       setCreateH_O
	#define setDeleteH       setDeleteH_O
	#define setSizeH         setSizeH_O
	#define setIsEmptyH      setIsEmptyH_O
	#define setIsMemberH     setIsMemberH_O
	#define setRemoveH       setRemoveH_O
	#define setTraverseItemH setTraverseItemH_O
	/* Macros for binary search tree represented sets. */
	#define setInitT         setInitT_O
	#define setFreeT         setFreeT_O
	#define setCreateT       setCreateT_O
	#define setDeleteT       setDeleteT_O
	#define setSizeT         setSizeT_O
	#define setIsEmptyT      setIsEmptyT_O
	#define setIsMemberT     setIsMemberT_O
#endif

#endif

