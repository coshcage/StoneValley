#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "../src/svgraph.h"
char fun(char v)
{
	switch (v) {
	case 'h': v = (char)__LINE__;
		break;
	case 'J': v = (char)__LINE__;
		break;
		/*
		 */
	case 'n': v = (char)__LINE__; break;
	case 'o': v = (char)__LINE__;
		break;
	default: return 0;
	}
	return v;
}
BITMAT bitmat; struct dot { int ln; int col; }; struct node {size_t vidx; struct dot info; }; struct node table[10];
int cbftvs (void * pitem, size_t param)  {
	if (!strSetBitBMap(&bitmat, table[((P_VERTEX_L)pitem)->vid - 1].info.ln  - 1,
	table[((P_VERTEX_L)pitem)->vid - 1].info.col - 1, TRUE)) return CBF_TERMINATE;
	DWC4100(param); return CBF_CONTINUE; }
int main(void)
{
	jmp_buf env;
	GRAPH_L grph;
	int val, i, j;
	char a[7], b[18], c[60], * pa = a;
	/* Error handling. */
	if (0 != (val = setjmp(env))) fprintf(stderr, "Error %d.", val), exit(val);
	grpInitL(&grph);
	if (NULL == strInitBMap(&bitmat, 4, 7, FALSE)) longjmp(env, 101); /* Can not initialize bit-matrix. */
	for (i = 1; i <= 10; ++i) if (TRUE != grpInsertVertexL(&grph, table[i - 1].vidx = i)) longjmp(env, 102); /* Vertex insertion failure. */
	table[0].info.ln = 4; table[0].info.col = 4; table[1].info.ln = 3; table[1].info.col = 2;
	table[2].info.ln = 2; table[2].info.col = 1; table[3].info.ln = 1; table[3].info.col = 2;
	table[4].info.ln = 1; table[4].info.col = 3; table[5].info.ln = 2; table[5].info.col = 4;
	table[6].info.ln = 3; table[6].info.col = 6; table[7].info.ln = 2; table[7].info.col = 7;
	table[8].info.ln = 1; table[8].info.col = 6; table[9].info.ln = 1; table[9].info.col = 5;
	if (TRUE != grpInsertEdgeL(&grph, table[0].vidx, table[6].vidx, 0)) longjmp(env, 103);
	if (TRUE != grpInsertEdgeL(&grph, table[6].vidx, table[7].vidx, 0)) longjmp(env, 104);
	if (TRUE != grpInsertEdgeL(&grph, table[7].vidx, table[8].vidx, 0)) longjmp(env, 105);
	if (TRUE != grpInsertEdgeL(&grph, table[8].vidx, table[9].vidx, 0)) longjmp(env, 106);
	if (TRUE != grpInsertEdgeL(&grph, table[9].vidx, table[5].vidx, 0)) longjmp(env, 107);
	if (TRUE != grpInsertEdgeL(&grph, table[0].vidx, table[1].vidx, 0)) longjmp(env, 108);
	if (TRUE != grpInsertEdgeL(&grph, table[1].vidx, table[2].vidx, 0)) longjmp(env, 109);
	if (TRUE != grpInsertEdgeL(&grph, table[2].vidx, table[3].vidx, 0)) longjmp(env, 110);
	if (TRUE != grpInsertEdgeL(&grph, table[3].vidx, table[4].vidx, 0)) longjmp(env, 111);
	if (TRUE != grpInsertEdgeL(&grph, table[4].vidx, table[5].vidx, 0)) longjmp(env, 112);
	if (CBF_CONTINUE != grpDFSL(&grph, 1, cbftvs, 0)) longjmp(env, 113); /* Broken heart error. */
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 7; ++j) {
			if (strGetBitBMap(&bitmat, i, j)) printf("%c", '.'); else printf("%c", ' ');
		}
		printf("%c", '\n');
	}
	*pa++ = fun('o') + fun('h') - fun('n');
	*pa++ = fun('n') + fun('h') - fun('J');
	*pa++ = fun('o') + fun('n') - fun('n');
	*pa++ = fun('h') + fun('J');
	*pa++ = fun('o') - fun('J');
	*pa++ = fun('n') + fun('o') - fun('J');
	*pa = fun('o') - fun('n');
	b[0] = a[5] + a[4] + a[1] + a[3] + a[1] + a[3];
	b[1] = (a[5] - a[0]) * (a[5] - a[0]) + a[2] + a[5] - a[3];
	b[2] = (a[5] - a[0]) * (a[5] - a[0]) + a[4] - a[5] + a[3];
	b[3] = (a[5] - a[0]) * (a[5] - a[0]) + a[5] - a[3];
	b[4] = a[3] * a[4] + a[3] - a[4] - a[4];
	b[5] = a[5] * a[4] + a[5] - a[0];
	b[6] = a[1] * a[0] - a[4];
	b[7] = (a[2] + a[0]) * a[4] - a[5] + a[3];
	b[8] = (a[5] - a[0]) * (a[5] - a[0]) - a[3] + a[2];
	b[9] = a[1] * a[2] - a[1] * a[4] + a[5] - a[3];
	b[10]= a[2] * a[4] + a[2] + a[2] + a[4] + a[4];
	b[11]= (a[5] - a[0]) * (a[5] - a[0]) + a[0];
	b[12]= a[1] + (a[5] - a[0]) * (a[5] - a[0]);
	b[13]= a[1] * a[0];
	b[14]= a[0] * (a[1] - a[5] + a[3]);
	b[15]= (a[2] + a[4]) * a[4];
	b[16]= a[1] + a[2] + a[3] * a[4];
	b[17]= (a[5] - a[0]) * (a[5] - a[0]) + a[1] - a[6];
	c[0] = b[0]; c[1] = b[2]; c[2] = b[3]; c[3] = b[1]- b[0];
	c[4] = b[4]; c[5] = b[5]; c[6] = b[6]; c[7] = c[3];
	c[8] = b[7]; c[9] = b[5]; c[10]= b[15];c[11]= b[3];
	c[12]= c[3]; c[13]= b[7]; c[14]= b[8]; c[15]= b[9];
	c[16]= c[12];c[17]= b[5]; c[18]= b[10];c[19]= c[3];
	c[20]= b[1]; c[21]= b[2]; c[22]= b[3]; c[23]= c[12];
	c[24]= b[10];c[25]= b[5]; c[26]= b[11];c[27]= b[12];
	c[28]= b[13];c[29]= b[3]; c[30]= c[23];c[31]= b[7];
	c[32]= b[8]; c[33]= b[9]; c[34]= b[9] -b[17]; c[35]=b[4];
	c[36]= b[16];c[37]= b[1]; c[38]= c[30];c[39]= b[1];
	c[40]= b[2]; c[41]= b[5]; c[42]= b[10];c[43]= b[1] -b[0];
	c[44]= b[5]; c[45]= b[10];c[46]= c[19];c[47]= b[14];
	c[48]= b[17];c[49]= b[11];c[50]= b[12];c[51]= b[13];
	c[52]= b[5]; c[53]= b[14];c[54]= b[8]; c[55]= b[1];
	c[56]= b[3]; c[57]= b[15];c[58]= 0; puts(c);
	grpFreeL(&grph); strFreeBMap(&bitmat);
	return 0;
}
//
//  exp_2018-09-25_1.c
//  This file might show the usage of graphs and bit-matrices.
//  Created by cyycoish#hotmail.com on 09/25/18.
//  Licence: Public domain.
//  Platform:Cross Platform.
//
