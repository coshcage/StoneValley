/*
 * Name:        svhash.c
 * Description: Hash tables.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0901171615K0118211528L00608
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

#include <string.h> /* Using function memset, memcmp, memcpy. */
#include "svhash.h"

/* Functions for separate chaining hash table. */

/* File level function declarations. */
int _hshCBFFreeBuckets       (void * pitem, size_t param);
int _hshCBFSizeBuckets       (void * pitem, size_t param);
int _hshCBFFetchPdataInNodeS (void * pitem, size_t param);
int _hshCBFTraverseCPuppet   (void * pitem, size_t param);
int _hshCBFCopyCPuppet       (void * pitem, size_t param);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFFreeBuckets
 * Description:   This function is used to free buckets of a separate chaining hash table.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in array.
 *      param N/A.
 * Return value:  CBF_CONTINUE only.
 */
int _hshCBFFreeBuckets(void * pitem, size_t param)
{
	strFreeLinkedListSC((P_LIST_S)pitem);
	DWC4100(param);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFSizeBuckets
 * Description:   This function is used to calculate the usage of a separate chaining hash table.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in array.
 *      param Pointer to a size_t integer which is use to be an accumulator.
 * Return value:  CBF_CONTINUE only.
 */
int _hshCBFSizeBuckets(void * pitem, size_t param)
{
	if (NULL != *(P_NODE_S *)pitem)
		*(size_t *)param += strLevelLinkedListSC(*(P_NODE_S *)pitem);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFFetchNodeSPdata
 * Description:   This function is used to fetch pdata in a NODE_S.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in array.
 *      param Pointer to a size_t[2] array.
 *            size_t[0] stores a pointer to callback function.
 *            size_t[1] stores the param argument.
 * Return value:  The same value as callback function returns.
 */
int _hshCBFFetchPdataInNodeS(void * pitem, size_t param)
{
	return ((CBF_TRAVERSE)0[(size_t *)param])(((P_NODE_S)pitem)->pdata, 1[(size_t *)param]);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFSizeBuckets
 * Description:   This function is used to traverse each bucket in a separate chaining hash table.
 * Parameters:
 *      pitem Pointer to each P_NODE_S in array.
 *      param Pointer to a size_t[2] array.
 *            size_t[0] stores a pointer to callback function.
 *            size_t[1] stores the param argument.
 * Return value:  The same value as function strTraverseLinkedListSC returns.
 */
int _hshCBFTraverseCPuppet(void * pitem, size_t param)
{
	return strTraverseLinkedListSC_X(*(P_NODE_S *)pitem, NULL, _hshCBFFetchPdataInNodeS, param);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFCopyCPuppet
 * Description:   This function is used to cooperate with function hshCopyC to copy the entirely separate chaining hash table.
 * Parameters:
 *      pitem Pointer to each NODE_S in bucket.
 *      param Pointer to a size_t[3] array.
 *            size_t[0] stores a pointer to the destination hash table.
 *            size_t[1] stores a pointer to hash function.
 *            size_t[2] stores size of each element in the source table.
 * Return value:  CBF_CONTINUE  Insert data in to destined table is done.
 *                CBF_TERMINATE Cannot insert data into destined table.
 */
int _hshCBFCopyCPuppet(void * pitem, size_t param)
{
	return FALSE ==
		hshInsertC
		(
			(P_HSHTBL_C) 0[(size_t *)param],
			(CBF_HASH)1[(size_t *)param],
			pitem,
			2[(size_t *)param]
		)
		? CBF_TERMINATE : CBF_CONTINUE;
}

/* Function name: hshInitC
 * Description:   Initialize a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to initialize.
 *    buckets Number of buckets in hash table.
 *            This value shall be a prime number.
 * Return value:  TRUE  Initialize succeeded.
 *                FALSE Cannot initialize.
 * Caution:       Address of pht Must Be Allocated first.
 */
BOOL hshInitC(P_HSHTBL_C pht, size_t buckets)
{
	if (NULL == strInitArrayZ(pht, buckets, sizeof(P_NODE_S)))
		return FALSE;
	/* Clear array. */
	memset(pht->pdata, 0, sizeof(P_NODE_S) * strLevelArrayZ(pht));
	return TRUE;
}

/* Function name: hshFreeC
 * Description:   Deallocate a separate chaining hash table of which is allocated by function hshInitC.
 * Parameter:
 *       pht Pointer to the hash table you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pht Must Be Allocated first.
 */
void hshFreeC(P_HSHTBL_C pht)
{
	strTraverseArrayZ(pht, sizeof(P_NODE_S), _hshCBFFreeBuckets, ENT_SINGLE, FALSE);
	strFreeArrayZ(pht);
}

/* Function name: hshCreateC
 * Description:   Create a new separate chaining hash table dynamically.
 * Parameter:
 *   buckets Number of buckets in hash table.
 *           This value shall be a prime number.
 * Return value:  Pointer to new hash table.
 */
P_HSHTBL_C hshCreateC(size_t buckets)
{
	P_HSHTBL_C pht = strCreateArrayZ(buckets, sizeof(P_NODE_S));
	if (NULL == pht)
		return NULL;
	/* Clear array. */
	memset(pht->pdata, 0, sizeof(P_NODE_S) * strLevelArrayZ(pht));
	return pht;
}

/* Function name: hshDeleteC
 * Description:   Delete a separate chaining hash table of which is allocated by function hshCreateC.
 * Parameter:
 *       pht Pointer to the hash table you want to deallocate.
 * Return value:  N/A.
 * Caution:       Parameter pht Must Be Allocated first.
 */
void hshDeleteC(P_HSHTBL_C pht)
{
	strTraverseArrayZ(pht, sizeof(P_NODE_S), _hshCBFFreeBuckets, ENT_SINGLE, FALSE);
	strDeleteArrayZ(pht);
}

/* Function name: hshSizeC
 * Description:   Check how many items there are stored in a separate chaining hash table.
 * Parameter:
 *       pht Pointer to the hash table you want to deallocate.
 * Return value:  Number of items.
 * Caution:       Parameter pht Must Be Allocated first.
 */
size_t hshSizeC(P_HSHTBL_C pht)
{
	size_t n = 0;
	strTraverseArrayZ(pht, sizeof(P_NODE_S), _hshCBFSizeBuckets, (size_t)&n, FALSE);
	return n;
}

/* Function name: hshTraverseC
 * Description:   Traverse each elements in a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbftvs Pointer to the callback function.
 *      param Parameter which can be transfer into callback function.
 * Return value:  The same value as callback function returns.
 * Caution:       Parameter pht Must Be Allocated first.
 *                The type of pitem of function cbftvs is the type of pointer to the element you inserted.
 */
int hshTraverseC(P_HSHTBL_C pht, CBF_TRAVERSE cbftvs, size_t param)
{
	size_t a[2];
	a[0] = (size_t)cbftvs;
	a[1] = param;
	return strTraverseArrayZ(pht, sizeof(P_NODE_S), _hshCBFTraverseCPuppet, (size_t)a, FALSE);
}

/* Function name: hshSearchC
 * Description:   Search an element in a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbfhsh Pointer to hash function.
 *            The same hash table should use the same hash function.
 *       pkey Pointer to an element. Casted into (const void *).
 *      param size Size of data of pkey.
 * Return value:  Pointer to a NODE_S that contains key value.
 * Caution:       Parameter pht Must Be Allocated first.
 */
P_NODE_S hshSearchC(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size)
{
	return strSearchLinkedListSC(*(P_NODE_S *)(pht->pdata + (cbfhsh(pkey) % strLevelArrayZ(pht)) * sizeof(P_NODE_S)), pkey, size);
}

/* Function name: hshInsertC
 * Description:   Insert an element into a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbfhsh Pointer to hash function.
 *            The same hash table should use the same hash function.
 *       pkey Pointer to an element. Casted into (const void *).
 *       size Size of key.
 * Return value:  TRUE  Insertion succeeded.
 *                FALSE Insertion failure.
 * Caution:       Parameter pht Must Be Allocated first.
 * Tip:           You may need to search an element before invoke this function.
 */
BOOL hshInsertC(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size)
{
	P_NODE_S * ppnode = (P_NODE_S *)(pht->pdata + cbfhsh(pkey) % strLevelArrayZ(pht) * sizeof(P_NODE_S));
	P_NODE_S pnew = strCreateNodeS(pkey, size);
	if (NULL == pnew)
		return FALSE; /* Allocation failure. */
	if (NULL == *ppnode) /* Bucket is empty. */
		*ppnode = pnew;
	else /* Locate the last item in the bucket. */
	{
		P_NODE_S pnode = strLocateLastItemSC(*ppnode);
		pnode->pnode = pnew;
	}
	return TRUE;
}

/* Function name: hshRemoveC
 * Description:   Remove an element from a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbfhsh Pointer to hash function.
 *            The same hash table should use the same hash function.
 *       pkey Pointer to an element. Casted into (const void *).
 *       size Size of key.
 * Return value:  TRUE  Removal succeeded.
 *                FALSE Removal failure.
 * Caution:       Parameter pht Must Be Allocated first.
 */
BOOL hshRemoveC(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size)
{
	P_NODE_S * pphead = (P_NODE_S *)(pht->pdata + cbfhsh(pkey) % strLevelArrayZ(pht) * sizeof(P_NODE_S));
	P_NODE_S pnode = hshSearchC(pht, cbfhsh, pkey, size);
	if (NULL == pphead || NULL == *pphead || NULL == pnode)
		return FALSE;
	if (*pphead == pnode) /* Put next item into bucket. */
		*pphead = pnode->pnode;
	pnode = strRemoveItemLinkedListSC(*pphead, pnode);
	strDeleteNodeS(pnode);
	return TRUE;
}

/* Function name: hshCopyC
 * Description:   Make a copy of the entire separate chaining hash table.
 * Parameters:
 *      pdest Pointer to the destined hash table.
 *     cbfhsh Pointer to hash function for the destined table.
 *       psrc Pointer to the source hash table.
 *       size Size of each key in the source table.
 * Return value:  TRUE  Copying succeeded.
 *                FALSE Copying failure.
 * Caution:       Parameter psrc Must Be Allocated first.
 *                Size of each key in the source table shall be in the same size.
 */
BOOL hshCopyC(P_HSHTBL_C pdest, CBF_HASH cbfhsh, P_HSHTBL_C psrc, size_t size)
{
	size_t a[3];
	if (NULL == pdest)
		return FALSE;
	a[0] = (size_t)pdest;
	a[1] = (size_t)cbfhsh;
	a[2] = size;
	return CBF_CONTINUE != hshTraverseC(psrc, _hshCBFCopyCPuppet, (size_t)a) ? FALSE : TRUE;
}

/* Functions for open addressing hash table using double hashing function. */

#define _FLAG size_t    /* Flag used to sign whether a slot is empty or not. */
#define _P_FLAG _FLAG * /* Pointer to flag. */
#define _FLAG_SIZE sizeof(_FLAG) /* Size of a flag. */

/* File level function declarations. */
int _hshCBFCountSlots      (void * pitem, size_t param);
int _hshCBFTraverseOPuppet (void * pitem, size_t param);
int _hshCBFCopyOPuppet     (void * pitem, size_t param);

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFCountSlots
 * Description:   This function is used to calculate the usage of an open addressing hash table.
 * Parameters:
 *      pitem Pointer to each item in array.
 *      param Pointer to a size_t integer which is use to be an accumulator.
 * Return value:  CBF_CONTINUE only.
 */
int _hshCBFCountSlots(void * pitem, size_t param)
{
	if (FALSE != *(_P_FLAG)pitem)
		++(*(size_t *)param);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFTraverseOPuppet
 * Description:   This function is used to traverse each element in an open addressing hash table.
 * Parameters:
 *      pitem Pointer to each element in array.
 *      param Pointer to a size_t[2] array.
 *            size_t[0] stores a pointer to callback function.
 *            size_t[1] stores the param argument.
 * Return value:  If the slot were valid, return value of this function would be
 *                the same value as callback function returns.
 *                If slot were invalid, function returned CBF_CONTINUE.
 */
int _hshCBFTraverseOPuppet(void * pitem, size_t param)
{
	if (FALSE != *(_P_FLAG)pitem)
		return ((CBF_TRAVERSE)0[(size_t *)param])((PUCHAR)pitem + _FLAG_SIZE, 1[(size_t *)param]);
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _hshCBFCopyOPuppet
 * Description:   This function is used to cooperate with function hshCopyA to copy the entirely open addressing hash table.
 * Parameters:
 *      pitem Pointer to each slot in array.
 *      param Pointer to a size_t[4] array.
 *            size_t[0] stores a pointer to the destination hash table.
 *            size_t[1] stores a pointer to 1st hash function.
 *            size_t[2] stores a pointer to 2nd hash function.
 *            size_t[3] stores size of each element in the source table.
 * Return value:  CBF_CONTINUE  Inserting data in to the destined table is done.
 *                CBF_TERMINATE Cannot insert data into the target table.
 */
int _hshCBFCopyOPuppet(void * pitem, size_t param)
{
	return NULL ==
		hshInsertA
		(
			(P_HSHTBL_A) 0[(size_t *)param],
			(CBF_HASH)1[(size_t *)param],
			(CBF_HASH)2[(size_t *)param],
			pitem,
			3[(size_t *)param]
		)
		? CBF_TERMINATE : CBF_CONTINUE;
}

/* Function name: hshInitA
 * Description:   Initialize an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to initialize.
 *      slots Number of slots in hash table.
 *            Number of slots shall greater than or equal to the number of elements that you wanna insert into the table.
 *       size Size of each element in the table.
 *            Size of each element in the table should be in the same value.
 * Return value:  TRUE  Initialize succeeded.
 *                FALSE Cannot initialize.
 * Caution:       Address of pht Must Be Allocated first.
 */
BOOL hshInitA(P_HSHTBL_A pht, size_t slots, size_t size)
{
	if (NULL == strInitArrayZ(pht, slots, _FLAG_SIZE + size))
		return FALSE;
	/* Clear array. */
	memset(pht->pdata, 0, (_FLAG_SIZE + size) * strLevelArrayZ(pht));
	return TRUE;
}

/* Function name: hshFreeA_O
 * Description:   Deallocate an open addressing hash table of which is allocated by function hshInitA.
 * Parameter:
 *       pht Pointer to the hash table you want to deallocate.
 * Return value:  N/A.
 * Caution:       Address of pht Must Be Allocated first.
 * Tip:           A macro version of this function named hshFreeA_M is available.
 */
void hshFreeA_O(P_HSHTBL_A pht)
{
	strFreeArrayZ(pht);
}

/* Function name: hshCreateA
 * Description:   Create a new open addressing hash table dynamically.
 * Parameters:
 *      slots Number of slots in hash table.
 *            Number of slots shall greater than or equal to number of element to wanna insert into the table.
 *       size Size of each element in the table.
 *            Size of each element in the table should be in the same value.
 * Return value:  Pointer to new hash table.
 */
P_HSHTBL_A hshCreateA(size_t slots, size_t size)
{
	P_HSHTBL_A phtn = strCreateArrayZ(slots, _FLAG_SIZE + size);
	if (NULL == phtn)
		return NULL;
	/* Clear array. */
	memset(phtn->pdata, 0, (_FLAG_SIZE + size) * strLevelArrayZ(phtn));
	return phtn;
}

/* Function name: hshDeleteA_O
 * Description:   Delete an open addressing hash table of which is allocated by function hshCreateA.
 * Parameter:
 *       pht Pointer to the hash table you want to deallocate.
 * Return value:  N/A.
 * Caution:       Parameter pht Must Be Allocated first.
 * Tip:           A macro version of this function named hshDeleteA_M is available.
 */
void hshDeleteA_O(P_HSHTBL_A pht)
{
	strDeleteArrayZ(pht);
}

/* Function name: hshSizeA
 * Description:   Check how many items there are stored in an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to deallocate.
 *       size Size of each element in the table.
 * Return value:  Number of items.
 * Caution:       Parameter pht Must Be Allocated first.
 */
size_t hshSizeA(P_HSHTBL_A pht, size_t size)
{
	size_t n = 0;
	strTraverseArrayZ(pht, _FLAG_SIZE + size, _hshCBFCountSlots, (size_t)&n, FALSE);
	return n;
}

/* Function name: hshTraverseA
 * Description:   Traverse each elements in an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbftvs Pointer to the callback function.
 *      param Parameter which can be transfer into callback function.
 * Return value:  The same value as callback function returns.
 * Caution:       Parameter pht Must Be Allocated first.
 *                The type of pitem of function cbftvs is the type of pointer to the element you inserted.
 */
int hshTraverseA(P_HSHTBL_A pht, size_t size, CBF_TRAVERSE cbftvs, size_t param)
{
	size_t a[2];
	a[0] = (size_t)cbftvs;
	a[1] = param;
	return strTraverseArrayZ(pht, _FLAG_SIZE + size, _hshCBFTraverseOPuppet, (size_t)a, FALSE);
}

/* Function name: hshSearchA
 * Description:   Search an element in an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *    cbfhsh1 Pointer to 1st hash function.
 *    cbfhsh2 Pointer to 2nd hash function.
 *            The same open addressing hash table should use the same double hashing function.
 *       pkey Pointer to an element. Casted into (const void *).
 *       size Size of each element in the table.
 * Return value:  Pointer to an element that contains key value.
 * Caution:       Parameter pht Must Be Allocated first.
 */
void * hshSearchA(P_HSHTBL_A pht, CBF_HASH cbfhsh1, CBF_HASH cbfhsh2, const void * pkey, size_t size)
{
	_P_FLAG pflag;
	REGISTER size_t i, j;
	for (i = 0; i < strLevelArrayZ(pht); ++i)
	{
		j = (cbfhsh1(pkey) + i * cbfhsh2(pkey)) % strLevelArrayZ(pht);
		pflag = (_P_FLAG)(pht->pdata + j * (_FLAG_SIZE + size));
		if (FALSE == *pflag) /* Comapre to determine whether slot is empty or not. */
			return NULL;
		else
		{
			if (0 == memcmp(pflag + _FLAG_SIZE, pkey, size))
				return pflag + _FLAG_SIZE;
		}
	}
	return NULL;
}

/* Function name: hshInsertA
 * Description:   Insert an element into an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *    cbfhsh1 Pointer to 1st hash function.
 *    cbfhsh2 Pointer to 2nd hash function.
 *            The same open addressing hash table should use the same double hashing function.
 *       pkey Pointer to an element. Casted into (const void *).
 *       size Size of key.
 * Return value:  Pointer to new inserted element. Casted to (void *).
 * Caution:       Parameter pht Must Be Allocated first.
 */
void * hshInsertA(P_HSHTBL_A pht, CBF_HASH cbfhsh1, CBF_HASH cbfhsh2, const void * pkey, size_t size)
{
	_P_FLAG pflag;
	REGISTER size_t i, j;
	for (i = 0; i < strLevelArrayZ(pht); ++i)
	{
		j = (cbfhsh1(pkey) + i * cbfhsh2(pkey)) % strLevelArrayZ(pht);
		pflag = (_P_FLAG)(pht->pdata + j * (_FLAG_SIZE + size));
		if (FALSE == *pflag) /* Comapre to determine whether slot is empty or not. */
		{
			*pflag = TRUE;
			return memcpy(pflag + _FLAG_SIZE, pkey, size);
		}
	}
	return NULL;
}

/* Function name: hshRemoveA
 * Description:   Remove an element from an open addressing hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *    cbfhsh1 Pointer to 1st hash function.
 *    cbfhsh2 Pointer to 2nd hash function.
 *            The same open addressing hash table should use the same double hashing function.
 *       pkey Pointer to an element. Casted into (const void *).
 *       size Size of key.
 * Return value:  TRUE  Removal succeeded.
 *                FALSE Removal failure.
 * Caution:       Parameter pht Must Be Allocated first.
 */
BOOL hshRemoveA(P_HSHTBL_A pht, CBF_HASH cbfhsh1, CBF_HASH cbfhsh2, const void * pkey, size_t size)
{
	_P_FLAG pflag;
	REGISTER size_t i, j;
	for (i = 0; i < strLevelArrayZ(pht); ++i)
	{
		j = (cbfhsh1(pkey) + i * cbfhsh2(pkey)) % strLevelArrayZ(pht);
		pflag = (_P_FLAG)(pht->pdata + j * (_FLAG_SIZE + size));
		if (FALSE == *pflag) /* Comapre to determine whether slot is empty or not. */
			return FALSE;
		else if (0 == memcmp(pflag + _FLAG_SIZE, pkey, size))
		{
			*pflag = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

/* Function name: hshCopyA
 * Description:   Make a copy of the entirely open addressing hash table.
 *                This function could be use while users attempt to change
 *                hash functions for an open addressing hash table.
 * Parameters:
 *      pdest Pointer to the destined hash table.
 *    cbfhsh1 Pointer to 1st hash function for the destined table.
 *    cbfhsh2 Pointer to 2nd hash function for the destined table.
 *       psrc Pointer to the source hash table.
 *       size Size of each key in the source table.
 * Return value:  TRUE  Copying succeeded.
 *                FALSE Copying failure.
 * Caution:       Parameter psrc Must Be Allocated first.
 *                Size of each key in the source table shall be in the same size.
 */
BOOL hshCopyA(P_HSHTBL_A pdest, CBF_HASH cbfhsh1, CBF_HASH cbfhsh2, P_HSHTBL_A psrc, size_t size)
{
	size_t a[4];
	a[0] = (size_t)pdest;
	a[1] = (size_t)cbfhsh1;
	a[2] = (size_t)cbfhsh2;
	a[3] = size;
	return CBF_CONTINUE != hshTraverseA(psrc, size, _hshCBFCopyOPuppet, (size_t)a) ? FALSE : TRUE;
}

#undef _FLAG
#undef _P_FLAG
#undef _FLAG_SIZE

/* Function name: hshCBFHashString
 * Description:   Hash a string.
 * Parameter:
 *      pstr Pointer to a char array.
 * Return value:  Hash result.
 * Notice:        This function comes from book Data Structures and Algorithm Analysis in C.
 *                The Author of this book is Mark Allen Weiss. (I altered this function for performance.)
 */
size_t hshCBFHashString(REGISTER char * pstr)
{
	REGISTER size_t hrtn = 0;
	while ('\0' != *pstr) hrtn = (hrtn << 5) + *pstr++;
	return hrtn;
}
