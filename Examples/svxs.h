/*
 * Name:        svxs.h
 * Description: External Sort Interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0415251642B0709251630L00085
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

#include "svdef.h"

typedef enum en_XSortError
{
	XSE_NONE,            /* Sorting succeeded. */
	XSE_BAD_ARGUMENT,    /* Parameter len or num or size equals to 0. */
	XSE_OPEN_INPUT_FILE, /* Can not open input file. */
	XSE_OPEN_CHUNK_FILE, /* Can not open chunk file. */
	XSE_OPEN_OUTPUT_FILE /* Can not open output file. */
} XSortError;

/* Function declaration goes here. */
XSortError svXSort(FILE * fpout, FILE * fpin, size_t len, size_t num, size_t size, CBF_COMPARE cbfcmp);

#endif

/* Test SVXS suit.
// Codes for wd.c(Generate 1000 int numbers into file data.).
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
	size_t i;
	FILE * fp = fopen("data", "wb");
	srand(time(NULL));
	for (i = 0; i < 1000; ++i) {
		int j = rand();
		fwrite(&j, sizeof(j), 1, fp);
	}
	fclose(fp);
	return 0;
}
// Codes for rd.c(Read file data to stdout.).
#include <stdio.h>
int main() {
	FILE * fp = fopen("data", "rb");
	while (!feof(fp)) {
		int j;
		fread(&j, sizeof(j), 1, fp);
		printf("%d\n", j);
	}
	fclose(fp);
	return 0;
}
// Codes for svxs_test.c(Sort front 500 numbers in file data.).
#include <stdio.h>
#include "svxs.h"
int cbfcmp(const void * px, const void * py) {
	return *(int *)px - *(int *)py;
}
int main() {
	XSortError r;
	FILE * fp = fopen("data", "rb+");
	r = svXSort(fp, fp, 500, 10, sizeof(int), cbfcmp);  // Sort front 500 numbers.
	if (fp)
		fclose(fp);
	return r;
}
*/

