//
//  exp_2019-01-22_1.c
//  A doubly-pointer linked-list solution to Josephus's ring.
//  Created by cyycoish#hotmail.com on 01/22/19.
//  Licence:  Public domain.
//  Platform: Cross Platform.
//
#include <stdio.h>
#include "../src/svstring.h"

// Function: cbftvs_print_list
// Desc:     Print item number in linked-list.
// Param:    pitem: pointer to each node in a list. param: N/A.
// Return:   CBF_CONTINUE only.
int cbftvs_print_list(void * pitem, size_t param)
{
	printf("%ld, ", *(long *)((P_NODE_D)pitem)->pdata);
	DWC4100(param);
	return CBF_CONTINUE;
}

// Function: main
// Desc:     Program entry.
// Param:    N/A.
// Return:   0: no error. 1: allocation failure.
int main(void)
{
	char c;
	long i, j;
	LIST_D jring;
	P_NODE_D pnew, pnode;
	const char * sfx[] = { "m", "ms", "is", "are" };
	strInitLinkedListDC(&jring);
	pnode = jring;
	printf("# A solution to Josephus's ring. <"); svPrintVersion(); printf("> #\n");
Lbl_Start:
	do
	{
		printf("How many items will be there in a ring? ");
		scanf("%ld", &j);
	}
	while (j < 1);
	for (i = 1; i <= j; ++i)
	{
		if (NULL == (pnew = strCreateNodeD(&i, sizeof(long))))
		{
			puts("Error! Allocation failure while building ring.");
			strFreeLinkedListDC(&jring, FALSE);
			return 1; /* Allocation failure. */
		}
		if (NULL != pnode)
		{
			pnode->ppnode[NEXT] = pnew;
			pnew->ppnode[PREV] = pnode;
		}
		else
			jring = pnew;
		pnode = pnew;
	}
	/* Make a ring. */
	pnode->ppnode[NEXT] = jring;
	jring->ppnode[PREV] = pnode;
	do
	{
		printf("\tNow, there %s %ld ite%s in the ring.\nWhich number would you like \
to choose to delete ite%s sequentially? ", sfx[(j > 1) + 2], j, sfx[j > 1], sfx[j > 1]);
		scanf("%ld", &i);
	}
	while (i < 1);
	for ( ;; )
	{
		pnode = strLocateItemDC_R(jring, i - 1);
		if (jring != pnode)
		{
			jring = pnode->ppnode[NEXT];
			strRemoveItemLinkedListDC(pnode);
			printf("\tRemove: %ld\n", *(long *)pnode->pdata);
			strDeleteNodeD(pnode);
		}
		else
			break;
	}
	if (NULL != pnode)
	{
		printf("The rest of ite%s in the ring %s labeled with: ",
		sfx[jring != jring->ppnode[NEXT]], sfx[(jring != jring->ppnode[NEXT]) + 2]);
		strTraverseLinkedListDC_X(jring, NULL, cbftvs_print_list, 0, FALSE); puts("\b\b.");
	}
	strFreeLinkedListDC(&jring, FALSE);
	pnode = NULL;
	do
	{
		fflush(stdin);
		printf("Would you like to continue playing(Y/n)? ");
		scanf("%c", &c);
		switch (c)
		{
		case 'Y': case 'y': goto Lbl_Start;
		case 'N': case 'n': return 0;
		}
	}
	while (c != 'Y' && c != 'y' && c != 'N' && c != 'n');
	return 0;
}
