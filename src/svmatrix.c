/*
 * Name:        svmatrix.c
 * Description: Matrices.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0213191430N0220251750L00986
 * License:     LGPLv3
 * Copyright (C) 2019-2025 John Cage
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
#include <string.h> /* Using function memcpy, memset, memmove. */
#include <limits.h> /* Using macro CHAR_BIT, UCHAR_MAX. */
#include "svstring.h"

/* Function name: strInitMatrix
 * Description:   Initialize a matrix.
 * Parameters:
 *       pbmx Pointer to a matrix you want to initialize.
 *         ln Number of lines in the matrix.
 *        col Number of columns in the matrix.
 *       size Size of each element in the matrix.
 * Return value:  Pointer to the buffer of a matrix.
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
 * Description:   Deallocate a matrix that is allocated by function strInitMatrix.
 * Parameter:
 *      pmtx Pointer to a matrix you want to deallocate.
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
	P_MATRIX pmtx = (P_MATRIX) malloc(sizeof(MATRIX));
	if (NULL != pmtx)
	{
		if (NULL == strInitMatrix(pmtx, ln, col, size))
		{
			free(pmtx);
			return NULL; /* Allocation failure. */
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
 *       psrc Pointer to the source of the matrix to be copied.
 *       size Size of each element in both two matrices.
 * Return value:  pdest->arrz.pdata
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       After calling, the size of pdest->arrz equaled to the size of psrc->arrz.
 *                Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 */
void * strCopyMatrix(P_MATRIX pdest, P_MATRIX psrc, size_t size)
{
	if
	(
		NULL !=
		(	/* Destination has not been initialized yet. */
			0 == strLevelArrayZ(&pdest->arrz) ?
			strInitArrayZ(&pdest->arrz, psrc->arrz.num, size) :
			strResizeArrayZ(&pdest->arrz, psrc->arrz.num, size)
		)
	)
	{
		pdest->ln  = psrc->ln;
		pdest->col = psrc->col;
		return strCopyArrayZ(&pdest->arrz, &psrc->arrz, size);
	}
	strFreeMatrix(pdest);
	return NULL;
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
	const size_t ol = pmtx->ln;
	REGISTER size_t oc = pmtx->col;
	REGISTER size_t i, j;
	const size_t k = size * col;
	if (ln * col > ol * oc) /* Matrix becomes bigger. */
	{
		if (strResizeArrayZ(&pmtx->arrz, ln * col, size))
		{
			for (i = ol; i > 1; --i)
			{
				j = (i - 1) * size;
				memmove(&pmtx->arrz.pdata[j * col], &pmtx->arrz.pdata[j * oc], k);
			}
			pmtx->ln = ln;
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
		if (strResizeArrayZ(&pmtx->arrz, ln * col, size))
		{
			pmtx->ln = ln;
			pmtx->col = col;
			return pmtx->arrz.pdata;
		}
		pmtx->ln = pmtx->col = 0;
		return NULL;
	}
	pmtx->ln = ln;
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
	if (ln < pmtx->ln && col < pmtx->col && size)
	{
		REGISTER void * ptr = &pmtx->arrz.pdata[(ln * pmtx->col + col) * size];
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
void * strSetValueMatrix_O(P_MATRIX pmtx, size_t ln, size_t col, void * pval, size_t size)
{
	return (ln < pmtx->ln && col < pmtx->col && size) ?
		memcpy(&pmtx->arrz.pdata[(ln * pmtx->col + col) * size], pval, size) : NULL;
}

/* Function name: strTransposeMatrix
 * Description:   Transpose a matrix. Swap line and column index for each element in a matrix.
 * Parameters:
 *       pmtx Pointer to a matrix.
 *       size Size of each element in the matrix.
 *     cbfcmp Pointer to a callback function that uses to compare each element in the matrix.
 *            Two parameters of cbfcmp may point to any element in the array.
 *            Please refer to the definition of type CBF_COMPARE.
 * Return value:  pmtx->arrz.pdata
 *                If this function returned value NULL, it would indicate an allocation failure.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void * strTransposeMatrix(P_MATRIX pmtx, size_t size, CBF_COMPARE cbfcmp)
{
	MATRIX mtxt;
	mtxt.arrz.num = 0;
	if (NULL != strCopyMatrix(&mtxt, pmtx, size))
	{
		REGISTER size_t i, j, m, n;
		REGISTER void * pa, * pb;
		for (i = 0; i < mtxt.ln; ++i)
		{
			m = i * size;
			for (j = 0; j < mtxt.col; ++j)
			{
				n = j * size;
				pa = &mtxt.arrz.pdata[mtxt.col * m + n];
				pb = &pmtx->arrz.pdata[mtxt.ln * n + m];
				if (0 != cbfcmp(pa, pb))
					memcpy(pb, pa, size);
			}
		}
		svSwap(&(pmtx->ln), &(pmtx->col), &(mtxt.ln), sizeof(size_t));
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
 * Return value:  TRUE  Projection succeeded.
 *                FALSE Projection failed.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                All line number dln, sln and column number dcol, scol start from 0.
 * Tip:           Assume that we have two matrices A and B, then a projection can be the following situation.
 *                strProjectMatrix(&A, 1, 1, &B, 1, 1);
 *                // Before projection:        : After projection:
 *                // A=| a b c d | B=| q r s | : A=| a b c d | B=| q r s |
 *                //   | e f g h |   | t u v | :   | e u.v.h |   | t u v |
 *                //   | i j k l |   | w x y | :   | i x.y.l |   | w x y |
 *                //   | m n o p |             :   | m n o p |
 */
BOOL strProjectMatrix(P_MATRIX pdest, size_t dln, size_t dcol, P_MATRIX psrc, size_t sln, size_t scol, size_t size)
{
	if (dln < pdest->ln && dcol < pdest->col && sln < psrc->ln && scol < psrc->col)
	{
		REGISTER size_t i, j, k, l, o, p;
		const size_t m = sln - dln, n = scol - dcol;
		if (psrc->ln - sln < pdest->ln - dln)
			o = psrc->ln;
		else
			o = pdest->ln;
		if (psrc->col - scol < pdest->col - dcol)
			p = psrc->col;
		else
			p = pdest->col;
		for (i = sln; i < o; ++i)
		{
			k = (i - m) * pdest->col - n;
			l = i * psrc->col;
			for (j = scol; j < p; ++j)
			{
				memmove
				(
					&pdest->arrz.pdata[(k + j) * size],
					&psrc->arrz.pdata[(l + j) * size],
					size
				);
			}
		}
		return TRUE;
	}
	return FALSE;
}

/* Function name: strM1Matrix
 * Description:   Do calculation on each element in a matrix with the value that pval pointed.
 * Parameters:
 *       pmtx Pointer to a matrix.
 *       pval Pointer to the value that you want to operate onto the matrix.
 *       size Size of each element in the matrix.
 *     cbfagb Pointer to a callback function that uses to handle calculations on elements.
 *            The left pointer of cbfagb pointes to any element in the matrix,
 *            and the right pointer of cbfagb always holds a same value as pval.
 *            Please refer to the definition of type CBF_ALGEBRA.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return depends on function cbfagb.
 * Caution:       Address of pmtx Must Be Allocated first.
 * Tip:           Users could use this function to multiply a number with a matrix like this way:
 *                int mul(const void * pa, const void * pb) { *(float *)pa *= *(float *)pb; return CBF_CONTINUE; }
 *                float f = 1.0f; strM1Matrix(&f, pmtx, sizeof(float), mul);
 */
int strM1Matrix(P_MATRIX pmtx, const void * pval, size_t size, CBF_ALGEBRA cbfagb)
{
	REGISTER size_t i, j;
	for (i = 0, j = pmtx->ln * pmtx->col * size; i < j; i += size)
		if (CBF_CONTINUE != cbfagb(&pmtx->arrz.pdata[i], pval))
			return CBF_TERMINATE;
	return CBF_CONTINUE;
}

/* Function name: strM2Matrix
 * Description:   Do calculation between two matrices,
 *                and store the result into the matrix that pmtxa pointed.
 * Parameters:
 *      pmtxa Pointer to a matrix.
 *      pmtxb Pointer to another matrix.
 *       size Size of each element in the matrix.
 *     cbfagb Pointer to a callback function that uses to handle calculations on elements.
 *            The left pointer of cbfagb pointes to any element in pmtxa,
 *            and the right pointer of cbfagb pointes to the corresponding value in pmtxb.
 *            Please refer to the definition of type CBF_ALGEBRA.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return depends on function cbfagb.
 * Caution:       Address of pmtxa and pmtxb Have to Be Allocated first.
 * Tip:           Users could use this function to add a matrix with another like this way:
 *                int plus(const void * pa, const void * pb) { *(float *)pa += *(float *)pb; return CBF_CONTINUE; }
 *                strM2Matrix(pmtxa, pmtxb, sizeof(float), plus);
 */
int strM2Matrix(P_MATRIX pmtxa, P_MATRIX pmtxb, size_t size, CBF_ALGEBRA cbfagb)
{
	if (pmtxa->ln == pmtxb->ln && pmtxa->col == pmtxb->col)
	{
		REGISTER size_t i, j;
		for (i = 0, j = pmtxa->ln * pmtxa->col * size; i < j; i += size)
			if (CBF_CONTINUE != cbfagb(&pmtxa->arrz.pdata[i], &pmtxb->arrz.pdata[i]))
				return CBF_TERMINATE;
		return CBF_CONTINUE;
	}
	return CBF_TERMINATE;
}

/* Enumeration describes index of matrices and algebraic operations on matrices. */
typedef enum _en_M3Matrix  { _M3M_C, _M3M_A, _M3M_B } _M3Matrix;
typedef enum _en_M3Algebra { _M3A_ADD, _M3A_MUL }     _M3Algebra;

/* Macros used to fetch line number and column number and data pointers of matrices. */
#define MAT_LN(index)   ((const size_t)ppmtx[index]->ln)
#define MAT_COL(index)  ((const size_t)ppmtx[index]->col)
#define MAT_DATA(index) (ppmtx[index]->arrz.pdata)

/* Function name: strM3Matrix
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
 * pcbfagb[2] pcbfagb[0] stores the pointer to a function that can perform addition.
 *            pcbfagb[1] stores the pointer to a function that can perform multiplication.
 *            Please refer to the definition of type CBF_ALGEBRA.
 * Return value:  Either CBF_CONTINUE or CBF_TERMINATE will return depends on function cbfagb.
 * Caution:       Address of ppmtx[0], ppmtx[1] and ppmtx[2] Must Be Allocated first.
 * Tip:           Users could use this function to multiply a matrix with another like this way:
 *                int add(const void * pa, const void * pb) { *(float *)pa += *(float *)pb; return CBF_CONTINUE; }
 *                int mul(const void * pa, const void * pb) { *(float *)pa *= *(float *)pb; return CBF_CONTINUE; }
 *                MATRIX mc, ma, mb;
 *                float a[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f }, tmp = 0.0f;
 *                float b[] = { 3.0f, 2.0f, 1.0f, 6.0f, 5.0f, 4.0f };
 *                CBF_ALGEBRA alg[2];
 *                P_MATRIX pm[3];
 *                alg[0] = add; alg[1] = mul;
 *                pm[0] = &mc; pm[1] = &ma; pm[2] = &mb;
 *                strInitMatrix(&ma, 2, 3, sizeof(float));
 *                strInitMatrix(&mb, 3, 2, sizeof(float));
 *                strInitMatrix(&mc, 2, 2, sizeof(float));
 *                strSetMatrix(&mc, &tmp, sizeof(float));
 *                memcpy(ma.arrz.pdata, a, sizeof a);
 *                memcpy(mb.arrz.pdata, b, sizeof b);
 *                strM3Matrix(pm, &tmp, sizeof(float), alg);
 *                strFreeMatrix(&mc);
 *                strFreeMatrix(&ma);
 *                strFreeMatrix(&mb);
 *                // | 1 2 3 |   | 3 2 |   | 20 26 |
 *                // | 4 5 6 | * | 1 6 | = | 47 62 |
 *                //             | 5 4 |
 *                  __      _n_
 *                 /  \     \  |
 *                |      ==  >   a  b
 *                 \__/ij   /__|  ik kj
 *                          k:=1
 */
int strM3Matrix(P_MATRIX ppmtx[3], void * ptemp, size_t size, CBF_ALGEBRA pcbfagb[2])
{
	if (MAT_COL(_M3M_A) == MAT_LN(_M3M_B))
	{
		REGISTER size_t i, j, k, m;
		REGISTER PUCHAR ptrmc = MAT_DATA(_M3M_C);
		for (i = 0; i < MAT_LN(_M3M_A); ++i)
		{
			m = i * MAT_COL(_M3M_A);
			for (j = 0; j < MAT_COL(_M3M_B); ++j)
			{
				for (k = 0; k < MAT_COL(_M3M_A); ++k)
				{
					memcpy(ptemp, &MAT_DATA(_M3M_A)[(m + k) * size], size);
					if (CBF_CONTINUE != pcbfagb[_M3A_MUL](ptemp, &MAT_DATA(_M3M_B)[(k * MAT_COL(_M3M_B) + j) * size]))
						return CBF_TERMINATE;
					if (0 == k)
						memcpy(ptrmc, ptemp, size);
					else
					{
						if (CBF_CONTINUE != pcbfagb[_M3A_ADD](ptrmc, ptemp))
							return CBF_TERMINATE;
					}
				}
				ptrmc += size;
			}
		}
		return CBF_CONTINUE;
	}
	return CBF_TERMINATE;
}

#undef MAT_LN
#undef MAT_COL
#undef MAT_DATA /* Undefine used macros. */

/* Assume that we have a bit-map that contains 4 lines and 5 columns.
 *         0 1 2 3 4
 * bm(0,x) 0 0 0 0 0
 * bm(1,x) 0 0 0.0 0
 * bm(2,x) 0 0 1 0 0
 * bm(3,x) 0.0 0 0 0
 *         0 0 0 0..
 * Bytes arranged in sequence, therefore this bit map has 3 consecutive bytes totally.
 */

/* Function name: strInitBMap
 * Description:   Initialize a bit-matrix.
 * Parameters:
 *        pbm Pointer to a bit-matrix you want to initialize.
 *         ln Number of lines in the bit-matrix.
 *        col Number of columns in the bit-matrix.
 *       bval Initialize all bits as TRUE or FALSE.
 * Return value:  Pointer to the buffer.
 * Caution:       Address of pbm Must Be Allocated first.
 */
void * strInitBMap(P_BITMAT pbm, size_t ln, size_t col, BOOL bval)
{
	stdiv_t dr = stdiv(ln * col, CHAR_BIT); /* ln bit * col bit / CHAR_BIT. */
	if (NULL == strInitArrayZ(&pbm->arrz, dr.rem ? dr.quot + 1 : dr.quot, sizeof(UCHART)))
	{
		pbm->ln = pbm->col = 0;
		return NULL;
	}
	memset(pbm->arrz.pdata, bval ? UCHAR_MAX : FALSE, sizeof(UCHART) * pbm->arrz.num);
	pbm->col = col;
	pbm->ln = ln;
	return pbm->arrz.pdata;
}

/* Function name: strFreeBMap_O
 * Description:   Deallocate a bit-matrix.
 * Parameter:
 *       pbm Pointer to a bit-matrix you want to fall it into disuse.
 * Return value:  N/A.
 * Caution:       Address of pbm Must Be Allocated first.
 * Tip:           A macro version of this function named strFreeBMap_M is available.
 */
void strFreeBMap_O(P_BITMAT pbm)
{
	strFreeMatrix(pbm);
}

/* Function name: strCreateBMap
 * Description:   Create a bit-matrix.
 * Parameters:
 *         ln Number of lines in a bit-matrix.
 *        col Number of columns in a bit-matrix.
 *       bval Initialize all bits as value TRUE or FALSE.
 * Return value:  Pointer to a new created bit-matrix.
 */
P_BITMAT strCreateBMap(size_t ln, size_t col, BOOL val)
{
	P_BITMAT pbm = (P_BITMAT) malloc(sizeof(BITMAT));
	if (NULL == pbm)
		return NULL;
	if (NULL == strInitBMap(pbm, ln, col, val))
	{	/* Allocation failure. */
		free(pbm);
		return NULL;
	}
	return pbm;
}

/* Function name: strDeleteBMap_O
 * Description:   Delete a bit-matrix.
 * Parameter:
 *       pbm Pointer to a bit-matrix you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pbm Must Be Allocated first.
 * Tip:           A macro version of this function named strDeleteBMap_M is available.
 */
void strDeleteBMap_O(P_BITMAT pbm)
{
	strDeleteMatrix(pbm);
}

/* Function name: strCopyBMap_O
 * Description:   Copy a bit-matrix from source to destination.
 * Parameters:
 *      pdest Pointer to the destination matrix whose content is to be copied.
 *       psrc Pointer to the source of matrix to be copied.
 * Return value:  pdest->arrz.pdata
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       After calling, the size of pdest->arrz equaled to the size of psrc->arrz.
 *                Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 * Tip:           A macro version of this function named strCopyBMap_M is available.
 */
void * strCopyBMap_O(P_BITMAT pdest, P_BITMAT psrc)
{
	return strCopyMatrix(pdest, psrc, sizeof(UCHART));
}

/* Function name: strGetBitBMap
 * Description:   Return the value from the specific position in a bit-matrix.
 * Parameters:
 *        pbm Pointer to a bit-matrix you want to operate with.
 *         ln Number of line in the bit-matrix. Line number starts from 0.
 *        col Number of column in the bit-matrix. Column number starts from 0.
 * Return value:  Either TRUE or FALSE at the specific line and column.
 *                If function returned value -1, it would indicate parameter ln or col is out of range.
 * Caution:       Address of pbm Must Be Allocated first.
 */
BOOL strGetBitBMap(P_BITMAT pbm, size_t ln, size_t col)
{
	stdiv_t dr = stdiv(ln * pbm->col + col + 1, CHAR_BIT);
	if (ln >= pbm->ln || col >= pbm->col)
		return FALSE; /* Over size. */
	/* Right shift a UCHART block to compare its LSBit with 1. */
	return 0x01 & (pbm->arrz.pdata[dr.rem ? dr.quot : dr.quot - 1] >> (dr.rem ? CHAR_BIT - dr.rem : 0));
}

/* Function name: strSetBitBMap
 * Description:   Set value for a bit-matrix onto the specific position.
 * Parameters:
 *        pbm Pointer to a bit-matrix you want to operate.
 *         ln Number of line in the bit-matrix. Line number starts from 0.
 *        col Number of column in the bit-matrix. Column number starts from 0.
 * Return value:  TRUE indicates operation succeeded.
 *                If function returned FALSE, it would indicate parameter ln or col is out of range.
 * Caution:       Address of pbm Must Be Allocated first.
 */
BOOL strSetBitBMap(P_BITMAT pbm, size_t ln, size_t col, BOOL bval)
{
	REGISTER UCHART t = 0x01;
	REGISTER size_t i;
	stdiv_t dr = stdiv(ln * pbm->col + col + 1, CHAR_BIT);
	if (ln >= pbm->ln || col >= pbm->col)
		return FALSE; /* Over size. */
	/* Left shift t and pile it onto the specific UCHART block. */
	t <<= (dr.rem ? CHAR_BIT - dr.rem : 0);
	i = dr.rem ? dr.quot : dr.quot - 1;
	pbm->arrz.pdata[i] = (UCHART)
	(
		FALSE == bval ?
		pbm->arrz.pdata[i] & (~t) :
		pbm->arrz.pdata[i] | t
	);
	return TRUE;
}

/* Functions for sparse matrices are implemented bellow. */
/* Sectional function declarations. */
void   _strBITUpdateItem(size_t    idx,  size_t    val,  P_ARRAY_Z parrz);
size_t _strBITLocateItem(size_t    idx,  P_ARRAY_Z parrz);
void   _strBITConstruct (P_ARRAY_Z parrz);

/* This macro is used to get lowest bit of an integer and is for Fenwick trees. */
#define _LOWBIT(x) ((x) & (~(x) + 1))

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strBITUpdateItem
 * Description:   Update array item.
 * Parameters:
 *        idx Index of the bit indexed tree array.
 *        val Incremental value which shall be added to array item.
 *      parrz Pointer to Fenwick tree which is an array of size_t integers.
 * Return value:  N/A.
 */
void _strBITUpdateItem(size_t idx, size_t val, P_ARRAY_Z parrz)
{
	while (idx < strLevelArrayZ(parrz))
	{
		*(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), idx) += val;
		idx += _LOWBIT(idx);
	}
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strBITLocateItem
 * Description:   Locate an item in a Fenwick tree.
 *                Count the summary of [0, index].
 * Parameters:
 *        idx Index of the bit indexed tree array.
 *      parrz Pointer to Fenwick tree which is an array of size_t integers.
 * Return value:  Summary value.
 */
size_t _strBITLocateItem(size_t idx, P_ARRAY_Z parrz)
{
	REGISTER size_t r = 0;
	while (idx)
	{
		r += *(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), idx);
		idx -= _LOWBIT(idx);
	}
	return r;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _strBITConstruct
 * Description:   Construct a Fenwick tree through a sized-array.
 * Parameter:
 *     parrz Pointer an array of size_t integers.
 * Return value:  N/A.
 */
void _strBITConstruct(P_ARRAY_Z parrz)
{
	REGISTER size_t i, p;
	for (i = 1; i <= strLevelArrayZ(parrz); ++i)
	{
		p = i + _LOWBIT(i);
		if (p <= strLevelArrayZ(parrz))
			*(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), p - 1) += *(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), i - 1);
	}
}

/* Function name: strInitSparseMatrix
 * Description:   Initialize a sparse matrix.
 * Parameters:
 *       pbmx Pointer to a sparse matrix you want to initialize.
 *         ln Number of lines in the sparse matrix.
 *        col Number of columns in the sparse matrix.
 * Return value:  TRUE  Initialization succeeded.
 *                FALSE Initialization failed.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
BOOL strInitSparseMatrix(P_SPAMAT pmtx, size_t ln, size_t col)
{
	if (NULL != strInitArrayZ(&pmtx->bita, ln, sizeof(size_t)))
		memset(pmtx->bita.pdata, 0, sizeof(size_t) * strLevelArrayZ(&pmtx->bita));
	strInitLinkedListSC(&pmtx->datlst);
	return (NULL != strInitBMap(&pmtx->bmask, ln, col, FALSE));
}

/* Function name: strFreeSparseMatrix
 * Description:   Deallocate a sparse matrix that is allocated by function strInitSparseMatrix.
 * Parameter:
 *      pmtx Pointer to a sparse matrix you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pmtx Must Be Allocated first.
 */
void strFreeSparseMatrix(P_SPAMAT pmtx)
{
	strFreeArrayZ(&pmtx->bita);
	strFreeBMap(&pmtx->bmask);
	strFreeLinkedListSC(&pmtx->datlst);
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
	P_SPAMAT pmtx = (P_SPAMAT) malloc(sizeof(SPAMAT));
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
 *      pdest Pointer to the destination sparse matrix whose content is to be copied.
 *       psrc Pointer to the source of the sparse matrix to be copied.
 *       size Size of each element in both two sparse matrices.
 * Return value:  If duplication succeeded, function would return the same pointer as pdest.
 *                If function returned NULL, it indicated a duplicating failure.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                Destination and source shall not overlap.
 */
P_SPAMAT strCopySparseMatrix(P_SPAMAT pdest, P_SPAMAT psrc, size_t size)
{
	if (NULL != strResizeArrayZ(&pdest->bita, strLevelArrayZ(&psrc->bita), sizeof(size_t)))
	{
		if (NULL != strCopyArrayZ(&pdest->bita, &psrc->bita, size))
		{
			if (NULL != strCopyBMap(&pdest->bmask, &psrc->bmask))
			{
				if (NULL != pdest->datlst)
					strFreeLinkedListSC(&pdest->datlst); /* Free old data chain first. */
				if (NULL != psrc->datlst)
					pdest->datlst = strCopyLinkedListSC(psrc->datlst, size);
				return pdest;
			}
		}
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
	stdiv_t dr = stdiv(ln * pmtx->bmask.col + col + 1, CHAR_BIT);
	if (ln >= pmtx->bmask.ln || col >= pmtx->bmask.col)
		return NULL; /* Over size. */
	else
	{
		REGISTER size_t i, j, l, m;
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
		if (FALSE != (0x01 & (pmtx->bmask.arrz.pdata[l] >> j)))
		{	/* Item exists. */
			REGISTER size_t s = 0;
			P_NODE_S pnode;
			/* Count items. */
			/* This isn't good, because we spent too much time during checking.
			for (i = 0; i < l; ++i)
				if (pmtx->bmask.arrz.pdata[i])
					for (j = 0; j < CHAR_BIT; ++j)
						if ((pmtx->bmask.arrz.pdata[i] >> j) & 0x01)
							++s;
			*/
			s = _strBITLocateItem(l, &pmtx->bita);
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
 * Description:   Set value for a sparse matrix onto a specific position.
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
	stdiv_t dr = stdiv(ln * pmtx->bmask.col + col + 1, CHAR_BIT);
	if (ln >= pmtx->bmask.ln || col >= pmtx->bmask.col)
		return FALSE; /* Over size. */
	else
	{
		REGISTER size_t i, j, l, m, s = 0, z = 0;
		P_NODE_S pnode;
		UCHART t, u;
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
		t = (UCHART)(0x01 << j);
		u = (UCHART)(pmtx->bmask.arrz.pdata[l] >> j);
		/* Count items. */
		/* This isn't good, because we spent too much time during checking.
		for (i = 0; i < l; ++i)
			if (pmtx->bmask.arrz.pdata[i])
				for (j = 0; j < CHAR_BIT; ++j)
					if ((pmtx->bmask.arrz.pdata[i] >> j) & 0x01)
						++s;
		*/
		s = _strBITLocateItem(l, &pmtx->bita);
		/* Count the rest of items. */
		for (i = l, j = 0; j < m; ++j)
			if (pmtx->bmask.arrz.pdata[i] & (_CHAR_SIGN >> j))
				++s, ++z;
		if (FALSE != (0x01 & u))
		{	/* Item exists. */
			if (NULL != (pnode = strLocateItemSC(pmtx->datlst, s)))
			{
				if (NULL != pval && 0 != size)
					return memcpy(pnode->pdata, pval, size); /* Fetch item's data and alter it. */
				else
				{
					if (pmtx->datlst == pnode) /* pnode is the header of data list. */
						pmtx->datlst = pnode->pnode;
					else
						strDeleteNodeS(strRemoveItemLinkedListSC(pmtx->datlst, pnode));
				}
			}
		}
		else if (NULL != pval && 0 != size) /* Insert new item. */
		{
			REGISTER P_NODE_S pnew;
			if (NULL != (pnew = strCreateNodeS(pval, size)))
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
				pmtx->bmask.arrz.pdata[l] = (UCHART)(pmtx->bmask.arrz.pdata[l] | t);
				/* Update Fenwick tree. */
				_strBITUpdateItem(l + 1, 1, &pmtx->bita);
				return pnew->pdata;
			}
		}
		/* Clear bit mask. */
		pmtx->bmask.arrz.pdata[l] = (UCHART)(pmtx->bmask.arrz.pdata[l] & ~t);
	}
	return NULL;
}

#undef _CHAR_SIGN /* Undefine used macro. */

/* Function name: strFillSparseMatrix
 * Description:   Fill a sparse matrix into a common matrix.
 * Parameters:
 *      pdest Pointer to a common matrix you want to flush.
 *       psrc Pointer to a sparse matrix you want to operate.
 *       size Size of each element in the sparse matrix and the common matrix.
 * Return value:  TRUE  Filling succeeded.
 *                FALSE Filling failed.
 * Caution:       Address of pdest and psrc Must Be Allocated first.
 *                (*) Line and column number of the common destination matrix shall
 *                greater than or equal to each value for the source sparse matrix.
 */
BOOL strFillSparseMatrix(P_MATRIX pdest, P_SPAMAT psrc, size_t size)
{
	if ((pdest->ln >= psrc->bmask.ln) && (pdest->col >= psrc->bmask.col))
	{
		REGISTER P_NODE_S pnode = psrc->datlst;
		REGISTER size_t i, j;
		/* Clean destination. */
		memset(pdest->arrz.pdata, 0, strLevelArrayZ(&pdest->arrz) * size);
		for (i = 0; i < psrc->bmask.ln; ++i)
			for (j = 0; j < psrc->bmask.col; ++j)
				if (strGetBitBMap(&psrc->bmask, i, j))
					strSetValueMatrix(pdest, i, j, pnode->pdata, size), pnode = pnode->pnode;
		return TRUE;
	}
	return FALSE;
}

