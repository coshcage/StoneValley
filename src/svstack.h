/*
 * Name:        svstack.h
 * Description: Stacks interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0318170912X0728191037L00159
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

#ifndef _SVSTACK_H_
#define _SVSTACK_H_

#include "svstring.h"

/* Array implementation of stacks. */
typedef struct st_STACK_A {
	size_t  top; /* Index of the top item of a stack. */
	ARRAY_Z arr; /* Stack data buffer. */
} STACK_A, * P_STACK_A;

/* Linked-list implementation of stacks. */
typedef LIST_S STACK_L, * P_STACK_L;

/* Functions for array represented stacks. */
void *    stkInitA      (P_STACK_A pstka, size_t       num,   size_t    size);
void      stkFreeA      (P_STACK_A pstka);
P_STACK_A stkCreateA    (size_t    num,   size_t       size);
void      stkDeleteA    (P_STACK_A pstka);
BOOL      stkIsEmptyA_O (P_STACK_A pstka);
BOOL      stkIsFullA_O  (P_STACK_A pstka);
void *    stkPushA_O    (P_STACK_A pstka, const void * pitem, size_t    size);
void      stkPopA_O     (void *    pitem, size_t       size,  P_STACK_A pstka);
void      stkPeepA_O    (void *    pitem, size_t       size,  P_STACK_A pstka);
size_t    stkLevelA_O   (P_STACK_A pstka);
/* Functions for linked-list represented stacks. */
void      stkInitL_O    (P_STACK_L pstkl);
void      stkFreeL_O    (P_STACK_L pstkl);
P_STACK_L stkCreateL_O  (void);
void      stkDeleteL_O  (P_STACK_L pstkl);
BOOL      stkIsEmptyL_O (P_STACK_L pstkl);
P_NODE_S  stkPushL      (P_STACK_L pstkl, const void * pitem, size_t    size);
P_NODE_S  stkPopL       (void *    pitem, size_t       size,  P_STACK_L pstkl);
void      stkPeepL_O    (void *    pitem, size_t       size,  P_STACK_L pstkl);
size_t    stkLevelL_O   (P_STACK_L pstkl);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svstack.c. */
#define stkIsEmptyA_M(pstka_M) (!(pstka_M)->top)
#define stkIsFullA_M(pstka_M) ((pstka_M)->arr.num == (pstka_M)->top)
#define stkPushA_M(pstka_M, pitem_M, size_M) (memcpy((pstka_M)->arr.pdata + ((pstka_M)->top++) * (size_M), (pitem_M), (size_M)))
#define stkPopA_M(pitem_M, size_M, pstka_M) do { \
	memcpy((pitem_M), (pstka_M)->arr.pdata + (--(pstka_M)->top) * (size_M), (size_M)); \
} while (0)
#define stkPeepA_M(pitem_M, size_M, pstka_M) do { \
	memcpy((pitem_M), (pstka_M)->arr.pdata + ((pstka_M)->top - 1) * (size_M), (size_M)); \
} while (0)
#define stkLevelA_M(pstka_M) ((pstka_M)->top)
/* Macros for linked-list represented stacks. */
#define stkInitL_M(pstkl_M) do { \
	strInitLinkedListSC(pstkl_M); \
} while (0)
#define stkFreeL_M(pstkl_M) do { \
	strFreeLinkedListSC(pstkl_M); \
} while (0)
#define stkDeleteL_M(pstkl_M) do { \
	strDeleteLinkedListSC(pstkl_M); \
} while (0)
#define stkIsEmptyL_M(pstkl_M) (!(*pstkl_M))
#define stkPeepL_M(pitem_M, size_M, pstkl_M) do { \
	memcpy((pitem_M), (*(pstkl_M))->pdata, (size_M)); \
} while (0)
#define stkLevelL_M(pstkl_M) (strLevelLinkedListSC(*(pstkl_M)))

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#include <string.h> /* Using function memcpy. */
	/* Macros for array represented stacks. */
	#define stkIsEmptyA stkIsEmptyA_M
	#define stkIsFullA  stkIsFullA_M
	#define stkPushA    stkPushA_M
	#define stkPopA     stkPopA_M
	#define stkPeepA    stkPeepA_M
	#define stkLevelA   stkLevelA_M
	/* Macros for linked-list represented stacks. */
	#define stkInitL    stkInitL_M
	#define stkFreeL    stkFreeL_M
	#define stkCreateL  strCreateLinkedListSC
	#define stkDeleteL  stkDeleteL_M
	#define stkIsEmptyL stkIsEmptyL_M
	#define stkPeepL    stkPeepL_M
	#define stkLevelL   stkLevelL_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#include <string.h> /* Using function memcpy. */
	/* Macros for array represented stacks. */
	#define stkIsEmptyA stkIsEmptyA_M
	#define stkIsFullA  stkIsFullA_M
	#define stkPushA    stkPushA_M
	#define stkPopA     stkPopA_M
	#define stkPeepA    stkPeepA_M
	#define stkLevelA   stkLevelA_M
	/* Macros for linked-list represented stacks. */
	#define stkInitL    stkInitL_M
	#define stkFreeL    stkFreeL_M
	#define stkCreateL  strCreateLinkedListSC
	#define stkDeleteL  stkDeleteL_M
	#define stkIsEmptyL stkIsEmptyL_M
	#define stkPeepL    stkPeepL_M
	#define stkLevelL   stkLevelL_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#include <string.h> /* Using function memcpy. */
	/* Macros for array represented stacks. */
	#define stkIsEmptyA stkIsEmptyA_M
	#define stkIsFullA  stkIsFullA_M
	#define stkPushA    stkPushA_M
	#define stkPopA     stkPopA_M
	#define stkPeepA    stkPeepA_M
	#define stkLevelA   stkLevelA_M
	/* Macros for linked-list represented stacks. */
	#define stkInitL    stkInitL_M
	#define stkFreeL    stkFreeL_M
	#define stkCreateL  strCreateLinkedListSC
	#define stkDeleteL  stkDeleteL_M
	#define stkIsEmptyL stkIsEmptyL_M
	#define stkPeepL    stkPeepL_M
	#define stkLevelL   stkLevelL_M
#else /* Optimization has been disabled. */
	/* Macros for array represented stacks. */
	#define stkIsEmptyA stkIsEmptyA_O
	#define stkIsFullA  stkIsFullA_O
	#define stkPushA    stkPushA_O
	#define stkPopA     stkPopA_O
	#define stkPeepA    stkPeepA_O
	#define stkLevelA   stkLevelA_O
	/* Macros for linked-list represented stacks. */
	#define stkInitL    stkInitL_O
	#define stkFreeL    stkFreeL_O
	#define stkCreateL  strCreateLinkedListSC
	#define stkDeleteL  stkDeleteL_O
	#define stkIsEmptyL stkIsEmptyL_O
	#define stkPeepL    stkPeepL_O
	#define stkLevelL   stkLevelL_O
#endif

#endif
