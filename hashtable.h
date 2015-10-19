/******************************************************************************\
 *                             hashtable.h                                    *
 *                            =============                                   *
 *                                                                            *
 *  This file advertises the hashtable system interface.                      *
 *                                                                            *
 *  This abstracts (as much as possible) the hashtable mechanism and gets     *
 *  the user to provide only the hash algorithm and a record comparison       *
 *  function.  The functions provided are:                                    *
 *                                                                            *
 *      HASHTABLE HashCreate(int numentries, HHASH hash, HCMP compare);       *
 *                                                                            *
 *          Creates a hashtable with 'numentries' slots. 'hash' is a function *
 *          used to create a hash value from the user data record. 'compare'  *
 *          is a function used to compare two data records. The function      *
 *          returns a token to be used when manipulating the new hashtable.   *
 *                                                                            *
 *      void HashInsert(HASHTABLE htable, void *record, int recsize);         *
 *                                                                            *
 *          Inserts a record into the hash table.  'htable' is the hashtable  *
 *          token returned from a HashCreate() call.  'record' is a pointer   *
 *          to the data record to be inserted. 'recsize' is record size.      *
 *                                                                            *
 *      void *HashLookup(HASHTABLE htable, void *record);                     *
 *                                                                            *
 *          Performs a lookup on the hashtable for a record matching the      *
 *          supplied lookup record 'record'.  Returns a pointer to the found  *
 *          record or NULL if the record isn't found.                         *
 *                                                                            *
 *      void HashDestroy(HASHTABLE htable);                                   *
 *                                                                            *
 *          Destroy a hashtable.                                              *
 *                                                                            *
 *  The records inserted into each hashtable must be the same.                *
 *                                                                            *
\******************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

/******
 * Typedefs for user.
 ******/

typedef void     *HASHTABLE;
typedef unsigned (*HHASH)(void *record);
typedef int      (*HCMP)(void *a, void *b);
typedef void     (*HDUMP)(void *record);


/******
 * Function prototypes.
 ******/

HASHTABLE HashCreate(int numentries, HHASH hash, HCMP compare);
void      HashInsert(HASHTABLE htable, void *record, int recsize);
void      *HashLookup(HASHTABLE htable, void *record);
void      HashDestroy(HASHTABLE htable);
void      HashDump(HASHTABLE htable, HDUMP dump);


#endif
