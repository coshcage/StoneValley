//
//  exp_2017-12-28_1.c
//  This file demonstrates how to use a double-ended linked list.
//  Created by cosh.cage#hotmail.com on 12/28/17.
//  Licence:  Public domain.
//  Platform: Windows NT.
//

#include <stdio.h>
#include <string.h>
#include "..\\src\\svqueue.h"
#include "..\\src\\svstring.h"
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
	BOOL refresh;
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
	for (refresh = TRUE;; refresh = FALSE)
	{
		if (GetAsyncKeyState(VK_UP))
		{	/* Move cursor up. */
			pos = pos->ppnode[PREV];
			refresh = TRUE;
			pneo = NULL;
		}
		else if (GetAsyncKeyState(VK_DOWN))
		{	/* Move cursor down. */
			pos = pos->ppnode[NEXT];
			refresh = TRUE;
			pneo = NULL;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{	/* Choose. */
			refresh = TRUE;
			pneo = *(char **) pos->pdata;
		}
		else if (GetAsyncKeyState(VK_ESCAPE))
			break; /* Exit program. */

		if (refresh)
		{	/* Need to refresh. */
			system("cls");
			printf("Type up, down or space key to show what happened! Type ESC to quit.\n\n");
			strTraverseLinkedListDC_X(pque->pfirst, NULL, PrintQueue, (size_t)pos, FALSE);
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
