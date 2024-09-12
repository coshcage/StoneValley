/*
 * Name:        svstring.h
 * Description: Strings interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306170921Y0907241355L00476
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

#ifndef _SVSTRING_H_
#define _SVSTRING_H_

#include "svdef.h"

/* The following two macros are used to address nodes pointers for NODE_D structures. */
#define PREV 0
#define NEXT 1

/* Sized array. */
typedef struct st_ARRAY_Z {
	size_t num;   /* Number of elements. */
	PUCHAR pdata; /* Pointer of the first element. */
} ARRAY_Z, * P_ARRAY_Z;

/* Node with a single pointer. */
typedef struct st_NODE_S {
	struct st_NODE_S * pnode;
	PUCHAR pdata;
} NODE_S, * P_NODE_S;

/* Single pointer linked list. */
typedef P_NODE_S LIST_S, * P_LIST_S;

/* Doubly pointer node. */
typedef struct st_NODE_D {
	struct st_NODE_D * ppnode[2];
	PUCHAR pdata;
} NODE_D, * P_NODE_D;

/* Double pointer linked list. */
typedef P_NODE_D LIST_D, * P_LIST_D;

/* An enumeration of node types. */
typedef enum en_NodeType {
	ENT_DOUBLE,
	ENT_SINGLE
} NodeType;

/* Definition of bit-stream. */
typedef struct st_BITSTREAM {
	ARRAY_Z arrz; /* Array that contains the stream data. */
	size_t  bilc; /* Number of bits in the last char. */
} BITSTREAM, * P_BITSTREAM;

/* Definition of common matrix structure. */
typedef struct st_MATRIX {
	ARRAY_Z arrz;   /* Data array. */
	size_t ln, col; /* Line and column number. */
} MATRIX, * P_MATRIX;

/* Definition of callback functions on matrices.
 * This function should return either CBF_CONTINUE or CBF_TERMINATE.
 * If callback algebraic function returned CBF_TERMINATE,
 *   caller would be interrupted,
 *     unless callee should return CBF_CONTINUE to calculate continually.
 */
typedef CBF_COMPARE CBF_ALGEBRA;

/* Definition of bit-matrix structure. */
typedef MATRIX BITMAT, * P_BITMAT;

/* Definition of sparse-matrix structure. */
typedef struct st_SPAMAT {
	BITMAT bmask;  /* Data bit mask. */
	LIST_S datlst; /* Data list. */
} SPAMAT, * P_SPAMAT;

/* Miscellaneous functions for linear data structures. */
void        svSwap                         (void *       pleft,    void *       pright,  void *       ptemp,   size_t       size);
void        svShellSort                    (void *       pbase,    void *       ptemp,   size_t       num,     size_t       size,   CBF_COMPARE cbfcmp);
void *      svQuickSort                    (void *       pbase,    size_t       num,     size_t       size,    CBF_COMPARE  cbfcmp);
void *      svMergeSort                    (void *       pbase,    size_t       num,     size_t       size,    CBF_COMPARE  cbfcmp);
void *      svHeapSort                     (void *       pbase,    size_t       num,     size_t       size,    CBF_COMPARE  cbfcmp);
void *      svBinarySearch                 (const void * pkey,     const void * pbase,   size_t       num,     size_t       size,   CBF_COMPARE cbfcmp);
/* Atomic element function declarations here. */
void *      strInitArrayZ                  (P_ARRAY_Z    parrz,    size_t       num,     size_t       size);
P_ARRAY_Z   strCreateArrayZ                (size_t       num,      size_t       size);
void        strSetArrayZ                   (P_ARRAY_Z    parrz,    const void * pval,    size_t       size);
void *      strResizeArrayZ                (P_ARRAY_Z    parrz,    size_t       num,     size_t       size);
void        strFreeArrayZ_O                (P_ARRAY_Z    parrz);
void        strDeleteArrayZ_O              (P_ARRAY_Z    parrz);
void *      strInitNodeS                   (P_NODE_S     pnode,    const void * pval,    size_t       size);
P_NODE_S    strCreateNodeS                 (const void * pval,     size_t       size);
void        strFreeNodeS_O                 (P_NODE_S     pnode);
void        strDeleteNodeS_O               (P_NODE_S     pnode);
void *      strInitNodeD                   (P_NODE_D     pnode,    const void * pval,    size_t       size);
P_NODE_D    strCreateNodeD                 (const void * pval,     size_t       size);
void        strFreeNodeD_O                 (P_NODE_D     pnode);
void        strDeleteNodeD_O               (P_NODE_D     pnode);
/* Functions for sized arrays. */
char *      strInitCharacterStringArrayZ   (P_ARRAY_Z    parrz,    const char * pstr);
P_ARRAY_Z   strCreateCharacterStringArrayZ (const char * pstr);
size_t      strLevelArrayZ_O               (P_ARRAY_Z    parrz);
int         strTraverseArrayZ              (P_ARRAY_Z    parrz,    size_t       size,    CBF_TRAVERSE cbftvs,  size_t       param,  BOOL        brev);
void *      strCopyArrayZ_O                (P_ARRAY_Z    pdest,    P_ARRAY_Z    psrc,    size_t       size);
void *      strMoveArrayZ_O                (P_ARRAY_Z    pdest,    P_ARRAY_Z    psrc,    size_t       size);
void *      strLocateItemArrayZ_O          (P_ARRAY_Z    parrz,    size_t       size,    size_t       index);
size_t      strLinearSearchArrayZ          (P_ARRAY_Z    parrz,    const void * pitem,   size_t       size,    BOOL         brev);
size_t      strBinaryLocateArrayZ          (P_ARRAY_Z    parrz,    const void * pitem,   size_t       size,    CBF_COMPARE  cbfcmp);
void *      strInsertItemArrayZ            (P_ARRAY_Z    parrz,    const void * pitem,   size_t       size,    size_t       index);
void        strRemoveItemArrayZ            (P_ARRAY_Z    parrz,    size_t       size,    size_t       index,   BOOL         bshrink);
void        strSortArrayZ_O                (P_ARRAY_Z    parrz,    size_t       size,    CBF_COMPARE  cbfcmp);
void *      strMergeSortedArrayZ           (P_ARRAY_Z    pdest,    P_ARRAY_Z    psrc,    size_t       size,    CBF_COMPARE  cbfcmp);
void *      strBinarySearchArrayZ_O        (P_ARRAY_Z    parrz,    const void * pkey,    size_t       size,    CBF_COMPARE  cbfcmp);
void        strReverseArrayZ               (P_ARRAY_Z    parrz,    void *       ptemp,   size_t       size);
void *      strGetLimitationArrayZ         (P_ARRAY_Z    parrz,    void *       ptemp,   size_t       size,    CBF_COMPARE  cbfcmp, BOOL        bmax);
void        strUniqueArrayZ                (P_ARRAY_Z    parrz,    void *       ptemp,   size_t       size,    CBF_COMPARE  cbfcmp, BOOL        bshrink);
BOOL        strPermuteArrayZ               (P_ARRAY_Z    parrz,    void *       ptemp,   size_t       size,    CBF_COMPARE  cbfcmp, BOOL        bnext);
BOOL        strCombineNextArrayZ           (P_ARRAY_Z    parrzr,   P_ARRAY_Z    parrzn,  size_t       size,    CBF_COMPARE  cbfcmp);
void        strShuffleArrayZ               (P_ARRAY_Z    parrz,    void *       ptemp,   size_t       size,    unsigned int seed);
P_ARRAY_Z   strCreateZSearchArrayZ         (P_ARRAY_Z    parrtxt,  P_ARRAY_Z    parrptn, size_t       size);
/* Functions for single pointer linked-lists. */
int         strTraverseLinkedListSC_R      (LIST_S       list,     P_NODE_S     pnil,    CBF_TRAVERSE cbftvs,  size_t       param);
int         strTraverseLinkedListSC_A      (LIST_S       list,     P_NODE_S     pnil,    CBF_TRAVERSE cbftvs,  size_t       param);
int         strTraverseLinkedListSC_N      (LIST_S       list,     P_NODE_S     pnil,    CBF_TRAVERSE cbftvs,  size_t       param);
void        strInitLinkedListSC_O          (P_LIST_S     plist);
void        strFreeLinkedListSC            (P_LIST_S     plist);
P_LIST_S    strCreateLinkedListSC          (void);
void        strDeleteLinkedListSC_O        (P_LIST_S     plist);
size_t      strLevelLinkedListSC           (LIST_S       list);
P_NODE_S    strCopyLinkedListSC            (LIST_S       psrc,     size_t       size);
int         strCompareLinkedListSC         (LIST_S       listx,    LIST_S       listy,   CBF_COMPARE  cbfcmp);
P_NODE_S    strSearchLinkedListSC          (LIST_S       list,     const void * pitem,   size_t       size);
P_NODE_S    strLocatePreviousItemSC        (LIST_S       list,     P_NODE_S     pnode);
P_NODE_S    strLocateLastItemSC            (LIST_S       list);
P_NODE_S    strLocateItemSC_R              (P_NODE_S     pnode,    size_t       incmtl);
P_NODE_S    strLocateItemSC_N              (P_NODE_S     pnode,    size_t       incmtl);
P_NODE_S    strInsertItemLinkedListSC      (LIST_S       list,     P_NODE_S     pdest,   P_NODE_S     pnode,   BOOL         bafter);
P_NODE_S    strRemoveItemLinkedListSC      (LIST_S       list,     P_NODE_S     pnode);
P_NODE_S    strReverseLinkedListSC         (LIST_S       phead);
void        strSwapItemLinkedListS         (P_NODE_S     pnodex,   P_NODE_S     pnodey);
LIST_S      strSortLinkedListS             (LIST_S       phead,    CBF_COMPARE  cbfcmp);
/* Functions for doubly linked-lists. */
int         strTraverseLinkedListDC_R      (LIST_D       list,     P_NODE_D     pnil,    CBF_TRAVERSE cbftvs,  size_t       param,  BOOL        brev);
int         strTraverseLinkedListDC_A      (LIST_D       list,     P_NODE_D     pnil,    CBF_TRAVERSE cbftvs,  size_t       param,  BOOL        brev);
int         strTraverseLinkedListDC_N      (LIST_D       list,     P_NODE_D     pnil,    CBF_TRAVERSE cbftvs,  size_t       param,  BOOL        brev);
void        strInitLinkedListDC_O          (P_LIST_D     plist);
void        strFreeLinkedListDC            (P_LIST_D     plist,    BOOL         brev);
P_LIST_D    strCreateLinkedListDC          (void);
void        strDeleteLinkedListDC_O        (P_LIST_D     plist,    BOOL         brev);
size_t      strLevelLinkedListDC           (LIST_D       list,     BOOL         brev);
P_NODE_D    strCopyLinkedListDC            (LIST_D       psrc,     size_t       size,    BOOL brev);
int         strCompareLinkedListDC         (LIST_D       listx,    LIST_D       listy,   CBF_COMPARE  cbfcmp,  BOOL         brev);
P_NODE_D    strSearchLinkedListDC          (LIST_D       list,     const void * pitem,   size_t       size,    BOOL         brev);
P_NODE_D    strLocateItemDC_R              (P_NODE_D     pnode,    ptrdiff_t    incmtl);
P_NODE_D    strLocateItemDC_N              (P_NODE_D     pnode,    ptrdiff_t    incmtl);
P_NODE_D    strInsertItemLinkedListDC      (P_NODE_D     pdest,    P_NODE_D     pnode,   BOOL         bafter);
P_NODE_D    strRemoveItemLinkedListDC      (P_NODE_D     pnode);
void        strSwapItemLinkedListD         (P_NODE_D     pnodex,   P_NODE_D     pnodey);
/* Functions for both linked-list_S and linked-list_D. */
void        strSwapContentLinkedListSD     (void *       pnodex,   size_t       sizex,   void *       pnodey,  size_t       sizey,  NodeType    bnodes);
void *      strIsCircularLinkedListSD      (void *       pfirst,   NodeType     ntp,     BOOL         brev);
/* Functions for bit-streams. */
void *      strInitBitStream               (P_BITSTREAM  pbstm);
void        strFreeBitStream               (P_BITSTREAM  pbstm);
P_BITSTREAM strCreateBitStream             (void);
void        strDeleteBitStream             (P_BITSTREAM  pbstm);
void *      strCopyBitStream               (P_BITSTREAM  pdest,    P_BITSTREAM  psrc);
BOOL        strBitStreamIsEmpty_O          (P_BITSTREAM  pbstm);
BOOL        strBitStreamPush               (P_BITSTREAM  pbstm,    BOOL         value);
BOOL        strBitStreamPop                (P_BITSTREAM  pbstm);
BOOL        strBitStreamAdd                (P_BITSTREAM  pbstm,    BOOL         value);
BOOL        strBitStreamExtract            (P_BITSTREAM  pbstm);
BOOL        strBitStreamLocate             (P_BITSTREAM  pbstm,    size_t       index);
void        strBitStreamReverse            (P_BITSTREAM  pbstm);
/* Function for common matrices. */
void *      strInitMatrix                  (P_MATRIX     pmtx,     size_t       ln,      size_t       col,     size_t       size);
void        strFreeMatrix_O                (P_MATRIX     pmtx);
P_MATRIX    strCreateMatrix                (size_t       ln,       size_t       col,     size_t       size);
void        strDeleteMatrix_O              (P_MATRIX     pmtx);
void *      strCopyMatrix                  (P_MATRIX     pdest,    P_MATRIX     psrc,    size_t       size);
void *      strResizeMatrix                (P_MATRIX     pmtx,     size_t       ln,      size_t       col,     size_t       size);
void        strSetMatrix_O                 (P_MATRIX     pmtx,     const void * pval,    size_t       size);
void *      strGetValueMatrix              (void *       pval,     P_MATRIX     pmtx,    size_t       ln,      size_t       col,    size_t      size);
void *      strSetValueMatrix_O            (P_MATRIX     pmtx,     size_t       ln,      size_t       col,     void *       pval,   size_t      size);
void *      strTransposeMatrix             (P_MATRIX     pmtx,     size_t       size,    CBF_COMPARE  cbfcmp);
BOOL        strProjectMatrix               (P_MATRIX     pdest,    size_t       dln,     size_t       dcol,    P_MATRIX     psrc,   size_t      sln,     size_t scol, size_t size);
int         strM1Matrix                    (P_MATRIX     pmtx,     const void * pval,    size_t       size,    CBF_ALGEBRA  cbfagb);
int         strM2Matrix                    (P_MATRIX     pmtxa,    P_MATRIX     pmtxb,   size_t       size,    CBF_ALGEBRA  cbfagb);
int         strM3Matrix                    (P_MATRIX     ppmtx[3], void *       ptemp,   size_t       size,    CBF_ALGEBRA  pcbfagb[2]);
/* Functions for bit-matrices. */
void *      strInitBMap                    (P_BITMAT     pbm,      size_t       ln,      size_t       col,     BOOL         bval);
void        strFreeBMap_O                  (P_BITMAT     pbm);
P_BITMAT    strCreateBMap                  (size_t       ln,       size_t       col,     BOOL         val);
void        strDeleteBMap_O                (P_BITMAT     pbm);
void *      strCopyBMap_O                  (P_BITMAT     pdest,    P_BITMAT     psrc);
BOOL        strGetBitBMap                  (P_BITMAT     pbm,      size_t       ln,      size_t       col);
BOOL        strSetBitBMap                  (P_BITMAT     pbm,      size_t       ln,      size_t       col,     BOOL         bval);
/* Functions for sparse matrices. */
BOOL        strInitSparseMatrix            (P_SPAMAT     pmtx,     size_t       ln,      size_t       col);
void        strFreeSparseMatrix            (P_SPAMAT     pmtx);
P_SPAMAT    strCreateSparseMatrix          (size_t       ln,       size_t       col);
void        strDeleteSparseMatrix          (P_SPAMAT     pmtx);
P_SPAMAT    strCopySparseMatrix            (P_SPAMAT     pdest,    P_SPAMAT     psrc,    size_t       size);
void *      strGetValueSparseMatrix        (void *       pval,     P_SPAMAT     pmtx,    size_t       ln,      size_t       col,    size_t      size);
void *      strSetValueSparseMatrix        (P_SPAMAT     pmtx,     size_t       ln,      size_t       col,     void *       pval,   size_t      size);
BOOL        strFillSparseMatrix            (P_MATRIX     pdest,    P_SPAMAT     psrc,    size_t       size);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svatom.c. */
#define strFreeArrayZ_M(parrz_M) do { \
	free((parrz_M)->pdata); \
	(parrz_M)->pdata = NULL; \
	(parrz_M)->num = 0; \
} while (0)
#define strDeleteArrayZ_M(parrz_M) do { \
	strFreeArrayZ_M(parrz_M); \
	free(parrz_M); \
} while (0)
/* Macros for nodes. */
#define strFreeNodeS_M(pnode_M) do { \
	free((pnode_M)->pdata); \
} while (0)
#define strDeleteNodeS_M(pnode_M) do { \
	strFreeNodeS_M(pnode_M); \
	free(pnode_M); \
} while (0)
#define strFreeNodeD_M(pnode_M) do { \
	free((pnode_M)->pdata); \
} while (0)
#define strDeleteNodeD_M(pnode_M) do { \
	strFreeNodeD_M(pnode_M); \
	free(pnode_M); \
} while (0)
/* Functions in svarray.c. */
#define strLevelArrayZ_M(parrz_M) ((const size_t)((parrz_M)->num))
#define strCopyArrayZ_M(pdest_M, psrc_M, size_M) (memcpy((pdest_M)->pdata, (psrc_M)->pdata, (pdest_M)->num * (size_M)))
#define strMoveArrayZ_M(pdest_M, psrc_M, size_M) (memmove((pdest_M)->pdata, (psrc_M)->pdata, (pdest_M)->num * (size_M)))
#define strLocateItemArrayZ_M(parrz_M, size_M, index_M) ((index_M) < strLevelArrayZ(parrz_M) ? (parrz_M)->pdata + (index_M) * (size_M) : NULL)
#define strSortArrayZ_M(parrz_M, size_M, cbfcmp_M) do { \
	svQuickSort((parrz_M)->pdata, (parrz_M)->num, (size_M), (cbfcmp_M)); \
} while (0)
#define strBinarySearchArrayZ_M(parrz_M, pkey_M, size_M, cbfcmp_M) ( \
	svBinarySearch((pkey_M), (parrz_M)->pdata, (parrz_M)->num, (size_M), (cbfcmp_M)) \
)
/* Functions in svlist.c */
#define strInitLinkedListSC_M(plist_M) do { \
	*(plist_M) = NULL; \
} while (0)
#define strDeleteLinkedListSC_M(plist_M) do { \
	strFreeLinkedListSC(plist_M); \
	free(plist_M); \
} while (0)
#define strInitLinkedListDC_M(plist_M) do { \
	*(plist_M) = NULL; \
} while (0)
#define strDeleteLinkedListDC_M(plist_M, brev_M) do { \
	strFreeLinkedListDC(plist_M, brev_M); \
	free(plist_M); \
} while (0)
/* Functions in svmatrix.c */
#define strFreeMatrix_M(pmtx_M) do { \
	strFreeArrayZ(&(pmtx_M)->arrz); \
	(pmtx_M)->ln = (pmtx_M)->col = 0; \
} while (0)
#define strDeleteMatrix_M(pmtx_M) do { \
	strFreeMatrix(pmtx_M); \
	free(pmtx_M); \
} while (0)
#define strSetMatrix_M(pmtx_M, pval_M, size_M) do { \
	strSetArrayZ(&(pmtx_M)->arrz, (pval_M), (size_M)); \
} while (0)
#define strSetValueMatrix_M(pmtx_M, ln_M, col_M, pval_M, size_M) \
	(((ln_M) < (pmtx_M)->ln && (col_M) < (pmtx_M)->col && (size_M)) ? \
	memcpy(&(pmtx_M)->arrz.pdata[((ln_M) * (pmtx_M)->col + (col_M)) * (size_M)], (pval_M), (size_M)) : NULL)
#define strFreeBMap_M(pbm_M) do { \
	strFreeMatrix(pbm_M); \
} while (0)
#define strDeleteBMap_M(pbm_M) do { \
	strDeleteMatrix(pbm_M); \
} while (0)
#define strCopyBMap_M(pdest_M, psrc_M) (strCopyMatrix((pdest_M), (psrc_M), sizeof(UCHART)))
#define strBitStreamIsEmpty_M(pbstm_M) (strLevelArrayZ(&(pbstm_M)->arrz) <= 1 && 0 == (pbstm_M)->bilc)

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#include <string.h> /* Using function memcpy. */
	/* Macros for miscellaneous data structures. */
	#define strBitStreamIsEmpty       strBitStreamIsEmpty_M
	/* Macros for atomic structures. */
	#define strFreeArrayZ             strFreeArrayZ_O
	#define strDeleteArrayZ           strDeleteArrayZ_O
	#define strFreeNodeS              strFreeNodeS_M
	#define strDeleteNodeS            strDeleteNodeS_O
	#define strFreeNodeD              strFreeNodeD_M
	#define strDeleteNodeD            strDeleteNodeD_O
	/* Macros for sized arrays. */
	#define strLevelArrayZ            strLevelArrayZ_M
	#define strCopyArrayZ             strCopyArrayZ_M
	#define strMoveArrayZ             strMoveArrayZ_M
	#define strLocateItemArrayZ       strLocateItemArrayZ_M
	#define strSortArrayZ             strSortArrayZ_M
	#define strBinarySearchArrayZ     strBinarySearchArrayZ_M
	/* Macros for linked-lists. */
	#define strTraverseLinkedListSC_X strTraverseLinkedListSC_A
	#define strInitLinkedListSC       strInitLinkedListSC_M
	#define strDeleteLinkedListSC     strDeleteLinkedListSC_O
	#define strLocateItemSC           strLocateItemSC_R
	#define strTraverseLinkedListDC_X strTraverseLinkedListDC_A
	#define strInitLinkedListDC       strInitLinkedListDC_M
	#define strDeleteLinkedListDC     strDeleteLinkedListDC_O
	#define strLocateItemDC           strLocateItemDC_R
	/* Macros for matrices. */
	#define strFreeMatrix             strFreeMatrix_O
	#define strDeleteMatrix           strDeleteMatrix_O
	#define strSetMatrix              strSetMatrix_M
	#define strSetValueMatrix         strSetValueMatrix_M
	#define strFreeBMap               strFreeBMap_M
	#define strDeleteBMap             strDeleteBMap_M
	#define strCopyBMap               strCopyBMap_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#include <stdlib.h> /* Using function free. */
	#include <string.h> /* Using function memcpy. */
	/* Macros for miscellaneous data structures. */
	#define strBitStreamIsEmpty       strBitStreamIsEmpty_M
	/* Macros for atomic structures. */
	#define strFreeArrayZ             strFreeArrayZ_M
	#define strDeleteArrayZ           strDeleteArrayZ_M
	#define strFreeNodeS              strFreeNodeS_M
	#define strDeleteNodeS            strDeleteNodeS_M
	#define strFreeNodeD              strFreeNodeD_M
	#define strDeleteNodeD            strDeleteNodeD_M
	/* Macros for sized arrays. */
	#define strLevelArrayZ            strLevelArrayZ_M
	#define strCopyArrayZ             strCopyArrayZ_M
	#define strMoveArrayZ             strMoveArrayZ_M
	#define strLocateItemArrayZ       strLocateItemArrayZ_M
	#define strSortArrayZ             strSortArrayZ_M
	#define strBinarySearchArrayZ     strBinarySearchArrayZ_M
	/* Macros for linked-lists. */
	#define strTraverseLinkedListSC_X strTraverseLinkedListSC_N
	#define strInitLinkedListSC       strInitLinkedListSC_M
	#define strDeleteLinkedListSC     strDeleteLinkedListSC_M
	#define strLocateItemSC           strLocateItemSC_N
	#define strTraverseLinkedListDC_X strTraverseLinkedListDC_N
	#define strInitLinkedListDC       strInitLinkedListDC_M
	#define strDeleteLinkedListDC     strDeleteLinkedListDC_M
	#define strLocateItemDC           strLocateItemDC_N
	/* Macros for matrices. */
	#define strFreeMatrix             strFreeMatrix_M
	#define strDeleteMatrix           strDeleteMatrix_M
	#define strSetMatrix              strSetMatrix_M
	#define strSetValueMatrix         strSetValueMatrix_M
	#define strFreeBMap               strFreeBMap_M
	#define strDeleteBMap             strDeleteBMap_M
	#define strCopyBMap               strCopyBMap_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#include <stdlib.h> /* Using function free. */
	#include <string.h> /* Using function memcpy. */
	/* Macros for miscellaneous data structures. */
	#define strBitStreamIsEmpty       strBitStreamIsEmpty_M
	/* Macros for atomic structures. */
	#define strFreeArrayZ             strFreeArrayZ_M
	#define strDeleteArrayZ           strDeleteArrayZ_M
	#define strFreeNodeS              strFreeNodeS_M
	#define strDeleteNodeS            strDeleteNodeS_M
	#define strFreeNodeD              strFreeNodeD_M
	#define strDeleteNodeD            strDeleteNodeD_M
	/* Macros for sized arrays. */
	#define strLevelArrayZ            strLevelArrayZ_M
	#define strCopyArrayZ             strCopyArrayZ_M
	#define strMoveArrayZ             strMoveArrayZ_M
	#define strLocateItemArrayZ       strLocateItemArrayZ_M
	#define strSortArrayZ             strSortArrayZ_M
	#define strBinarySearchArrayZ     strBinarySearchArrayZ_M
	/* Macros for linked-lists. */
	#define strTraverseLinkedListSC_X strTraverseLinkedListSC_A
	#define strInitLinkedListSC       strInitLinkedListSC_M
	#define strDeleteLinkedListSC     strDeleteLinkedListSC_M
	#define strLocateItemSC           strLocateItemSC_R
	#define strTraverseLinkedListDC_X strTraverseLinkedListDC_A
	#define strInitLinkedListDC       strInitLinkedListDC_M
	#define strDeleteLinkedListDC     strDeleteLinkedListDC_M
	#define strLocateItemDC           strLocateItemDC_R
	/* Macros for matrices. */
	#define strFreeMatrix             strFreeMatrix_M
	#define strDeleteMatrix           strDeleteMatrix_M
	#define strSetMatrix              strSetMatrix_M
	#define strSetValueMatrix         strSetValueMatrix_M
	#define strFreeBMap               strFreeBMap_M
	#define strDeleteBMap             strDeleteBMap_M
	#define strCopyBMap               strCopyBMap_M
#else /* Optimization has been disabled. */
	/* Macros for miscellaneous data structures. */
	#define strBitStreamIsEmpty       strBitStreamIsEmpty_O
	/* Macros for atomic structures. */
	#define strFreeArrayZ             strFreeArrayZ_O
	#define strDeleteArrayZ           strDeleteArrayZ_O
	#define strFreeNodeS              strFreeNodeS_O
	#define strDeleteNodeS            strDeleteNodeS_O
	#define strFreeNodeD              strFreeNodeD_O
	#define strDeleteNodeD            strDeleteNodeD_O
	/* Macros for sized arrays. */
	#define strLevelArrayZ            strLevelArrayZ_O
	#define strCopyArrayZ             strCopyArrayZ_O
	#define strMoveArrayZ             strMoveArrayZ_O
	#define strLocateItemArrayZ       strLocateItemArrayZ_O
	#define strSortArrayZ             strSortArrayZ_O
	#define strBinarySearchArrayZ     strBinarySearchArrayZ_O
	/* Macros for linked-lists. */
	#define strTraverseLinkedListSC_X strTraverseLinkedListSC_A
	#define strInitLinkedListSC       strInitLinkedListSC_O
	#define strDeleteLinkedListSC     strDeleteLinkedListSC_O
	#define strLocateItemSC           strLocateItemSC_N
	#define strTraverseLinkedListDC_X strTraverseLinkedListDC_A
	#define strInitLinkedListDC       strInitLinkedListDC_O
	#define strDeleteLinkedListDC     strDeleteLinkedListDC_O
	#define strLocateItemSC           strLocateItemSC_N
	/* Macros for matrices. */
	#define strFreeMatrix             strFreeMatrix_O
	#define strDeleteMatrix           strDeleteMatrix_O
	#define strSetMatrix              strSetMatrix_O
	#define strSetValueMatrix         strSetValueMatrix_O
	#define strFreeBMap               strFreeBMap_O
	#define strDeleteBMap             strDeleteBMap_O
	#define strCopyBMap               strCopyBMap_O
#endif

#endif

/* A diagram shows users a sparse matrix:
 * # Assume that this following sparse matrix contains 4 lines and 5 columns,
 * and (0,0)=2; (1,1)=4; (2,2)=6; (3,3)=8;
 * Matrix bit mask:  | Matrix data:
 *         0 1 2 3 4 |         0 1 2 3 4
 * bm(0,x) 1 0 0 0 0 | bm(0,x) 2 0 0 0 0
 * bm(1,x) 0 1 0.0 0 | bm(1,x) 0 4 0 0 0
 * bm(2,x) 0 0 1 0 0 | bm(2,x) 0 0 6 0 0
 * bm(3,x) 0.0 0 1 0 | bm(3,x) 0 0 0 8 0
 *         0 0 0 0.. |
 * Matrix data list: 2->4->6->8->NULL;
 * # The above sparse matrix in memory is:
 * +_SPAMAT______________
 * |BITMAT               |
 * |________________bmask|
 * |size_t|size_t|ARRAY_Z|
 * |____ln|___col|size_t |
 * |_____4|_____5|____num|
 * |LIST_S       |______3|
 * |______ datlst|PUCHAR |
 * |*____________|__pdata|
 *  |            |______*->[0x82][0x12][0x20]
 *  |
 *  +_NODE_S_  +-+_NODE_S_  +-+_NODE_S_  +-+_NODE_S_
 *  |P_NODE_S| | |P_NODE_S| | |P_NODE_S| | |P_NODE_S|
 *  |___pnode| | |___pnode| | |___pnode| | |___pnode|
 *  |_______*--+ |_______*--+ |_______*--+ |____NULL|
 *  |PUCHAR  |   |PUCHAR  |   |PUCHAR  |   |PUCHAR  |
 *  |___pdata|   |___pdata|   |___pdata|   |___pdata|
 *  |_______*-[2]|_______*-[4]|_______*-[6]|_______*-[8]
 */
