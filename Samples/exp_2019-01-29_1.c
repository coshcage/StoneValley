//
//  exp_2019-01-29_1.c
//  An example of permutation and combination.
//  Created by cosh.cage#hotmail.com on 01/29/19.
//  Licence:  Public domain.
//  Platform: Windows NT.
//
#include <stdio.h>
#include <string.h>
#include "..\\src\\svstring.h"

#define N_ 4
#define R_ 3
#define strize(a) # a
#define numerize(s) strize(s)
#define paste(a, b, c) a ## b ## c

typedef char MYTYPE;

// Function: cbfcmp
// Desc:     Compare values for MYTYPE values.
// Param:    px: a pointer to a MYTYPE value. py: a pointer to another MYTYPE value.
// Return:   Either 1, -1 or 0 depends on comparison result.
int cbfcmp(const void * px, const void * py)
{
	if (*(MYTYPE *)px > *(MYTYPE *)py) return  1;
	if (*(MYTYPE *)px < *(MYTYPE *)py) return -1;
	return 0;
}

// Function: PrintArrayZ
// Desc:     Print data for an sized array from index 0 to (n - 1).
// Param:    parrz:  pointer to a sized array.
//           n:      Number of items you want to print.
//           size:   Size of each element in the array.
//           bchar:  TRUE: print char; FALSE: print number.
// Return:   N/A.
void PrintArrayZ(P_ARRAY_Z parrz, size_t size, BOOL bchar)
{
	char c;
	size_t i;
	/* Hide a private member by using a macro or a function is not elegant,
	 * Therefore we cannot use classes in C, this is a better way to circumvent
	 * altering parrz->num, so that strLevelArrayZ might not be an appendix.
	 */
	for (i = 0; i < strLevelArrayZ(parrz); ++i)
		printf("%c", (c = *(MYTYPE *)(parrz->pdata + i * size), bchar ? c : c - '0'));
	printf("\n");
}

// Function: main
// Desc:     Program entry.
// Param:    N/A.
// Return:   0: no error; 1, 2: allocation failure.
int main(void)
{
	char q;
	int i = 0;
	ARRAY_Z n, r;
	char pstr[] = "abcd", t;
	/* Initialize two arrays. */
	if (NULL == strInitArrayZ(&n, N_, sizeof(MYTYPE)))
		return 1; /* Allocation failure. */
	if (NULL == strInitArrayZ(&r, R_, sizeof(MYTYPE)))
	{	/* Another alocation failure. */
		i = 2;
		goto Lbl_Bad_Allocation;
	}
	/* Initialize two arrays. */
	memcpy(n.pdata, pstr, N_ * sizeof(MYTYPE));
	memcpy(r.pdata, pstr, R_ * sizeof(MYTYPE));
	do
		fflush(stdin), printf("Would you like to print the result numerically(Y/n)? "), scanf("%c", &q);
	while (q != 'Y' && q != 'y' && q != 'N' && q != 'n');
	q = !(q & 1);
	printf("P(%d, %d) =\n", N_, R_);
	do
	{	/* Generate all permutations of the current subset for combination. */
		while
		(	/* Some versions of GCCs would mis-parse the following sentence while VC won't. */
			printf(paste("%", numerize(N_), "d:\t"), ++i),
			PrintArrayZ(&r, sizeof(MYTYPE), (BOOL)q),
			strPermuteArrayZ(&r, &t, sizeof(MYTYPE), cbfcmp, TRUE)
		);
	}	/* Generate (nCr) circularly. */
	while (strCombineNextArrayZ(&r, &n, sizeof(MYTYPE), cbfcmp));
	i = 0;
	strFreeArrayZ(&r);
Lbl_Bad_Allocation:
	strFreeArrayZ(&n);
	return i;
}
