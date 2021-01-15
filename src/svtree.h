/*
 * Name:        svtree.h
 * Description: Trees interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0809171737V0115211440L00446
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

#ifndef _SVTREE_H_
#define _SVTREE_H_

#include "svstring.h"

/* The following two macros are used to direct nodes for TNODE_B structures in binary trees. */
#define LEFT  PREV
#define RIGHT NEXT

/* Types for binary trees. */
typedef NODE_D     TNODE_B;   /* Node for binary trees. */
typedef P_NODE_D   P_TNODE_B; /* Pointer of tree nodes. */
typedef P_NODE_D   BTREE;     /* Binary tree. */
typedef P_NODE_D * P_BTREE;   /* Pointer to a binary tree. */

/* Align a NODE_D structure on the head of BSTNODE structure,
 * then put the rest of node's information such as parent and param after NODE_D structure,
 * so that we are able to traverse a binary tree and cope with a BSTNODE structure simultaneously.
 */
typedef struct st_BSTNODE { /* Binary node with addtional information. */
	TNODE_B knot; /* Left and right children and data pointer. */
	size_t param; /* Parameter for a treap node. */
} BSTNODE, * P_BSTNODE, * BST, ** P_BST;

#define P2P_TNODE_B(pnode) ((P_TNODE_B)(pnode)) /* Cast a pointer to P_TNODE_B. */
#define P2P_BSTNODE(pnode) ((P_BSTNODE)(pnode)) /* Cast a pointer to P_BSTNODE. */

/* Generic tree. */
typedef struct st_TNODE_G {
	ARRAY_Z children; /* An array for children pointers. */
	PUCHAR  pdata;    /* Data pointer of a node. */
} TNODE_G, * P_TNODE_G, * GTREE, ** P_GTREE;

/* Heap tree using fixed array. */
typedef struct st_HEAP_A {
	ARRAY_Z hdarr; /* Heap data array. */
	size_t  irear; /* Index of the last element. */
} HEAP_A, * P_HEAP_A;

/* Types for B-plus trees. */
typedef NODE_D     BPTNODE;
typedef P_NODE_D   P_BPTNODE;
typedef P_NODE_D   BPT;
typedef P_NODE_D * P_BPT;

/* An enumeration for tree traversal methods. */
typedef enum en_TvsMtd {
	ETM_PREORDER     = 001, /* Pre-order. */
	ETM_INORDER      = 002, /* In-order. */
	ETM_POSTORDER    = 004, /* Post-order. */
	/* ETM_DEPTHFIRST = ETM_PREORDER + ETM_INORDER + ETM_POSTORDER, */
	ETM_LEVELORDER   = 010  /* Breadth first. */
	/* ETM_BREADTHFIRST = ETM_LEVELORDER */
} TvsMtd;

/* This structure describes a symbol of Huffman coding trees. */
typedef struct st_HFM_SYMBOL {
	UCHART name; /* Symbol name. */
	UCHART bits; /* Number of bits in a symbol. */
	UCHART sgnb; /* Significant bits in the symbol name. */
} HFM_SYMBOL, * P_HFM_SYMBOL;

/* Types for array implemented tries. */
typedef P_ARRAY_Z TRIE_A, * P_TRIE_A;

/* Functions for binary trees. */
int             treTraverseBPre    (P_TNODE_B       pnode,   CBF_TRAVERSE cbftvs, size_t       param);
int             treTraverseBIn     (P_TNODE_B       pnode,   CBF_TRAVERSE cbftvs, size_t       param);
int             treTraverseBPost   (P_TNODE_B       pnode,   CBF_TRAVERSE cbftvs, size_t       param);
int             treTraverseBLevel  (P_TNODE_B       pnode,   CBF_TRAVERSE cbftvs, size_t       param);
void            treInitB_O         (P_BTREE         ptreb);
void            treFreeB           (P_BTREE         ptreb);
P_BTREE         treCreateB         (void);
void            treDeleteB_O       (P_BTREE         ptreb);
P_TNODE_B       treInsertLeftB     (P_TNODE_B       pnode,   const void * pitem,  size_t       size);
P_TNODE_B       treInsertRightB    (P_TNODE_B       pnode,   const void * pitem,  size_t       size);
void            treRemoveLeftB     (P_TNODE_B       pnode);
void            treRemoveRightB    (P_TNODE_B       pnode);
size_t          treArityB          (P_TNODE_B       pnode);
size_t          treHeightB         (P_TNODE_B       pnode);
P_TNODE_B       treGetParentNodeB  (P_TNODE_B       proot,   P_TNODE_B    pchild);
P_TNODE_B       treSearchDataB     (P_TNODE_B       pnode,   const void * pitem,  size_t       size,    TvsMtd       tm);
BOOL            treDescendantB     (P_TNODE_B       proot,   P_TNODE_B    pnode);
P_TNODE_B       treMergeNodesB     (P_TNODE_B       proot,   const void * pitem,  size_t       size,    P_TNODE_B    pleft,  P_TNODE_B   pright);
P_TNODE_B       treSwapNodesB      (P_TNODE_B       proot1,  P_TNODE_B    pnode1, P_TNODE_B    proot2,  P_TNODE_B    pnode2);
P_TNODE_B       treCopyB           (P_TNODE_B       proot,   size_t       size);
/* Functions for generic trees. */
int             treTraverseGLevel  (P_TNODE_G       pnode,   CBF_TRAVERSE cbftvs, size_t       param);
size_t          treArityG          (P_TNODE_G       pnode);
size_t          treHeightG         (P_TNODE_G       pnode);
void *          treInitTNodeG      (P_TNODE_G       pnode,   const void * pitem,  size_t       size);
void            treFreeTNodeG      (P_TNODE_G       pnode);
P_TNODE_G       treCreateTNodeG    (const void *    pitem,   size_t       size);
void            treDeleteTNodeG    (P_TNODE_G       pnode);
void            treInitG_O         (P_GTREE         ptreg);
void            treFreeG           (P_GTREE         ptreg);
P_GTREE         treCreateG         (void);
void            treDeleteG         (P_GTREE         ptreg);
P_TNODE_G       treInsertG         (P_TNODE_G       pnode,   const void * pitem,  size_t       size);
P_TNODE_G       treRemoveSubtreeG  (P_TNODE_G       parent,  P_TNODE_G    pchild, BOOL         bclear);
P_TNODE_G       treGetParentNodeG  (P_TNODE_G       proot,   P_TNODE_G    pchild);
P_TNODE_G       treSearchDataG     (P_TNODE_G       proot,   const void * pitem,  size_t       size);
P_TNODE_G       treSwapNodesG      (P_TNODE_G       proot1,  P_TNODE_G    pnode1, P_TNODE_G    proot2,  P_TNODE_G    pnode2);
P_TNODE_G       treCopyG           (P_TNODE_G       proot,   size_t       size);
P_TNODE_B       treG2BConvert      (P_TNODE_G       pnode,   size_t       size);
/* Functions for heap trees. */
void            treInitHeapA       (P_HEAP_A        pheap,   size_t       num,    size_t       size);
void            treFreeHeapA       (P_HEAP_A        pheap);
P_HEAP_A        treCreateHeapA     (size_t          num,     size_t       size);
void            treDeleteHeapA     (P_HEAP_A        pheap);
BOOL            treIsEmptyHeapA_O  (P_HEAP_A        pheap);
BOOL            treIsFullHeapA_O   (P_HEAP_A        pheap);
void            treInsertHeapA     (P_HEAP_A        pheap,   const void * pitem,  void *       ptemp,   size_t       size,   CBF_COMPARE cbfcmp, BOOL        bmax);
void            treRemoveHeapA     (void *          pitem,   void *       ptemp,  size_t       size,    P_HEAP_A     pheap,  CBF_COMPARE cbfcmp, BOOL        bmax);
BOOL            trePeepHeapA       (void *          pitem,   size_t       size,   P_HEAP_A     pheap);
/* Functions for binary search trees(BSTs). */
void *          treInitBSTNode     (P_BSTNODE       pnode,   const void * pitem,  size_t       size,    size_t       param);
void            treFreeBSTNode_O   (P_BSTNODE       pnode);
P_BSTNODE       treCreateBSTNode   (const void *    pitem,   size_t       size,   size_t       param);
void            treDeleteBSTNode_O (P_BSTNODE       pnode);
void            treInitBST_O       (P_BST           pbst);
void            treFreeBST         (P_BST           pbst);
P_BST           treCreateBST       (void);
void            treDeleteBST_O     (P_BST           pbst);
P_BSTNODE       treCopyBST         (P_BSTNODE       proot,   size_t       size);
P_BSTNODE       treBSTFindData_R   (P_BSTNODE       proot,   const void * pitem, CBF_COMPARE   cbfcmp);
P_BSTNODE       treBSTFindData_A   (P_BSTNODE       proot,   const void * pitem, CBF_COMPARE   cbfcmp);
P_BSTNODE       treBSTInsertAA     (P_BSTNODE       pnode,   const void * pitem, size_t        size,    CBF_COMPARE  cbfcmp);
P_BSTNODE       treBSTRemoveAA     (P_BSTNODE       pnode,   const void * pitem, size_t        size,    CBF_COMPARE  cbfcmp);
P_BSTNODE       treBSTInsertAVL    (P_BSTNODE       pnode,   const void * pitem, size_t        size,    CBF_COMPARE  cbfcmp, BOOL *      pblc);
P_BSTNODE       treBSTRemoveAVL    (P_BSTNODE       pnode,   const void * pitem, size_t        size,    CBF_COMPARE  cbfcmp, BOOL *      pblc);
/* Functions for B-plus trees. */
void *          treInitBPTNode     (P_BPTNODE       pnode,   P_TNODE_B    parent, P_TNODE_B    pnext);
void            treFreeBPTNode     (P_BPTNODE       pnode);
P_BPTNODE       treCreateBPTNode   (P_BPTNODE       parent,  P_BPTNODE    pnext);
void            treDeleteBPTNode   (P_BPTNODE       pnode);
void            treInitBPT_O       (P_BPT           pbpt);
void            treFreeBPT         (P_BPT           pbpt);
P_BPT           treCreateBPT       (void);
void            treDeleteBPT       (P_BPT           pbpt);
int             treTraverseKeyBPT  (P_BPT           pbpt,    CBF_TRAVERSE cbftvs, size_t       param);
void *          treSearchDataBPT   (P_BPT           pbpt,    const void * pkey,   CBF_COMPARE  cbfcmp);
BOOL            treInsertBPT       (P_BPT           pbpt,    const size_t degree, const void * pkey,    CBF_COMPARE  cbfcmp);
BOOL            treBulkLoadBPT     (P_BPT           pbpt,    const size_t degree, PUCHAR       pkeys[], size_t num);
BOOL            treRemoveBPT       (P_BPT           pbpt,    const size_t degree, const void * pkey,    CBF_COMPARE  cbfcmp);
/* Functions for tries. */
void            treInitTrieA_O     (P_TRIE_A        ptrie);
void            treFreeTrieA_O     (P_TRIE_A        ptrie,   size_t       size);
P_TRIE_A        treCreateTrieA     (void);
void            treDeleteTrieA_O   (P_TRIE_A        ptrie,   size_t       size);
size_t *        treSearchTrieA     (P_TRIE_A        ptrie,   const void * pstr,   size_t       num,     size_t       size,   CBF_COMPARE cbfcmp);
BOOL            treInsertTrieA     (P_TRIE_A        ptrie,   const void * pstr,   size_t       num,     size_t       size,   size_t      vapdx,  CBF_COMPARE cbfcmp);
BOOL            treRemoveTrieA     (P_TRIE_A        ptrie,   const void * pstr,   size_t       num,     size_t       size,   CBF_COMPARE cbfcmp);
/* Functions for Huffman coding trees. */
P_BITSTREAM     treHuffmanEncoding (P_ARRAY_Z *     pptable, const PUCHAR s,      const size_t n);
P_BITSTREAM     treHuffmanDecoding (P_ARRAY_Z       ptable,  P_BITSTREAM  s);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svbtree.c. */
#define treInitB_M(ptreb_M) do { \
	(*(ptreb_M)) = NULL; \
} while (0)
#define treDeleteB_M(ptreb_M) do { \
	treFreeB(ptreb_M); \
	free(ptreb_M); \
} while (0)
/* Functions in svgtree.c. */
#define treInitG_M(ptreg_M) do { \
	*(ptreg_M) = NULL; \
} while (0)
/* Functions in svhtree.c. */
#define treIsEmptyHeapA_M(pheap_M) (!(pheap_M)->irear)
#define treIsFullHeapA_M(pheap_M) ((pheap_M)->irear == (pheap_M)->hdarr.num)
/* Functions in svstree.c. */
#define treFreeBSTNode_M(pnode_M) do { \
	strFreeNodeD(&(pnode_M)->knot); \
	(pnode_M)->knot.pdata = NULL; \
} while (0)
#define treDeleteBSTNode_M(pnode_M) do { \
	treFreeBSTNode(pnode_M); \
	free(pnode_M); \
} while (0)
#define treInitBST_M(pbst_M) do { \
	*(pbst_M) = NULL; \
} while (0)
#define treDeleteBST_M(pbst_M) do { \
	treFreeBST(pbst_M); \
	free(pbst_M); \
} while (0)
/* Macros for B-plus trees. */
#define _treInitBPTInfo_M(pbi_M) do { \
	(pbi_M)->headptr = NULL; \
	(pbi_M)->keyarr.num = 0; \
	(pbi_M)->keyarr.pdata = NULL; \
} while (0)
#define _treDeleteBPTInfo_M(pbi_M) do { \
	_treFreeBPTInfo(pbi_M); \
	free(pbi_M); \
} while (0)
#define _treGetParentBPTNode_M(pnode_M) ((pnode_M)->ppnode[PARENTPTR])
#define _treGetNextBPTNode_M(pnode_M) ((pnode_M)->ppnode[NEXTPTR])
#define _treIsLeafBPTNode_M(pnode_M) (!((_P_BPT_INFO)(pnode_M)->pdata)->headptr)
#define treInitBPT_M(pbpt_M) do { \
	*(pbpt_M) = NULL; \
} while (0)
/* Macros for tries. */
#define treInitTrieA_M(ptrie_M) do { \
	*(ptrie_M) = NULL; \
} while (0)
#define treFreeTrieA_M(ptrie_M, size_M) do { \
	_treFreeTrieNode(ptrie_M, size_M); \
	*(ptrie_M) = NULL; \
} while(0)
#define treDeleteTrieA_M(ptrie_M, size_M) do { \
	_treFreeTrieNode(ptrie_M, size_M); \
	free(ptrie_M); \
} while (0)

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	#include <stdlib.h> /* Using function free. */
	/* Functions in svbtree.c. */
	#define treInitB             treInitB_M
	#define treDeleteB           treDeleteB_O
	/* Functions in svgtree.c. */
	#define treInitG             treInitG_M
	/* Functions in svhtree.c. */
	#define treIsEmptyHeapA      treIsEmptyHeapA_M
	#define treIsFullHeapA       treIsFullHeapA_M
	/* Functions in svstree.c. */
	#define treBSTFindData_X     treBSTFindData_R
	#define treFreeBSTNode       treFreeBSTNode_O
	#define treDeleteBSTNode     treDeleteBSTNode_O
	#define treInitBST           treInitBST_M
	#define treDeleteBST         treDeleteBST_M
	/* Macros for B-plus trees. */
	#define _treInitBPTInfo      _treInitBPTInfo_O
	#define _treDeleteBPTInfo    _treDeleteBPTInfo_M
	#define _treGetParentBPTNode _treGetParentBPTNode_M
	#define _treGetNextBPTNode   _treGetNextBPTNode_M
	#define _treIsLeafBPTNode    _treIsLeafBPTNode_M
	#define treInitBPT           treInitBPT_M
	/* Macros for tries. */
	#define treInitTrieA         treInitTrieA_M
	#define treFreeTrieA         treFreeTrieA_O
	#define treDeleteTrieA       treDeleteTrieA_O
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	#include <stdlib.h> /* Using function free. */
	/* Functions in svbtree.c. */
	#define treInitB             treInitB_M
	#define treDeleteB           treDeleteB_M
	/* Functions in svgtree.c. */
	#define treInitG             treInitG_M
	/* Functions in svhtree.c. */
	#define treIsEmptyHeapA      treIsEmptyHeapA_M
	#define treIsFullHeapA       treIsFullHeapA_M
	/* Functions in svstree.c. */
	#define treBSTFindData_X     treBSTFindData_A
	#define treFreeBSTNode       treFreeBSTNode_M
	#define treDeleteBSTNode     treDeleteBSTNode_M
	#define treInitBST           treInitBST_M
	#define treDeleteBST         treDeleteBST_M
	/* Macros for B-plus trees. */
	#define _treInitBPTInfo      _treInitBPTInfo_M
	#define _treDeleteBPTInfo    _treDeleteBPTInfo_M
	#define _treGetParentBPTNode _treGetParentBPTNode_M
	#define _treGetNextBPTNode   _treGetNextBPTNode_M
	#define _treIsLeafBPTNode    _treIsLeafBPTNode_M
	#define treInitBPT           treInitBPT_M
	/* Macros for tries. */
	#define treInitTrieA         treInitTrieA_M
	#define treFreeTrieA         treFreeTrieA_M
	#define treDeleteTrieA       treDeleteTrieA_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	/* Functions in svbtree.c. */
	#define treInitB             treInitB_M
	#define treDeleteB           treDeleteB_O
	/* Functions in svgtree.c. */
	#define treInitG             treInitG_M
	/* Functions in svhtree.c. */
	#define treIsEmptyHeapA      treIsEmptyHeapA_M
	#define treIsFullHeapA       treIsFullHeapA_M
	/* Functions in svstree.c. */
	#define treBSTFindData_X     treBSTFindData_A
	#define treFreeBSTNode       treFreeBSTNode_O
	#define treDeleteBSTNode     treDeleteBSTNode_O
	#define treInitBST           treInitBST_M
	#define treDeleteBST         treDeleteBST_O
	/* Macros for B-plus trees. */
	#define _treInitBPTInfo      _treInitBPTInfo_M
	#define _treDeleteBPTInfo    _treDeleteBPTInfo_O
	#define _treGetParentBPTNode _treGetParentBPTNode_M
	#define _treGetNextBPTNode   _treGetNextBPTNode_M
	#define _treIsLeafBPTNode    _treIsLeafBPTNode_M
	#define treInitBPT           treInitBPT_M
	/* Macros for tries. */
	#define treInitTrieA         treInitTrieA_M
	#define treFreeTrieA         treFreeTrieA_M
	#define treDeleteTrieA       treDeleteTrieA_O
#else /* Optimization has been disabled. */
	/* Functions in svbtree.c. */
	#define treInitB             treInitB_O
	#define treDeleteB           treDeleteB_O
	/* Functions in svgtree.c. */
	#define treInitG             treInitG_O
	/* Functions in svhtree.c. */
	#define treIsEmptyHeapA      treIsEmptyHeapA_O
	#define treIsFullHeapA       treIsFullHeapA_O
	/* Functions in svstree.c. */
	#define treBSTFindData_X     treBSTFindData_R
	#define treFreeBSTNode       treFreeBSTNode_O
	#define treDeleteBSTNode     treDeleteBSTNode_O
	#define treInitBST           treInitBST_O
	#define treDeleteBST         treDeleteBST_O
	/* Macros for B-plus trees. */
	#define _treInitBPTInfo      _treInitBPTInfo_O
	#define _treDeleteBPTInfo    _treDeleteBPTInfo_O
	#define _treGetParentBPTNode _treGetParentBPTNode_O
	#define _treGetNextBPTNode   _treGetNextBPTNode_O
	#define _treIsLeafBPTNode    _treIsLeafBPTNode_O
	#define treInitBPT           treInitBPT_O
	/* Macros for tries. */
	#define treInitTrieA         treInitTrieA_O
	#define treFreeTrieA         treFreeTrieA_O
	#define treDeleteTrieA       treDeleteTrieA_O
#endif

#endif

/* # A valid AA binary search tree will be...
 *  _P_BST__    2
 * |*_______|  / \
 *  |         1   3
 *  +_BSTNODE________________________
 *  |_TNODE_B_________________ size_t|
 *  |_knot____________________|______|
 *  |ppnode[0]|ppnode[1]|pdata| param|
 *  |*________|*________|*____|_____2|
 *   |         |         | _int______
 *   |         |         +|0x00000002|
 *   |         +_BSTNODE________________________
 *   |         |_TNODE_B_________________ size_t|
 *   |         |_knot____________________|______|
 *   |         |ppnode[0]|ppnode[1]|pdata| param|
 *   |         |_____NULL|_____NULL|*____|_____1|
 *   |                   _int______ |
 *   |                  |0x00000003|+
 *   +_BSTNODE________________________
 *   |_TNODE_B_________________ size_t|
 *   |_knot____________________|______|
 *   |ppnode[0]|ppnode[1]|pdata| param|
 *   |_____NULL|_____NULL|*____|_____1|
 *             _int______ |
 *            |0x00000001|+
 * __________________________________________________________
 * # A trie which formed after inserting "cat", "cd" and "c",
 *   and an appendix to (const char *)"lynx" tailed string "cat".
 *  _P_TRIE_
 * |*_______|
 *  |
 *  +_ARRAY_Z_
 *  |num|pdata|
 *  |__1|*____|
 *       |
 *       +_size_ _name_____________ _value_
 *       |size  |              data|    'c'|
 *       |TRIE_A|         next_trie|      *--+_ARRAY_Z_
 *       |size_t| reference_counter|      3| |num|pdata|
 *       |size_t|          appendix|       | |__2|*____|
 *       |UCHART|______________flag|___TRUE|      |
 *               _value_ _name_____________ _size_+
 *              |'a'    |data              |  size|
 *   _ARRAY_Z_+--*      |next_trie         |TRIE_A|
 *  |num|pdata| |1      |reference_counter |size_t|
 *  |__1|*____| |       |appendix          |size_t|
 *       |      |FALSE__|flag______________|UCHART|
 *       |      |'d'    |data              |  size|
 *       |      |NULL   |next_trie         |TRIE_A|
 *       |      |1      |reference_counter |size_t|
 *       |      |       |appendix          |size_t|
 *       |      |TRUE___|flag______________|UCHART|
 *       +_size_ _name_____________ _value_
 *       |size  |              data|    't'|
 *       |TRIE_A|         next_trie|   NULL|
 *       |size_t| reference_counter|      1|
 *       |size_t|          appendix|      *--+_char[5]_
 *       |UCHART|______________flag|___TRUE| |_"lynx"__|
 *
 * ## Because the using of reference counters, this trie implementation
 *    allows users to insert a same string repeatedly.
 * ____________________________________________________________________
 *  _P_BPT_  # A B-plus tree in memory.
 * |*______| # Consider that you are playing a game.
 *  |        # Each structure is a room and each '*' is a door.
 *  +>+>+>+_BPTNODE___________________________
 *  | | | |___value|_____________name|____size| +>+___BPT_INFO___________
 *  | | | |NULL____|ppnode[PARENTPTR]|P_NODE_D| | |_ARRAY_Z_:keyarr______| +>+__BPT_KEY_INFO______ ____________________
 *  | | | |NULL____|ppnode[NEXTPTR]__|P_NODE_D| | |___size_t|num____|___2| | |PUCHAR____|P_BPTNODE|PUCHAR____|P_BPTNODE|
 *  | | | |size____|name_____________|value___| | |___PUCHAR|pdata__|___*->+ |pkey______|pchild___|pkey______|pchild___|
 *  | | | |PUCHAR__|pdata____________|_______*->+ |P_BPTNODE|headptr|___*|   |0x0000FFF8|*________|0x0000FFF0|*________|
 *  | | ^  _____________________________________________________________|     |          |__________   _______|
 *  | ^ | |                                                                 Fly to data             | |
 *  ^ | | +_BPTNODE___________________________                            array right here.         | |
 *  | | | |___value|_____________name|____size| +>+___BPT_INFO___________                           | |
 *  | | +<-*_______|ppnode[PARENTPTR]|P_NODE_D| | |_ARRAY_Z_:keyarr______| +>+__BPT_KEY_INFO______  | |
 *  | | +<-*_______|ppnode[NEXTPTR]__|P_NODE_D| | |___size_t|num____|___1| | |PUCHAR____|P_BPTNODE| V |
 *  | | | |size____|name_____________|value___| | |___PUCHAR|pdata__|___*->+ |pkey______|pchild___| | V
 *  | | | |PUCHAR__|pdata____________|_______*->+ |P_BPTNODE|headptr|NULL|   |0x0000FFF4|NULL_____| | |
 *  | | V  _________________________________________________________________________________________| |
 *  | | | |                                                 ## Leaf nodes are                        _|
 *  ^ | +>+_BPTNODE___________________________                            nodes with NULL headptr.  |
 *  | |   |___value|_____________name|____size| +>+___BPT_INFO___________                           |
 *  | +<---*_______|ppnode[PARENTPTR]|P_NODE_D| | |_ARRAY_Z_:keyarr______| +>+__BPT_KEY_INFO______  |
 *  |   +<-*_______|ppnode[NEXTPTR]__|P_NODE_D| | |___size_t|num____|___1| | |PUCHAR____|P_BPTNODE| V
 *  |   | |size____|name_____________|value___| | |___PUCHAR|pdata__|___*->+ |pkey______|pchild___| |
 *  |   | |PUCHAR__|pdata____________|_______*->+ |P_BPTNODE|headptr|NULL|   |0x0000FFF8|NULL_____| |
 *  |   V  _________________________________________________________________________________________|
 *  |   | |
 *  |   +>+_BPTNODE___________________________
 *  |     |___value|_____________name|____size| +>+___BPT_INFO___________
 *  +<-----*_______|ppnode[PARENTPTR]|P_NODE_D| | |_ARRAY_Z_:keyarr______| +>+__BPT_KEY_INFO______
 *        |NULL____|ppnode[NEXTPTR]__|P_NODE_D| | |___size_t|num____|___1| | |PUCHAR____|P_BPTNODE|
 *        |size____|name_____________|value___| | |___PUCHAR|pdata__|___*->+ |pkey______|pchild___|
 * *Data  |PUCHAR__|pdata____________|_______*->+ |P_BPTNODE|headptr|NULL|   |0x0000FFF0|NULL_____|
 *  array___________________________________
 * |Address|0x0000FFF0|0x0000FFF4|0x0000FFF8|
 * |Data___|_________3|_________1|_________2|
 */
