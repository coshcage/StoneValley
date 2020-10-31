/*
 * Name:        svhshtbl.h
 * Description: Hash tables interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171615U0609191235L00112
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

#ifndef _SVHSHTBL_H_
#define _SVHSHTBL_H_

#include "svstring.h"

/* Prototype of callback hash functions. */
typedef size_t (* CBF_HASH) (const void * pkey);

/* Types for separate chaining hash table. */
typedef struct st_ARRAY_Z HSHTBL_C, * P_HSHTBL_C;
/* Types for open addressing hash table. */
typedef struct st_ARRAY_Z HSHTBL_A, * P_HSHTBL_A;

/* Functions for separate chaining hash table. */
BOOL       hshInitC         (P_HSHTBL_C pht,   size_t       buckets);
void       hshFreeC         (P_HSHTBL_C pht);
P_HSHTBL_C hshCreateC       (size_t     buckets);
void       hshDeleteC       (P_HSHTBL_C pht);
size_t     hshSizeC         (P_HSHTBL_C pht);
int        hshTraverseC     (P_HSHTBL_C pht,   CBF_TRAVERSE cbftvs,  size_t       param);
P_NODE_S   hshSearchC       (P_HSHTBL_C pht,   CBF_HASH     cbfhsh,  const void * pkey,    size_t  size);
BOOL       hshInsertC       (P_HSHTBL_C pht,   CBF_HASH     cbfhsh,  const void * pkey,    size_t  size);
BOOL       hshRemoveC       (P_HSHTBL_C pht,   CBF_HASH     cbfhsh,  const void * pkey,    size_t  size);
BOOL       hshCopyC         (P_HSHTBL_C pdest, CBF_HASH     cbfhsh,  P_HSHTBL_C   psrc,    size_t  size);
/* Functions for open addressing hash table using double hashing. */
BOOL       hshInitA         (P_HSHTBL_A pht,   size_t       buckets, size_t size);
void       hshFreeA_O       (P_HSHTBL_A pht);
P_HSHTBL_A hshCreateA       (size_t     slots, size_t       size);
void       hshDeleteA_O     (P_HSHTBL_A pht);
size_t     hshSizeA         (P_HSHTBL_A pht,   size_t       size);
int        hshTraverseA     (P_HSHTBL_A pht,   size_t       size,    CBF_TRAVERSE cbftvs,  size_t       param);
void *     hshSearchA       (P_HSHTBL_A pht,   CBF_HASH     cbfhsh1, CBF_HASH     cbfhsh2, const void * pkey, size_t size);
void *     hshInsertA       (P_HSHTBL_A pht,   CBF_HASH     cbfhsh1, CBF_HASH     cbfhsh2, const void * pkey, size_t size);
BOOL       hshRemoveA       (P_HSHTBL_A pht,   CBF_HASH     cbfhsh1, CBF_HASH     cbfhsh2, const void * pkey, size_t size);
BOOL       hshCopyA         (P_HSHTBL_A pdest, CBF_HASH     cbfhsh1, CBF_HASH     cbfhsh2, P_HSHTBL_A   psrc, size_t size);
/* Some built-in hash functions are declared below. */
size_t     hshCBFHashString (REGISTER char * pstr);

/* Macros for function inline to accelerate execution speed. */
/* Functions in svhash.c. */
#define hshFreeA_M(pht_M) do { \
	strFreeArrayZ(pht_M); \
} while (0)
#define hshDeleteA_M(pht_M) do { \
	strDeleteArrayZ(pht_M); \
} while (0)

/* Library optimal switch. */
#if   SV_OPTIMIZATION == SV_OPT_MINISIZE
	/* Macros for open addressing hash tables. */
	#define hshFreeA   hshFreeA_M
	#define hshDeleteA hshDeleteA_M
#elif SV_OPTIMIZATION == SV_OPT_MAXSPEED
	/* Macros for open addressing hash tables. */
	#define hshFreeA   hshFreeA_M
	#define hshDeleteA hshDeleteA_M
#elif SV_OPTIMIZATION == SV_OPT_FULLOPTM
	/* Macros for open addressing hash tables. */
	#define hshFreeA   hshFreeA_M
	#define hshDeleteA hshDeleteA_M
#else /* Optimization has been disabled. */
	/* Macros for open addressing hash tables. */
	#define hshFreeA   hshFreeA_O
	#define hshDeleteA hshDeleteA_O
#endif

#endif

/* # A diagram shows what a separate chaining hash table looks like:
 * num == 3; // 3 is a prime number.
 * pdata:
 * [0x0000FFF1][0x00000000][0x00000000] : This is an array that used to store buckets.
 *      |                                 A NULL value indicates an empty bucket.
 * [0x0000FFF2][0x0000FFF3]->[0x65] : This is a NODE_S of a single linked-list.
 *      |                             A bucket is made of a single linked-list.
 * [0x00000000][0x0000FFF4]->[0x66]
 *     pnode      pdata      Real data.
 * ______________________________________________________________________________
 * # Another diagram illustrated what an open addressing hash table looks like:
 * num == 5; // 5 is a prime number.
 * pdata:         __Slot __   This slot is valid.
 *               /  _ Data_\  Cuz the flag of this slot == TRUE.
 *              |  |        | And its data is an integer of value 0xABCD1234.
 * [00|00000000][01|ABCD1234][00|FFFFFFFF][01|EEEEEEEE][00|00000000]
 *   |                         \                         |
 *   +-This is a Flag.          +These two slots are invalid.
 *     Flag size is determined
 *     by macro _FLAG_SIZE in svhash.c
 *     A flag is used to determine whether a slot is valid or not.
 *  ## An open addressing hash table is actually an ARRAY_Z.
 *  ## When function hshRemoveA removes an item from an open addressing hash table, it does not
 *     replace its content with zero in a slot rather than put the flag of this slot into FALSE.
 *     So that a removal could be fast.
 */
