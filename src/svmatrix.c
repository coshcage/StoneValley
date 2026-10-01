/*
 * Name:        svmatrix.c
 * Description: Matrices.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0213191430N1001261123L01317
 * License:     LGPLv3
 * Copyright (C) 2019-2026 John Cage
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

#include <stdlib.h> /* Use function malloc, free. */
#include <string.h> /* Use function memcpy, memset, memmove, memcmp. */
#include "svstring.h"

/* Function name: strInitMatrix
 * Description:   Initialize a matrix.
 * Parameters:
 *       pbmx Pointer to a matrix you want to initialize.
 *         ln Number of lines in the matrix.
 *        col Number of columns in the matrix.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to the memory buffer of a matrix.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void * strInitMatrix(P_MATRIX pmtx, size_t ln, size_t col, size_t size)
{
	if (NULL == strInitArrayZ(&pmtx->arrz, ln * col, size))
	{
		pmtx->ln = pmtx->col = 0;
		return NULL; /* Allocation failure. */
	}
	pmtx->ln  = ln;
	pmtx->col = col;
	return pmtx->arrz.pdata;
}

/* Function name: strFreeMatrix_O
 * Description:   Retract a matrix in main memory that is allocated by function strInitMatrix.
 * Parameter:
 *      pmtx Pointer to a matrix you want to release.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeMatrix_M is available.
 */
void strFreeMatrix_O(P_MATRIX pmtx)
{
	strFreeArrayZ(&pmtx->arrz);
	pmtx->ln = pmtx->col = 0;
}

/* Function name: strCreateMatrix
 * Description:   Create a matrix.
 * Parameters:
 *         ln Number of lines in a matrix.
 *        col Number of columns in a matrix.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to a new created matrix.
 */
P_MATRIX strCreateMatrix(size_t ln, size_t col, size_t size)
{
	REGISTER P_MATRIX pmtx = (P_MATRIX) malloc(sizeof(MATRIX));
	if (NULL != pmtx)
	{
		if (NULL == strInitMatrix(pmtx, ln, col, size))
		{
			free(pmtx);
			pmtx = NULL; /* Allocation failure. */
		}
	}
	return pmtx;
}

/* Function name: strDeleteMatrix_O
 * Description:   Delete a matrix which is allocated by function strCreateMatrix.
 * Parameter:
 *      pmtx Pointer to a matrix you want to defuse.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteMatrix_M is available.
 */
void strDeleteMatrix_O(P_MATRIX pmtx)
{
	strFreeMatrix(pmtx);
	free(pmtx);
}

/* Function name: strCopyMatrix
 * Description:   Copy a matrix from source to destination.
 * Parameters:
 *      pdest Pointer to the destination matrix whose content is to be copied.
 *       psrc Pointer to the source of the matrix to copy from.
 *       size Size of each element in both two matrices.
 * Return value:  pdest->arrz.pdata
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       After calling, the size of pdest->arrz equaled to the size of psrc->arrz.
 *                Address of pdest and psrc Must Be Allocated first.
 *                (*) The buffer of destination and source shall not overlap.
 */
void * strCopyMatrix(P_MATRIX pdest, P_MATRIX psrc, size_t size)
{
	if (strLevelArrayZ(&pdest->arrz) == strLevelArrayZ(&psrc->arrz)) /* Two matrices have length equivalent buffers respectively. */
	{
		pdest->ln  = psrc->ln;
		pdest->col = psrc->col;
		
		if (pdest->arrz.pdata != psrc->arrz.pdata)
			return strMoveArrayZ(&pdest->arrz, &psrc->arrz, size); /* In case buffers overlapped together. */
		else
			return (void *)pdest->arrz.pdata; /* Two matrices share a same buffer. */
	}
	else
	{
		if (pdest->arrz.pdata != psrc->arrz.pdata)
		{
			if
			(
				NULL !=
				(	/* Destination has not been initialized yet. */
					0 == strLevelArrayZ(&pdest->arrz) && NULL == pdest->arrz.pdata ?
					strInitArrayZ  (&pdest->arrz, strLevelArrayZ(&psrc->arrz), size) :
					strResizeArrayZ(&pdest->arrz, strLevelArrayZ(&psrc->arrz), size)
				)
			)
			{
				pdest->ln  = psrc->ln;
				pdest->col = psrc->col;
				
				return strMoveArrayZ(&pdest->arrz, &psrc->arrz, size); /* In case buffers overlapped together. */
			}
			/* Resize failed. The original buffer of pdest will be kept. */
		}
		else /* Two matrices share a same buffer with different sizes of element. */
			return (void *)pdest->arrz.pdata; /* Leave this circumstance alone. */
	}
	return NULL;
}

/* Function name: strCreateCopyMatrix
 * Description:   Make a copy of matrix from a template source.
 * Parameters:
 *       psrc Pointer to the template source matrix to copy from.
 *       size Size of each element in the matrix.
 * Return value:  A pointer to the new copied matrix.
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       Address of psrc Must Be Allocated first.
 */
P_MATRIX strCreateCopyMatrix(P_MATRIX psrc, size_t size)
{
	REGISTER P_MATRIX prtn = strCreateMatrix(psrc->ln, psrc->col, size);
	if (NULL != prtn)
		strCopyArrayZ(&prtn->arrz, &psrc->arrz, size);
	return prtn;
}

/* Function name: strResizeMatrix
 * Description:   Resize a matrix.
 * Parameters:
 *       pbmx Pointer to a matrix you want to resize.
 *         ln New number of rows of the matrix.
 *        col New number of columns of the matrix.
 *       size Size of each element in the matrix.
 * Return value:  pmtx->arrz.pdata
 * Caution:       (*) Function altered row and column number after calling.
 *                If the new column were shorter than the elder one, lines in the matrix would be truncated.
 *                If the new column were larger than the elder one, lines in the matrix would be appended to new position,
 *                (*) and the rest of the data in the matrix would be invalid random values, thus(of which '#' represents random value):
 *                A(2*3)= | 1 2 3 | => B(3*4)= | 1 2 3 # |
 *                |       | 4 5 6 |            | 4 5 6 # |
 *                +====> C(2*2)=| 1 2 |        | # # # # |
 *                              | 4 5 | => D(1,1)= | 1 |
 *                Address of pmtx Must Be Allocated first.
 */
void * strResizeMatrix(P_MATRIX pmtx, size_t ln, size_t col, size_t size)
{
	REGISTER size_t i, j;
	const size_t ol = pmtx->ln;
	const size_t oc = pmtx->col;
	const size_t k  = size * col;
	if (ln * col > ol * oc) /* Matrix becomes bigger. */
	{
		if (NULL != strResizeArrayZ(&pmtx->arrz, ln * col, size))
		{
			for (i = ol; i > 1; --i)
			{
				j = (i - 1) * size;
				memmove(&pmtx->arrz.pdata[j * col], &pmtx->arrz.pdata[j * oc], k);
			}
			pmtx->ln  = ln;
			pmtx->col = col;
			return pmtx->arrz.pdata;
		}
		pmtx->ln = pmtx->col = 0;
		return NULL;
	}
	else if (ln * col < ol * oc) /* Matrix becomes smaller. */
	{
		for (i = 2; i <= ol; ++i)
		{
			j = (i - 1) * size;
			memmove(&pmtx->arrz.pdata[j * col], &pmtx->arrz.pdata[j * oc], k);
		}
		if (NULL != strResizeArrayZ(&pmtx->arrz, ln * col, size))
		{
			pmtx->ln  = ln;
			pmtx->col = col;
			return pmtx->arrz.pdata;
		}
		pmtx->ln = pmtx->col = 0;
		return NULL;
	}
	pmtx->ln  = ln;
	pmtx->col = col;
	return pmtx->arrz.pdata;
}

/* Function name: strSetMatrix_O
 * Description:   Fill up a matrix with a specific value.
 * Parameters:
 *       pmtx Pointer to a matrix you want to fill.
 *       pval Pointer to the value you want to set into a matrix.
 *       size Size of each element.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           A macro version of this function named strSetMatrix_M is available.
 */
void strSetMatrix_O(P_MATRIX pmtx, const void * pval, size_t size)
{
	strSetArrayZ(&pmtx->arrz, pval, size);
}

/* Function name: strFetchValuePointerMatrix_O
 * Description:   Return a pointer to the value in matrix by specified line and column number.
 * Parameters:
 *       pmtx Pointer to a matrix you want to operate with.
 *         ln Number of line in the matrix. Line number starts from 0.
 *        col Number of column in the matrix. Column number starts from 0.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to the value on the specific position in matrix.
 *                If function returned NULL, it would indicate that parameter ln or col might be out of range.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           A macro version of this function named strFetchValuePointerMatrix_M is available.
 */
void * strFetchValuePointerMatrix_O(P_MATRIX pmtx, size_t ln, size_t col, size_t size)
{
	if (SV_ASSERT(ln < pmtx->ln && col < pmtx->col && 0 != size))
		return &pmtx->arrz.pdata[(ln * pmtx->col + col) * size];
	return NULL;
}

/* Function name: strGetValueMatrix
 * Description:   Return the value and its pointer from the specific position in a matrix.
 * Parameters:
 *       pval Pointer to a buffer to store the value you have got.
 *            If pval equaled NULL, this function would not put value into buffer rather than return its pointer.
 *       pmtx Pointer to a matrix you want to operate with.
 *         ln Number of line in the matrix. Line number starts from 0.
 *        col Number of column in the matrix. Column number starts from 0.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to the value on the specific position in matrix.
 *                If function returned NULL, it would indicate that parameter ln or col might be out of range.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void * strGetValueMatrix(void * pval, P_MATRIX pmtx, size_t ln, size_t col, size_t size)
{
	if (SV_ASSERT(ln < pmtx->ln && col < pmtx->col && 0 != size))
	{
		REGISTER void * ptr = strFetchValuePointerMatrix(pmtx, ln, col, size);
		if (NULL != pval)
			memcpy(pval, ptr, size);
		return ptr;
	}
	return NULL;
}

/* Function name: strSetValueMatrix_O
 * Description:   Set value for a matrix onto a specific position.
 * Parameters:
 *       pmtx Pointer to a matrix you want to operate.
 *         ln Number of line in the matrix. Line number starts from 0.
 *        col Number of column in the matrix. Column number starts from 0.
 *       pval Pointer to the new value you want to set into the matrix.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to the value in the matrix you have set.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           A macro version of this function named strSetValueMatrix_M is available.
 */
void * strSetValueMatrix_O(P_MATRIX pmtx, size_t ln, size_t col, const void * pval, size_t size)
{
	if (SV_ASSERT(ln < pmtx->ln && col < pmtx->col && NULL != pval && 0 != size))
		return memcpy(&pmtx->arrz.pdata[(ln * pmtx->col + col) * size], pval, size);
	return NULL;
}

/* Function name: strTransposeMatrix
 * Description:   Transpose a matrix that is to swap line and column index for each element in a matrix.
 * Parameters:
 *       pmtx Pointer to a matrix.
 *       size Size of each element in the matrix.
 *     cbfmch Pointer to a callback function that uses to match each element in the matrix.
 *            Two parameters of cbfmch may point to any element in the matrix.
 *            This function returns CBF_CMP_EQUAL when data match or a non zero value when data mismatch.
 *            Please refer to svdef.h to see more details about type CBF_COMPARE.
 * Return value:  pmtx->arrz.pdata
 *                If this function returned value NULL, it would indicate an allocation failure.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void * strTransposeMatrix(P_MATRIX pmtx, size_t size, CBF_COMPARE cbfmch)
{
	MATRIX mtxt = { 0 };
	if (NULL != strCopyMatrix(&mtxt, pmtx, size))
	{
		REGISTER size_t i, j, m, n;
		REGISTER void * pa, * pb;
		size_t t;
		for (i = 0; i < mtxt.ln; ++i)
		{
			m = i * size;
			for (j = 0; j < mtxt.col; ++j)
			{
				n = j * size;
				pa = &mtxt.arrz.pdata[mtxt.col * m + n];
				pb = &pmtx->arrz.pdata[mtxt.ln * n + m];
				if (CBF_CMP_EQUAL != cbfmch(pb, pa))
					memcpy(pb, pa, size);
			}
		}
		svSwap(&pmtx->ln, &t, &pmtx->col, sizeof(size_t));
		strFreeMatrix(&mtxt);
		return pmtx->arrz.pdata;
	}
	return NULL;
}

/* Function name: strProjectMatrix
 * Description:   Project a source matrix into the specific position on the destination matrix.
 * Parameters:
 *      pdest Pointer to the destination matrix that contains the projection.
 *        dln Number of line for the item on the left up corner of the destination matrix.
 *       dcol Number of column for the item on the left up corner of the destination matrix.
 *       psrc Pointer to the source matrix that you want to project a part of it onto the destination.
 *        sln Number of line for the item on the left up corner of the source matrix.
 *       scol Number of column for the item on the left up corner of the source matrix.
 *       size Size of each element in both two matrices.
 * Return value:  true  Projection succeeded.
 *                false Projection failed.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                All line number dln, sln and column number dcol, scol start from 0.
 * Tip:           Assume that we have two matrices A and B, then a projection can be the following situation.
 *                strProjectMatrix(&A, 1, 1, &B, 1, 1);
 *                Before projection:        : After projection:
 *                A=| a b c d | B=| q r s | : A=| a b c d | B=| q r s |
 *                  | e f g h |   | t u v | :   | e u.v.h |   | t u v |
 *                  | i j k l |   | w x y | :   | i x.y.l |   | w x y |
 *                  | m n o p |             :   | m n o p |
 */
bool strProjectMatrix(P_MATRIX pdest, size_t dln, size_t dcol, P_MATRIX psrc, size_t sln, size_t scol, size_t size)
{
	if (SV_ASSERT(dln < pdest->ln && dcol < pdest->col && sln < psrc->ln && scol < psrc->col))
	{
		REGISTER size_t i, j, k, l, m, n;
		
		i = psrc->ln - sln;
		j = pdest->ln - dln;
		m = i < j ? i : j;
		
		i = psrc->col - scol;
		j = pdest->col - dcol;
		n = i < j ? i : j;
		
		for (i = 0; i < m; ++i)
		{
			k = dln + (i * pdest->col) + dcol;
			l = sln + (i * psrc->col) + scol;
			
			for (j = 0; j < n; ++j)
			{
				memmove
				(
					&pdest->arrz.pdata[(k + j) * size],
					&psrc->arrz.pdata[(l + j) * size],
					size
				);
			}
		}
		return true;
	}
	return false;
}

/* Function name: strMathMatrix
 * Description:   Do calculation on each element in a matrix with the value that pval pointed.
 * Parameters:
 *       pmtx Pointer to a matrix.
 *       pval Pointer to the value that you want to operate onto the matrix.
 *       size Size of each element in the matrix.
 *     cbfagb Pointer to a callback function that uses to handle calculations on elements.
 *            The left pointer of cbfagb points to any element in the matrix,
 *            and the right pointer of cbfagb always holds a same value as pval.
 *            Please refer to the definition of type CBF_ALGEBRA.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return depended on function cbfagb.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           Users could use this function to multiply a number with a matrix like this way:
 *                int mul(const void * pa, const void * pb) { *(float *)pa *= *(float *)pb; return CBF_CONTINUE; }
 *                float f = 2.0f; strMathMatrix(pmtx, &f, sizeof(float), mul);
 */
int strMathMatrix(P_MATRIX pmtx, const void * pval, size_t size, CBF_ALGEBRA cbfagb)
{
	REGISTER size_t i, j;
	for (i = 0, j = pmtx->ln * pmtx->col * size; i < j; i += size)
		if (CBF_CONTINUE != cbfagb(&pmtx->arrz.pdata[i], pval))
			return CBF_TERMINATE;
	return CBF_CONTINUE;
}

/* Function name: strMatrixMatrix
 * Description:   Do calculation between two matrices,
 *                and store the result into the matrix that pmtxa pointed.
 * Parameters:
 *      pmtxa Pointer to a matrix.
 *      pmtxb Pointer to another matrix.
 *       size Size of each element in the matrix.
 *     cbfagb Pointer to a callback function that uses to handle calculations on elements.
 *            The left pointer of cbfagb points to any element in pmtxa,
 *            and the right pointer of cbfagb points to the corresponding value in pmtxb.
 *            Please refer to the definition of type CBF_ALGEBRA.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return depended on function cbfagb.
 * Caution:       Address of pmtxa and pmtxb Have to Be Allocated first.
 * Tip:           Users could use this function to add a matrix with another like this way:
 *                int plus(const void * pa, const void * pb) { *(float *)pa += *(float *)pb; return CBF_CONTINUE; }
 *                strMatrixMatrix(pmtxa, pmtxb, sizeof(float), plus);
 */
int strMatrixMatrix(P_MATRIX pmtxa, P_MATRIX pmtxb, size_t size, CBF_ALGEBRA cbfagb)
{
	if (SV_ASSERT(pmtxa->ln == pmtxb->ln && pmtxa->col == pmtxb->col))
	{
		REGISTER size_t i, j;
		for (i = 0, j = pmtxa->ln * pmtxa->col * size; i < j; i += size)
			if (CBF_CONTINUE != cbfagb(&pmtxa->arrz.pdata[i], &pmtxb->arrz.pdata[i]))
				return CBF_TERMINATE;
		return CBF_CONTINUE;
	}
	return CBF_TERMINATE;
}

/* An enumeration describes index of matrices during multiplication. */
typedef enum _en_MatrixMultiplication { _EMM_C, _EMM_A, _EMM_B } _MatrixMultiplication;

/* Macros used to fetch line number and column number and data pointers of matrices. */
#define MAT_LN(index)   ((const size_t)ppmtx[index]->ln)
#define MAT_COL(index)  ((const size_t)ppmtx[index]->col)
#define MAT_DATA(index) (ppmtx[index]->arrz.pdata)

/* Function name: strMultiplyMatrix
 * Description:   Do multiplication between two matrices A and B, and store the result into matrix C.
 *                Thus, C := A * B. Notice that A * B != B * A.
 * Parameters:
 *   ppmtx[3] ppmtx[0] stores a pointer to matrix C.
 *            (*) Lines of matrix C shall equal to lines of matrix A,
 *                and columns of matrix C shall equal to columns of matrix B, thus:
 *                ppmtx[0] = strCreateMatrix(ppmtx[1]->ln, ppmtx[2]->col, size);
 *            ppmtx[1] stores a pointer to matrix A.
 *            ppmtx[2] stores a pointer to matrix B.
 *      ptemp Pointer to a buffer that can hold an element.
 *            Size of the buffer that ptemp pointed shall equal to parameter size.
 *       size Size of each element in the matrix.
 * pcbfagb[2] pcbfagb[EMA_ADD] stores the pointer to a function that can perform addition.
 *            pcbfagb[EMA_MUL] stores the pointer to a function that can perform multiplication.
 *            Please refer to the definition of type CBF_ALGEBRA and enumeration MatrixAlgebra.
 * Return value:  true indicates calculation succeeded. false indicates calculation failed.
 * Caution:       Address of ppmtx[0], ppmtx[1] and ppmtx[2] Must Be Allocated first.
 * Tip:           Users could use this function to multiply a matrix with another like this way:
 *                <test.c>
 *                #include <string.h>
 *                #include "svstring.h"
 *                void Print(P_MATRIX p) {
 *                    size_t i, j;
 *                    for (i = 0; i < p->ln; ++i) {
 *                        for (j = 0; j < p->col; ++j)
 *                            printf("%0.0f ", *(float *)strGetValueMatrix(NULL, p, i, j, sizeof(float)));
 *                        printf("\n");
 *                    }
 *                }
 *                int add(const void * pa, const void * pb) { *(float *)pa += *(float *)pb; return CBF_CONTINUE; }
 *                int mul(const void * pa, const void * pb) { *(float *)pa *= *(float *)pb; return CBF_CONTINUE; }
 *                int main() {
 *                    MATRIX mc, ma, mb;
 *                    float a[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f }, tmp = 0.0f;
 *                    float b[] = { 3.0f, 2.0f, 1.0f, 6.0f, 5.0f, 4.0f };
 *                    CBF_ALGEBRA alg[2]; P_MATRIX pm[3];
 *                    alg[EMA_ADD] = add; alg[EMA_MUL] = mul;
 *                    pm[0] = &mc; pm[1] = &ma; pm[2] = &mb;
 *                    strInitMatrix(&ma, 2, 3, sizeof(float));
 *                    strInitMatrix(&mb, 3, 2, sizeof(float));
 *                    strInitMatrix(&mc, 2, 2, sizeof(float));
 *                    strSetMatrix(&mc, &tmp, sizeof(float));
 *                    memcpy(ma.arrz.pdata, a, sizeof a);
 *                    memcpy(mb.arrz.pdata, b, sizeof b);
 *                    strMultiplyMatrix(pm, &tmp, sizeof(float), alg); Print(&mc);
 *                    strFreeMatrix(&mc); strFreeMatrix(&ma); strFreeMatrix(&mb);
 *                    return 0;
 *                }
 *                Result and explanation:
 *                | 1 2 3 |   | 3 2 |   | 20 26 |
 *                | 4 5 6 | * | 1 6 | = | 47 62 |
 *                            | 5 4 |
 *                  __      _n_
 *                 /  \     \  |
 *                |      ==  >   a  b
 *                 \__/ij   /__|  ik kj
 *                          k:=1
 */
bool strMultiplyMatrix(P_MATRIX ppmtx[3], void * ptemp, size_t size, CBF_ALGEBRA pcbfagb[2])
{
	if (SV_ASSERT(MAT_COL(_EMM_A) == MAT_LN(_EMM_B)))
	{
		REGISTER size_t i, j, k, m;
		REGISTER PUCHAR ptrmc = MAT_DATA(_EMM_C);
		for (i = 0; i < MAT_LN(_EMM_A); ++i)
		{
			m = i * MAT_COL(_EMM_A);
			for (j = 0; j < MAT_COL(_EMM_B); ++j)
			{
				for (k = 0; k < MAT_COL(_EMM_A); ++k)
				{
					memcpy(ptemp, &MAT_DATA(_EMM_A)[(m + k) * size], size);
					pcbfagb[EMA_MUL](ptemp, &MAT_DATA(_EMM_B)[(k * MAT_COL(_EMM_B) + j) * size]);
					if (0 == k)
						memcpy(ptrmc, ptemp, size);
					else
						pcbfagb[EMA_ADD](ptrmc, ptemp);
				}
				ptrmc += size;
			}
		}
		return true;
	}
	return false;
}

#undef MAT_LN
#undef MAT_COL
#undef MAT_DATA /* Undefine used macros. */

/* Function name: strInvertMatrix
 * Description:   Invert a matrix.
 * Parameters:
 *       pmtx Pointer to a matrix.
 *       pnil Pointer to a zero element.
 *       pidt Pointer to an identity or unit element.
 *       size Size of each element you used.
 * pcbfagb[4] pcbfagb[0] is omitted. Usually set to NULL.
 *            pcbfagb[EMA_MUL] stores the pointer to a function that can perform multiplication.
 *            pcbfagb[EMA_SUB] stores the pointer to a function that can perform subtraction.
 *            pcbfagb[EMA_DIV] stores the pointer to a function that can perform division.
 *            Please refer to the definition of type CBF_ALGEBRA and enumeration MatrixAlgebra.
 * Return value:  true indicates calculation succeeded. false indicates calculation failed.
 * Caution:       Address of ppmtx Must Be Allocated first.
 *                If function returned false, data of pmtx could be contaminated.
 *                That is, pmtx is not the original matrix before you invoke this function.
 * Tip:           A comprehensive guide:
 *                <test.c>
 *                #include <stdio.h>
 *                #include <stdlib.h>
 *                #include <string.h>
 *                #include "svstring.h"
 *                typedef float MYTYPE;
 *                void PrintMatrix(P_MATRIX pm) {
 *                    size_t i, j;
 *                    for (i = 0; i < pm->ln; ++i) {
 *                        for (j = 0; j < pm->col; ++j)
 *                            printf("%g ", *(MYTYPE *)strFetchValuePointerMatrix(pm, i, j, sizeof(MYTYPE)));
 *                        printf("\n");
 *                    }
 *                }
 *                int cbfadd(const void * px, const void * py) {
 *                    *(MYTYPE *)px += *(MYTYPE *)py;
 *                    return CBF_CONTINUE;
 *                }
 *                int cbfmul(const void * px, const void * py) {
 *                    *(MYTYPE *)px *= *(MYTYPE *)py;
 *                    return CBF_CONTINUE;
 *                }
 *                int cbfsub(const void * px, const void * py) {
 *                    *(MYTYPE *)px -= *(MYTYPE *)py;
 *                    return CBF_CONTINUE;
 *                }
 *                int cbfdiv(const void * px, const void * py) {
 *                    *(MYTYPE *)px /= *(MYTYPE *)py;
 *                    return CBF_CONTINUE;
 *                }
 *                int main() {
 *                    CBF_ALGEBRA pcbfagb[4] = {cbfadd, cbfmul, cbfsub, cbfdiv};
 *                    MYTYPE data[] = { 1.0f, 1.0f, 3.0f, 3.0f, 6.0f, 4.0f, 2.0f, 4.0f, 3.0f };
 *                    MYTYPE zero = 0.0f, unit = 1.0f;
 *                    MATRIX mm, mc, ma, mb;
 *                    P_MATRIX pp[] = { &mc, &mb, &ma };
 *                    strInitMatrix(&mm, 3, 3, sizeof(MYTYPE));
 *                    strInitMatrix(&ma, 3, 3, sizeof(MYTYPE));
 *                    strInitMatrix(&mb, 3, 3, sizeof(MYTYPE));
 *                    strInitMatrix(&mc, 3, 3, sizeof(MYTYPE));
 *                    memcpy(mm.arrz.pdata, data, sizeof(data));
 *                    strCopyMatrix(&ma, &mm, sizeof(MYTYPE));
 *                    strInvertMatrix(&mm, &zero, &unit, sizeof(MYTYPE), pcbfagb);
 *                    strCopyMatrix(&mb, &mm, sizeof(MYTYPE));
 *                    strMultiplyMatrix(pp, &unit, sizeof(MYTYPE), pcbfagb);
 *                    PrintMatrix(&mc); // Print identity matrix.
 *                    strFreeMatrix(&mm);
 *                    return 0;
 *                }
 *                Result and explanation:
 *                If A^(-1) = B then A * B = B * A = E. (A^(-1) is A's inversion, E is an identity matrix.)
 *                A = | 1 1 3 | B = |  2   9 -14 | E = | 1 0 0 |
 *                    | 3 6 4 |     | -1  -3   5 |     | 0 1 0 |
 *                    | 2 4 3 |     |  0  -2   3 |     | 0 0 1 |
 */
bool strInvertMatrix(P_MATRIX pmtx, const void * pnil, const void * pidt, size_t size, CBF_ALGEBRA pcbfagb[4])
{
	if (SV_ASSERT(pmtx->ln == pmtx->col && 0 != size))
	{
		MATRIX maug; /* Augmented matrix. */
		bool rtn = true;
		bool diag = true;
		REGISTER PUCHAR pvec;
		REGISTER void * ptmp;
		const size_t aln = pmtx->ln, acol = aln << 1;
		REGISTER size_t i, j, m = size * acol, n, x, y;
		
		if (NULL == strInitMatrix(&maug, aln + 2, acol, size))
			return false; /* Allocation failure. */
		
		/* Load identity matrix. */
		for (i = 0; i < aln; ++i)
		{
			for (j = aln; j < acol; ++j)
			{
				if (i + aln == j)
					strSetValueMatrix(&maug, i, j, pidt, size);
				else
				{
					strSetValueMatrix(&maug, i, j, pnil, size);
					/* Test whether pmtx is a diagonal matrix. */
					if (diag && 0 != memcmp(pnil, strFetchValuePointerMatrix(pmtx, i, j - aln, size), size))
						diag = false;
				}
			}
		}
		
		if (diag) /* pmtx is a diagonal matrix. */
		{
			for (i = 0; i < aln; ++i)
			{
				for (j = 0; j < aln; ++j)
				{
					if (i == j)
					{
						strSetValueMatrix(&maug, 0, 0, strFetchValuePointerMatrix(pmtx, i, j, size), size);
						strSetValueMatrix(pmtx, i, j, pidt, size);
						pcbfagb[EMA_DIV](strFetchValuePointerMatrix(pmtx, i, j, size), strFetchValuePointerMatrix(&maug, 0, 0, size));
					}
				}
			}
			goto Lbl_End;
		}
		
		/* Load augmented matrix. */
		strProjectMatrix(&maug, 0, 0, pmtx, 0, 0, size);

		/* Posit to vector in augmented matrix. */
		pvec = (PUCHAR) strFetchValuePointerMatrix(&maug, aln, 0, size);
		
		/* Transform the lower triangular matrix. */
		for (i = 0; i < aln; ++i)
		{
			for (j = i; j < aln; ++j)
			{
				if (i == j)
				{
					if (0 == memcmp(pnil, strFetchValuePointerMatrix(&maug, i, j, size), size))
					{	/* Find non zero element. */
						for (n = j + 1; n < aln; ++n)
						{
							if (0 != memcmp(pnil, strFetchValuePointerMatrix(&maug, n, j, size), size))
								break;
						}
						
						if (n >= aln)
						{
							rtn = false;
							goto Lbl_End; /* Matrix is singular. */
						}
						
						/* Line swap. */
						svSwap(strFetchValuePointerMatrix(&maug, n, 0, size), pvec, strFetchValuePointerMatrix(&maug, i, 0, size), m);
					}
					
					/* Put the first non zero leading line in a vector. */
					memcpy(pvec, strFetchValuePointerMatrix(&maug, i, j, size), size);
					
					if (0 != memcmp(pidt, pvec, size))
						for (n = j; n < acol; ++n)
							pcbfagb[EMA_DIV](strFetchValuePointerMatrix(&maug, i, n, size), pvec);
					
					if (i < aln - 1)
						memcpy(pvec, strFetchValuePointerMatrix(&maug, i, 0, size), m);
				}
				else
				{
					memcpy(pvec + m, pvec, m);
					
					ptmp = strFetchValuePointerMatrix(&maug, j, i, size);
					if (0 != memcmp(pidt, ptmp, size))
						for (n = i; n < acol; ++n)
							pcbfagb[EMA_MUL](pvec + m + size * n, ptmp);
					
					for (n = i; n < acol; ++n)
						pcbfagb[EMA_SUB](strFetchValuePointerMatrix(&maug, j, n, size), pvec + m + size * n);
				}
			}
		}
		
		/* Transform the upper triangular matrix. */
		for (i = 0; i < aln; ++i)
		{
			for (j = i; j < aln; ++j)
			{
				x = aln - i - 1;
				y = aln - j - 1;
				if (0 != x)
				{
					if (x == y && i < aln - 1)
						memcpy(pvec, strFetchValuePointerMatrix(&maug, x, 0, size), m);
					else
					{
						memcpy(pvec + m, pvec, m);

						ptmp = strFetchValuePointerMatrix(&maug, y, x, size);
						if (0 != memcmp(pidt, ptmp, size))
							for (n = x - i; n < acol; ++n)
								pcbfagb[EMA_MUL](pvec + m + size * n, ptmp);

						for (n = y; n < acol; ++n)
							pcbfagb[EMA_SUB](strFetchValuePointerMatrix(&maug, y, n, size), pvec + m + size * n);
					}
				}
			}
		}
		
		strProjectMatrix(pmtx, 0, 0, &maug, 0, aln, size); /* Output result. */
		
	Lbl_End:
		strFreeMatrix(&maug);
		return rtn;
	}
	return false;
}

/* Assume that we have a bit map that contains 4 lines and 5 columns.
 *         0 1 2 3 4
 * bm(0,x) 0 0 0 0 0
 * bm(1,x) 0 0 0.0 0
 * bm(2,x) 0 0 1 0 0
 * bm(3,x) 0.0 0 0 0
 *         0 0 0 0..
 * Bytes arranged in sequence, therefore this bit map has 3 consecutive bytes totally.
 * Practically, a bmap_block_t block is a size_t block which equals 2 bytes in a 16-bit system
 *  or 4 bytes in a 32-bit system or 8 bytes in a 64-bit system.
 * Endianness must be put in consideration either to calculate bits in a specific position, for instance,
 *  for a 32-bit little endian machine, the above mentioned map has a datum of value 0x0008_0000,
 *  as for a 64-bit little endian machine, the value is 0x0008_0000_0000_0000.
 */

/* Bit map block type. */
typedef size_t bmap_block_t;

/* Number of bits in a bit map block. */
#define BMAP_BLOCK_BIT (sizeof(bmap_block_t) * CHAR_BIT)

/* Pointer to a bit map block to index.
 * Usage: BMAP_BLOCK(pbm)[index] = (bmap_block_t)false;
 */
#define BMAP_BLOCK(pbmap_M) ((bmap_block_t *)(pbmap_M)->arrz.pdata)

/* Function name: strInitBMap
 * Description:   Initialize a bit matrix.
 * Parameters:
 *        pbm Pointer to a bit matrix you want to initialize.
 *         ln Number of lines in the bit matrix.
 *        col Number of columns in the bit matrix.
 *       bini true  to call memset to initialize the bit matrix as value bval.
 *            This parameter enables bval.
 *            false not to initialize the bit matrix to save execution time.
 *       bval Initialize all bits as true or false.
 * Return value:  Pointer to the buffer.
 * Caution:       Address of pbm Must Be Allocated first.
 */
void * strInitBMap(P_BITMAT pbm, size_t ln, size_t col, bool bini, bool bval)
{
	stdiv_t dr = stdiv(ln * col, BMAP_BLOCK_BIT); /* line number * column number / BMAP_BLOCK_BIT. */
	
	if (NULL == strInitArrayZ(&pbm->arrz, dr.rem ? dr.quot + 1 : dr.quot, BMAP_BLOCK_BIT))
	{
		pbm->ln = pbm->col = 0;
		return NULL;
	}
	
	if (bini)
		memset(pbm->arrz.pdata, bval ? ~(unsigned int)0 : (int)false, BMAP_BLOCK_BIT * pbm->arrz.num);
	
	pbm->ln  = ln;
	pbm->col = col;
	return pbm->arrz.pdata;
}

/* Function name: strFreeBMap_O
 * Description:   Retract a bit matrix.
 * Parameter:
 *       pbm Pointer to a bit matrix you want to fall it into disuse.
 * Return value:  N/A.
 * Caution:       Address of pbm Must Be Allocated first.
 * Tip:           This function can be macro inline to use strFreeBMap.
 */
void strFreeBMap_O(P_BITMAT pbm)
{
	strFreeMatrix(pbm);
}

/* Function name: strCreateBMap
 * Description:   Create a bit matrix.
 * Parameters:
 *         ln Number of lines in a bit matrix.
 *        col Number of columns in a bit matrix.
 *       bini true  to initialize the bit matrix as value bval.
 *            This parameter enables bval.
 *            false not to initialize the bit matrix to save execution time.
 *       bval Initialize all bits as value true or false.
 * Return value:  Pointer to a new created bit matrix.
 */
P_BITMAT strCreateBMap(size_t ln, size_t col, bool bini, bool bval)
{
	REGISTER P_BITMAT pbm = (P_BITMAT) malloc(sizeof(BITMAT));
	if (NULL != pbm)
	{
		if (NULL == strInitBMap(pbm, ln, col, bini, bval))
		{	/* Allocation failure. */
			free(pbm);
			pbm = NULL;
		}
	}
	return pbm;
}

/* Function name: strDeleteBMap_O
 * Description:   Delete a bit matrix.
 * Parameter:
 *       pbm Pointer to a bit matrix you want to release.
 * Return value:  N/A.
 * Caution:       Address of pbm Must Be Allocated first.
 * Tip:           This function can be macro inline to use strDeleteBMap.
 */
void strDeleteBMap_O(P_BITMAT pbm)
{
	strDeleteMatrix(pbm);
}

/* Function name: strCopyBMap_O
 * Description:   Copy a bit matrix from source to destination.
 * Parameters:
 *      pdest Pointer to the destination bit map whose content is a copy of source.
 *       psrc Pointer to the source of bit map to be copied.
 * Return value:  pdest->arrz.pdata
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       After calling, the size of pdest->arrz equaled to the size of psrc->arrz.
 *                Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 * Tip:           A macro version of this function named strCopyBMap_M is available.
 */
void * strCopyBMap_O(P_BITMAT pdest, P_BITMAT psrc)
{
	return strCopyMatrix(pdest, psrc, BMAP_BLOCK_BIT);
}

/* Function name: strCreateCopyBMap
 * Description:   Create a duplicate of a bit matrix/map from source.
 * Parameter:
 *      psrc Pointer to the source matrix to be copied.
 * Return value:  A new pointer points to duplicate map.
 *                If function returned NULL, it would indicate a duplicating failure.
 * Caution:       Address of psrc Must Be Allocated first.
 */
P_BITMAT strCreateCopyBMap(P_BITMAT psrc)
{
	REGISTER P_BITMAT prtn = strCreateBMap(psrc->ln, psrc->col, false, false);
	if (NULL != prtn)
		strCopyMatrix(prtn, psrc, BMAP_BLOCK_BIT);
	return prtn;
}

/* Function name: strGetBitBMap
 * Description:   Return the value from the specific position in a bit matrix.
 * Parameters:
 *        pbm Pointer to a bit matrix you want to operate with.
 *         ln Number of line in the bit matrix. Line number starts from 0.
 *        col Number of column in the bit matrix. Column number starts from 0.
 * Return value:  Either true or false at the specific line and column.
 *                If function returned value -1, it would indicate parameter ln or col is out of range.
 * Caution:       Address of pbm Must Be Allocated first.
 */
bool strGetBitBMap(P_BITMAT pbm, size_t ln, size_t col)
{
	if (SV_ASSERT(ln < pbm->ln && col < pbm->col))
	{	/* Right shift a bmap_block_t block to compare its least significant bit with 1. */
		stdiv_t dr = stdiv(ln * pbm->col + col + 1, BMAP_BLOCK_BIT);
		return BOOLIZE(0x1 & (BMAP_BLOCK(pbm)[dr.rem ? dr.quot : dr.quot - 1] >> (dr.rem ? BMAP_BLOCK_BIT - dr.rem : 0)));
	}
	return false; /* Over size. */
}

/* Function name: strSetBitBMap
 * Description:   Set value for a bit matrix onto the specific position.
 * Parameters:
 *        pbm Pointer to a bit matrix you want to operate.
 *         ln Number of line in the bit matrix. Line number starts from 0.
 *        col Number of column in the bit matrix. Column number starts from 0.
 * Return value:  true indicates operation succeeded.
 *                If function returned false, it would indicate parameter ln or col is out of range.
 * Caution:       Address of pbm Must Be Allocated first.
 */
bool strSetBitBMap(P_BITMAT pbm, size_t ln, size_t col, bool bval)
{
	if (SV_ASSERT(ln < pbm->ln && col < pbm->col))
	{
		REGISTER bmap_block_t t = 0x1;
		REGISTER size_t i;
		stdiv_t dr = stdiv(ln * pbm->col + col + 1, BMAP_BLOCK_BIT);
		
		/* Left shift t and pile it onto the specific bmap_block_t block. */
		t <<= (dr.rem ? BMAP_BLOCK_BIT - dr.rem : 0);
		i = dr.rem ? dr.quot : dr.quot - 1;
		
		BMAP_BLOCK(pbm)[i] = (bmap_block_t)
		(
			bval ?
			BMAP_BLOCK(pbm)[i] | t :
			BMAP_BLOCK(pbm)[i] & ~t
		);
		return true;
	}
	return false; /* Over size. */
}

/* Functions for sparse matrices are implemented bellow. */

/* Sectional function declarations. */
void   _strBITAdd (size_t idx, ptrdiff_t val,  P_ARRAY_Z parrz);
size_t _strBITSum (size_t idx, P_ARRAY_Z parrz);

/* This macro is used to get lowest bit of an integer and is for Fenwick trees. */
#define _LOWBIT(x) ((x) & (~(x) + 1))

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strBITAdd
 * Description:   Update array item and add new value.
 * Parameters:
 *        idx Index of the binary indexed tree array.
 *        val Incremental value which shall be added onto array item.
 *      parrz Pointer to Fenwick tree which is an array of size_t integers.
 * Return value:  N/A.
 */
void _strBITAdd(size_t idx, ptrdiff_t val, P_ARRAY_Z parrz)
{
	while (idx < strLevelArrayZ(parrz))
	{
		*(ptrdiff_t *)strLocateItemArrayZ(parrz, sizeof(ptrdiff_t), idx) += val;
		idx += _LOWBIT(idx);
	}
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strBITSum
 * Description:   Locate an item in a Fenwick tree.
 *                Count the summary of [0, idx].
 * Parameters:
 *        idx Index of the binary indexed tree array.
 *      parrz Pointer to a Fenwick tree which is an array of size_t integers.
 * Return value:  Summary value.
 */
size_t _strBITSum(size_t idx, P_ARRAY_Z parrz)
{
	REGISTER size_t r = 0;
	while (idx)
	{
		r += *(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), idx);
		idx -= _LOWBIT(idx);
	}
	return r;
}

/* Function name: strInitSparseMatrix
 * Description:   Initialize a sparse matrix.
 * Parameters:
 *       pbmx Pointer to a sparse matrix you want to initialize.
 *         ln Number of lines in the sparse matrix.
 *        col Number of columns in the sparse matrix.
 * Return value:  true  Initialization succeeded.
 *                false Initialization failed.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
bool strInitSparseMatrix(P_SPAMAT pmtx, size_t ln, size_t col)
{
	if (NULL != strInitArrayZ(&pmtx->bita, ln, sizeof(size_t)))
		memset(pmtx->bita.pdata, 0, sizeof(size_t) * strLevelArrayZ(&pmtx->bita));
	strInitLinkedListSC(&pmtx->datlst);
	return NULL != strInitBMap(&pmtx->bmask, ln, col, true, false);
}

/* Function name: strFreeSparseMatrix
 * Description:   Retract a sparse matrix that is allocated by function strInitSparseMatrix.
 * Parameter:
 *      pmtx Pointer to a sparse matrix you want to release.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void strFreeSparseMatrix(P_SPAMAT pmtx)
{
	strFreeArrayZ(&pmtx->bita);
	strFreeBMap(&pmtx->bmask);
	strFreeLinkedListSC(&pmtx->datlst);
	pmtx->datlst = NULL;
}

/* Function name: strCreateSparseMatrix
 * Description:   Create a sparse matrix.
 * Parameters:
 *         ln Number of lines in a sparse matrix.
 *        col Number of columns in a sparse matrix.
 * Return value:  Pointer to the new created sparse matrix.
 */
P_SPAMAT strCreateSparseMatrix(size_t ln, size_t col)
{
	REGISTER P_SPAMAT pmtx = (P_SPAMAT) malloc(sizeof(SPAMAT));
	if (NULL != pmtx)
		strInitSparseMatrix(pmtx, ln, col);
	return pmtx;
}

/* Function name: strDeleteSparseMatrix
 * Description:   Delete a sparse matrix which is allocated by function strCreateSparseMatrix.
 * Parameter:
 *      pmtx Pointer to a sparse matrix you want to defuse.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void strDeleteSparseMatrix(P_SPAMAT pmtx)
{
	strFreeSparseMatrix(pmtx);
	free(pmtx);
}

/* Function name: strCopySparseMatrix
 * Description:   Copy a sparse matrix from source to destination.
 * Parameters:
 *      pdest Pointer to the destination sparse matrix whose content is a copy of source.
 *       psrc Pointer to the source of the sparse matrix as a template to be copied from.
 *       size Size of each element in the source sparse matrix.
 * Return value:  If duplication succeeded, function would return the same pointer as pdest.
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 */
P_SPAMAT strCopySparseMatrix(P_SPAMAT pdest, P_SPAMAT psrc, size_t size)
{
	if (pdest != psrc)
	{
		if (strLevelArrayZ(&pdest->bita) != strLevelArrayZ(&psrc->bita))
			if (NULL == strResizeArrayZ(&pdest->bita, strLevelArrayZ(&psrc->bita), sizeof(size_t)))
				return NULL; /* Can not resize Fenwick tree. */
		
		/* Copy embedded Fenwick tree in the first step. */
		if (0 != strLevelArrayZ(&psrc->bita) && 0 != strLevelArrayZ(&pdest->bita))
		{
			if (NULL == strMoveArrayZ(&pdest->bita, &psrc->bita, sizeof(size_t)))
				return NULL;
		}
		else
			return NULL; /* A corrupted Fenwick tree makes further procedure impossible. */
		
		/* Then copy embedded bit map in case if the Fenwick tree is valid. */
		if (NULL != strCopyBMap(&pdest->bmask, &psrc->bmask))
		{	/* Finally copy data in list. */
			if (pdest->datlst != psrc->datlst)
			{	/* In case data lists are not overlapped. */
				if (NULL != pdest->datlst)
				{
					strFreeLinkedListSC(&pdest->datlst); /* Free old data chain first. */
					pdest->datlst = NULL; /* No data in source to be copied from. It is an invalid but sensible sparse matrix. */
				}
				if (NULL != psrc->datlst)
					pdest->datlst = strCopyLinkedListSC(psrc->datlst, size);
			}
			return pdest;
		}
		return NULL;
	}
	return pdest;
}

/* Function name: strCreateCopySparseMatrix
 * Description:   Create a copy of a sparse matrix from source.
 * Parameters:
 *       psrc Pointer to the source of the sparse matrix to copy from.
 *       size Size of each element in the source sparse matrix.
 * Return value:  A new pointer points to a new allocated sparse matrix as a copy from psrc.
 *                If function returned NULL, it indicates a duplicating failure.
 * Caution:       Address of psrc Must Be Allocated first.
 */
P_SPAMAT strCreateCopySparseMatrix(P_SPAMAT psrc, size_t size)
{
	REGISTER P_SPAMAT prtn = strCreateSparseMatrix(psrc->bmask.ln, psrc->bmask.col);
	if (NULL != prtn)
	{
		if (NULL == strCopySparseMatrix(prtn, psrc, size))
			strDeleteSparseMatrix(prtn); /* Failed to resize the Fenwick tree of return, return NULL. */
		else
			return prtn;
	}
	return NULL;
}

#define _CHAR_SIGN ((UCHART)1 << (CHAR_BIT - 1)) /* Make a byte whose value equals to 0x80. */

/* Function name: strGetValueSparseMatrix
 * Description:   Return the value and its pointer from the specific position in a sparse matrix.
 * Parameters:
 *       pval Pointer to a buffer to store the value you have got.
 *            If pval equaled NULL, this function would not put value into buffer rather than return its pointer.
 *            (*) Especially, if a location in a sparse matrix were not assigned yet, function would return NULL,
 *            and function would NOT assign any data to the memory block that pval pointed.
 *       pmtx Pointer to a sparse matrix you want to operate with.
 *         ln Number of line in the sparse matrix. Line number starts from 0.
 *        col Number of column in the sparse matrix. Column number starts from 0.
 *       size Size of each element in the sparse matrix.
 * Return value:  Pointer to the value on the specific position in sparse matrix.
 *                If function returned NULL, it would indicate that parameter ln or col might be out of range or value is not assigned yet.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void * strGetValueSparseMatrix(void * pval, P_SPAMAT pmtx, size_t ln, size_t col, size_t size)
{
	if (SV_ASSERT(ln < pmtx->bmask.ln && col < pmtx->bmask.col))
	{
		REGISTER size_t i, j, l, m;
		stdiv_t dr = stdiv(ln * pmtx->bmask.col + col + 1, CHAR_BIT);
		/* Initialize variables. */
		if (dr.rem)
		{
			j = CHAR_BIT - dr.rem;
			l = dr.quot;
			m = dr.rem - 1;
		}
		else
		{
			j = 0;
			l = dr.quot  - 1;
			m = CHAR_BIT - 1;
		}
		if (BOOLIZE(0x01 & (pmtx->bmask.arrz.pdata[l] >> j)))
		{	/* Item exists. */
			REGISTER size_t s = 0;
			REGISTER P_NODE_S pnode;
			/* Count items. */
			s = _strBITSum(l, &pmtx->bita);
			/* Count the rest of items. */
			for (i = l, j = 0; j < m; ++j)
				if (pmtx->bmask.arrz.pdata[i] & (_CHAR_SIGN >> j))
					++s;
			/* Fetch item's data. */
			if (NULL != (pnode = strLocateItemSC(pmtx->datlst, s)))
			{
				if (NULL != pval) /* Output value if necessary. */
					memcpy(pval, pnode->pdata, size);
				return pnode->pdata;
			}
		}
	}
	return NULL;
}

/* Function name: strSetValueSparseMatrix
 * Description:   Set value for a sparse matrix onto the specific position.
 * Parameters:
 *       pmtx Pointer to a sparse matrix you want to operate.
 *         ln Number of line in the sparse matrix. Line number starts from 0.
 *        col Number of column in the sparse matrix. Column number starts from 0.
 *       pval Pointer to the new value you want to set into the sparse matrix.
 *       size Size of each element in the sparse matrix.
 * Return value:  Pointer to the value in the sparse matrix you have set.
 * Caution:       Address of pmtx Must Be Allocated first.
 *                (*) If pval equals value NULL or size equals value 0,
 *                function would remove existed item in a sparse matrix.
 */
void * strSetValueSparseMatrix(P_SPAMAT pmtx, size_t ln, size_t col, void * pval, size_t size)
{
	if (SV_ASSERT(ln < pmtx->bmask.ln && col < pmtx->bmask.col))
	{
		REGISTER size_t i, j, l, m, s = 0;
		REGISTER P_NODE_S pnode;
		REGISTER UCHART t, u;
		stdiv_t dr = stdiv(ln * pmtx->bmask.col + col + 1, CHAR_BIT);
		/* Initialize variables. */
		if (dr.rem)
		{
			j = CHAR_BIT - dr.rem;
			l = dr.quot;
			m = dr.rem - 1;
		}
		else
		{
			j = 0;
			l = dr.quot  - 1;
			m = CHAR_BIT - 1;
		}
		t = (UCHART) (0x01 << j);
		u = (UCHART) (pmtx->bmask.arrz.pdata[l] >> j);
		/* Count items. */
		s = _strBITSum(l, &pmtx->bita);
		/* Count the rest of items. */
		for (i = l, j = 0; j < m; ++j)
			if (pmtx->bmask.arrz.pdata[i] & (_CHAR_SIGN >> j))
				++s;
		if (BOOLIZE(0x01 & u))
		{	/* Item exists. */
			if (NULL != (pnode = strLocateItemSC(pmtx->datlst, s)))
			{
				if (NULL != pval && 0 != size)
					return memcpy(pnode->pdata, pval, size); /* Fetch item's data and alter it. */
				else
				{
					strDeleteNodeS(strRemoveItemLinkedListSC(&pmtx->datlst, pnode));
					/* Update the Fenwick tree. */
					_strBITAdd(l + 1, -1, &pmtx->bita);
					/* Clear bit mask. */
					pmtx->bmask.arrz.pdata[l] = (UCHART) (pmtx->bmask.arrz.pdata[l] & ~t);
				}
			}
		}
		else if (NULL != pval && 0 != size) /* Insert new item. */
		{
			REGISTER P_NODE_S pnew = strCreateNodeS(pval, size);
			if (NULL != pnew)
			{
				if (0 == s || NULL == pmtx->datlst) /* Assign new header. */
				{
					pnew->pnode = pmtx->datlst;
					pmtx->datlst = pnew;
				}
				else
				{	/* Locate the previous item. */
					pnode = strLocateItemSC(pmtx->datlst, s - 1);
					pnew->pnode = pnode->pnode;
					pnode->pnode = pnew;
				}
				/* Sign a bit on bit mask. */
				pmtx->bmask.arrz.pdata[l] = (UCHART) (pmtx->bmask.arrz.pdata[l] | t);
				/* Update the Fenwick tree. */
				_strBITAdd(l + 1, +1, &pmtx->bita);
				return pnew->pdata;
			}
		}
	}
	return NULL;
}

#undef _CHAR_SIGN /* Undefine used macro. */

/* Function name: strFillSparseMatrix
 * Description:   Fill a sparse matrix into a common matrix.
 * Parameters:
 *      pdest Pointer to a common matrix you want to fill.
 *       psrc Pointer to a sparse matrix you want to fill into the common matrix.
 *       size Size of each element in the sparse matrix and the common matrix.
 * Return value:  true  Filling succeeded.
 *                false Filling failed.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                Users may need to set matrix pdest into an empty one by their own before invoking this function.
 *                Nevertheless, this function remains matrix elements untouched if they do not appear in the sparse one.
 *                (*) Line and column number of the common destination matrix shall
 *                greater than or equal to each value of the source sparse matrix.
 */
bool strFillSparseMatrix(P_MATRIX pdest, P_SPAMAT psrc, size_t size)
{
	if (SV_ASSERT(pdest->ln >= psrc->bmask.ln && pdest->col >= psrc->bmask.col))
	{
		REGISTER P_NODE_S pnode = psrc->datlst;
		REGISTER size_t i, j;
		/* Clean destination. */
		memset(pdest->arrz.pdata, 0, strLevelArrayZ(&pdest->arrz) * size);
		for (i = 0; i < psrc->bmask.ln; ++i)
			for (j = 0; j < psrc->bmask.col; ++j)
				if (strGetBitBMap(&psrc->bmask, i, j))
					strSetValueMatrix(pdest, i, j, pnode->pdata, size), pnode = pnode->pnode;
		return true;
	}
	return false;
}

