/*
 * Name:        svstack.c
 * Description: Stacks.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0318171803E0604231336L00309
 *
 * The following text is copied from the source code of SQLite and padded
 * with a little bit addition to fit the goals for StoneValley project:
 *
 * The author disclaims copyright to this source code. In place of
 * a legal notice, here is a blessing:
 *   May you do good and not evil.
 *   May you find forgiveness for yourself and forgive others.
 *   May you share freely, never taking more than you give.
 *   Hope you never need to push yourself or other people too hard.
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
 * Description:   Deallocate a stack of which is allocated by function stkInitA.
 * Parameter:
 *     pstka Pointer to the stack you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pstka->arr.pdata Must Be Allocated first.
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
 *       size Size of each elements.
 * Return value:  Pointer to the new stack.
 */
P_STACK_A stkCreateA(size_t num, size_t size)
{
	P_STACK_A pstkn = (P_STACK_A) malloc(sizeof(STACK_A));
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
 * Description:   Delete a stack of which is allocated by function stkCreateA.
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
 * Description:   Make a judgement whether a stack is empty or not.
 * Parameter:
 *     pstka Pointer to the stack you want to check.
 * Return value:
 *         TRUE Stack is empty.
 *        FALSE Stack is NOT empty.
 * Tip:           A macro version of this function named stkIsEmptyA_M is available.
 */
BOOL stkIsEmptyA_O(P_STACK_A pstka)
{
	return !pstka->top;
}

/* Function name: stkIsFullA_O
 * Description:   Make a judgement whether a stack is full or not.
 * Parameter:
 *     pstka Pointer to the stack you want to check.
 * Return value:
 *         TRUE Stack is full.
 *        FALSE Stack is NOT full.
 * Tip:           A macro version of this function named stkIsFullA_M is available
 */
BOOL stkIsFullA_O(P_STACK_A pstka)
{
	return (pstka->arr.num == pstka->top);
}

/* Function name: stkPushA_O
 * Description:   Push an element onto a stack.
 * Parameters:
 *      pstka Pointer to the stack you want to operate with.
 *      pitem Pointer to the address of an element.
 * Return value:  Address of the new inserted element.
 * Caution:       You should check whether the target stack is full or not before invoking.
 * Tip:           A macro version of this function named stkPushA_M is available.
 */
void * stkPushA_O(P_STACK_A pstka, const void * pitem, size_t size)
{
	return memcpy(pstka->arr.pdata + (pstka->top++) * size, pitem, size);
}

/* Function name: stkPopA_O
 * Description:   Pop an element out of a stack.
 * Parameters:
 *      pitem Pointer to the address of an element.
 *      pstka Pointer to the stack you want to operate with.
 * Return value:  N/A.
 * Caution:       You should check whether the target stack is empty or not before invoking.
 * Tip:           A macro version of this function named stkPopA_M is available.
 */
void stkPopA_O(void * pitem, size_t size, P_STACK_A pstka)
{
	memcpy(pitem, pstka->arr.pdata + (--pstka->top) * size, size);
}

/* Function name: stkPeepA_O
 * Description:   Have a peek at the top of the stack.
 * Parameters:
 *      pitem Pointer to the address of an element.
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
 * Tip:           You should better inline this function while linking.
 *                A macro version of this function named stkLevelA_M is available.
 */
size_t stkLevelA_O(P_STACK_A pstka)
{
	return pstka->top;
}

/* Function name: stkInitL_O
 * Description:   Initialize a Linked-list stack.
 * Parameters:
 *      pstkl Pointer to the stack you want to create.
 *       size Size of each element in the stack.
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
 * Description:   Deallocate a linked-list stack of which is allocated by function stkInitL.
 * Parameter:
 *     pstkl Pointer to the stack you want to deallocate.
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
 * Description:   Create a new pointer with a new allocated Linked-list stack.
 * Parameter:     N/A.
 * Return value:  A pointer to a new allocated linked-list stack.
 * Tip:           This function can be inline for better performance.
 */
P_STACK_L stkCreateL_O(void)
{
	return strCreateLinkedListSC();
}

/* Function name: stkDeleteL_O
 * Description:   Destroy a linked-list stack of which is allocated by function stkCreateL.
 * Parameter:
 *     pstkl Pointer to the stack you want to delete from the main-memory.
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
 * Description:   Make a judgement whether a stack is empty or not.
 * Parameter:
 *     pstkl Pointer to the stack you want to check.
 * Return value:
 *         TRUE Stack is empty.
 *        FALSE Stack is not empty.
 * Caution:       Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkIsEmptyA_M is available.
 */
BOOL stkIsEmptyL_O(P_STACK_L pstkl)
{
	return !(*pstkl);
}

/* Function name: stkPushL
 * Description:   Push an element onto stack.
 * Parameters:
 *      pstkl Pointer to the stack you want to operate.
 *      pitem Pointer to the address of an element.
 *       size Size of that element.
 * Return value:   NULL if pushing failed or a valid pointer of the top element for the current stack.
 * Caution:        Address of pstkl Must Be Allocated first.
 */
P_NODE_S stkPushL(P_STACK_L pstkl, const void * pitem, size_t size)
{
	REGISTER P_NODE_S ptmp;
	if (NULL != (ptmp = strCreateNodeS(pitem, size)))
	{
		ptmp->pnode = *pstkl;
		*pstkl = ptmp;
	}
	return ptmp;
}

/* Function name: stkPopL
 * Description:   Pop an element from a stack.
 * Parameters:
 *      pitem Pointer to the address of an element.
 *       size Size of each elements.
 *      pstkl Pointer to the stack you want to operate.
 * Return value: Address of current element.
 *               If function returned a NULL, it should mean there were no element settled in the stack anymore.
 * Caution:      You should check whether the target stack is empty or not before invoking.
 *               Address of pstkl Must Be Allocated first.
 */
P_NODE_S stkPopL(void * pitem, size_t size, P_STACK_L pstkl)
{
	REGISTER P_NODE_S ptmp = NULL;
	if (NULL != *pstkl)
	{
		ptmp = (*pstkl)->pnode;
		memcpy(pitem, (*pstkl)->pdata, size);
		strDeleteNodeS(*pstkl);
	}
	return *pstkl = ptmp;
}

/* Function name: stkPeepL_O
 * Description:   Have a peek at the top of the stack.
 * Parameters:
 *      pitem Pointer to the address of an element.
 *       size Size of that element.
 *      pstkl Pointer to the stack you want to operate.
 * Return value:  N/A.
 * Caution:       You should check whether the target stack is empty or not before invoking.
 *                Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkPeepL_M is available.
 */
void stkPeepL_O(void * pitem, size_t size, P_STACK_L pstkl)
{
	memcpy(pitem, (*pstkl)->pdata, size);
}

/* Function name: stkLevelL_O
 * Description:   Get tier number of a stack.
 * Parameter:
 *     pstkl Pointer to the stack you want to operate.
 * Return value:  Number of tiers.
 * Caution:       You should check whether the target stack is empty or not before invoking.
 *                Address of pstkl Must Be Allocated first.
 * Tip:           A macro version of this function named stkLevelL_M is available.
 */
size_t stkLevelL_O(P_STACK_L pstkl)
{
	return strLevelLinkedListSC(*pstkl);
}
