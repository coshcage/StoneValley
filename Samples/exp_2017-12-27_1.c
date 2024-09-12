//
//  exp_2017-12-27_1.c
//  This file demonstrates how to use stacks, queues and binary trees.
//  The program is used to compute or convert infix expressions.
//  Created by cosh.cage#hotmail.com on 12/27/17.
//  License:  LGPLv3
//  Copyright (C) 2017-2024 John Cage
//  Platform: Unix|GNU/Linux
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "../src/svstack.h"
#include "../src/svqueue.h"
#include "../src/svtree.h"

#define BUF_SIZ (100)
#define OPR_MAX (8)

/* A structure describes operators. */
typedef struct st_Operator {
	char name;  /* Operator name. */
	char level; /* Precedence of an operator. */
} OPERATOR, * P_OPERATOR;

/* Operator table. */
static const OPERATOR oprTable[OPR_MAX] = {
	'+', 1,
	'-', 1,
	'*', 2,
	'/', 2,
	'%', 2,
	'^', 3,
	'+', 4,
	'-', 4
};

/* A structure describes elements in expression. */
typedef struct st_Element {
	union ut_EData {
		float  value; /* Operand. */
		OPERATOR opr; /* Operator. */
	} edata;          /* Element data. */
	int bnum;         /* TRUE while element is an operand. */
} ELEMENT, * P_ELEMENT;

static P_QUEUE_L queRPNExpr  = NULL; /* A queue used to build an execution sequence. */
static P_STACK_L stkOperand  = NULL; /* Operand stack used to hold operands. */
static P_STACK_L stkOperator = NULL; /* Operator stack used to handle precedence. */

static char * strException = "Unhandled exception while pushing an operator into stack.\n";

// Function: PrintInfo.
// Desc:     Print helping information.
// Param:    b: 0 - Print welcome text. Not 0 - Print Helping information.
// Return:   N/A.
void PrintInfo(int b)
{
	if (!b)
	{
		void svPrintVersion(void);
		printf("############################\n\
#      Expression Tool     #\n");
		printf("#    "); svPrintVersion(); printf("   #\n");
		printf("#    exp_2017-12-26_2.c    #\n\
############################\n");
	}
	else
	{
		puts("Type \'h\' or \'?\' for helping. Type \'m\' to change mode. Type \'q\' to quit.\n\
Help:\n\
\t* In Calc-Infix mode, to calculate an infix expression, \n\
\t  for example, just input -(3.14 + 5) * 2 and press enter.\n\
\t* In Conv-Suffix mode, the inputted infix expression\n\
\t  will be converted into proper suffix(RPN) expression.\n\
\t* In Conv-Postfix mode, the inputted infix expression\n\
\t  will be converted into proper postfix(Polish Notation) expression.\n\
\t* Scientific numbers like 2.35E+15 are NOT supported.\n\
\t* Float point numbers all will be converted into 32-bit floats.");
		printf("\t* Length of expressions to input are limited into %d characters.\n", BUF_SIZ);
		puts("Bugs:\n\
\t* Postfix expression converting mode may produce a bad result when\n\
\t  program is coping in an expression which is like -1+2*3.\n");
	}
}

// Function: GetOprByName.
// Desc:     Get an operator's level by its name.
// Param:    n: Name of an operator.
// Return:   Level of an operator.
//           0: Can not find operator in table.
char GetOprByName(char n)
{
	int i;
	for (i = 0; i < OPR_MAX; ++i)
		if (n == oprTable[i].name)
			return oprTable[i].level;
	return '\0';
}

// Function: PrintExecutionQueue.
// Desc:     Print expression in RPN form.
// Param:    pitem: Pointer to each NODE_S structure in the list.
//           param: N/A.
// Return:   The same value as a callback function returns.
int PrintExecutionQueue(void * pitem, size_t param)
{
	P_ELEMENT pelmt = (P_ELEMENT) ((P_NODE_S) pitem)->pdata;
	if (pelmt->bnum)
		printf("%g ", pelmt->edata.value);
	else
	{
		if ('-' == pelmt->edata.opr.name && 4 == pelmt->edata.opr.level)
			printf("_ ");
		else
			printf("%c ", pelmt->edata.opr.name);
	}
	DWC4100(param);
	return CBF_CONTINUE;
}

// Function: ComputeExecutionQueue.
// Desc:     Calculate the value of an RPN expression.
// Param:    pitem: Pointer to each NODE_S structure in the list.
//           param: N/A.
// Return:   The same value as a callback function returns.
int ComputeExecutionQueue(void * pitem, size_t param)
{
	P_ELEMENT pelmt = (P_ELEMENT) ((P_NODE_S) pitem)->pdata;
	if (pelmt->bnum)
	{
		if (NULL == stkPushL(stkOperand, &pelmt->edata.value, sizeof(float)))
			goto Lbl_Allocation_Failure;
	}
	else
	{
		float f1, f2;
		if (! stkIsEmptyL(stkOperand))
			stkPopL(&f1, sizeof(float), stkOperand);
		else
			goto Lbl_Bad_Operator;
		if (pelmt->edata.opr.level != 4)
		{
			if (! stkIsEmptyL(stkOperand))
				stkPopL(&f2, sizeof(float), stkOperand);
			else
				goto Lbl_Bad_Operator;
			switch (pelmt->edata.opr.name)
			{
				case '+': f2 += f1; break;
				case '-': f2 -= f1; break;
				case '*': f2 *= f1; break;
				case '/': if (0.0f == f1) goto Lbl_Math_Error; f2 /= f1; break;
				case '%': f2 = (float)((long)f2 % (long)f1); break;
				case '^': f2 = powf(f2, f1); break;
			}
			f1 = f2;
		}
		else
		{
			if ('-' == pelmt->edata.opr.name)
				f1 *= -1.0f;
		}

		if (NULL == stkPushL(stkOperand, &f1, sizeof(float)))
			goto Lbl_Allocation_Failure;
	}
	DWC4100(param);
	return CBF_CONTINUE;
Lbl_Allocation_Failure:
	puts(strException);
	return CBF_TERMINATE;
Lbl_Bad_Operator:
	printf("Operator \'%c\' lost corresponding operand.\n\n", pelmt->edata.opr.name);
	return CBF_TERMINATE;
Lbl_Math_Error:
	puts("Divided by zero. The result is not a number.\n");
	return CBF_TERMINATE;
}

// Function: ConvertExecutionQueue.
// Desc:     Convert RPN expression into a binary expression tree.
// Param:    pitem: Pointer to each NODE_S structure in the list.
//           param: Pointer to a TNODE_B[2] array.
// Return:   The same value as a callback function returns.
int ConvertExecutionQueue(void * pitem, size_t param)
{
	P_ELEMENT pelmt = (P_ELEMENT) ((P_NODE_S) pitem)->pdata;
	P_BYTREE ptree = (P_BYTREE) param;
	P_TNODE_BY pnode;
	if (NULL == (pnode = strCreateNodeD(pelmt, sizeof(ELEMENT))))
	{
		treFreeBY(ptree);
		return CBF_TERMINATE;
	}
	if (*ptree == NULL)
		*ptree = pnode;
	else if (1[ptree] == NULL)
		1[ptree] = pnode;
	else
	{
		pnode->ppnode[0] = ptree[0];
		pnode->ppnode[1] = ptree[1];
		0[ptree] = pnode;
		1[ptree] = NULL;
	}
	return CBF_CONTINUE;
}

// Function: PrintPostfixExpression.
// Desc:     Print nodes in the expression tree in pre-order.
// Param:    pitem: Pointer to each TNODE_B structure in the tree.
//           param: N/A.
// Return:   The same value as a callback function returns.
int PrintPostfixExpression(void * pitem, size_t param)
{
	P_ELEMENT pelmt = (P_ELEMENT) ((P_TNODE_BY) pitem)->pdata;
	if (pelmt->bnum)
		printf("%g ", pelmt->edata.value);
	else
	{
		if ('-' == pelmt->edata.opr.name && 4 == pelmt->edata.opr.level)
			printf("_ ");
		else
			printf("%c ", pelmt->edata.opr.name);
	}
	DWC4100(param);
	return CBF_CONTINUE;
}

// Function: Scanner.
// Desc:     Split token in expression string.
// Param:    exp: Pointer to an expression string.
//           buf: Pointer to a buffer that can hold string presented numerical values.
// Return:   0 - Succeed. Not 0 - Failed.
int Scanner(char * expr, char * buf)
{
	int i, j;
	char l = 0;
	OPERATOR opr;
	BOOL ob = TRUE; /* Previous token is null or an operator. */
	char c[2] = { 0 };
	ELEMENT elmt = { 0 };
	for (j = (int) strlen(expr), i = 0; *c = *(expr + i), i <= j; ++i)
	{
		if (isdigit(0[c]) || 0[c] == '.')
		{
			strcat(buf, c);
			ob = FALSE;
		}
		else
		{
			if ('\0' != *buf)
			{
				elmt.bnum = TRUE;
				elmt.edata.value = (float) atof(buf); /* Convert string to number. */
				queInsertL(queRPNExpr, &elmt, sizeof(ELEMENT)); /* Insert number into queue. */
				*buf = '\0'; /* Reset buffer. */
			}
			if (0 != (l = GetOprByName(0[c])))
			{	/* Current character is an operator. */
				if (ob) /* Handle unary operator. */
				{
					switch (0[c])
					{
						case '+':
						case '-':
							l = 4;
					}
				}
				if (stkIsEmptyL(stkOperator))
				{
					opr.name  = 0[c];
					opr.level = l;
					if (NULL == stkPushL(stkOperator, &opr, sizeof(OPERATOR)))
					{
						puts(strException);
						return 1;
					}
				}
				else /* Operator stack is not empty. */
				{
					stkPeepL(&opr, sizeof(OPERATOR), stkOperator);
					while (opr.level >= l)
					{
						stkPopL(&opr, sizeof(OPERATOR), stkOperator);
						elmt.bnum = FALSE;
						memcpy(&elmt.edata.opr, &opr, sizeof(OPERATOR));
						queInsertL(queRPNExpr, &elmt, sizeof(ELEMENT));
						if (stkIsEmptyL(stkOperator))
							break;
						else
							stkPeepL(&opr, sizeof(OPERATOR), stkOperator);
					}
					opr.name  = 0[c];
					opr.level = l;
					if (NULL == stkPushL(stkOperator, &opr, sizeof(OPERATOR)))
					{
						puts(strException);
						return 1;
					}
				}
				ob = TRUE;
			}
			else if (0[c] == '(')
			{
				opr.name  = 0[c];
				opr.level = -1;
				if (NULL == stkPushL(stkOperator, &opr, sizeof(OPERATOR)))
				{
					puts(strException);
					return 1;
				}
				ob = TRUE;
			}
			else if (0[c] == ')')
			{
				if (! stkIsEmptyL(stkOperator))
				{
					stkPeepL(&opr, sizeof(OPERATOR), stkOperator);
					while (opr.name != '(')
					{
						stkPopL(&opr, sizeof(OPERATOR), stkOperator);
						elmt.bnum = FALSE;
						memcpy(&elmt.edata.opr, &opr, sizeof(OPERATOR));
						queInsertL(queRPNExpr, &elmt, sizeof(ELEMENT));
						if (stkIsEmptyL(stkOperator))
							break;
						else
							stkPeepL(&opr, sizeof(OPERATOR), stkOperator);
					}
					stkPopL(&opr, sizeof(OPERATOR), stkOperator); /* Drop right brace. */
				}
				else
				{
					printf("Right brace mismatch at position %d.\n\n", i + 1);
					return 1;
				}
				ob = TRUE;
			}
			else if (0[c] != ' ' && 0[c] != '\t' && 0[c] != '\0' && 0[c] != '\n' && 0[c] != '\r')
			{	/* Bypass spaces. */
				printf("Inaccessible character \'%c\' at position %d.\n\n", 0[c], i + 1);
				return 1;
			}
		}
	}
	while (! stkIsEmptyL(stkOperator))
	{
		stkPopL(&opr, sizeof(OPERATOR), stkOperator);
		if (opr.name == '(')
		{
			puts("Left brace mismatch.\n");
			return 1;
		}
		elmt.bnum = FALSE;
		memcpy(&elmt.edata.opr, &opr, sizeof(OPERATOR));
		queInsertL(queRPNExpr, &elmt, sizeof(ELEMENT));
	}
	if (queIsEmptyL(queRPNExpr))
	{
		puts("Invalid Expression.\n");
		return 1;
	}
	return 0;
}

// Function: PhoneyParser.
// Desc:     Do parsing stuff.
// Param:    mode: 0 - For Calc-Infix mode. 1 - For Conv-Postfix mode. 2 - For Conv-Suffix mode.
// Return:   N/A.
void PhoneyParser(int mode)
{
	BYTREE ptree[2] = { NULL };
	switch (mode)
	{
		case 0:
			printf("=> ");
			if (CBF_CONTINUE == strTraverseLinkedListSC_X(queRPNExpr->pfront, NULL, ComputeExecutionQueue, 0))
			{
				float f;
				stkPopL(&f, sizeof(float), stkOperand);
				if (f == -0.0f) /* Don't print -0. */
					f = (float)fabs(f);
				printf("%g\n\n", f);
			}
			break;
		case 1:
			printf("=> ");
			if (CBF_TERMINATE != strTraverseLinkedListSC_X(queRPNExpr->pfront, NULL, ConvertExecutionQueue, (size_t)ptree))
			{
				treTraverseBYPre(ptree[0], PrintPostfixExpression, 0);
				treFreeBY(ptree);
			}
			puts("\n");
			break;
		case 2:
			printf("=> ");
			strTraverseLinkedListSC_X(queRPNExpr->pfront, NULL, PrintExecutionQueue, 0);
			puts("\n");
			break;
	}
}

// Function: DoCleanup.
// Desc:     Cleanup or reset stack and queues.
// Param:    style: FALSE - Reset but not deleting. TRUE - Delete entities.
// Return:   N/A.
void DoCleanup(BOOL style)
{
	if (style)
	{	/* Destroy entities. */
		stkDeleteL(stkOperator);
		stkDeleteL(stkOperand);
		queDeleteL(queRPNExpr);
	}
	else
	{	/* Reset but don't destroy them. */
		stkFreeL(stkOperator);
		stkFreeL(stkOperand);
		queFreeL(queRPNExpr);
	}
}

static const char * tips[2] = { "Stack", "Queue" };
static const char * mode[4] = { "Calc-Infix", "Conv-Postfix", "Conv-Suffix" };

// Program entry point.
int main(void)
{
	int err = 0, m = 0;
	/* String buffer used to store expression. */
	char expr[BUF_SIZ] = { 0 }, buf[BUF_SIZ] = { 0 };
	PrintInfo(0);
	/* Create a stack that used to store operators. */
	if (NULL == (stkOperator = stkCreateL()))
	{ /* Can not initialize stack. */
		err = 0;
		goto Lbl_Error;
	}
	if (NULL == (stkOperand = stkCreateL()))
	{ /* Can not initialize stack. */
		err = 0;
		goto Lbl_Error;
	}
	if (NULL == (queRPNExpr = queCreateL()))
	{ /* Can not initialize queue. */
		err = 1;
		goto Lbl_Error;
	}

	for ( ;; )
	{
	Lbl_Continue:
		printf("%s> ", mode[m]);
		fgets(expr, BUF_SIZ, stdin);

		switch (*expr)
		{
			case 'h':
			case '?':
				PrintInfo(1);
				break;
			case 'm':
				for ( ;; )
				{
					printf("\tc: Calculate infix expression.\n \
\tp: Convert infix expression to postfix(Polish Notation) style. \n \
\ts: Convert infix expression to suffix(Reversed Polish Notation) style. \n \
\tq: Quit mode setting.\n\tc/p/s/q?>");
					fgets(expr, BUF_SIZ, stdin);
					printf("\n");
					switch (*expr)
					{
						case 'c': m = 0; goto Lbl_Continue;
						case 'p': m = 1; goto Lbl_Continue;
						case 's': m = 2; goto Lbl_Continue;
						case 'q': goto Lbl_Continue;
					}
				}
				break;
			case 'q':
				goto Lbl_Quit;
			default:
				if (0 == Scanner(expr, buf))
					PhoneyParser(m);
		}
		DoCleanup(FALSE);
	}
Lbl_Quit:
	DoCleanup(TRUE);
	return 0;
Lbl_Error:
	DoCleanup(TRUE);
	printf("%s initializing failure.\n", tips[err]);
	return 1;
}
