/*
 * Name:        svctree.c
 * Description: Huffman coding tree.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0914171200J0513250808L00490
 * License:     LGPLv3
 * Copyright (C) 2017-2025 John Cage
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

#include <stdio.h>  /* Using macro BUFSIZ. */
#include <limits.h> /* Using macro UCHAR_MAX, CHAR_BIT. */
#include "svtree.h"

/* A macro that defines symbol table length. */
#define _SMB_TBL_LEN (UCHAR_MAX + 1)

 /* Symbol information of Huffman trees. */
typedef struct _st_SMBINF {
	HFM_SYMBOL Symbol; /* Symbol structure. */
	size_t       freq; /* Symbol frequency. */
} _SMBINF, * _P_SMBINF;

/* Node data of Huffman trees. */
typedef struct _st_HFMNOD {
	P_TNODE_BY  parent;   /* Pointer to parent node. */
	union _ut_NodeData {  /* A union used to store both internal node data and leaf data. */
		size_t    sbfreq; /* Frequency value for internal nodes. */
		_P_SMBINF psbinf; /* Pointer to symbol info in symbol table for leaf nodes. */
		P_HFM_SYMBOL psb; /* Pointer to a HFM_SYMBOL structure in outputted symbol table. */
	} NodeData;
} _HFMNOD, * _P_HFMNOD;

/* File-level function declarations here. */
P_ARRAY_Z  _treHFMCreateSymbolTable          (PUCHAR       s,     size_t       n);
int        _treCBFHFMCompareSymbolFreqInNode (const void * x,     const void * y);
P_TNODE_BY _treHFMBuildHuffmanTree           (P_ARRAY_Z    stbl);
int        _treCBFHFMFillSymbolTable         (void *       pitem, size_t       param);
int        _treCBFHFMCompareSymbolFreq       (const void * x,     const void * y);
P_TNODE_BY _treHFMRebuildHuffmanTree         (P_ARRAY_Z    stbl);

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
	REGISTER P_ARRAY_Z pt = strCreateArrayZ(_SMB_TBL_LEN, sizeof(_SMBINF));
	if (NULL != pt)
	{
		REGISTER size_t i;
		REGISTER _P_SMBINF psi = (_P_SMBINF)pt->pdata;
		/* Initialize the symbol table. */
		for (i = 0; i < _SMB_TBL_LEN; ++i)
		{
			(psi + i)->Symbol.name = (UCHART)i;
			(psi + i)->freq = 0;
		}
		/* Count symbol frequency. */
		for (i = 0; i < n; ++i)
			++(psi + *(s + i))->freq;
	}
	return pt;
}

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
	f0 = (NULL == (*(P_TNODE_BY *)x)->ppnode[LEFT] && NULL == (*(P_TNODE_BY *)x)->ppnode[RIGHT] ?
		((_P_HFMNOD)(*(P_TNODE_BY *)x)->pdata)->NodeData.psbinf->freq :
		((_P_HFMNOD)(*(P_TNODE_BY *)x)->pdata)->NodeData.sbfreq);
	f1 = (NULL == (*(P_TNODE_BY *)y)->ppnode[LEFT] && NULL == (*(P_TNODE_BY *)y)->ppnode[RIGHT] ?
		((_P_HFMNOD)(*(P_TNODE_BY *)y)->pdata)->NodeData.psbinf->freq :
		((_P_HFMNOD)(*(P_TNODE_BY *)y)->pdata)->NodeData.sbfreq);
	if (f0 > f1) return  1;
	if (f0 < f1) return -1;
	return 0;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treHFMBuildHuffmanTree
 * Description:   This function is used to build a Huffman tree.
 * Parameter:
 *      stbl Pointer to a symbol table.
 * Return value:  Pointer to the root node of a new created binary tree.
 * Tip:           You need to free binary tree after coding.
 */
P_TNODE_BY _treHFMBuildHuffmanTree(P_ARRAY_Z stbl)
{
	REGISTER size_t i;
	HEAP_A heap;
	P_TNODE_BY pnode;
	_HFMNOD hfmnode = { 0 };

	treInitHeapA(&heap, _SMB_TBL_LEN, sizeof(P_TNODE_BY));

	/* Create leaf nodes and push them into heap. */
	for (i = 0; i < _SMB_TBL_LEN; ++i)
	{
		_P_SMBINF psi = (_P_SMBINF)strLocateItemArrayZ(stbl, sizeof(_SMBINF), i);
		if (psi->freq)
		{
			P_TNODE_BY ptemp;
			hfmnode.NodeData.psbinf = psi;
			if (NULL == (pnode = strCreateNodeD(&hfmnode, sizeof(_HFMNOD))))
				goto Lbl_Allocation_Error; /* Allocation failure. */
			treInsertHeapA(&heap, &pnode, &ptemp, sizeof(P_TNODE_BY), _treCBFHFMCompareSymbolFreqInNode, FALSE);
		}
	}
	while (heap.irear > 1)
	{
		P_TNODE_BY pnode1, pnode2, ptemp;
		REGISTER _P_HFMNOD phnode1, phnode2;
		/* Pop two node-pointers from heap. */
		treRemoveHeapA(&pnode1, &ptemp, sizeof(P_TNODE_BY), &heap, _treCBFHFMCompareSymbolFreqInNode, FALSE);
		treRemoveHeapA(&pnode2, &ptemp, sizeof(P_TNODE_BY), &heap, _treCBFHFMCompareSymbolFreqInNode, FALSE);

		phnode1 = (_P_HFMNOD)pnode1->pdata;
		phnode2 = (_P_HFMNOD)pnode2->pdata;

		/* Assign new frequency. */
		hfmnode.NodeData.sbfreq =
			(NULL == pnode1->ppnode[LEFT] && NULL == pnode1->ppnode[RIGHT] ? phnode1->NodeData.psbinf->freq : phnode1->NodeData.sbfreq) +
			(NULL == pnode2->ppnode[LEFT] && NULL == pnode2->ppnode[RIGHT] ? phnode2->NodeData.psbinf->freq : phnode2->NodeData.sbfreq);
		hfmnode.parent = NULL;

		/* Create a root node. */
		if (NULL == (ptemp = strCreateNodeD(&hfmnode, sizeof(_HFMNOD))))
			goto Lbl_Allocation_Error;

		/* Assign children. */
		ptemp->ppnode[LEFT]  = pnode1;
		ptemp->ppnode[RIGHT] = pnode2;

		/* Set parents pointer. */
		phnode1->parent = ptemp;
		phnode2->parent = ptemp;

		treInsertHeapA(&heap, &ptemp, &pnode1, sizeof(P_TNODE_BY), _treCBFHFMCompareSymbolFreqInNode, FALSE);
	}
	/* Extract the last node pointer in the heap. */
	trePeepHeapA(&pnode, sizeof(P_TNODE_BY), &heap);
	treFreeHeapA(&heap);
	return pnode;
Lbl_Allocation_Error:
	while (! treIsEmptyHeapA(&heap))
	{	/* Treat every pointer in the heap as a tree. */
		P_TNODE_BY ptemp;
		treRemoveHeapA(&pnode, &ptemp, sizeof(P_TNODE_BY), &heap, _treCBFHFMCompareSymbolFreqInNode, FALSE);
		treFreeBY(&pnode);
	}
	treFreeHeapA(&heap);
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treCBFHFMFillSymbolTable
 * Description:   Used to build symbol table from a Huffman tree.
 * Parameters:
 *      pitem Pointer to each node in the Huffman tree.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _treCBFHFMFillSymbolTable(void * pitem, size_t param)
{
	/* Current node is an external node. */
	REGISTER P_TNODE_BY pnode = (P_TNODE_BY)pitem;
	if (NULL == pnode->ppnode[LEFT] && NULL == pnode->ppnode[RIGHT])
	{	/* Node is a leaf. */
		REGISTER P_TNODE_BY parent = ((_P_HFMNOD)pnode->pdata)->parent;
		REGISTER _P_SMBINF psi = (_P_SMBINF)((_P_HFMNOD)pnode->pdata)->NodeData.psbinf;
		if (NULL != parent)
		{	/* Clear frequency. Get ready for bits assigning. */
			psi->Symbol.bits = 0;
			psi->Symbol.sgnb = 0;
			while (NULL != parent)
			{
				/* Padding bit one onto value. */
				if (parent->ppnode[RIGHT] == pnode)
					psi->Symbol.sgnb |= (size_t)(1 << psi->Symbol.bits);
				/* Increase number of bits. */
				++psi->Symbol.bits;
				/* Propagate upward to root. */
				pnode = parent;
				parent = ((_P_HFMNOD)pnode->pdata)->parent;
			}
		}
		else
		{	/* Set unique symbol for one-root-node-tree. */
			psi->Symbol.bits = 1;
			psi->Symbol.sgnb = 0;
		}
		DWC4100(param); /* Disable warning. */
	}
	return CBF_CONTINUE;
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

/* Function name: treCreateHuffmanTable
 * Description:   Create a Huffman encoding symbol table.
 * Parameters:
 *          s The buffer you want to encode.
 *          n Number of symbols in the buffer. The unit of n is sizeof(unsigned char).
 * Return value:  Pointer to a new created sized array.
 *                Each element in the sized array that this function returned is a HFM_SYMBOL structure.
 *                If any error occurred during encoding, function would be interrupted and return a NULL.
 * Tip:           Symbol table is important for decoding. You may need to store the symbol table onto an external disk.
 */
P_ARRAY_Z treCreateHuffmanTable(const PUCHAR s, const size_t n)
{
	REGISTER P_ARRAY_Z stbl = _treHFMCreateSymbolTable(s, n);
	REGISTER P_ARRAY_Z otbl = NULL;
	if
	(	/* Both buffer and its length are not empty. */
		NULL != s && n != 0 &&
		NULL != stbl
	)
	{
		P_TNODE_BY proot = _treHFMBuildHuffmanTree(stbl);
		if (NULL != proot)
		{
			REGISTER P_HFM_SYMBOL ptbl;
			REGISTER size_t i;

			/* Traverse the whole Huffman tree to fill symbol table. */
			treTraverseBYPre(proot, _treCBFHFMFillSymbolTable, 0);

			/* Delete Huffman tree. Tree is used to generate table. Drop it directly after generating. */
			treFreeBY(&proot);
			
			/* Sort symbol table by its frequency if possible, so that we can retrieve them much faster in the next decoding time. */
			strSortArrayZ(stbl, sizeof(_SMBINF), _treCBFHFMCompareSymbolFreq);
			
			/* Convert symbol table. */
			if (NULL != (otbl = strCreateArrayZ(strLevelArrayZ(stbl), sizeof(HFM_SYMBOL))))
				for (i = 0, ptbl = (P_HFM_SYMBOL)otbl->pdata; i[(_P_SMBINF)stbl->pdata].freq && i < _SMB_TBL_LEN; ++i)
					*(ptbl++) = i[(_P_SMBINF)stbl->pdata].Symbol; /* Copy a structure once a time. */
			else
				goto Lbl_Failed;
			/* Resize output table. */
			strResizeArrayZ(otbl, i, sizeof(HFM_SYMBOL));
		}
	}
Lbl_Failed:
	/* Delete the original large table. */
	if (NULL != stbl)
		strDeleteArrayZ(stbl);
	return otbl;
}

/* Function name: treHuffmanEncoding
 * Description:   Huffman encoding algorithm.
 * Parameters:
 *     ptable Input a pointer to a symbol table.
 *            Call function treCreateHuffmanTable to generate a symbol table.
 *            Each element in the array that ptable pointed is a HFM_SYMBOL structure.
 *          s The buffer you want to encode.
 *          n Number of elements in the buffer. The unit of n is sizeof(unsigned char).
 * Return value:  Pointer to a new created bit-stream. This bit-stream stores encoded string.
 *                If any error occurred during encoding, 
 *                function would be interrupted and return a NULL pointer.
 * Tip:           Symbol table is important for decoding. You may need to store the symbol table onto an external disk.
 *                This function can encode sub string but use the symbol table of a whole string.
 * Usage:         // #include <stdio.h> // Invoke function printf.
 *                // #define STR1 "This is a test, and that is another test."
 *                // #define STR2 "This" // Sub-string.
 *                // P_ARRAY_Z par = treCreateHuffmanTable(STR1, strlen(STR1) + 1);
 *                // P_BITSTREAM pbi = treHuffmanEncoding(par, STR2, strlen(STR2) + 1);
 *                // P_BITSTREAM pbo = treHuffmanDecoding(par, pbi);
 *                // printf("%s\n", pbo->arrz.pdata);	
 *                // strDeleteBitStream(pbi);
 *                // strDeleteBitStream(pbo);
 *                // strDeleteArrayZ(par);
 *                Result: This
 */
P_BITSTREAM treHuffmanEncoding(P_ARRAY_Z ptable, const PUCHAR s, const size_t n)
{
	if (NULL != s && n != 0 && NULL != ptable)
	{	/* Both buffer and it's length are not empty. */
		REGISTER P_BITSTREAM pbstm = strCreateBitStream();
		REGISTER P_ARRAY_Z   pltbl = strCreateArrayZ(_SMB_TBL_LEN, sizeof(_SMBINF));
		if
		(
			NULL != pbstm &&
			NULL != pltbl
		)
		{
			REGISTER size_t i, j;
			REGISTER _P_SMBINF psi;
			REGISTER P_HFM_SYMBOL pstbl;
			
			/* Expand symbol table. */
			for (i = 0; i < strLevelArrayZ(ptable); ++i)
			{
				pstbl = (P_HFM_SYMBOL)strLocateItemArrayZ(ptable, sizeof(HFM_SYMBOL), i);
				psi   = (_P_SMBINF)strLocateItemArrayZ(pltbl, sizeof(_SMBINF), (size_t)pstbl->name);
				psi->Symbol = *pstbl;
			}
			
			/* Compress. */
			for (i = 0; i < n; ++i)
			{
				psi = (_P_SMBINF)strLocateItemArrayZ(pltbl, sizeof(_SMBINF), s[i]);
				/* Fill bit-stream. */
				for (j = 1; j <= psi->Symbol.bits; ++j)
					strBitStreamAdd(pbstm, ((size_t)1 << (psi->Symbol.bits - j)) & psi->Symbol.sgnb);
			}
		}
		
		/* Cannot create large table. */
		if (NULL == pltbl && NULL != pbstm)
		{
			strDeleteBitStream(pbstm);
			pbstm = NULL;
		}
		
		/* Delete the large table. */
		if (NULL != pltbl)
			strDeleteArrayZ(pltbl);
		
		return pbstm;
	}
	/* Cannot find appropriate symbol table. */
	return NULL;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _treHFMRebuildHuffmanTree
 * Description:   This function is used to build a Huffman tree from symbol table.
 * Parameter:
 *      stbl Pointer to a symbol table array.
 * Return value:  Pointer to the root node of a new created binary tree.
 * Tip:           You need to free binary tree after decoding.
 */
P_TNODE_BY _treHFMRebuildHuffmanTree(P_ARRAY_Z stbl)
{
	P_TNODE_BY proot;
	_HFMNOD hfmnode = { 0 };
	REGISTER size_t i, j, k;
	REGISTER P_HFM_SYMBOL psmb;
	REGISTER P_TNODE_BY pnode, pnew;

	/* Create a root node. */
	if (NULL == (pnode = proot = strCreateNodeD(&hfmnode, sizeof(_HFMNOD))))
		return NULL; /* Allocation failure. */

	/* Scan symbol table. */
	for (i = 0; i < stbl->num; ++i)
	{
		psmb = (P_HFM_SYMBOL)strLocateItemArrayZ(stbl, sizeof(HFM_SYMBOL), i);
		for (j = 1; j <= psmb->bits; ++j)
		{
			if (j == psmb->bits) /* Searching reaches at leaf node. */
				hfmnode.NodeData.psb = psmb;
			else
				hfmnode.NodeData.sbfreq = 0;
			hfmnode.parent = pnode;

			k = (0 != (((size_t)1 << (psmb->bits - j)) & psmb->sgnb));
			if (NULL != pnode->ppnode[k])
				pnew = pnode->ppnode[k];
			else
			{
				if (NULL == (pnew = strCreateNodeD(&hfmnode, sizeof(_HFMNOD))))
					goto Lbl_Building_Failed; /* Allocation failure. */
				pnode->ppnode[k] = pnew;
			}

			pnode = pnew;
		}
		pnode = proot;
	}
	return proot;
Lbl_Building_Failed:
	treFreeBY(&proot);
	return NULL;
}

/* Function name: treHuffmanDecoding
 * Description:   Huffman decoding algorithm.
 * Parameters:
 *     ptable Pointer to a symbol table which is used to decode.
 *            Each element in the array that ptable pointed is a HFM_SYMBOL structure.
 *          s The encoded bit-stream you want to decode.
 * Return value:  Pointer to a new created bit-stream. This bit-stream stores the decoded string.
 *                If any error occurred during decoding, function would be interrupted and return a NULL.
 * Caution:       ptable must NOT be value NULL.
 *                (*) if users called this function, parameter bit-stream s would be altered to empty.
 * Tip:           You could get a symbol table from invoking function treHuffmanEncoding.
 *                A symbol table will output from the parameter pptable of function treHuffmanEncoding.
 *                You may either get a bit-stream as the parameter of function treHuffmanDecoding from
 *                the return value of function treHuffmanEncoding.
 */
P_BITSTREAM treHuffmanDecoding(P_ARRAY_Z ptable, P_BITSTREAM s)
{
	if (NULL != s && NULL != ptable && strLevelArrayZ(ptable) > 0)
	{
		P_BITSTREAM pbout = strCreateBitStream();
		if (NULL != pbout)
		{
			size_t i = 0, j = 0;
			P_TNODE_BY proot, pnode;
			if (NULL == (pnode = proot = _treHFMRebuildHuffmanTree(ptable)))
				goto Lbl_Decoding_Failure;
			for ( ;; )
			{
				if (NULL != pnode)
				{
					if (NULL == pnode->ppnode[LEFT] && NULL == pnode->ppnode[RIGHT]) /* Searching reaches at a leaf node. */
					{
						if (i >= strLevelArrayZ(&pbout->arrz))
						{	/* Increase the length of bit-stream. */
							if (NULL == strResizeArrayZ(&pbout->arrz, strLevelArrayZ(&pbout->arrz) + BUFSIZ, sizeof(UCHART)))
								goto Lbl_Decoding_Failure;
						}
						/* Assign symbol to bit-stream. */
						pbout->arrz.pdata[i] = ((_P_HFMNOD)pnode->pdata)->NodeData.psb->name;
						pnode = proot;
						++i;
						continue;
					}
					if (j >= (s->arrz.num - 1) * CHAR_BIT + s->bilc)
						break;
					if (strBitStreamLocate(s, j++))
						pnode = pnode->ppnode[RIGHT];
					else
						pnode = pnode->ppnode[LEFT];
				}
				else
				{
					treFreeBY(&proot);
					goto Lbl_Decoding_Failure; /* Can not find symbol. */
				}
			}
			if (NULL == strResizeArrayZ(&pbout->arrz, i, sizeof(UCHART)))
			{
				if (NULL != pbout)
					strDeleteBitStream(pbout);
				pbout = NULL; /* Can not resize bit-stream. */
			}
			treFreeBY(&proot);
			return pbout;
	Lbl_Decoding_Failure:
			strDeleteBitStream(pbout);
		}
	}
	return NULL; /* No symbol table. */
}

