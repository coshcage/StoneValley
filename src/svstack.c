/*
 * Name:        svstack.c
 * Description: Stacks.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0318171803E0721260821L00321
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

#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function memcpy. */
#include "svstack.h"

/* Function name: stkInitA
 * Description:   Allocate a stack.
 * Parameters:
 *      pstka Pointer to the stack you want to allocate.
 *        num Number of elements.
 *       size Size of each elements.
 * Return value:  Pointer to the new allocated buffer.
 * Caution:       Address of pstka Must Be Allocated first.
 */
void * stkInitA(P_STACK_A pstka, size_t num, size_t size)
{
	pstka->top = 0;
	return strInitArrayZ(&pstka->arr, num, size);
}

/* Function name: stkFreeA
 * Description:   Retract a stack which is allocated by function stkInitA.
 * Parameter:
 *     pstka Pointer to the stack you want to release.
 * Return value:  N/A.
 * Caution:       Address of pstka Must Be Allocated first.
 */
void stkFreeA(P_STACK_A pstka)
{
	strFreeArrayZ(&pstka->arr);
	pstka->top = 0;
}

/* Function name: stkCreateA
 * Description:   Create a new array style stack dynamically.
 * Parameters:
 *        num Number of elements.
 *       size Size of each element.
 * Return value:  Pointer to the new stack.
 */
P_STACK_A stkCreateA(size_t num, size_t size)
{
	REGISTER P_STACK_A pstkn = (P_STACK_A) malloc(sizeof(STACK_A));
	if (NULL != pstkn)
	{
		if (NULL == stkInitA(pstkn, num, size))
		{	/* Allocation failure. */
			free(pstkn);
			return NULL;
		}
	}
	return pstkn;
}

/* Function name: stkDeleteA
 * Description:   Delete a stack which is allocated by function stkCreateA.
 * Parameter:
 *     pstka Pointer to the stack you want to delete.
 * Return value:  N/A.
 * Caution:       Parameter pstka Must Be Allocated first.
 */
void stkDeleteA(P_STACK_A pstka)
{
	stkFreeA(pstka);
	free(pstka);
}

/* Function name: stkIsEmptyA_O
 * Description:   Make a judgment whether a stack is empty or not.
 * Parameter:
 *     pstka Pointer to the stack you want to check.
 * Return value:  true  Stack is empty.
 *                false Stack is NOT empty.
 * Tip:           A macro version of this function named stkIsEmptyA_M is available.
 */
bool stkIsEmptyA_O(P_STACK_A pstka)
{
	return !pstka->top;
}

/* Function name: stkIsFullA_O
 * Description:   Make a judgment whether a stack is full or not.
 * Parameter:
 *     pstka Pointer to the stack you want to check.
 * Return value:  true  Stack is full.
 *                false Stack is NOT full.
 * Tip:           A macro version of this function named stkIsFullA_M is available
 */
bool stkIsFullA_O(P_STACK_A pstka)
{
	return pstka->arr.num == pstka->top;
}

/* Function name: stkPushA_O
 * Description:   Push an element onto a stack.
 * Parameters:
 *      pstka Pointer to the stack you want to operate with.
 *      pitem Pointer to an element you want to copy to the stack.
 *            Set pitem to NULL to only pile one tier up to the stack.
 *       size Size of each element in the stack array and the data that pitem pointed.
 * Return value:  N/A.
 * Caution:       You should check whether the target stack is full or not before invoking.
 * Tip:           A macro version of this function named stkPushA_M is available.
 */
void stkPushA_O(P_STACK_A pstka, const void * pitem, size_t size)
{
	if (NULL != pitem)
		memcpy(pstka->arr.pdata + pstka->top * size, pitem, size);
	++pstka->top;
}

/* Function name: stkPopA_O
 * Description:   Pop an element from a stack.
 * Parameters:
 *      pitem Pointer to an element to receive popped data.
 *            Set pitem to NULL to omit popped item from stack.
 *       size Size of each element in the stack array and the data that pitem pointed.
 *      pstka Pointer to the stack you want to operate with.
 * Return value:  N/A.
 * Caution:       You should check whether the target stack is empty or not before invoking.
 * Tip:           A macro version of this function named stkPopA_M is available.
 */
void stkPopA_O(void * pitem, size_t size, P_STACK_A pstka)
{
	if (NULL != pitem)
		memcpy(pitem, pstka->arr.pdata + (--pstka->top) * size, size);
	else
		--pstka->top;
}

/* Function name: stkPeepA_O
 * Description:   Have a peek at the top of the stack.
 * Parameters:
 *      pitem Pointer to an element to receive top data in the stack.
 *       size Size of element in the stack.
 *      pstka Pointer to the stack you want to operate with.
 * Return value:  N/A.
 * Caution:       You should check whether the target stack is empty or not before invoking.
 * Tip:           A macro version of this function named stkPeepA_M is available.
 */
void stkPeepA_O(void * pitem, size_t size, P_STACK_A pstka)
{
	memcpy(pitem, pstka->arr.pdata + (pstka->top - 1) * size, size);
}

/* Function name: stkLevelA_O
 * Description:   Get tier number of a stack.
 * Parameter:
 *     pstka Pointer to the stack you want to check.
 * Return value:  Number of tiers of stack.
 * Tip:           You should better inline this function during linkage.
 *                A macro version of this function named stkLevelA_M is available.
 */
size_t stkLevelA_O(P_STACK_A pstka)
{
	return pstka->top;
}

/* Function name: stkInitL_O
 * Description:   Initialize a linked list stack.
 * Parameter:
 *     pstkl Pointer to the stack you want to initialize.
 * Return value:  N/A.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           This function can be inline for better performance.
 *                A macro version of this function named stkInitL_M is available.
 */
void stkInitL_O(P_STACK_L pstkl)
{
	strInitLinkedListSC(pstkl);
}

/* Function name: stkFreeL_O
 * Description:   Retract a linked list stack that is allocated by function stkInitL.
 * Parameter:
 *     pstkl Pointer to the stack you want to release.
 * Return value:  N/A.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           This function can be inline for better performance.
 *                A macro version of this function named stkFreeL_M is available.
 */
void stkFreeL_O(P_STACK_L pstkl)
{
	strFreeLinkedListSC(pstkl);
}

/* Function name: stkCreateL_O
 * Description:   Create a new pointer to a new allocated linked list stack.
 * Parameter:     N/A.
 * Return value:  A pointer to a new allocated linked list stack.
 * Tip:           This function can be inline for better performance.
 */
P_STACK_L stkCreateL_O(void)
{
	return strCreateLinkedListSC();
}

/* Function name: stkDeleteL_O
 * Description:   Destroy a linked list stack which is allocated by function stkCreateL.
 * Parameter:
 *     pstkl Pointer to the stack you want to delete from the main memory.
 * Return value:  N/A.
 * Caution:       Address of pstkl Must Be Allocated By Function stkCreateL at first.
 * Tip:           This function can be inline for better performance.
 *                A macro version of this function named stkDeleteL_M is available.
 */
void stkDeleteL_O(P_STACK_L pstkl)
{
	strDeleteLinkedListSC(pstkl);
}

/* Function name: stkIsEmptyL
 * Description:   Make a judgment whether a stack is empty or not.
 * Parameter:
 *     pstkl Pointer to the stack you want to check.
 * Return value:  true  Stack is empty.
 *                false Stack is not empty.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkIsEmptyL_M is available.
 */
bool stkIsEmptyL_O(P_STACK_L pstkl)
{
	return NULL == *pstkl;
}

/* Function name: stkPushL
 * Description:   Push an element onto stack.
 * Parameters:
 *      pstkl Pointer to the stack you want to operate.
 *      pitem Pointer to an element to be pushed onto stack.
 *       size Size of that element.
 * Return value:   NULL if pushing failed or a valid pointer of the top element from the current stack.
 * Caution:        Address of pstkl Must Be Allocated first.
 */
P_NODE_S stkPushL(P_STACK_L pstkl, const void * pitem, size_t size)
{
	REGISTER P_NODE_S ptmp = strCreateNodeS(pitem, size);
	if (NULL != ptmp)
	{
		ptmp->pnode = *pstkl;
		*pstkl = ptmp;
	}
	return ptmp;
}

/* Function name: stkPopL
 * Description:   Pop an element from a stack.
 * Parameters:
 *      pitem Pointer to an element to receive popped data from stack.
 *            Set pitem to NULL to omit popped item from stack.
 *       size Size of element that pitem pointed.
 *      pstkl Pointer to the stack you want to operate.
 * Return value: Address of the top node of a linked list stack.
 *               If function returned NULL, it should mean there were no element in the stack anymore.
 * Caution:      You should check whether the target stack is empty or not before invoking.
 *               Address of pstkl Must Be Allocated first.
 */
P_NODE_S stkPopL(void * pitem, size_t size, P_STACK_L pstkl)
{
	REGISTER P_NODE_S ptmp = NULL;
	if (NULL != *pstkl)
	{
		ptmp = (*pstkl)->pnode;
		if (NULL != pitem)
			memcpy(pitem, (*pstkl)->pdata, size);
		strDeleteNodeS(*pstkl);
	}
	return *pstkl = ptmp;
}

/* Function name: stkPeepL_O
 * Description:   Have a peek at the top of the stack.
 * Parameters:
 *      pitem Pointer to an element to receive the top item data of stack.
 *       size Size of that element and the data of the top item of stack.
 *      pstkl Pointer to the stack you want to operate.
 * Return value:  Peek successfully.
 *                Peek failed.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkPeepL_M is available.
 */
bool stkPeepL_O(void * pitem, size_t size, P_STACK_L pstkl)
{
	return stkIsEmptyL(pstkl) ? false : (memcpy(pitem, (*pstkl)->pdata, size), true);
}

/* Function name: stkLevelL_O
 * Description:   Get tier number of a stack.
 * Parameter:
 *     pstkl Pointer to the stack you want to operate.
 * Return value:  Number of tiers.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkLevelL_M is available.
 */
size_t stkLevelL_O(P_STACK_L pstkl)
{
	return strLevelLinkedListSC(*pstkl);
}

