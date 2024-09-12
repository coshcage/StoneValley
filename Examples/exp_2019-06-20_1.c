//
//  exp_2019-06-20_1.c
//  This file demonstrates usage of matrix by a puzzle game.
//  Created by cosh.cage#hotmail.com on 06/20/19.
//  License:  LGPLv3
//  Copyright (C) 2019-2024 John Cage
//  Platform: Unix|GNU/Linux
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "../src/svstring.h"
#include "../src/svstack.h"

/* The following structure defines game actions in undo list. */
typedef struct st_Action {
	size_t x;
	size_t y;
} ACTION;

/* Strings that consist squares in a puzzle. */
const static char * const pAlphabet    = " ABCDEFGHIJKLMNOPQRSTUVWX";
const static char * const pInscription = " nullum_numen_abest_si_sit_prudentia";
const static char * const pValleyMap   = " Librarysss_ALGs___ORIsss_THM__stonesss_\\_/yellaV";
static const char * pAnswer = NULL;

static STACK_L stkAction = NULL; /* Action list. */
static volatile size_t gCtrMove = 0; /* Number of times of movements. */

// Function: PrintMatrixInMat
// Desc:     Both print the matrix presented puzzle and its contour.
// Param:    pmtx: Pointer to a matrix presented puzzle.
// Return:   N/A.
void PrintMatrixInMat(P_MATRIX pmtx)
{
	register char * pc = (char *)(pAnswer + 1);
	register size_t i, j;
	/* Print header. */
	printf("  ");
	for (i = 0; i < pmtx->col; ++i)
		putc(i + 'a', stdout);
	printf("  Move: %d.\n +", gCtrMove);
	for (i = 0; i < pmtx->col; ++i)
		putc('-', stdout);
	printf("+\n");
	/* Print puzzle body. */
	for (i = 0; i < pmtx->ln; ++i)
	{
		printf("%c|", i + 'a');
		for (j = 0; j < pmtx->col; ++j)
			printf("%c", *(char *)strGetValueMatrix(NULL, pmtx, i, j, sizeof(char)));
		printf("| ");
		for (j = 0; j < pmtx->col; ++j)
			if (!(i == pmtx->ln - 1 && j == pmtx->col - 1))
				putc(*pc++, stdout);
		printf(" \n");
	}
	/* Print tail. */
	printf(" +");
	for (i = 0; i < pmtx->col; ++i)
		putc('-', stdout);
	printf("+\n");
}

// Function: ValidateMatrix
// Desc:     Validate a puzzle to check whether it has been finished.
// Param:    pmtx: Pointer to a matrix presented puzzle.
// Return:   TRUE: Solved; FALSE Not solved.
BOOL ValidateMatrix(P_MATRIX pmtx)
{
	if (*(char *)strGetValueMatrix(NULL, pmtx, pmtx->ln - 1, pmtx->col - 1, sizeof(char)) == 32)
	{	/* Only the last square in the puzzle is a space can indicate that the puzzle might be solved. */
		if (0 == memcmp(pAnswer + 1, pmtx->arrz.pdata, strLevelArrayZ(&pmtx->arrz) - 1))
		{	/* Good job. */
			printf(":) CONGRATULATIONS! You win the game in %d movement%c!\n\n", gCtrMove, gCtrMove > 1 ? 's' : '!');
			return TRUE;
		}
	}
	return FALSE;
}

// Function: MoveMatrix
// Desc:     Take an action on a matrix presented puzzle.
// Param:    pmtx: Pointer to a matrix presented puzzle.
//           x:    Line that uses want to hit.
//           y:    Column that uses want to hit.
//           bundo:TRUE Undoing movement; FALSE Normal movement.
// Return:   N/A.
void MoveMatrix(P_MATRIX pmtx, size_t x, size_t y, BOOL bundo)
{
	register char * pchar = (char *)strGetValueMatrix(NULL, pmtx, x, y, sizeof(char));
	if (NULL != pchar)
	{	/* x and y are in range. */
		if (! isspace(*pchar))
		{	/* If the current hit square is not a space,
			 * then take it as the center of a crucifix,
			 * check whether there is a space in the line of the crucifix
			 * or on the column of the crucifix. If it does,
			 * move squares toward the space.
			 */
			ACTION act;
			auto char c;
			register size_t i, j;
			/* Check current column. */
			for (i = 0; i < pmtx->col; ++i)
			{
				if (isspace(*(char *)strGetValueMatrix(NULL, pmtx, i, y, sizeof(char))))
				{
					if (! bundo)
					{
						act.x = i;
						act.y = y;
						stkPushL(&stkAction, &act, sizeof(ACTION));
						++gCtrMove;
					}
					/* Move bricks toward the space. */
					if (i < x)
						for (j = i; j < x; ++j)
							svSwap(strGetValueMatrix(NULL, pmtx, j, y, sizeof(char)),
							strGetValueMatrix(NULL, pmtx, j + 1, y, sizeof(char)), &c, sizeof(char));
					else
						for (j = i; j > x; --j)
							svSwap(strGetValueMatrix(NULL, pmtx, j, y, sizeof(char)),
							strGetValueMatrix(NULL, pmtx, j - 1, y, sizeof(char)), &c, sizeof(char));
					return;
				}
			}
			/* Check current line. */
			for (i = 0; i < pmtx->ln; ++i)
			{
				if (isspace(*(char *)strGetValueMatrix(NULL, pmtx, x, i, sizeof(char))))
				{
					if (! bundo)
					{
						act.x = x;
						act.y = i;
						stkPushL(&stkAction, &act, sizeof(ACTION));
						++gCtrMove;
					}
					/* Move bricks toward the space. */
					if (i < y)
						for (j = i; j < y; ++j)
							svSwap(strGetValueMatrix(NULL, pmtx, x, j, sizeof(char)),
							strGetValueMatrix(NULL, pmtx, x, j + 1, sizeof(char)), &c, sizeof(char));
					else
						for (j = i; j > y; --j)
							svSwap(strGetValueMatrix(NULL, pmtx, x, j, sizeof(char)),
							strGetValueMatrix(NULL, pmtx, x, j - 1, sizeof(char)), &c, sizeof(char));
					return;
				}
			}
		}
	}
	printf(":( You have hit the wrong place at (%c,%c).\n", x <= 26 ? x + 'a' : '?', y <= 26 ? y + 'a' : '?');
}

// Function: UndoMoving
// Desc:     Undo an action on a puzzle.
// Param:    pmtx: Pointer to a matrix presented puzzle.
// Return:   N/A.
void UndoMoving(P_MATRIX pmtx)
{
	if (! stkIsEmptyL(&stkAction))
	{
		ACTION act;
		stkPopL(&act, sizeof(ACTION), &stkAction);
		MoveMatrix(pmtx, act.x, act.y, TRUE); /* Restore previous status. */
		--gCtrMove;
	}
	else
		printf("Undo list is empty.\n");
}

// Function: ShowHelp
// Desc:     Show help information.
// Param:    b: TRUE Show welcome text; FALSE Show usage.
// Return:   N/A.
void ShowHelp(BOOL b)
{
	switch (b)
	{
	case FALSE:
		printf("Welcome to Puzzle at ");
		svPrintVersion();
		printf(".\n\tPlease input:\n\t16: playing a 4*4 puzzle. (Alphabet)\n\t25: playing a 5*5 puzzle. (Alphabet)\n\
\t36: playing a 6*6 puzzle. (Inscription)\n\t49: playing a 7*7 puzzle. (Map)\n\t 0: Exit.\n?> ");
		break;
	case TRUE:
		printf("\tInput 'xy' to hit a square in the puzzle,\n\
\twhereas x denotes line, y denotes column.\n\tInput 'z' to undo.\n\
\tInput '0' or 'q' to quit.\n\tInput 'h' or \'?\' for help.\n\
\tA sequence of actions is allowed to input, such that \'aadaq?dd\'.\n");
	}
}

// Function: CommandParser
// Desc:     Parse a string of command sequence.
// Param:    pmtx: Pointer to a matrix presented puzzle.
//           pcmd: A string that contains commands.
// Return:   0: Command parsed succeeded. 1: Escaping required.
int CommandParser(P_MATRIX pmtx, char * pcmd)
{
	int i = 0, r = 0;
	BOOL bhelp = FALSE;
	do
	{
		if ('\0' == pcmd[0] || '\n' == pcmd[0]) /* Buffer end. */
		{
			if (1 != r)
				r = 0;
			break;
		}
		if ('0' == pcmd[0] || 'q' == pcmd[0]) /* Quit. */
		{
			r = 1;
			++pcmd;
			continue;
		}
		pcmd[0] = tolower(pcmd[0]);
		pcmd[1] = tolower(pcmd[1]);
		if ('z' == pcmd[0])
		{	/* Undo. */
			UndoMoving(pmtx);
			++pcmd;
			continue;
		}
		if (('h' == pcmd[0]) || ('?' == pcmd[0]))
		{	/* Help. */
			if (FALSE == bhelp)
				ShowHelp(bhelp = TRUE);
			++pcmd;
			continue;
		}
		/* Translate the current command. */
		MoveMatrix(pmtx, pcmd[0] - 'a', pcmd[1] - 'a', FALSE);
		pcmd += 2;
		++i;
	}
	while (i < BUFSIZ);
	PrintMatrixInMat(pmtx);
	return r;
}

// Function: main
// Desc:     Program entry.
// Param:    N/A.
// Return:   0 only.
signed int main(void)
{
	MATRIX mtxPuzzle;
	auto char t, ipb[BUFSIZ] = { 0 }; /* Input buffer. */
	stkInitL(&stkAction);
Lbl_Again:
	for (gCtrMove = 0; ; )
	{	/* Select a mode to play or quit game. */
		ShowHelp(FALSE);
		fgets(ipb, BUFSIZ, stdin);
		t = atoi(ipb);
		switch (t)
		{
		case 0:
			exit(0);
		case 16:
		case 25: pAnswer = pAlphabet;    break;
		case 36: pAnswer = pInscription; break;
		case 49: pAnswer = pValleyMap;   break;
		default: pAnswer = NULL;
		}
		if (NULL != pAnswer)
		{
			t = (char)pow(t, 0.5);
			break;
		}
	}
	strInitMatrix(&mtxPuzzle, t, t, sizeof(char));
	memcpy(mtxPuzzle.arrz.pdata, pAnswer, mtxPuzzle.ln * mtxPuzzle.col);
	strShuffleArrayZ(&mtxPuzzle.arrz, &t, sizeof(char), (unsigned int) time(NULL));
	PrintMatrixInMat(&mtxPuzzle);
	do
	{
Lbl_Resume:
		fflush(stdin);
		memset(ipb, 0, BUFSIZ);
		printf("Location h/?> ");
		fgets(ipb, BUFSIZ, stdin);
		if (1 == CommandParser(&mtxPuzzle, ipb))
		{
			int i;
			do
			{	/* Query users to ensure their choices correct. */
				printf("?Are you sure to exit?Y/n/r?> ");
				i = strlen(fgets(ipb, BUFSIZ, stdin));
				if (i > 1)
					i -= 2;
				ipb[i] = tolower(ipb[i]);
				if ('r' == ipb[i])
					goto Lbl_Restart;
				if ('n' == ipb[i])
				{
					printf("Resume...\n");
					PrintMatrixInMat(&mtxPuzzle);
					goto Lbl_Resume;
				}
				putc('\n', stdin);
			}
			while (ipb[i] != 'y');
			stkFreeL(&stkAction);
			strFreeMatrix(&mtxPuzzle);
			return 0;
		}
	}
	while (TRUE != ValidateMatrix(&mtxPuzzle));
Lbl_Restart:
	stkFreeL(&stkAction);
	strFreeMatrix(&mtxPuzzle);
goto Lbl_Again;
	return 0;
}
