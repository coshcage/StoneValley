/*
 * Name:        svxs.h
 * Description: External Sort Interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0415251642B0415251642L00039
 * License:     LGPLv3
 * Copyright (C) 2025 John Cage
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
#ifndef _SVXS_H_
#define _SVXS_H_

#include "svstring.h"

typedef enum en_XSortError
{
	XSE_NONE,            /* Sorting succeeded. */
	XSE_OPEN_INPUT_FILE, /* Can not open input file. */
	XSE_OPEN_CHUNK_FILE, /* Can not open chunk file. */
	XSE_OPEN_OUTPUT_FILE /* Can not open output file. */
} XSortError;

/* Function declaration goes here. */
XSortError svXSort(const char * szfin, const char * szfout, size_t num, size_t size, CBF_COMPARE cbfcmp);

#endif

