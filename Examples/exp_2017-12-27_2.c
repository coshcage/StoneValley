//
//  exp_2017-12-27_2.c
//  This file demonstrates how to use array represented circular queues.
//  Created by cosh.cage#hotmail.com on 12/27/17.
//  License:  LGPLv3
//  Platform: Unix|GNU/Linux
//  Copyright (C) 2017-2026 John Cage
//
// This file is part of StoneValley.
//
// StoneValley is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// StoneValley is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along with StoneValley.
// If not, see <https://www.gnu.org/licenses/>.
//
#include <stdio.h>
#include <string.h>
#include "svqueue.h"

static const char * slnefmt[4] = {
	"      %c\n",
	"   %c     %c\n",
	" %c         %c\n",
	"%c           %c\n"
};

void PrintQueue(P_QUEUE_A pque)
{
	printf(slnefmt[0], pque->arr.pdata[0]);
	printf(slnefmt[1], pque->arr.pdata[11], pque->arr.pdata[1]);
	printf(slnefmt[2], pque->arr.pdata[10], pque->arr.pdata[2]);
	printf(slnefmt[3], pque->arr.pdata[9],  pque->arr.pdata[3]);
	printf(slnefmt[2], pque->arr.pdata[8],  pque->arr.pdata[4]);
	printf(slnefmt[1], pque->arr.pdata[7],  pque->arr.pdata[5]);
	printf(slnefmt[0], pque->arr.pdata[6]);
}

static char * strtip = "Queue is initial.\n\n";

int main(void)
{
	char m;
	char c = 'a';
	P_QUEUE_A pque;
	if (NULL == (pque = queCreateAC(12, sizeof(char))))
	{
		printf("Can not initialize queue.\n");
		return 1;
	}
	memcpy(pque->arr.pdata, "............", 12);
	PrintQueue(pque);
	printf("%s", strtip);
	for ( ;; )
	{
		printf("%s", "ix/r/h/q> ");
		scanf("%c%c", &m, &c);
		switch (m)
		{
			case 'i': /* Insertion. */
				queInsertAC(pque, &c, sizeof(char));
				PrintQueue(pque);
				if (queIsInitialAC(pque))
					printf("%s", strtip);
				else
					puts("\n");
				break;
			case 'r': /* Deletion. */
				queRemoveAC(&c, sizeof(char), pque);
				pque->arr.pdata[!pque->front ? 11 : pque->front - 1] = '.';
				printf("\'%c\' Removed.\n", c);
				PrintQueue(pque);
				if (queIsInitialAC(pque))
					printf("%s", strtip);
				break;
			case 'h': /* Show help. */
			case '?':
				printf(" * Type ix to insert a character into queue.\n\
   x denotes any ASCII character.\n\
 * Type r to remove a character from queue.\n\
 * Type h or ? to view help.\n\
 * Type q to quit program.\n\n");
				break;
			case 'q': /* Quit. */
				goto Lbl_Exit;
			default:
				printf("Bad command: \'%c\'. Type \'h\' or \'?\' for help.\n\n", m);
		}
		fflush(stdin);
	}
Lbl_Exit:
	queDeleteAC(pque);
	return 0;
}

