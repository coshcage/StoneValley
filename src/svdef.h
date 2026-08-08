/*
 * Name:        svdef.c
 * Description: Common definitions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306171808Z0806261900L00130
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

#ifndef _SVDEF_H_
#define _SVDEF_H_

#include <stddef.h>  /* Using type size_t, ptrdiff_t. */
#include <stdbool.h> /* Boolean type and constants. */

/* Library version. */
#define SV_LIB_VER "1.3.4.1"
/* This following macro turns library version string into a size_t integer to be calculated by users. */
#define SV_LIB_VER_VAL ((size_t) ((SV_LIB_VER[0] - '0') * 1000UL + (SV_LIB_VER[2] - '0') * 100UL + (SV_LIB_VER[4] - '0') * 10UL + (SV_LIB_VER[6] - '0')))

/* Common data types. */
typedef unsigned char   UCHART;
typedef unsigned char * PUCHAR; /* Usually, sizeof(unsigned char) == 1. */

/* Convert the result of an expression into a boolean value. */
#define BOOLIZE(exp_term) (!!(exp_term))

/* Callback function returning values. */
typedef enum en_CBFReturn {
	CBF_CONTINUE  = false, /* Continue to proceed. */
	CBF_TERMINATE = true,  /* Break caller in most cases. */
	CBF_CMP_EQUAL =  0,    /* Data are equivalent. */
	CBF_CMP_GT    =  1,    /* Great than. */
	CBF_CMP_LT    = -1     /* Less than. */
} CBFReturn;

/* Redefine the aforementioned enumeration to preserve sign. */
typedef int en_CBFReturn;

/* Callback function types. */
typedef int (* CBF_TRAVERSE) (void * pitem, size_t param);
typedef int (* CBF_COMPARE)  (const void *, const void *);
/* If callback traversal function returned CBF_TERMINATE, traversal would be interrupted in most cases,
 * otherwise, traversal would continue to run till traversal function reached the end.
 * Uses need to reference to the caller for more details of callback usage.
 *
 * A typical comparing function would like the following lines of codes
 * in which MYTYPE is a type that user defined previously:
 * int cbfcmp(const void * px, const void * py) {
 *     if (*(MYTYPE *)px > *(MYTYPE *)py) return CBF_CMP_GT;
 *     if (*(MYTYPE *)px < *(MYTYPE *)py) return CBF_CMP_LT;
 *     return CBF_CMP_EQUAL;
 * }
 *
 * Especially, when we only need to match data through pointers, we can use:
 * int cbfmch(const void * px, const void * py) {
 *     return (int)BOOLIZE(*(MYTYPE *)px - *(MYTYPE *)py); // Use a subtraction between scalar types.
 * }
 * In the above type of comparison function, px always points to left value, for example the datum in a set.
 * While py always points to the datum that stored in the right value such as the value that pitem pointed.
 */

/* Result of unsigned integer division. */
typedef struct st_stdiv_t {
	size_t quot; /* Quotient.  */
	size_t rem;  /* Remainder. */
} stdiv_t;

/* A utility function used to print library version. */
void svPrintVersion(void);

/* Unsigned integer division. */
stdiv_t stdiv(size_t numerator, size_t denominator);

/* Disable warning C4100 for Visual C/C++ compiler.
 * As for gcc/clang, use this macro to disable compiler warning for unused parameter.
 * Usage: DISUSE(variable); DISUSE(fun(a, b)); // Drop function return value by this way.
 */
#define DISUSE(param) (void)(param)

/* Register qualifier. */
#define REGISTER register

/* A macro that is used to align size to the multiply of sizeof(size_t)
 * with a little bit alteration to prevent Visual C 19.0 compiler to trigger a C4146 error.
 * Users may watch this technique on the book Hacker's Delight wrote by Henry S. Warren.
 * With ISBN 0-201-91465-4. Chapter 3-1.
 */
#define ALIGN_SIZET(size) (((size) + sizeof(size_t) - 1) & -(ptrdiff_t)sizeof(size_t))

/* Macros for library optimization. */
#define SV_OPT_DISABLED  0x00
#define SV_OPT_MINISIZE  0x01
#define SV_OPT_MAXSPEED  0x02
#define SV_OPT_FULLOPTM  0x03

/* Library optimization switch. */
#define SV_OPTIMIZATION (SV_OPT_DISABLED)

/* Use this macro to print a concise diagnostic information for assertion. */
#define DBG_PRINT fprintf(stderr, "Assertion failure at line %d in file %s!\n", __LINE__, __FILE__)

/* Library optimal switch for assertion. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#define SV_ASSERT(expr) true
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#define SV_ASSERT(expr) true
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	#define SV_ASSERT(expr) true
#else /* Optimization has been disabled. */
	#include <stdio.h> /* Use fprintf and stderr for assertion. */
	#define SV_ASSERT(expr) (BOOLIZE(expr) ? true : (DISUSE(DBG_PRINT), false))
#endif

#endif

