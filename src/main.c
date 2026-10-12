#include <stdio.h>
#include <string.h>
#include "svstring.h"

typedef float MYTYPE;

void PrintMatrix(P_MATRIX p) {
    size_t i, j;
    for (i = 0; i < p->ln; ++i) {
        for (j = 0; j < p->col; ++j)
            printf("%3.3g ", *(MYTYPE *)strFetchValuePointerMatrix(p, i, j, sizeof(MYTYPE)));
        printf("\n");
    }
    printf("\n");
}

void PrintMatrixST(P_MATRIX p)
{
	size_t i, j;
	for (i = 0; i < p->ln; ++i)
	{
		for (j = 0; j < p->col; ++j)
		{
			printf("%zu ", *(size_t *)strFetchValuePointerMatrix(p, i, j, sizeof(size_t)));
		}
		printf("\n");
	}
	printf("\n");
}

int cbfadd(const void * px, const void * py) {
*(MYTYPE *)px += *(MYTYPE *)py;
return CBF_CONTINUE;
}
int cbfmul(const void * px, const void * py) {
*(MYTYPE *)px *= *(MYTYPE *)py;
return CBF_CONTINUE;
}
int cbfsub(const void * px, const void * py) {
*(MYTYPE *)px -= *(MYTYPE *)py;
return CBF_CONTINUE;
}
int cbfdiv(const void * px, const void * py) {
*(MYTYPE *)px /= *(MYTYPE *)py;
return CBF_CONTINUE;
}
int cbfcmp(const void * px, const void * py) {
if (*(MYTYPE *)px > *(MYTYPE *)py) return CBF_CMP_GT;
else if (*(MYTYPE *)px < *(MYTYPE *)py) return CBF_CMP_LT;
return CBF_CMP_EQUAL;
}
int main() {
int r = 0;
MYTYPE nbigm = -999999, nil = 0, idt = 1;
// MYTYPE d[] = {9,4,1,0,0,360,4,5,0,1,0,200,3,10,0,0,1,300,7,12,0,0,0,0};
MYTYPE d[] = {5,1,1,8,1,0,10,2,4,3,2,0,1,10,5,3,2,4,nbigm,nbigm,0};
//MYTYPE d[] = {-1,1,1,0,1,-1,2,0,1,4,2,2,0,0,0};
//MYTYPE d[] = {3,5,1,0,15,5,2,0,1,10,2.5,1,0,0,0};
CBF_ALGEBRA pcbfagb[5] = {cbfadd, cbfmul, cbfsub, cbfdiv, cbfcmp};
// P_MATRIX p = strCreateMatrix(4, 6, sizeof(MYTYPE));
P_MATRIX p = strCreateMatrix(3, 7, sizeof(MYTYPE)), pr;
memcpy(p->arrz.pdata, d, sizeof(d));
PrintMatrix(p);
pr = strStandardMaxSimplexMatrix(p, &nil, &idt, &nbigm, sizeof(MYTYPE), pcbfagb, &r);
printf("r=%d\n", r);
if (NULL != pr) {
PrintMatrix(pr);
strDeleteMatrix(pr);
}
strDeleteMatrix(p);
return 0;
}

