/*
 * Name:        svset.h
 * Description: Sets interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171620T0115211509L00170
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

#ifndef _SVSET_H_
#define _SVSET_H_

#include "svhash.h"
#include "svtree.h"

/* Definition of sets using chaining hash tables. */
typedef HSHTBL_C SET_H, * P_SET_H;
/* Definition of sets using binary search trees. */
typedef BST SET_T, * P_SET_T;

/* Functions for hash-table style sets. */
BOOL     setInitH_O             (P_SET_H pset,   size_t   buckets);
void     setFreeH_O             (P_SET_H pset);
P_SET_H  setCreateH_O           (size_t  buckets);
void     setDeleteH_O           (P_SET_H pset);
size_t   setSizeH_O             (P_SET_H pset);
BOOL     setIsEmptyH_O          (P_SET_H pset);
BOOL     setIsMemberH_O         (P_SET_H pset,   CBF_HASH cbfhsh, const void * pitem,     size_t      size);
BOOL     setIsSubsetH           (P_SET_H pseta,  P_SET_H  psetb,  CBF_HASH     cbfhsh,    size_t      size);
BOOL     setIsEqualH            (P_SET_H pseta,  P_SET_H  psetb,  CBF_HASH     cbfhsh,    size_t      size);
BOOL     setInsertH             (P_SET_H pset,   CBF_HASH cbfhsh, const void * pitem,     size_t      size);
BOOL     setRemoveH             (P_SET_H pset,   CBF_HASH cbfhsh, const void * pitem,     size_t      size);
P_SET_H  setCreateUnionH        (P_SET_H pseta,  P_SET_H  psetb,  CBF_HASH     cbfhsh,    size_t      size);
P_SET_H  setCreateIntersectionH (P_SET_H pseta,  P_SET_H  psetb,  CBF_HASH     cbfhsh,    size_t      size);
P_SET_H  setCreateDifferenceH   (P_SET_H pseta,  P_SET_H  psetb,  CBF_HASH     cbfhsh,    size_t      size);
/* Functions for BST style sets. */
void     setInitT_O             (P_SET_T pset);
void     setFreeT_O             (P_SET_T pset);
P_SET_T  setCreateT_O           (void);
void     setDeleteT_O           (P_SET_T pset);
P_SET_T  setCopyT               (P_SET_T pset,   size_t       size);
size_t   setSizeT_O             (P_SET_T pset);
BOOL     setIsEmptyT_O          (P_SET_T pset);
BOOL     setIsMemberT_O         (P_SET_T pset,   const void * pitem,  CBF_COMPARE cbfcmp);
BOOL     setIsSubsetT           (P_SET_T pseta,  P_SET_T      psetb,  CBF_COMPARE cbfcmp);
BOOL     setIsEqualT            (P_SET_T pseta,  P_SET_T      psetb,  CBF_COMPARE cbfcmp);
void     setInsertT             (P_SET_T pset,   const void * pitem,  size_t      size,   CBF_COMPARE cbfcmp);
void     setRemoveT             (P_SET_T pset,   const void * pitem,  size_t      size,   CBF_COMPARE cbfcmp);
P_SET_T  setCreateUnionT        (P_SET_T pseta,  P_SET_T      psetb,  size_t      size,   CBF_COMPARE cbfcmp);
P_SET_T  setCreateIntersectionT (P_SET_T pseta,  P_SET_T      psetb,  size_t      size,   CBF_COMPARE cbfcmp);
P_SET_T  setCreateDifferenceT   (P_SET_T pseta,  P_SET_T      psetb,  size_t      size,   CBF_COMPARE cbfcmp);
int      setTraverseT           (P_SET_T pset,   CBF_TRAVERSE cbftvs, size_t      param,  TvsMtd      tm);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svset.c. */
#define setInitH_M(pset_M, buckets_M) (hshInitC((pset_M), (buckets_M)))
#define setFreeH_M(pset_M) do { \
	hshFreeC(pset_M); \
} while (0)
#define setCreateH_M(buckets_M) (hshCreateC(buckets_M))
#define setDeleteH_M(pset_M) do { \
	hshDeleteC(pset_M); \
} while (0)
#define setSizeH_M(pset_M) (NULL == (pset_M) ? 0 : hshSizeC(pset_M))
#define setIsEmptyH_M(pset_M) (!setSizeH(pset_M))
#define setIsMemberH_M(pset_M, cbfhsh_M, pitem_M, size_M) \
	(NULL == (pset_M) ? FALSE : \
	(NULL != hshSearchC((pset_M), (cbfhsh_M), (pitem_M), (size_M)) ? TRUE : FALSE))
/* Macros for binary search tree represented sets. */
#define setInitT_M(pset_M) do { \
	treInitBST(pset_M); \
} while (0)
#define setFreeT_M(pset_M) do { \
	treFreeBST(pset_M); \
} while (0)
#define setCreateT_M() (treCreateBST())
#define setDeleteT_M(pset_M) do { \
	treDeleteBST(pset_M); \
} while (0)
#define setSizeT_M(pset_M) (NULL == (pset_M) ? 0 : treArityBY(P2P_TNODE_BY(*(pset_M))))
#define setIsEmptyT_M(pset_M) (NULL == (pset_M) ? TRUE: !(*(pset_M)))
#define setIsMemberT_M(pset_M, pitem_M, cbfcmp_M) (NULL == treBSTFindData_X(*(pset_M), (pitem_M), (cbfcmp_M)) ? FALSE : TRUE)

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	/* Macros for hash table represented sets. */
	#define setInitH     setInitH_O
	#define setFreeH     setFreeH_O
	#define setCreateH   setCreateH_O
	#define setDeleteH   setDeleteH_O
	#define setSizeH     setSizeH_O
	#define setIsEmptyH  setIsEmptyH_O
	#define setIsMemberH setIsMemberH_O
	/* Macros for binary search tree represented sets. */
	#define setInitT     setInitT_O
	#define setFreeT     setFreeT_O
	#define setCreateT   setCreateT_O
	#define setDeleteT   setDeleteT_O
	#define setSizeT     setSizeT_O
	#define setIsEmptyT  setIsEmptyT_O
	#define setIsMemberT setIsMemberT_O
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	/* Macros for hash table represented sets. */
	#define setInitH     setInitH_M
	#define setFreeH     setFreeH_M
	#define setCreateH   setCreateH_M
	#define setDeleteH   setDeleteH_M
	#define setSizeH     setSizeH_M
	#define setIsEmptyH  setIsEmptyH_M
	#define setIsMemberH setIsMemberH_M
	/* Macros for binary search tree represented sets. */
	#define setInitT     setInitT_M
	#define setFreeT     setFreeT_M
	#define setCreateT   setCreateT_M
	#define setDeleteT   setDeleteT_M
	#define setSizeT     setSizeT_M
	#define setIsEmptyT  setIsEmptyT_M
	#define setIsMemberT setIsMemberT_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	/* Macros for hash table represented sets. */
	#define setInitH     setInitH_M
	#define setFreeH     setFreeH_M
	#define setCreateH   setCreateH_M
	#define setDeleteH   setDeleteH_M
	#define setSizeH     setSizeH_M
	#define setIsEmptyH  setIsEmptyH_M
	#define setIsMemberH setIsMemberH_M
	/* Macros for binary search tree represented sets. */
	#define setInitT     setInitT_M
	#define setFreeT     setFreeT_M
	#define setCreateT   setCreateT_M
	#define setDeleteT   setDeleteT_M
	#define setSizeT     setSizeT_M
	#define setIsEmptyT  setIsEmptyT_M
	#define setIsMemberT setIsMemberT_M
#else /* Optimization has been disabled. */
	/* Macros for hash table represented sets. */
	#define setInitH     setInitH_O
	#define setFreeH     setFreeH_O
	#define setCreateH   setCreateH_O
	#define setDeleteH   setDeleteH_O
	#define setSizeH     setSizeH_O
	#define setIsEmptyH  setIsEmptyH_O
	#define setIsMemberH setIsMemberH_O
	/* Macros for binary search tree represented sets. */
	#define setInitT     setInitT_O
	#define setFreeT     setFreeT_O
	#define setCreateT   setCreateT_O
	#define setDeleteT   setDeleteT_O
	#define setSizeT     setSizeT_O
	#define setIsEmptyT  setIsEmptyT_O
	#define setIsMemberT setIsMemberT_O
#endif

#endif
