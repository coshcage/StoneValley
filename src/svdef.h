/*
 * Name:        svdef.c
 * Description: Common definitions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0306171808Z0503231713L00096
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

#ifndef _SVDEF_H_
#define _SVDEF_H_

#include <stddef.h> /* Using type size_t, ptrdiff_t. */

/* Library version. */
#define SV_LIB_VER "1.2.1.0"

/* Boolean constants. */
#define TRUE  1
#define FALSE 0

/* Callback function returning values. */
#define CBF_TERMINATE TRUE
#define CBF_CONTINUE FALSE

/* Common data types. */
typedef unsigned int    BOOL;
typedef unsigned char   UCHART;
typedef unsigned char * PUCHAR; /* Usually, sizeof(unsigned char) == 1. */

/* Callback function types. */
typedef int (* CBF_TRAVERSE) (void * pitem, size_t param);
typedef int (* CBF_COMPARE)  (const void *, const void *);
/* If callback traversal function returned CBF_TERMINATE, traversal would be interrupted,
 *  otherwise, traversal would continue to run till traversal function reached the end.
 * A typical comparing function would like the following lines of codes
 *  in which MYTYPE is a type that user defined previously:
 * int cbfcmp(const void * px, const void * py)
 * {
 *     if (*(MYTYPE *)px > *(MYTYPE *)py) return  1;
 *     if (*(MYTYPE *)px < *(MYTYPE *)py) return -1;
 *     return 0;
 * }
 */

/* A structure describes finding information. */
typedef struct st_FindingInfo {
	void *       result; /* Finding result. */
	const void * pitem;  /* Finding target. */
	size_t       size;   /* Size of target. */
} FindingInfo, * P_FindingInfo;

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
 * As for gcc/clang, please alter the following macro as DWC4100(param) // Noting appears at tail.
 */
#define DWC4100(param) (void)(param)

/* Register qualifier. */
#define REGISTER register

/* Macros for library optimization. */
#define SV_OPT_DISABLED (0x00)
#define SV_OPT_MINISIZE (0x01)
#define SV_OPT_MAXSPEED (0x02)
#define SV_OPT_FULLOPTM (0x03)
/* Library optimization switch. */
#define SV_OPTIMIZATION (SV_OPT_DISABLED)

#endif
