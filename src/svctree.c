/*
 * Name:        svctree.c
 * Description: Huffman coding tree.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0914171200J0619191031L00494
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
#include <limits.h> /* Using macro CHAR_BIT. */
#include "svtree.h"

/* Symbol information of Huffman trees. The length of this structure is sizeof(size_t[2]). */
typedef struct _st_SMBINF { /* This union is used to align structures in language level. */
	union _ut_SymbolCombo { /* Most compilers will automatically align this structure.   */
		HFM_SYMBOL Symbol;  /* Symbol structure. */
		size_t _alignment;  /* This integer is used to align the above struct to size_t. */
	} SymbolCombo;          /* Alignment worked if sizeof(HFM_SYMBOL) <= sizeof(size_t). */
	size_t freq;            /* Symbol frequency. */
} _SMBINF, * _P_SMBINF;

/* Node data of Huffman trees. */
typedef struct _st_HFMNOD {
	P_TNODE_B  parent;    /* Pointer to parent node. */
	union _ut_NodeData {  /* A union used to store both internal node data and leaf data. */
		size_t    sbfreq; /* Frequency value for internal nodes. */
		_P_SMBINF psbinf; /* Pointer to symbol info in symbol table for leaf nodes. */
	} NodeData;
} _HFMNOD, * _P_HFMNOD;

/* File-level function declarations here. */
int       _treCBFHFMCompareSymbolFreqInNode (const void * x,     const void * y);
int       _treCBFHFMCompareSymbolFreq       (const void * x,     const void * y);
int       _treCBFHFMCompareSymbolName       (const void * x,     const void * y);
int       _treCBFHFMCompareKey              (const void * x,     const void * y);
int       _treCBFHFMFindSymbol              (void *       pitem, size_t       param);
int       _treCBFHFMFillSymbolTable         (void *       pitem, size_t       param);
P_ARRAY_Z _treHFMCreateSymbolTable          (PUCHAR       s,     size_t       n);
P_BTREE   _treHFMBuildHuffmanTree           (P_ARRAY_Z    stbl);
BOOL      _treHFMGetBitFromStream           (P_BITSTREAM  pbsref);

/* This macro detects whether the dummy bit-stream pbsref reaches at the end of it. */
#define _treHFMAtEndOfBitStream(pbstm, pbsref) \
	((strLevelArrayZ(&(pbsref)->arrz) == strLevelArrayZ(&(pbstm)->arrz)) && ((pbsref)->bilc == (pbstm)->bilc))

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMCompareSymbolFreqInNode
 * Description:   Compare symbol by its frequency between nodes.
 * Parameters:
 *          x Pointer to any node pointer in array. Casted into (void *).
 *          y Pointer to any node pointer in array. Casted into (void *).
 * Return value:  Please refer to the type definition of CBF_COMPARE in svdef.h.
 */
int _treCBFHFMCompareSymbolFreqInNode(const void * x, const void * y)
{
	REGISTER size_t f0, f1;
	f0 = (NULL == (*(P_TNODE_B *)x)->ppnode[LEFT] && NULL == (*(P_TNODE_B *)x)->ppnode[RIGHT] ?
		 ((_P_HFMNOD)(*(P_TNODE_B *)x)->pdata)->NodeData.psbinf->freq :
		 ((_P_HFMNOD)(*(P_TNODE_B *)x)->pdata)->NodeData.sbfreq);
	f1 = (NULL == (*(P_TNODE_B *)y)->ppnode[LEFT] && NULL == (*(P_TNODE_B *)y)->ppnode[RIGHT] ?
		 ((_P_HFMNOD)(*(P_TNODE_B *)y)->pdata)->NodeData.psbinf->freq :
		 ((_P_HFMNOD)(*(P_TNODE_B *)y)->pdata)->NodeData.sbfreq);
	if (f0 > f1) return  1;
	if (f0 < f1) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMCompareSymbolFreq
 * Description:   Compare symbol by its frequency.
 * Parameters:
 *          x Pointer to any symbol in array. And casted it into (void *).
 *          y Pointer to any symbol in array. And casted it into (void *).
 * Return value:  Please refer to the type definition of CBF_COMPARE in svdef.h.
 */
int _treCBFHFMCompareSymbolFreq(const void * x, const void * y)
{
	/* Sort frequency from high to low. This would speed up retrieving. */
	if (((_P_SMBINF)x)->freq < ((_P_SMBINF)y)->freq) return  1;
	if (((_P_SMBINF)x)->freq > ((_P_SMBINF)y)->freq) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMFillSymbolTable
 * Description:   Compare symbol by its name.
 * Parameters:
 *          x Pointer to any symbol in an array. Casted into (void *).
 *          y Pointer to any symbol in an array. Casted into (void *).
 * Return value:  Please refer to the type definition of CBF_COMPARE in svdef.h.
 */
int _treCBFHFMCompareSymbolName(const void * x, const void * y)
{
	if (((_P_SMBINF)x)->SymbolCombo.Symbol.name > ((_P_SMBINF)y)->SymbolCombo.Symbol.name) return  1;
	if (((_P_SMBINF)x)->SymbolCombo.Symbol.name < ((_P_SMBINF)y)->SymbolCombo.Symbol.name) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMCompareKey
 * Description:   This function is used to do binary search in a symbol array.
 * Parameters:
 *          x Pointer to a size_t variable which stores the key value. And casted it into (void *).
 *          y Pointer to any symbol in an array. Casted into (void *).
 * Return value:  Please refer to the type definition of CBF_COMPARE in svdef.h.
 */
int _treCBFHFMCompareKey(const void * x, const void * y)
{
	if (*(PUCHAR)x > ((_P_SMBINF)y)->SymbolCombo.Symbol.name) return  1;
	if (*(PUCHAR)x < ((_P_SMBINF)y)->SymbolCombo.Symbol.name) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMFindSymbol
 * Description:   This function is used to find symbol int array.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param Key value casted into type size_t.
 * Return value:  If the specific symbol were fond in the array, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _treCBFHFMFindSymbol(void * pitem, size_t param)
{
	if ((size_t)(((_P_SMBINF)pitem)->SymbolCombo.Symbol.name) == param)
	{
		++((_P_SMBINF)pitem)->freq; /* Increase frequency. */
		return CBF_TERMINATE; /* Match. */
	}
	return CBF_CONTINUE; /* Not match. */
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMFillSymbolTable
 * Description:   Used to build symbol table from a Huffman tree.
 * Parameters:
 *      pitem Pointer to each node in the tree.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFHFMFillSymbolTable(void * pitem, size_t param)
{
	/* Current node is an external node. */
	REGISTER P_TNODE_B pnode = (P_TNODE_B) pitem;
	if (NULL == pnode->ppnode[LEFT] && NULL == pnode->ppnode[RIGHT])
	{	/* Node is a leaf. */
		REGISTER P_TNODE_B parent = ((_P_HFMNOD) pnode->pdata)->parent;
		REGISTER _P_SMBINF psi = (_P_SMBINF) ((_P_HFMNOD) pnode->pdata)->NodeData.psbinf;
		if (NULL != parent)
		{	/* Clear frequency. Get ready for bits assigning. */
			psi->SymbolCombo.Symbol.bits = psi->SymbolCombo.Symbol.sgnb = 0;
			while (NULL != parent)
			{
				psi->SymbolCombo.Symbol.sgnb <<= 1;
				/* Padding bit one onto value. */
				if (parent->ppnode[RIGHT] == pnode)
					++psi->SymbolCombo.Symbol.sgnb;
				/* Increase number of bits. */
				++psi->SymbolCombo.Symbol.bits;
				/* Propagate upward to root. */
				pnode = parent;
				parent = ((_P_HFMNOD)pnode->pdata)->parent;
			}
		}
		else
		{	/* Set unique symbol for one-root-node-tree. */
			psi->SymbolCombo.Symbol.bits = 1;
			psi->SymbolCombo.Symbol.sgnb = 0;
		}
		DWC4100(param); /* Disable warning. */
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treHFMCreateSymbolTable
 * Description:   This function is used to create a symbol by frequency.
 * Parameters:
 *          s Pointer to the buffer you want to coding with.
 *          n Length of the buffer.
 * Return value:  Pointer to a new created array.
 * Tip:           You need to free array after using.
 */
P_ARRAY_Z _treHFMCreateSymbolTable(PUCHAR s, size_t n)
{
	REGISTER P_ARRAY_Z pt = (P_ARRAY_Z) malloc(sizeof(ARRAY_Z));
	if (NULL != pt)
	{
		REGISTER size_t i;
		REGISTER UCHART k;
		REGISTER _P_SMBINF psi;
		pt->num = 0;
		pt->pdata = NULL;
		for (i = 0; i < n; ++i)
		{
			if (CBF_CONTINUE == strTraverseArrayZ(pt, sizeof(_SMBINF), _treCBFHFMFindSymbol, k = *(s + i), FALSE))
			{	/* Symbol can not be found. */
				if (NULL == strResizeArrayZ(pt, strLevelArrayZ(pt) + 1, sizeof(_SMBINF)))
				{
					strDeleteArrayZ(pt);
					return NULL; /* Allocation failure. */
				}
				psi = &(strLevelArrayZ(pt) - 1)[(_P_SMBINF)pt->pdata];
				psi->SymbolCombo.Symbol.name = k;
				psi->freq = 1;
			}
		}
	}
	return pt;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treHFMBuildHuffmanTree
 * Description:   This function is used to build a Huffman tree.
 * Parameter:
 *      stbl Pointer to a symbol table.
 * Return value:  Pointer to a new created binary tree.
 * Tip:           You need to free binary tree after coding.
 */
P_BTREE _treHFMBuildHuffmanTree(P_ARRAY_Z stbl)
{
	P_BTREE ptree = (P_BTREE) malloc(sizeof(BTREE));
	if (NULL != ptree)
	{
		ARRAY_Z arrtier;
		REGISTER size_t i;
		REGISTER P_TNODE_B pnode;
		/* Create an array which is used to store tree nodes. */
		if (NULL == strInitArrayZ(&arrtier, strLevelArrayZ(stbl), sizeof(P_TNODE_B)))
		{	/* Allocation failure. */
			free(ptree);
			return NULL;
		}
		/* Create leaf node for each symbol and insert them into array. */
		for (*(P_TNODE_B *)arrtier.pdata = NULL, i = 0; i < strLevelArrayZ(stbl); ++i)
		{
			if (NULL == (pnode = strCreateNodeD(NULL, sizeof(_HFMNOD))))
				goto Lbl_Allocation_Error;
			/* Put the entry of a symbol into union.  */
			((_P_HFMNOD)pnode->pdata)->NodeData.psbinf = &i[(_P_SMBINF)stbl->pdata];
			((_P_HFMNOD)pnode->pdata)->parent = NULL; /* Parent is not assigned yet. */
			i[(P_TNODE_B *)arrtier.pdata] = pnode;    /* Add node into array. */
			/* To prevent freeing an invalid pointer. */
			if (i < strLevelArrayZ(stbl) - 1)
				(i + 1)[(P_TNODE_B *)arrtier.pdata] = NULL;
		}
		/* Build a Huffman tree from here. */
		strSortArrayZ(&arrtier, sizeof(P_TNODE_B), _treCBFHFMCompareSymbolFreqInNode);
		while (arrtier.num > 1)
		{	/* Extract the first two nodes from array. And combine them to a common parent. */
			REGISTER P_TNODE_B * pn;
			if (NULL == (pnode = strCreateNodeD(NULL, sizeof(_HFMNOD))))
				goto Lbl_Allocation_Error;
			pn = (P_TNODE_B *)arrtier.pdata;
			pnode->ppnode[LEFT]  = pn[0];
			pnode->ppnode[RIGHT] = pn[1];
			/* Alter frequency value in parent node. */
			((_P_HFMNOD)pnode->pdata)->NodeData.sbfreq =
				(NULL == pn[0]->ppnode[LEFT] && NULL == pn[0]->ppnode[RIGHT] ?
				((_P_HFMNOD)pn[0]->pdata)->NodeData.psbinf->freq :
				((_P_HFMNOD)pn[0]->pdata)->NodeData.sbfreq);
			((_P_HFMNOD)pnode->pdata)->NodeData.sbfreq +=
				(NULL == pn[1]->ppnode[LEFT] && NULL == pn[1]->ppnode[RIGHT] ?
				((_P_HFMNOD)pn[1]->pdata)->NodeData.psbinf->freq :
				((_P_HFMNOD)pn[1]->pdata)->NodeData.sbfreq);
			/* Assign parent for two children node. */
			((_P_HFMNOD)pn[0]->pdata)->parent = ((_P_HFMNOD)pn[1]->pdata)->parent = pnode;
			/* Find a position to insert parent in array. */
			pn[1] = pnode;
			/* It ran faster, if we didn't shrink array. */
			strRemoveItemArrayZ(&arrtier, sizeof(P_TNODE_B), 0, FALSE);
			/* Sort array. */
			for (i = 0; i < arrtier.num - 1; ++i)
			{
				pn = &i[(P_TNODE_B *)arrtier.pdata];
				if (_treCBFHFMCompareSymbolFreqInNode(pn, (pn + 1)) > 0)
				{	/* Switch adjacent nodes. */
					pnode = *pn;
					*pn = *(pn + 1);
					*(pn + 1) = pnode;
				}
			}
		}
		goto Lbl_Build_Succeeded;
	Lbl_Allocation_Error:
		/* Do cleanup. */
		for (i = 0; i < arrtier.num; ++i)
		{	/* Treat every pointer in array as a tree. */
			*ptree = i[(P_TNODE_B *)arrtier.pdata];
			if (NULL != *ptree)
				treFreeB(ptree);
			else
				break;
		}
		strFreeArrayZ(&arrtier);
		free(ptree);
		return NULL;
	Lbl_Build_Succeeded:
		/* The last item left in array is the pointer to root node. And the root's parent is nil. */
		((_P_HFMNOD)(*ptree = *(P_TNODE_B *)arrtier.pdata)->pdata)->parent = NULL;
		strFreeArrayZ(&arrtier); /* Erase array. */
		/* Traverse the whole Huffman tree to fill symbol table. */
		treTraverseBPre(*ptree, _treCBFHFMFillSymbolTable, 0);
		/* Alter symbols in table. */
		for (i = 0; i < strLevelArrayZ(stbl); ++i)
		{	/* This procedure is used to reverse bits for symbol code. */
			REGISTER UCHART k;
			REGISTER size_t j;
			for (k = 0, j = 0; j < i[(_P_SMBINF)stbl->pdata].SymbolCombo.Symbol.bits; ++j)
			{
				k <<= 1;
				if (((UCHART) 0x01 << (UCHART) j) & i[(_P_SMBINF)stbl->pdata].SymbolCombo.Symbol.sgnb)
					++k;
			}
			i[(_P_SMBINF)stbl->pdata].SymbolCombo.Symbol.sgnb = k;
		}
	}
	return ptree;
}

/* Function name: treHuffmanEncoding
 * Description:   Huffman encoding algorithm.
 * Parameters:
 *    pptable Pointer to a pointer of a symbol table.
 *            This parameter is used to output a symbol table after a Huffman tree is built.
 *            If you do not need this symbol table, assign a NULL for this parameter.
 *            Recommend you to store this symbol table for decoding later.
 *            Each element in the array that (*pptable) pointed is a HFM_SYMBOL structure.
 *          s The buffer you want to encode.
 *            Acceptable capacity of s ranges from 1 byte to 4 MB.
 *            Stream whose capacity is larger than 8 MB is difficult to treat.
 *          n Number of symbols in the buffer. The unit of n is sizeof(unsigned char).
 * Return value:  Pointer to a new created bit-stream. This bit-stream stores encoded string.
 *                If any error occurred during encoding, function would be interrupted and return a NULL,
 *                and no symbol table would output into the parameter pptable,
 *                that is function would assign a NULL value into (*pptable).
 * Tip:           Symbol table is important for decoding. You may need to store the symbol table onto an external disk.
 */
P_BITSTREAM treHuffmanEncoding(P_ARRAY_Z * pptable, const PUCHAR s, const size_t n)
{
	REGISTER P_ARRAY_Z stbl;
	P_BITSTREAM pbstm;
	if (NULL != s && n != 0 && /* Both buffer and its length are not empty. */
		NULL != (pbstm = strCreateBitStream()) &&
		NULL != (stbl  = _treHFMCreateSymbolTable(s, n))
		)
	{
		P_BTREE ptree;
		if (NULL != (ptree = _treHFMBuildHuffmanTree(stbl)))
		{
			REGISTER _P_SMBINF psi;
			REGISTER size_t i, j;
			/* Delete Huffman tree. Tree is used to generate table. Drop it directly after generating. */
			treDeleteB(ptree);
			/* Sort array by symbol name. So that we can retrieve them much faster. */
			strSortArrayZ(stbl, sizeof(_SMBINF), _treCBFHFMCompareSymbolName);
			for (i = 0; i < n; ++i)
			{	/* Binary search symbol table. */
				psi = (_P_SMBINF) strBinarySearchArrayZ(stbl, &s[i], sizeof(_SMBINF), _treCBFHFMCompareKey);
				if (NULL == psi) /* Error occurred while program can not find key in table. */
					goto Lbl_Building_Failed;
				else /* Filling bit-stream. */
					for (j = 1; j <= psi->SymbolCombo.Symbol.bits; ++j)
						strBitStreamAdd(pbstm, ((UCHART) 0x01 << (psi->SymbolCombo.Symbol.bits - j)) & psi->SymbolCombo.Symbol.sgnb);
			}
			if (NULL != pptable)
			{
				P_HFM_SYMBOL ptbl;
				P_ARRAY_Z otbl = NULL;
				/* Sort symbol table by its frequency if possible, so that we can retrieve them much faster in the next decoding time. */
				strSortArrayZ(stbl, sizeof(_SMBINF), _treCBFHFMCompareSymbolFreq);
				/* Convert symbol table. */
				if (NULL != (otbl = strCreateArrayZ(strLevelArrayZ(stbl), sizeof(HFM_SYMBOL))))
					for (i = 0, ptbl = (P_HFM_SYMBOL)otbl->pdata; i < strLevelArrayZ(stbl); ++i)
						*(ptbl++) = i[(_P_SMBINF)stbl->pdata].SymbolCombo.Symbol; /* Copy 3 bytes. */
				/* Delete the original large table. */
				strDeleteArrayZ(stbl);
				*pptable = otbl; /* Output small table. */
			}
			return pbstm;
		}
Lbl_Building_Failed: /* Can not build a Huffman tree. */
		strDeleteBitStream(pbstm);
		strDeleteArrayZ(stbl);
		if (NULL != ptree) /* Delete Huffman tree if needed. */
			treDeleteB(ptree);
		if (NULL != pptable) /* Output a null table. */
			*pptable = NULL;
	}
	return NULL; /* There're no symbols in table. */
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treHFMGetBitFromStream
 * Description:   This function fetches a bit from a bit-stream but not modify it.
 * Parameter:
 *    pbsref Pointer to the dummy bit-stream you want to operate with.
 * Return value:  Either TRUE or FALSE would return.
 */
BOOL _treHFMGetBitFromStream(P_BITSTREAM pbsref)
{
	REGISTER BOOL r = ((UCHART) 0x01 << (CHAR_BIT - pbsref->bilc)) & (*pbsref->arrz.pdata);
	if (++pbsref->bilc > CHAR_BIT)
	{
		pbsref->bilc = 1; /* Reset current bit position. */
		++pbsref->arrz.num; /* Increase integer block. */
		pbsref->arrz.pdata += 1; /* Move integer 1 block forward. */
	}
	return r;
}

/* Function name: treHuffmanDecoding
 * Description:   Huffman decoding algorithm.
 * Parameters:
 *     ptable Pointer to a symbol table which is used to decode.
 *            Each element in the array that ptable pointed is a HFM_SYMBOL structure.
 *          s The encoded bit-stream you want to decode.
 *            Acceptable capacity of s ranges from 1 byte to 4 MB.
 *            Stream whose capacity is larger than 8 MB is difficult to treat.
 * Return value:  Pointer to a new created bit-stream. This bit-stream stores the decoded string.
 *                If any error occurred during decoding, function would be interrupted and return a NULL.
 * Caution:       ptable must NOT be value NULL.
 * Tip:           You could get a symbol table from invoking function treHuffmanEncoding.
 *                A symbol table will output from the parameter pptable of function treHuffmanEncoding.
 *                You may either get a bit-stream as the parameter of function treHuffmanDecoding from
 *                the return value of function treHuffmanEncoding.
 *                To free bit-stream s is required after invoke this function.
 */
P_BITSTREAM treHuffmanDecoding(P_ARRAY_Z ptable, P_BITSTREAM s)
{
	P_BITSTREAM pbout;
	if (NULL != s && NULL != ptable && strLevelArrayZ(ptable) > 0 &&
		NULL != (pbout = strCreateBitStream())
		)
	{
		BITSTREAM bsref;
		REGISTER size_t i;
		REGISTER P_HFM_SYMBOL psbl;
		REGISTER UCHART j = 0, k = 0;
		/* Here, we manually manage bit-stream for the
		 * reason that we need a better performance.
		 */
		/* Initialize bit-stream structure. */
		pbout->arrz.num = 0;
		pbout->arrz.pdata = NULL;
		/* Initialize number of bits. */
		pbout->bilc = CHAR_BIT;
		/* Set a dummy bit-stream header. */
		bsref.bilc = 1;
		bsref.arrz.num = 0;
		bsref.arrz.pdata = s->arrz.pdata;
		while (! _treHFMAtEndOfBitStream(s, &bsref))
		{
			k <<= 1; /* Left shift 1. */
			/* Pop a bit from bit-stream and fill it into temporary char. */
			if (_treHFMGetBitFromStream(&bsref))
				++k; /* Padding LSB with value 1. */
			++j; /* Increase number of bits. */
			/* Search k as a symbol in the table. */
			for (i = 0; i < strLevelArrayZ(ptable); ++i)
			{
				psbl = &i[(P_HFM_SYMBOL)ptable->pdata];
				if (psbl->bits == j && psbl->sgnb == k)
				{	/* Found symbol code. Then fill bitstream. */
					if (NULL == strResizeArrayZ(&pbout->arrz, pbout->arrz.num + 1, sizeof(UCHART)))
						goto Lbl_Allocation_Failure; /* Increase the length of bit-stream. */
					/* Assign symbol to bit-stream. */
					pbout->arrz.pdata[pbout->arrz.num - 1] = psbl->name;
					k = j = 0; /* Clear key and counter. */
					break;
				}
			}
			if (j > CHAR_BIT) /* Too long to fit a symbol. */
			{	/* Symbol in the buffer doesn't fit symbol in the table at all. */
Lbl_Allocation_Failure:
				strDeleteBitStream(pbout);
				goto Lbl_Symbol_Table_Corrupted;
			}
		}
		return pbout;
	}
Lbl_Symbol_Table_Corrupted:
	return NULL; /* No symbol table. */
}
