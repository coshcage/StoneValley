//
//  exp_2017-12-28_1.c
//  This file demonstrates how to use a double-ended linked list.
//  Created by cosh.cage#hotmail.com on 12/28/17.
//  License:  LGPLv3
//  Platform: Windows NT.
//  Copyright (C) 2017-2025 John Cage
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
#include "svstring.h"
#include <Windows.h>

static char * pstr[16] = {
	"Bill",
	"Adlai",
	"Bob",
	"Boyce",
	"Carter",
	"Curt",
	"Danny",
	"Francisco",
	"Jeff",
	"Juliet",
	"Keith",
	"Leo",
	"Stu",
	"Doug",
	"Elizabeth",
	"Eliot"
};

int PrintQueue(void * pitem, size_t param)
{
	if (param != (size_t)pitem)
		printf("\t   %s\n", *(char **) ((P_NODE_D) pitem)->pdata);
	else
		printf("\t=> %s\n", *(char **) ((P_NODE_D) pitem)->pdata);
	return CBF_CONTINUE;
}

int main(void)
{
	int i;
	bool refresh;
	P_NODE_D pos;
	P_DEQUE_DL pque;
	char * pneo = NULL;
	if (NULL == (pque = queCreateDL()))
	{
		printf("%s\n", "Can not initialize queue.");
		return 1;
	}
	/* Fill up queue. */
	for (i = 0; i < 16; ++i)
		queInjectDL(pque, &pstr[i], sizeof(char *));
	pos = pque->pfirst;
	/* Snake ate its tail. */
	pque->pfirst->ppnode[PREV] = pque->plast;
	pque->plast->ppnode[NEXT] = pque->pfirst;
	for (refresh = true;; refresh = false)
	{
		if (GetAsyncKeyState(VK_UP))
		{	/* Move cursor up. */
			pos = pos->ppnode[PREV];
			refresh = true;
			pneo = NULL;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{	/* Move cursor down. */
			pos = pos->ppnode[NEXT];
			refresh = true;
			pneo = NULL;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{	/* Choose. */
			refresh = true;
			pneo = *(char **) pos->pdata;
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
			break; /* Exit program. */

		if (refresh)
		{	/* Need to refresh. */
			system("cls");
			printf("Type up, down or space key to show what happened! Type ESC to quit.\n\n");
			strTraverseLinkedListDC_X(pque->pfirst, NULL, PrintQueue, (size_t)pos, false);
			putc('\n', stdout);
			if (pneo != NULL)
			{
				printf("%s has been chosen.\n\n", pneo);
			}
		}
		Sleep(100); /* Delay for 100ms. */
	}
	/* Don't forget to clear queue after using. */
	queDeleteDL(pque);
	return 0;
}

